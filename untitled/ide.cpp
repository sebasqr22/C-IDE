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
    int contador = 0;

    tmp = ui->editor->toPlainText();
    qInfo() << tmp;
    tmp.remove("\n");
    qInfo() << tmp;
    cantidadPuntoComa = tmp.count(";");

    // Despues se crea el array con la info correspondiente



    QString arr[cantidadPuntoComa];
    QString tmp2 = "";

    qInfo() << "VAriables creadas";

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
}
