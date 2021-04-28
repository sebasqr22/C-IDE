/**
  *@file logger.cpp
  * @title logger
  * @brief Clase que permite mostrar informacion al usuario
 */

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

    /**
     * @brief fecha Metodo que crea la fecha para el log
     * @return Fecha en formato nacional (Costa Rica)
     */
    QString fecha(){
        dia = QString::number(tiempoLocaL->tm_mday);
        mes = QString::number((tiempoLocaL->tm_mon)+1);
        year = QString::number((tiempoLocaL->tm_year)-100);
        return dia + "//" + mes + "//" + year;
    }
    /**
     * @brief horaExacta Metodo que da el tiempo exacto para el log
     * @return QString con la hora exacta
     */
    QString horaExacta(){
        hora = QString::number(tiempoLocaL->tm_hour);
        minuto = QString::number(tiempoLocaL->tm_min);
        segundo = QString::number(tiempoLocaL->tm_sec);
        return hora + ":" + minuto + ":" + segundo;
    }
    /**
     * @brief updateTiempo Metodo que vuelve a generar la variable "actual" para tener el tiempo exacto
     */
    void updateTiempo(){
        time_t actual = time(0);
        tiempoLocaL = localtime(&actual);
    }


public:
    /**
     * @brief logger Constrcutor de la clase logger
     */
    logger(){
        time_t actual = time(0);
        tiempoLocaL = localtime(&actual);
    }
    /**
     * @brief mostrar Muestra el log con la informacion requerida, usa los metodos anteriores que son de caracter privado
     * @param tipo Entero que corresponde al nivel de criticalidad, siendo 0 el mas bajo y 2 el mayor
     * @param mensaje QString que contiene el mensaje a mostrar el el logger del IDE
     * @return
     */
    QString mostrar(int tipo, QString mensaje){
        updateTiempo();
        switch(tipo){
        case 0:{
            return "---" + fecha() + "---" + horaExacta() + "---" + "[info] " + "---" + mensaje + "---";
        }
        case 1:{
            return "---" + fecha() + "---" + horaExacta() + "---" + "[warning] " + "---" + mensaje + "---";
        }
        case 2:{
            return "---" + fecha() + "---" + horaExacta() + "---" + "[error] " + "---" + mensaje + "---";
        }
        }
    }
};
