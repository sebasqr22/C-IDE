#include <QString>
#include <QStringList>

#include <iostream>

class operaciones{
private:
    QString num1g, num2g;
    QStringList all;
    QStringList results;
    int operacionInt(int num1, int num2, QString simb){
        if(simb == "+"){
            return num1 + num2;
        }
        else if(simb == "-"){
            return num1 - num2;
        }
        else if(simb == "*"){
            return num1 * num2;
        }
        else{
            return num1 / num2;
        }

    }
    float operacionFloat(float num1, float num2, QString simb){
        if(simb == "+"){
            return num1 + num2;
        }
        else if(simb == "-"){
            return num1 - num2;
        }
        else if(simb == "*"){
            return num1 * num2;
        }
        else{
            return num1 / num2;
        }

    }

    QString separar(QString str, QString simb){
        int largo = str.size();
        QString p1;
        QString num1;
        QString num2;
        bool p1a = true;
        bool p2b = true;

        for(int i=0; i<largo; i++){
            if(str[i] != "=" && p1a){
                p1 += str[i];
            }
            else if(str[i] != "=" && str[i] != simb && p2b){
                num1 += str[i];
            }
            else if(str[i] != "=" && str[i] != simb){
                num2 += str[i];
            }
            else if(str[i] == simb){
                p2b = false;
            }
            else{
                p1 += str[i];
                p1a = false;
            }
        }
        int resultado = operacionInt(num1.toInt(), num2.toInt(), simb);
        return p1+QString::number(resultado);
    }
    QString separarFloats(QString str, QString simb){
        int largo = str.size();
        QString p1;
        QString num1;
        QString num2;
        bool p1a = true;
        bool p2b = true;

        for(int i=0; i<largo; i++){
            if(str[i] != "=" && p1a){
                p1 += str[i];
            }
            else if(str[i] != "=" && str[i] != simb && p2b){
                num1 += str[i];
            }
            else if(str[i] != "=" && str[i] != simb){
                num2 += str[i];
            }
            else if(str[i] == simb){
                p2b = false;
            }
            else{
                p1 += str[i];
                p1a = false;
            }
        }
        float resultado = operacionFloat(num1.toFloat(), num2.toFloat(), simb);
        return p1+QString::number(resultado);
    }
    QString separacionEspe(QString str){
        int largoList = all.size();
        int largo = str.size();
        QString aux;
        QString p1;
        QString p2;
        bool p1a = true;
        bool encontrado = false;

        for(int i=0; i<largo; i++){
            if(str[i] != "=" && p1a){
                p1 += str[i];
            }
            else if(p1a == false){
                p2 += str[i];
            }
            else{
                p1 += str[i];
                p1a = false;
            }
        }
        //tenemos la info separada, buscamos el equivalente
        for(int j=0; j<largoList; j++){
            if(all[j].contains(p2)){
                int largoAux = all[j].size();
                bool listo;
                for(int y=0; y<largoAux; y++){
                    if(listo){
                        aux += all[j][y];
                    }
                    else if(all[j][y] == "="){
                        listo = true;
                    }
                }
                break;
            }
        }
        return p1 + aux;
    }
    void verificarEspeInt(QString str){
        QStringList nums;
        QString p1;
        QString p2;
        nums << "0" << "1" << "2" << "3" << "4"<< "5"<< "6"<< "7"<< "8"<< "9";

        bool listo = false;
        int largo = str.size();

        for(int i=0; i<largo; i++){
             if(str[i] != "=" && listo== false){
                 p1 += str[i];
             }
             else if(str[i] != "=" && listo==true){
                 p2 += str[i];
             }
             else{
                 p1 += str[i];
                 listo = true;
             }
        }
        bool ok;
        int prueba = p2.toInt(&ok, 10);
        num1g = prueba;
        QString h = QString::number(prueba);
        if(p2.contains(h)){//es un numero
            all << str;
        }
        else{//es una palabra
            all << separacionEspe(str);
        }

    }

public:
    void realizarOperacionesInt(QStringList lista){
        int largo = lista.size();
        QString curr;
        QStringList vacia;
        //all = vacia; //se limpia

        for(int i=0; i<largo; i++){
            curr = lista[i];
            if(curr.contains("+")){
                all << separar(curr, "+");
            }
            else if(curr.contains("-")){
                all << separar(curr, "-");
            }
            else if(curr.contains("*")){
                all << separar(curr, "*");
            }
            else if(curr.contains("/")){
                all << separar(curr, "/");
            }
            else{
                verificarEspeInt(curr);
            }
        }
    }
    void realizarOperacionesFloats(QStringList lista){
        int largo = lista.size();
        QString curr;
        QStringList vacia;
        //all = vacia; //se limpia

        for(int i=0; i<largo; i++){
            curr = lista[i];
            if(curr.contains("+")){
                all << separarFloats(curr, "+");
            }
            else if(curr.contains("-")){
                all << separarFloats(curr, "-");
            }
            else if(curr.contains("*")){
                all << separarFloats(curr, "*");
            }
            else if(curr.contains("/")){
                all << separarFloats(curr, "/");
            }
            else{
                all << separacionEspe(curr);
            }
        }
    }
    void setResults(QStringList lista){
        results = lista;
    }
    QStringList getAll(){
        return all;
    }
    QString get1(){
        return num1g;
    }
    QString get2(){
        return num2g;
    }
};
