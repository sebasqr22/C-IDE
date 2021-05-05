/**
  *@file ide.cpp
  *@title IDE
  *@brief Clase que contiene los pricipales métodos de para ser usados con la clase main.cpp
*/

#include "ide.h"
#include "ui_ide.h"
#include <QDebug>
#include <QMessageBox>
#include "creadorListas.cpp"
#include "operaciones.cpp"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "logger.cpp"
#include "operacionesEstructs.cpp"
#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;

//variables globales
bool corriendo = false;
int depurLine = 0;
QStringList codigo;
QStringList badLine;
logger Log;
string jsonEnviar;
QStringList recibidos;
QStringList mostrarRam;
bool malosPrints = false;

//Preparacion del socket
int sock = 0, valread;
struct sockaddr_in serv_addr;
int port = 8080;
char buffer[1024] = {0};
bool socket_exists = false;

/**
 * @brief json_recieve Hace los parse respectivos para JSON
 * @param str Recibe un string
 * @return Un Document con el parse
 */
Document json_recieve(string str){
    const char* pchar = str.c_str();
    Document ptd;
    ptd.Parse(pchar);
    return ptd;
}


/**
 * @brief ide::ide Constructor de la clase ide.cpp
 * @param parent
 */
ide::ide(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ide)
{
    ui->setupUi(this);
}
/**
 * @brief ide::~ide
 */
ide::~ide()
{
    delete ui;
}
/**
 * @brief ide::imprimirMalas Imprime lineas malas o logs
 * @param tipo Tipo de impresion
 * @param crit Nivel de criticalidad
 * @param msg Mensaje para logger
 */
void ide::imprimirMalas(int tipo, int crit, QString msg){
    if(tipo == 1){
        int largo = badLine.size();
        ui->viendo->setText("");
        ui->viendo->append("Se encontro errores en las lineas: ");
        QString malas = "";
        QString aux = "";
        for(int i=0; i < largo; i++){
            aux += "-";
            aux += badLine[i];
            aux+= "-";
            malas += aux;
            aux = "";
        }
        ui->viendo->append(malas);
    }
    else{
        ui->log->append(Log.mostrar(crit, msg));
    }
}
/**
 * @brief separadorJSON Permite obtener el valor y el nombre de las variables para crear el JSON
 * @param str Recibe un string de una variable
 * @return QStringList con nombre y valor de una variable
 */
QStringList separadorJSON(QString str){ // hola = 45;
    int largo = str.size();
    QString name;
    QString value;
    QStringList tmp;
    bool despuesIgual = false;

    for(int i=0; i<largo; i++){
        if(str[i] == "="){
            despuesIgual = true;
        }
        else if(despuesIgual == false){
            name += str[i];
        }
        else if(despuesIgual == true){
            value += str[i];
        }
    }
    tmp << name << value;
    return tmp;
}
/**
 * @brief JSON_Adapter Metodo para acomdar las variables en formato JSON y ser enviadas al servidor
 * @param lista Lista con las variables
 */
void JSON_Adapter(QStringList lista){
    int largo = lista.size();
    int memoria;
    QString type;
    QString name;
    QString value;
    QString curr;

    for(int i=0; i<largo; i++){
        QStringList aux;
        curr = lista[i];

        if(curr.contains("int")){
            type = "int";
            memoria = 4;
            aux = separadorJSON(curr.remove("int"));
            name = aux[0];
            value = aux[1];
        }
        else if(curr.contains("float")){
            type = "float";
            memoria = 4;
            aux = separadorJSON(curr.remove("float"));
            name = aux[0];
            value = aux[1];
        }
        else if(curr.contains("long")){
            type = "long";
            memoria = 8;
            aux = separadorJSON(curr.remove("long"));
            name = aux[0];
            value = aux[1];
        }
        else if(curr.contains("char")){
            type = "char";
            memoria = 1;
            aux = separadorJSON(curr.remove("char"));
            name = aux[0];
            value = aux[1];
        }
        else if(curr.contains("double")){
            type = "double";
            memoria = 8;
            aux = separadorJSON(curr.remove("double"));
            name = aux[0];
            value = aux[1];
        }
        else{
            type = "reference";
            memoria = 4;
            aux = separadorJSON(curr);
            name = aux[0];
            value = aux[1];
        }
        name.remove('\"');
        type.remove('\"');
        value.remove('\"');
        jsonEnviar = R"({"name":")"+ name.toStdString() + R"(","type":")" + type.toStdString() + R"(","value":")" + value.toStdString() + R"(","memory":")" + std::to_string(memoria) +  "\"}";
        string s = jsonEnviar;
        qInfo() <<"ENVIADO: "<< QString::fromStdString(s);

        char *message = &s[0];
        send(sock , message , strlen(message) , 0 );//Se envia la info
        qInfo() << "ENVIADO: " << QString::fromStdString(s);
    }
}
/**
 * @brief crearSocket Metodo que genera el socket entre el ide y el servidor
 * @return numero entero en caso de error
 */
