#include "ide.h"
#include "ui_ide.h"
#include <QDebug>
#include <QMessageBox>

class objetoPruebas{
private:
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
        //validamos float
        for(int i=0; i<largo; i++){
            analizando = agrega(tmp[i], 5);
            qInfo() << analizando;
            if(analizando == "float " || analizando == "print("){
                contador++;
                qInfo() << "Sumando contador por float o print";
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
public:
    objetoPruebas(){

    }
    bool hacerPruebas(QStringList lista){

    }
};
