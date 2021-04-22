#include <QString>
#include <QStringList>

class operaciones{
private:
    QStringList all;
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

public:
    void realizarOperaciones(QStringList lista){
        int largo = lista.size();
        QString curr;

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

            }
        }
    }
    QStringList getAll(){
        return all;
    }
};