int crearSocket(){
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            qInfo() << "\n Socket creation error \n";
            return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        qInfo() << "\nInvalid address/ Address not supported \n";
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        qInfo() << "\nConnection Failed \n";
        return -1;
    }
    qInfo() << "SOCKET CREADO";
}
/**
 * @brief validarComas Metodo para validar si los puntos y comas estan puestos de forma correcta
 * @param aux String con informacion de una linea de codigo
 * @return Booleano con informacion sobre validacion
 */
bool validarComas(QString aux){
    int largo = aux.size();
    bool good = true;
    int lineNum = 0;

    for(int i=0; i <= largo; i++){
        if(aux[i] == "\n"){
            lineNum += 1;
            if(aux[i-1] != ";"){
                if(aux[i-1] != "{" && aux[i-1] != "}" && aux[i-1] != "\n"){
                    badLine << QString::number(i-1);
                    qInfo() << "Menos 1" << QString::number(lineNum);
                    good = false;
                    break;
                }
            }
        }
        else if(i==largo-1){
           if(aux[i]!= ";" && aux[i] != "}" && aux[i] != "\n"){
               badLine << QString::number(i);
               qInfo() << "Normal" << QString::number(i);
               good = false;
               break;
           }
        }
    }
   return good;
}
/**
 * @brief adapter Obtiene el codigo escrito y lo transforma a un formato legible por el software
 * @param tmp String de una linea de codigo
 * @return Lista con la informacion en formato correcto
 */
QStringList adapter(QString tmp){
    tmp.remove("\n");
    qInfo() << tmp;

    QString temp2 = "";
    QStringList listaInfo;
    int largo = tmp.size();

    for(int i=0; i < largo; i++){
        if(tmp[i] == "{"){
            QStringList aux;
            int largo2 = i;
            for(largo2; tmp[largo2] != "}"; largo2++){
                temp2.append(tmp[largo2]);
            }
            temp2.append("}");
            i = largo2;
            aux << temp2;
            listaInfo << aux;
            temp2 = "";
        }
        else if(tmp[i] != ";"){
            temp2.append(tmp[i]);
        }
        else{
            listaInfo << temp2;
            temp2 = "";
        }

    }
    qInfo() << listaInfo;
    return listaInfo;
}
/**
 * @brief agrega Crea un QString con partes de otro QString, se repite n veces
 * @param aux QString con la informacion necesaria
 * @param veces Cantidad de veces que se repetira el proceso
 * @return QString con la informacion generada
 */
QString agrega(QString aux, int veces){
    QString todo = "";
    for(int i=0; i<=veces; i++){
        todo.append(aux[i]);
    }
    return todo;
}
/**
 * @brief validaResto Metodo para validar que todo menos las estructuras tengan buen formato
 * @param tmp QStringList con la informacion a validar
 * @param largo Largo de la lista "tmp"
 * @param analizando QString que se analizará
 * @return Número entero con cantidad de datos correctos
 */
