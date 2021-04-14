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
                good = false;
                break;
            }
        }
        else if(i==largo){
            if(aux[i] != ";"){
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

void ide::on_runBut_clicked()//basicamente esto es un adapter
{
    QString original;

    original = ui->editor->toPlainText();
    qInfo() << original;

    if(validarComas(original) == true){ //validacion
        QStringList codigo = adapter(original);
    }
    else{
        QMessageBox::critical(this, "ERROR", "Debe revisar los puntos y comas...");
    }
}
