#include <QString>
#include <QStringList>

QStringList sumas;
QStringList restas;
QStringList divisiones;
QStringList multiplicaciones;
QStringList sinTipo;

QStringList identificar(QString str){
    int largo = str.size();
    QString tmp;
    QString num1;
    QString num2;

    for(int i=0; i<largo; i++){
        if(str[i] != "+" && str[i] != "-" && str[i] != "*" && str[i] != "/"){
            tmp += str[i];
        }
        else if(i == largo-1){
            tmp += str[i];
            num2 = tmp;
        }
        else{
            num1 = tmp;
            tmp = "";
        }
    }
    QStringList aux;
    aux << num1 << num2;

    return aux;
}
QStringList quitarIgual(QString str){
    int largo = str.size();
    QString tmp;
    QString tmp2;
    QStringList all;
    bool antes = true;

    for(int i=0; i<largo; i++){
        if(str[i] != "=" && antes){
            tmp += str[i];
        }
        else if(str[i] != "="){
            tmp2 += str[i];
        }
        else if(i == largo-1){
            tmp += str[i];
            break;
        }
        else{
            tmp += str[i];
            antes = false;
        }
    }
    all << tmp << tmp2;
    return all;
}

int completar(QString str, int operacion){
    QString curr = str;
    QStringList separados = identificar(str);
    int num1 = separados[0].toInt();
    int num2 = separados[1].toInt();

    switch (operacion) {
    case 0:{
        return num1 + num2;
    }
    case 1:{
        return num1 - num2;
    }
    case 2:{
        return num1 * num2;
    }
    case 3:{
        return num1 / num2;
    }
    }

}

void realizarOperaciones(QStringList lista){
    int largo = lista.size();

    //se procede a dejar en listas separadas
    for(int i=0; i<largo; i++){
        if(lista[i].contains("+")){
            sumas << lista[i];
        }
        else if(lista[i].contains("-")){
            restas << lista[i];
        }
        else if(lista[i].contains("*")){
            multiplicaciones << lista[i];
        }
        else if(lista.contains("/")){
            divisiones << lista[i];
        }
        else{
            sinTipo << lista[i];
        }
    }
    QStringList todo;
    for(int k=0; k<largo; k++){
        QString antesIgual;
        QString despuesIgual;
        QString result;

        antesIgual = quitarIgual(sumas[k])[0];
        despuesIgual = quitarIgual(sumas[k])[1];
        result = completar(despuesIgual, 0);
        todo << antesIgual + result;
    }
    //se procede a realizar las operaciones, 0:suma; 1:resta; 2:multiplicacion; 3:division

}
