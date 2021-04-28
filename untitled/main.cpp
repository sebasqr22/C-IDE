/**
  *@file main.cpp
  * @title main
  * @brief Clase principal que corre todas las funciones del ide
*/
#include "ide.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ide w;
    w.show();
    return a.exec();
}
