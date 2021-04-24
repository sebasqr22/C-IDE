#include "ide.h"
#include "ui_ide.h"
#include <QDebug>
#include <QMessageBox>
#include "creadorListas.cpp"
#include "operaciones.cpp"

using namespace std;

//variables globales
bool corriendo = false;
int depurLine = 0;
QStringList codigo;
QStringList badLine;
//
ide::ide(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ide)
{
    ui->setupUi(this);
}

ide::~ide()
{
    delete ui;
}

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
QString agrega(QString aux, int veces){
    QString todo = "";
    for(int i=0; i<=veces; i++){
        todo.append(aux[i]);
    }
    return todo;
}
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
void ide::verCorriendo(int pos){
    int largo = codigo.size();
    if(pos > largo-1){
        pos = 0;
    }
    else if(pos < 0){
        pos = largo-1;
    }
    ui->viendo->setText(codigo[pos]);
}
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
void ide::on_runBut_clicked()//basicamente esto es un adapter
{
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

            qInfo() << strcs;
            qInfo() << ints;
            qInfo() << chars;
            qInfo() << longs;
            qInfo() << floats;
            qInfo() << doubles;
            qInfo() << prints;
            qInfo() << refs;

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
            opr.realizarOperacionesFloats(doubles);
            opr.realizarOperacionesInt(longs);
            res << opr.getAll();
            qInfo() << res;

            //realizar prints
            prints = imprimir(prints, res);
            int largoPrints = prints.size();
            for(int l=0; l<largoPrints; l++){
                ui->stdout->append(prints[l]);
            }
        }
        else{
            QMessageBox::critical(this, "ERROR", "Debe revisar los tipos de datos...");
            ui->viendo->setEnabled(true);
            badLine = quitarRepetidos(badLine);
            qInfo() << "Sin repetidos" << badLine;
            imprimirMalas();
        }
    }
    else{
        QMessageBox::critical(this, "ERROR", "Debe revisar los puntos y comas...");
        ui->viendo->setEnabled(true);
        badLine = quitarRepetidos(badLine);
        qInfo() << "Sin repetidos" << badLine;
        imprimirMalas();
    }
    //int-long-char-float-double-{-reference<
}

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