int validaResto(QStringList tmp, int largo, QString analizando){
    int contador = 0;
    QStringList incompletos;
    QStringList correctos;
    //validamos int
    for(int i=0; i<largo; i++){
        analizando = agrega(tmp[i], 3);
        if(analizando == "int "){
            contador++;
            correctos << QString::number(i);
            qInfo() << "Sumando contador por int";
        }
        else{
            incompletos << QString::number(i);
            qInfo() << "INT" << incompletos;
        }
    }
    //validamos char y long
    for(int i=0; i<largo; i++){
        analizando = agrega(tmp[i], 4);
        if(analizando == "char " || analizando == "long "){
            contador++;
            correctos << QString::number(i);
        }
        else{
            incompletos << QString::number(i);
            qInfo() << "CHAR - LONG" << incompletos;
        }
    }
    //validamos float y print(
    for(int i=0; i<largo; i++){
        analizando = agrega(tmp[i], 5);
        if(analizando == "float " || analizando == "print("){
            contador++;
            correctos << QString::number(i);
            qInfo() << "Sumando contador por float";
            qInfo() << "FLOAT - PRINT" << incompletos;
        }
        else{
            incompletos << QString::number(i);
        }
    }
    //validamos double
    for(int i=0; i<largo; i++){
        analizando = agrega(tmp[i], 6);
        if(analizando == "double "){
            contador++;
            correctos << QString::number(i);
            qInfo() << "Sumando contador por double";
        }
        else{
            incompletos << QString::number(i);
            qInfo() << "DOUBLE" << incompletos;
        }
    }
    //validamos reference<
    for(int i=0; i<largo; i++){
        analizando = agrega(tmp[i], 9);
        if(analizando == "reference<"){
            contador++;
            correctos << QString::number(i);
            qInfo() << "Sumando contador por reference";
        }
        else{
            incompletos << QString::number(i);
            qInfo() << "REFERENCE" << incompletos;
        }
    }
    int largoMal = incompletos.size();
    QStringList bien;
    for(int x=0; x<largoMal; x++){
        if(correctos.contains(incompletos[x]) == false){
            bien << incompletos[x];
        }
    }
    int good = bien.size();
    for(int j=0; j<good; j++){
        badLine << bien[j];
    }
    return contador;
}
/**
 * @brief validarTipos Método para validar el formato del código de C!
 * @param tmp Lista con el código generado
 * @return Booleano que indica si el formato es correcto o no
 */
