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
#include "json.hpp"
#include "logger.cpp"
#include "operacionesEstructs.cpp"

using namespace std;
using json = nlohmann::json;

//variables globales
bool corriendo = false;
int depurLine = 0;
QStringList codigo;
QStringList badLine;
json j;
logger Log;

//Preparacion del socket
int sock = 0, valread;
struct sockaddr_in serv_addr;
int port = 8080;
char buffer[1024] = {0};

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
 * @brief ide::mensaje Envia un log al logger
 * @param criticalidad Nivel de error para el logger
 * @param msg Mensaje que mostrará el logger
 */
void ide::mensaje(int criticalidad, QString msg){
    ui->log->append(Log.mostrar(criticalidad, msg));
}
/**
 * @brief separadorJSON Permite obtener el valor y el nombre de las variables para crear el JSON
 * @param str Recibe un string de una variable
 * @return QStringList con nombre y valor de una variable
 */
QStringList separadorJSON(QString str){
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
        else if(curr.contains("struct")){
            type = "long"; //corregir despues
            memoria = 4;
        }
        else{
            type = "reference";
            memoria = 4;
            curr.remove("reference<");
            curr.remove(">");
            aux = separadorJSON(curr);
            name = aux[0];
            value = aux[1];
        }
        j[name.toStdString()] = {{"type", type.toStdString()}, {"value", value.toStdString()}, {"memory", memoria}};
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
    QString tmp;
    QStringList listaAux;
    for(int i=0; i<largo; i++){
         if(lista[i].contains('\"')){
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
    if(pos > largo-1){
        pos = 0;
        depurLine =0;
    }
    else if(pos < 0){
        pos = largo-1;
        depurLine = largo-1;
    }
    ui->viendo->setText(codigo[pos]);
}
/**
 * @brief ide::imprimirMalas Muestra las líneas en que se encuentra un error en el código
 */
void ide::imprimirMalas(){
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
 * @brief ide::on_runBut_clicked Función que al tocar el botón RUN, lee el código y ejecuta las demás funciones
 */
void ide::on_runBut_clicked()//basicamente esto es un adapter
{
    crearSocket(); //se crea el socket entre el server y el ide
    ui->viendo->setEnabled(false);
    removeAll();
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
            qInfo() << refs;
            */

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

            res << opr2.getAll();

            qInfo() << res;

            if(hayNulos(res) == false){
                res = quitaEspacios(res);
                JSON_Adapter(res);//se prepara el JSON
                string s = j.dump();
                qInfo() << QString::fromStdString(s);
                char *message = &s[0];
                send(sock , message , strlen(message) , 0 );//Se envia la info
                qInfo() << "ENVIADO";

                //se procede a recibir info
                valread = read(sock, buffer, 1024);
                qInfo() << "BUFFER: " << buffer;
                j = buffer;
                //j[name]["address"]


                //una ves se terminan los casos basicos, se procede con los structs
                //realizar prints
                prints = imprimir(prints, res);
                int largoPrints = prints.size();
                for(int l=0; l<largoPrints; l++){
                    ui->stdout->append(prints[l]);
                }
            }
            else{
                ui->log->append(Log.mostrar(2, "Debe revisar el valor de las variables"));
                badLine = quitarRepetidos(badLine);
                imprimirMalas();
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
            imprimirMalas();
        }
    }
    else{
        //QMessageBox::critical(this, "ERROR", "Debe revisar los puntos y comas...");
        //ui->viendo->setEnabled(true);
        ui->log->append(Log.mostrar(2, "Debe revisar los puntos y comas"));
        badLine = quitarRepetidos(badLine);
        qInfo() << "Sin repetidos" << badLine;
        imprimirMalas();
    }
    //int-long-char-float-double-{-reference<
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
    depurLine = 0;
}

void ide::on_atras_clicked()
{
    depurLine --;
    verCorriendo(depurLine);
}

void ide::on_delante_clicked()
{
    depurLine ++;
    verCorriendo(depurLine);
}

void ide::on_clearBut_clicked()
{
    ui->log->setText("");
}
