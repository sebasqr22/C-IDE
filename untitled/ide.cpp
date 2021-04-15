#include "ide.h"
#include "ui_ide.h"
#include <QDebug>
#include <QMessageBox>

using namespace std;
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
bool validarTipos(QStringList tmp){
    int largo = tmp.size();
    int contador = 0;
    QString analizando = "";

    //validamos primero las structuras
    for(int i=0; i<largo; i++){
        if(tmp[i][0] == "{"){
            contador++;
            qInfo() << "Sumando contador por {";
        }
    }
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
    //validamos float
    for(int i=0; i<largo; i++){
        analizando = agrega(tmp[i], 5);
        if(analizando == "float "){
            contador++;
            qInfo() << "Sumando contador por int";
        }
    }
    //validamos double
    for(int i=0; i<largo; i++){
        analizando = agrega(tmp[i], 6);
        if(analizando == "double "){
            contador++;
            qInfo() << "Sumando contador por int";
        }
    }
    //validamos reference<
    for(int i=0; i<largo; i++){
        analizando = agrega(tmp[i], 9);
        if(analizando == "reference<"){
            contador++;
            qInfo() << "Sumando contador por int";
        }
    }

    return contador==largo;
}

void ide::on_runBut_clicked()//basicamente esto es un adapter
{
    QString original;
    QStringList codigo;

    original = ui->editor->toPlainText();
    qInfo() << original;

    if(validarComas(original) == true){ //validacion
        codigo = adapter(original);
        //se procede con validacion de tipos de datos
        qInfo() << validarTipos(codigo);
    }
    else{
        QMessageBox::critical(this, "ERROR", "Debe revisar los puntos y comas...");
    }
    //int-long-char-float-double-{-reference<
}