bool validarTipos(QStringList tmp){
    qInfo() << "Malas: " <<badLine;
    int largo = tmp.size();
    int contador = 0;
    int contadorEstructura = 0;
    bool hayStruct = false;
    bool goodStructs = false;
    QString analizando = "";

    //validamos primero las structuras
    for(int i=0; i<largo; i++){
        if(tmp[i][0] == "{"){
            contador++;
            qInfo() << "Sumando contador por {";
            hayStruct = true;
            QStringList listaAux;
            QString stringAux = "";
            int largoEst = tmp[i].size();
            analizando = tmp[i];
            for(int a=1; a<largoEst; a++){
                if(analizando[a] != ";"){
                    stringAux.append(analizando[a]);
                }
                else{
                    listaAux << stringAux;
                    stringAux = "";
                }
            }
            largoEst = listaAux.size();
            qInfo() << listaAux;
            contadorEstructura = validaResto(listaAux, largoEst, "");
            qInfo() << "Malas Segundo: " <<badLine;
            qInfo() << contadorEstructura;
            goodStructs = contadorEstructura == largoEst;
        }
       // else{
            //badLine << QString::number(i);
        //}
    }
    int aux = validaResto(tmp, largo, "");
    contador += aux;
    qInfo() << "Malas tres: " <<badLine;

    if(hayStruct == true){
        if(contador == largo && goodStructs==true){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return contador == largo;
    }
}
/**
 * @brief imprimir Método para hacer uso de los prints de C!
 * @param lista Lista con los prints del código generado en C!
 * @param res Lista con todas las variables y sus valores
 * @return Lista con los prints y valores correctos
 */
QStringList imprimir(QStringList lista, QStringList res){
    int largo = lista.size();
    int largoRes = res.size();
    bool esta = false;
    QString tmp;
    QStringList listaAux;
    int contadorMal = 0;
    for(int i=0; i<largo; i++){
         if(lista[i].contains('\"')){
             esta = true;
             tmp = lista[i].remove("print(");
             tmp.remove(")");
             tmp.remove('\"');
             listaAux << tmp;
         }
         else{
             QString curr = lista[i].remove("print(");
             curr.remove(")");
             for(int j=0; j<largoRes; j++){
                 if(res[j].contains(curr)){
                     int largoStr = res[j].size();
                     esta = true;
                     bool listo = false;
                     QString valor;
                     for(int x=0; x<largoStr; x++){
                         if(res[j][x] == "="){
                             listo = true;
                         }
                         else if(listo){
                             valor += res[j][x];
                         }
                     }
                     listaAux << valor;
                 }
             }
         }
         if(esta==false){
             contadorMal++;
         }
         else{
             esta = false;
         }
    }
    if(contadorMal != 0){
        malosPrints = true;
    }
    qInfo() << listaAux;
    return listaAux;
}
/**
 * @brief ide::verCorriendo Método para ver una línea específica de código en debugger
 * @param pos Posición de la lista del código
 */
void ide::verCorriendo(int pos){
    int largo = codigo.size();
    if(pos == largo-1){
        ui->delante->setEnabled(false);
        if(largo == 2){
            ui->atras->setEnabled(true);
        }
    }
    else if(pos == 0){
        ui->atras->setEnabled(false);
        if(largo == 2){
            ui->delante->setEnabled(true);
        }
    }
    else{
        ui->atras->setEnabled(true);
        ui->delante->setEnabled(true);
    }
    /*
    if(pos > largo-1){
        pos = 0;
        depurLine =0;
    }
    else if(pos < 0){
        pos = largo-1;
        depurLine = largo-1;
    }*/
    ui->viendo->setText(codigo[pos]);
}
QString getLine(int numLine){
    return codigo[numLine];
}
/**
 * @brief ide::imprimirMalas Muestra las líneas en que se encuentra un error en el código
 */

/**
 * @brief removeAll Borra la info de la lista de lineas con error
 */
void removeAll(){
    QStringList aux;
    int largo = badLine.size();
    for(int i=0; i<largo; i++){
        aux << badLine[i];
    }
    for(int j=0; j<largo; j++){
        badLine.removeAll(aux[j]);
    }
}
/**
 * @brief quitarRepetidos Método para quitar elementos repetidos de una lista
 * @param lista Lista a la que se le quitará los elementos repetidos
 * @return Lista sin elementos repetidos
 */
QStringList quitarRepetidos(QStringList lista){
    QStringList tmp;
    int largo = lista.size();
    for(int i=0; i<largo; i++){
        if(tmp.contains(lista[i]) == false){
            tmp << lista[i];
        }
    }
    return tmp;
}
/**
 * @brief cambioFormatoStructs Método para crear el formato correcto para las estructuras
 * @param lista Recibe lista con las estructuras respectivas
 * @return
 */
QStringList cambioFormatoStructs(QStringList lista){
    int largo = lista.size();
    QStringList buenFormato;
    QString tmp;
    QString actual;

    for(int i=0; i<largo; i++){
        int largoAct = lista[i].size();
        actual = lista[i];

        for(int y = 0; y<largoAct; y++){
            if(actual[y] != "{" && actual[y] != ";" && actual[y] != "}"){
                tmp += actual[y];
            }
            else if(actual[y] == ";"){
                buenFormato << tmp;
                tmp = "";
            }
            else if(actual[y] == "}"){
               buenFormato << tmp;
            }
        }
    }
    return buenFormato;
}
/**
 * @brief hayNulos Verifica si hay variables sin valor
 * @param lista Lista de resultados post-operaciones
 * @return Valor de verdad, en caso de variables sin valor "true"
 */
bool hayNulos(QStringList lista){
    int largo = lista.size();
    QString curr;
    QString p2 = "";
    bool nulos = false;
    bool despuesIgual = false;

    for(int i=0; i<largo; i++){
        curr = lista[i];
        int largoCurr = curr.size();

        for(int j=0; j<largoCurr; j++){
            if(despuesIgual == true){
                p2 += curr[j];
            }
            else if(curr[j] == "="){
                despuesIgual = true;
            }
        }
        if(p2 == ""){
            nulos = true;
            break;
        }
    }
    return nulos;
}
QStringList quitaEspacios(QStringList lista){
    int largo = lista.size();
    QStringList tmp;

    for(int i=0; i<largo; i++){
        tmp << lista[i].replace(" ", "");

    }
    return tmp;
}
/**
 * @brief validaRefs Valida contenido de las referencias
 * @param lista Lista con todas las referencias
 * @return Bool true si todo esta correcto
 */
bool validaRefs(QStringList lista, QStringList listaVerificar){
    int largo = lista.size();
    bool correcto;
    bool p2 = false;
    bool tieneAn = false;
    QString curr;
    QString tipo;
    QString variable;

    for(int i=0; i<largo; i++){
        curr = lista[i];
        curr.remove("reference<");
        int largoCurr = curr.size();
        for(int j=0; j<largoCurr; j++){
            if(p2 == false && curr[j] != ">"){
                tipo += curr[j];
            }
            else if(curr[j] == ">"){
                p2 = true;
            }
            else if(curr[j] == "&"){
                tieneAn = true;
            }
            else{
                variable += variable;
            }
        }
        if(listaVerificar.contains(variable)){
            int largoVer = listaVerificar.size();
            QString curr2;
            for(int y=0; y<largoVer; y++){
                curr2 = listaVerificar[y];
                if(curr2.contains(variable)){
                    if(curr2.contains(tipo)){
                        correcto = true;
                    }
                    else{
                        return false;
                    }
                }
            }
        }
        else{
            return false;
        }
    }
    return correcto;
}
QStringList ordenarRam(QStringList codigo, QStringList aOrdenar){
    int largoCod = codigo.size();
    int largoRam = aOrdenar.size();
    QString nombre;
    QString curr;
    QStringList ordenada;

    for(int i=0; i<largoCod; i++){
        curr = codigo[i];
        curr.remove("int");
        curr.remove("long");
        curr.remove("char");
        curr.remove("float");
        curr.remove("double");
        curr.remove("reference<");
        curr.remove(">");
        curr.remove(" ");
        int largoDentro = curr.size();

        for(int j=0; j<largoDentro; j++){
            if(curr[j] == "="){
                break;
            }
            else{
                nombre += curr[j];
            }
        }
        for(int r=0; r<largoRam; r++){
            if(aOrdenar[r].contains(nombre)){
                ordenada << aOrdenar[r];
                break;
            }
        }

    }
    return ordenada;
}
/**
 * @brief avanzarRam Imprimir elementos de la lista
 * @param lineaCodigo String con linea de codigo
 */
void avanzarRam(QString lineaCodigo){
    qInfo() << "LINEA= " << lineaCodigo;
    int largoLista = recibidos.size();
    int largo = lineaCodigo.size();
    QString nombre;
    Document documentPet;
    lineaCodigo.remove("int");
    lineaCodigo.remove("long");
    lineaCodigo.remove("char");
    lineaCodigo.remove("float");
    lineaCodigo.remove("double");
    lineaCodigo.remove("reference<");
    lineaCodigo.remove(">");
    lineaCodigo.remove("{");

    for(int j=0; j<largo; j++){
        if(lineaCodigo[j] != "="){
            nombre += lineaCodigo[j];
        }
        else{
            break;
        }
    }
    nombre.remove(" ");
    qInfo() << "NOMBRE: " << nombre;

    for(int i=0; i<largoLista; i++){
        documentPet = json_recieve(recibidos[i].toStdString());
        QString name = documentPet["name"].GetString();
        qInfo() << "NAME = " << name;
        if(nombre == name){
            qInfo() << "PONIENDO EN LISTA RAM";
            mostrarRam << recibidos[i];
        }
    }
}
/**
 * @brief limpiarRam Limpia la lista de datos a mostrar en modo debug
 */
void limpiarRam(){
    QStringList listaVacia;
    mostrarRam = listaVacia;
}
/**
 * @brief ide::on_runBut_clicked Función que al tocar el botón RUN, lee el código y ejecuta las demás funciones
 */
void ide::on_runBut_clicked()//basicamente esto es un adapter
{
    if(ui->editor->toPlainText() != ""){
        try{


            if (socket_exists == false){
                crearSocket(); //se crea el socket entre el server y el ide
                socket_exists = true;
            }
            Document documentPet;
            ui->viendo->setEnabled(false);
            ui->ramLiveView->setText("");
            removeAll();
            limpiarRam();
            QString original;


            QStringList ints; //listo
            QStringList chars;//listo
            QStringList floats;//listo
            QStringList doubles;//listo
            QStringList refs;
            QStringList prints;//listo
            QStringList strcs;//listo
            QStringList longs;//listo

            original = ui->editor->toPlainText();
            ui->stdout->setText("");
            qInfo() << original;
            depurLine = 0;

            if(validarComas(original) == true){ //validacion
                codigo = adapter(original);
                //se procede con validacion de tipos de datos
                if(validarTipos(codigo) == true){
                    qInfo()<< "Todo good";
                    creadorListas separador;
                    separador.organizar(codigo);//crea las sublistas

                    strcs = separador.get("{");
                    ints = separador.get("int");
                    chars = separador.get("char");
                    longs = separador.get("long");
                    floats = separador.get("float");
                    doubles = separador.get("double");
                    prints = separador.get("pr");
                    refs = separador.get("refsa");
                    /*
                    qInfo() << strcs;
                    qInfo() << ints;
                    qInfo() << chars;
                    qInfo() << longs;
                    qInfo() << floats;
                    qInfo() << doubles;
                    qInfo() << prints;
                    */
                    qInfo() << refs;

                    //validar refs completamente

                    //comienza depuración
                    ui->atras->setEnabled(true);
                    ui->delante->setEnabled(true);
                    ui->viendo->setEnabled(true);
                    ui->stop->setEnabled(true);

                    verCorriendo(depurLine);

                    //realizar operaciones
                    operaciones opr;
                    opr.realizarOperacionesInt(ints);
                    QStringList res;
                    opr.realizarOperacionesFloats(floats);
                    opr.realizarOperacionesLong(longs);
                    opr.realizarOperacionesDouble(doubles);
                    opr.realizarOperacionesChar(chars);
                    opr.realizarOperacionesReference(refs);

                    res << opr.getAll(); //se ponen los primeros resultados

                    //estructuras
                    qInfo() << "Mal Formato" << strcs;
                    strcs = cambioFormatoStructs(strcs);
                    qInfo() << "Buen formato: " << strcs;
                    creadorListas separador2;
                    separador2.organizar(strcs);
                    QStringList intsS = separador2.get("int");
                    QStringList charsS = separador2.get("char");
                    QStringList longsS = separador2.get("long");
                    QStringList floatsS = separador2.get("float");
                    QStringList doublesS = separador2.get("double");
                    QStringList printsS = separador2.get("pr");
                    QStringList refsS = separador2.get("refsa");

                    operacionesEstructs opr2;
                    opr2.setResults(res);

                    opr2.realizarOperacionesInt(intsS);
                    opr2.realizarOperacionesFloats(floatsS);
                    opr2.realizarOperacionesLong(longsS);
                    opr2.realizarOperacionesDouble(doublesS);
                    opr2.realizarOperacionesChar(charsS);
                    opr2.realizarOperacionesReference(refsS);

                    res << opr2.getAll();

                    qInfo() << res;

                    if(hayNulos(res) == false){
                        res = quitaEspacios(res);
                        string starting_s = "initial";
                        char *message_i = &starting_s[0];
                        send(sock , message_i , strlen(message_i) , 0 );
                        JSON_Adapter(res);//se prepara el JSON
                        qInfo() << "Termina envio...";

                        string line;
                          ifstream myfile ("/home/sebas/Escritorio/P1.15/C-IDE/untitled/info.txt");
                          QStringList listaRecibidos;
                          if (myfile.is_open())
                          {
                            while ( getline (myfile,line) )
                            {
                              qInfo() << QString::fromStdString(line);
                              listaRecibidos << QString::fromStdString(line);
                            }
                            myfile.close();
                            ofstream file;
                            file.open("/home/sebas/Escritorio/P1.15/C-IDE/untitled/info.txt");
                            file << "";
                            file.close();
                            recibidos = listaRecibidos;
                            ui->atras->setEnabled(false);
                          }

                          else {
                              ui->stdout->append(Log.mostrar(1, "No es posible leer las variables enviadas por SERVER-PORT-8080"));
                          }
                          string ending_s = "end";
                          char *message_end = &ending_s[0];
                          send(sock , message_end , strlen(message_end) , 0 );
                           // termina envio, comienza desempaquetacion

                        //realizar prints
                        prints = imprimir(prints, res);
                        int largoPrints = prints.size();
                        if(malosPrints == true){
                            imprimirMalas(2, 1, "Variables no encontradas en los prints");
                            malosPrints = false;
                        }
                        for(int l=0; l<largoPrints; l++){
                            ui->stdout->append(prints[l]);
                        }
                        //qInfo() << "RAM ORDENADA: " << ordenarRam(codigo, listaRecibidos);
                        //mostrarRam = ordenarRam(codigo, mostrarRam);
                        mostrarRam << listaRecibidos[0];

                        //proceder a utilizar el ram live view
                        //int largoRecibo = listaRecibidos.size();
                        QString curr = listaRecibidos[0];
                        QString name;
                        QString dir;
                        QString value;
                        QString ref;
                        documentPet = json_recieve(curr.toStdString());
                        name = documentPet["name"].GetString();
                        dir = documentPet["address"].GetString();
                        value = documentPet["value"].GetString();
                        ref = documentPet["count"].GetString();
                        ui->ramLiveView->append(dir + " | " + value + " | " + name + " | " + ref);
                        /*
                        for(int r = 0; r<largoRecibo; r++){
                            curr = listaRecibidos[r];
                            documentPet = json_recieve(curr.toStdString());
                            name = documentPet["name"].GetString();
                            dir = documentPet["address"].GetString();
                            value = documentPet["value"].GetString();
                            ref = documentPet["count"].GetString();
                            ui->ramLiveView->append(dir + " | " + value + " | " + name + " | " + ref);
                        }
                        */
                    }
                    else{
                        ui->log->append(Log.mostrar(2, "Debe revisar el valor de las variables"));
                        badLine = quitarRepetidos(badLine);
                        imprimirMalas(1, 1, "");
                        ui->viendo->setEnabled(false);
                        ui->atras->setEnabled(false);
                        ui->delante->setEnabled(false);
                        ui->stop->setEnabled(false);
                    }
                }
                else{
                    ui->log->append(Log.mostrar(2, "Debe revisar los tipos de datos"));
                    //QMessageBox::critical(this, "ERROR", "Debe revisar los tipos de datos...");
                    //ui->viendo->setEnabled(true);
                    badLine = quitarRepetidos(badLine);
                    qInfo() << "Sin repetidos" << badLine;
                    imprimirMalas(1, 1, "");
                }
            }
            else{
                //QMessageBox::critical(this, "ERROR", "Debe revisar los puntos y comas...");
                //ui->viendo->setEnabled(true);
                ui->log->append(Log.mostrar(2, "Debe revisar los puntos y comas"));
                badLine = quitarRepetidos(badLine);
                qInfo() << "Sin repetidos" << badLine;
                imprimirMalas(1, 1, "");
            }
            //int-long-char-float-double-{-reference<
        }
        catch(int e){
            ui->log->append(Log.mostrar(2, "Error de comunicación del socket PORT-8080"));
        }
    }
    else{
        ui->log->append(Log.mostrar(0, "Debe escribir código de C!"));
    }

}
/**
 * @brief ide::on_stop_clicked Función
 */
void ide::on_stop_clicked()
{
    ui->viendo->setText("");
    ui->atras->setEnabled(false);
    ui->delante->setEnabled(false);
    ui->viendo->setEnabled(false);
    ui->stop->setEnabled(false);
    ui->ramLiveView->setText("");
    depurLine = 0;
}
QString getText(QString str){
    QString curr;
    QString name;
    QString dir;
    QString value;
    QString ref;
    Document documentPet;

    documentPet = json_recieve(str.toStdString());
    name = documentPet["name"].GetString();
    dir = documentPet["address"].GetString();
    value = documentPet["value"].GetString();
    ref = documentPet["count"].GetString();
    return dir + " | " + value + " | " + name + " | " + ref;
}
void ide::on_atras_clicked()
{
    depurLine --;
    verCorriendo(depurLine);
    mostrarRam.removeAll(mostrarRam[depurLine+1]);
    int largo = mostrarRam.size();
    ui->ramLiveView->setText("");
    for(int i=0; i<largo; i++){
        ui->ramLiveView->append(getText(mostrarRam[i]));
    }
}

// :)
void ide::on_delante_clicked()
{
    depurLine ++;
    verCorriendo(depurLine);
    avanzarRam(getLine(depurLine));
    //mostrarRam = ordenarRam(codigo, mostrarRam);
    int largo = mostrarRam.size();
    ui->ramLiveView->setText("");
    for(int i=0; i<largo; i++){
        ui->ramLiveView->append(getText(mostrarRam[i]));
    }
}

void ide::on_clearBut_clicked()
{
    ui->log->setText("");
}
