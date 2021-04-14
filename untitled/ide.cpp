#include "ide.h"
#include "ui_ide.h"
#include <QDebug>

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

void ide::on_runBut_clicked()
{
    QString tmp;
    int cantidadPuntoComa;

    tmp = ui->editor->toPlainText();
    qInfo() << tmp;
    tmp.remove("\n");
    qInfo() << tmp;
    cantidadPuntoComa = tmp.count(";");

    QString temp2 = "";
    QStringList listaInfo;
    int largo = tmp.size();

    for(int i=0; i > largo; i++){
        if(tmp[i] == ";"){
            qInfo()<<"entrando al if";
            listaInfo << temp2;
            temp2 = "";
            qInfo()<<temp2;
        }
        else{
            qInfo()<<"ENtrando al else";
            temp2.append(tmp[i]);        }
            qInfo()<<temp2;
    }
    qInfo() << listaInfo;

    // Despues se crea el array con la info correspondiente



/*
    QString arr[cantidadPuntoComa];
    QString tmp2 = "";

    qInfo() << "Variables creadas";

    for(int i = 0; i == tmp.size(); i++){
        if(tmp[i] != ";"){
            tmp2 += tmp[i];
        }
        else{
            arr[contador] = tmp2;
            tmp2 = "";
            contador++;
        }
    }
    qInfo() << "Termino primer for";

    for(int a=0; a==cantidadPuntoComa; a++){
        qInfo() << "Posicion: " << a << "" << arr[a];
    }

    qInfo() << "Termino segundo for";
*/
}
