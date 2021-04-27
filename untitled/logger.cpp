#include <ctime>
#include <iostream>
#include <sstream>
#include <QString>

using namespace std;

class logger{
private:
    tm *tiempoLocaL;
    QString dia;
    QString mes;
    QString year;
    QString hora;
    QString minuto;
    QString segundo;

public:
    logger(){
        time_t actual = time(0);
        tiempoLocaL = localtime(&actual);
    }

    QString fecha(){
        dia = QString::number(tiempoLocaL->tm_mday);
        mes = QString::number(tiempoLocaL->tm_mon);
        year = QString::number(tiempoLocaL->tm_year);
        return dia + "//" + mes + "//" + year;
    }
    QString horaExacta(){
        hora = QString::number(tiempoLocaL->tm_hour);
        minuto = QString::number(tiempoLocaL->tm_min);
        segundo = QString::number(tiempoLocaL->tm_sec);
        return hora + ":" + minuto + ":" + segundo;
    }
    QString mostrar(int tipo, QString mensaje){
        switch(tipo){
        case 0:{
            return "---" + fecha() + " | " + horaExacta() + " ||| " + "[info] " + mensaje;
        }
        case 1:{
            return "---" + fecha() + " | " + horaExacta() + " ||| " + "[warning] " + mensaje;
        }
        case 2:{
            return "---" + fecha() + " | " + horaExacta() + " ||| " + "[error] " + mensaje;
        }
        }
    }
};
