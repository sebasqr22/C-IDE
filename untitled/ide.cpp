#include "ide.h"
#include "ui_ide.h"
#include <QDebug>
#include <QMessageBox>
#include "creadorListas.cpp"

using namespace std;

//variables globales
bool corriendo = false;
int depurLine = 0;
QStringList codigo;
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

    for(int i=0; i <= largo; i++){
        if(aux[i] == "\n"){
            if(aux[i-1] != ";"){
                if(aux[i-1] != "{" && aux[i-1] != "}" && aux[i-1] != "\n"){
                    good = false;
                    break;
                }
            }
        }
        else if(i==largo-1){
           if(aux[i]!= ";" && aux[i] != "}" && aux[i] != "\n"){
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
    //validamos int
    for(int i=0; i<largo; i++){
        analizando = agrega(tmp[i], 3);
        if(analizando == "int "){
            contador++;
            qInfo() << "Sumando contador por int";
        }
    }
    //validamos char y long
    for(int i=0; i<largo; i++){
        analizando = agrega(tmp[i], 4);
        if(analizando == "char " || analizando == "long "){
            contador++;
        }
    }
    //validamos float y print(
    for(int i=0; i<largo; i++){
        analizando = agrega(tmp[i], 5);
        if(analizando == "float " || analizando == "print("){
            contador++;
            qInfo() << "Sumando contador por float";
        }
    }
    //validamos double
    for(int i=0; i<largo; i++){
        analizando = agrega(tmp[i], 6);
        if(analizando == "double "){
            contador++;
            qInfo() << "Sumando contador por double";
        }
    }
    //validamos reference<
    for(int i=0; i<largo; i++){
        analizando = agrega(tmp[i], 9);
        if(analizando == "reference<"){
            contador++;
            qInfo() << "Sumando contador por reference";
        }
    }
    return contador;
}
bool validarTipos(QStringList tmp){
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
            qInfo() << contadorEstructura;
            goodStructs = contadorEstructura == largoEst;
        }
    }
    int aux = validaResto(tmp, largo, "");
    contador += aux;

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

QStringList imprimir(QStringList lista){
    int largo = lista.size();
    QString tmp;
    QStringList listaAux;
    for(int i=0; i<largo; i++){
        tmp = lista[i].remove("print(");
        tmp = lista[i].remove(")");
        listaAux << tmp;
    }
    for(int j=0; j<largo; j++){
        if(listaAux[j][0] == '\"'){
            listaAux[j].remove('\"');
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
void ide::on_runBut_clicked()//basicamente esto es un adapter
{
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

            prints = imprimir(prints);
            int largoPrints = prints.size();
            for(int l=0; l<largoPrints; l++){
                ui->stdout->append(prints[l]);
            }

            //comienza depuración
            ui->atras->setEnabled(true);
            ui->delante->setEnabled(true);
            ui->viendo->setEnabled(true);
            ui->stop->setEnabled(true);

            verCorriendo(depurLine);
        }
        else{
            QMessageBox::critical(this, "ERROR", "Debe revisar los tipos de datos...");
        }
    }
    else{
        QMessageBox::critical(this, "ERROR", "Debe revisar los puntos y comas...");
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
