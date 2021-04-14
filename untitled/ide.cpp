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

void ide::on_runBut_clicked()//basicamente esto es un adapter
{
    QString tmp;

    tmp = ui->editor->toPlainText();
    qInfo() << tmp;
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
}
