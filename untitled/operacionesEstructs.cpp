#include <QString>
#include <QStringList>
#include <QApplication>
#include <iostream>

class operacionesEstructs{
private:
    QString num1g;
    bool num2g;
    QString Stringp1;
    bool pp2;
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
    double operacionDouble(double num1, double num2, QString simb){
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
    long operacionLong(long num1, long num2, QString simb){
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
    QString separarDouble(QString str, QString simb){
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
        double resultado = operacionDouble(num1.toDouble(), num2.toDouble(), simb);
        return p1+QString::number(resultado);
    }
    QString separarLong(QString str, QString simb){
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
        long resultado = operacionLong(num1.toLong(), num2.toLong(), simb);
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
        int largoList = results.size();
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
            if(results[j].contains(p2)){
                int largoAux = results[j].size();
                bool listo;
                for(int y=0; y<largoAux; y++){
                    if(listo){
                        aux += results[j][y];
                    }
                    else if(results[j][y] == "="){
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
        QString h = QString::number(prueba);
        if(p2.contains(h)){//es un numero
            all << str;
        }
        else{//es una palabra
            all << separacionEspe(str);
        }

    }
    void verificarEspeLong(QString str){
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
        long prueba = p2.toLong(&ok, 10);
        QString h = QString::number(prueba);
        if(p2.contains(h)){//es un numero
            all << str;
        }
        else{//es una palabra
            all << separacionEspe(str);
        }

    }
    void verificarEspeDouble(QString str){
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
        double prueba = p2.toDouble(&ok);
        QString h = QString::number(prueba);
        if(p2.contains(h)){//es un numero
            all << str;
        }
        else{//es una palabra
            all << separacionEspe(str);
        }

    }
    void verificarEspeFloat(QString str){
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
        float prueba = p2.toFloat(&ok);
        QString h = QString::number(prueba);
        if(p2.contains(h)){//es un numero
            all << str;
        }
        else{//es una palabra
            all << separacionEspe(str);
        }

    }
    QString encontrar(QString str){
        int largo = results.size();
        QString val;
        num1g = QString::number(largo);

        for(int i=0; i<largo; i++){
            //num1g = "FUCKING PENE";
            if(results[i].contains(str) == true){
                Stringp1 = "SI ESTA ENTRANDO";
                bool despuesIgual = false;
                QString curr = results[i];
                int largoPos = curr.size();

                for(int j=0; j<largoPos; j++){
                    if(despuesIgual == true){
                        val += curr[j];
                    }
                    else if(curr[j] == "=" && despuesIgual == false){
                        despuesIgual = true;
                    }
                }
                //num1g = val;
                break;
            }
        }
        return val;
    }
    void verificaOperacion(QString str, QString opr){
        QString p1; //termino1
        QString p2; //termino2
        QString init;
        bool despuesIgual = false;
        bool segunda = false;
        int largo = str.size();

        for(int i=0; i<largo; i++){
            if(str[i] != opr && despuesIgual == true && segunda == false){
                p1 += str[i];
            }
            else if(str[i] != opr && despuesIgual == true && segunda == true){
                p2 += str[i];
            }
            else if(str[i] == opr){
                segunda = true;
            }
            else if(str[i] == "="){
                init += str[i];
                despuesIgual = true;
            }
            else if(despuesIgual == false){
                init += str[i];
            }
        }// tenemos las dos partes dividias
        bool ok1;
        bool ok2;
        QString val1;
        QString val2;
        int p1I = p1.toInt(&ok1);
        int p2I = p2.toInt(&ok2);

        if(ok1 == false){
            val1 = encontrar(p1);
            if(ok2 == false){
                val2 = encontrar(p2);
                all << separar(init+val1+opr+val2, opr);
            }
            else{
                all << separar(init+val1+opr+p2, opr);
            }
        }
        else{
            if(ok2 == false){
                val2 = encontrar(p2);
                all << separar(init+p1+opr+val2, opr);
            }
            else{
                all << separar(init+p1+opr+p2, opr);
            }
        }
    }
    void verificaOperacionFloat(QString str, QString opr){
        QString p1; //termino1
        QString p2; //termino2
        QString init;
        bool despuesIgual = false;
        bool segunda = false;
        int largo = str.size();

        for(int i=0; i<largo; i++){
            if(str[i] != opr && despuesIgual == true && segunda == false){
                p1 += str[i];
            }
            else if(str[i] != opr && despuesIgual == true && segunda == true){
                p2 += str[i];
            }
            else if(str[i] == opr){
                segunda = true;
            }
            else if(str[i] == "="){
                init += str[i];
                despuesIgual = true;
            }
            else if(despuesIgual == false){
                init += str[i];
            }
        }// tenemos las dos partes dividias
        bool ok1;
        bool ok2;
        QString val1;
        QString val2;
        int p1I = p1.toFloat(&ok1);
        int p2I = p2.toFloat(&ok2);

        if(ok1 == false){
            val1 = encontrar(p1);
            if(ok2 == false){
                val2 = encontrar(p2);
                all << separarFloats(init+val1+opr+val2, opr);
            }
            else{
                all << separarFloats(init+val1+opr+p2, opr);
            }
        }
        else{
            if(ok2 == false){
                val2 = encontrar(p2);
                all << separarFloats(init+p1+opr+val2, opr);
            }
            else{
                all << separarFloats(init+p1+opr+p2, opr);
            }
        }
    }
    void verificaOperacionLong(QString str, QString opr){
        QString p1; //termino1
        QString p2; //termino2
        QString init;
        bool despuesIgual = false;
        bool segunda = false;
        int largo = str.size();

        for(int i=0; i<largo; i++){
            if(str[i] != opr && despuesIgual == true && segunda == false){
                p1 += str[i];
            }
            else if(str[i] != opr && despuesIgual == true && segunda == true){
                p2 += str[i];
            }
            else if(str[i] == opr){
                segunda = true;
            }
            else if(str[i] == "="){
                init += str[i];
                despuesIgual = true;
            }
            else if(despuesIgual == false){
                init += str[i];
            }
        }// tenemos las dos partes dividias
        bool ok1;
        bool ok2;
        QString val1;
        QString val2;
        long p1I = p1.toLong(&ok1);
        long p2I = p2.toLong(&ok2);

        if(ok1 == false){
            val1 = encontrar(p1);
            if(ok2 == false){
                val2 = encontrar(p2);
                all << separar(init+val1+opr+val2, opr);
            }
            else{
                all << separar(init+val1+opr+p2, opr);
            }
        }
        else{
            if(ok2 == false){
                val2 = encontrar(p2);
                all << separar(init+p1+opr+val2, opr);
            }
            else{
                all << separar(init+p1+opr+p2, opr);
            }
        }
    }
    void verificaOperacionDouble(QString str, QString opr){
        QString p1; //termino1
        QString p2; //termino2
        QString init;
        bool despuesIgual = false;
        bool segunda = false;
        int largo = str.size();

        for(int i=0; i<largo; i++){
            if(str[i] != opr && despuesIgual == true && segunda == false){
                p1 += str[i];
            }
            else if(str[i] != opr && despuesIgual == true && segunda == true){
                p2 += str[i];
            }
            else if(str[i] == opr){
                segunda = true;
            }
            else if(str[i] == "="){
                init += str[i];
                despuesIgual = true;
            }
            else if(despuesIgual == false){
                init += str[i];
            }
        }// tenemos las dos partes dividias
        bool ok1;
        bool ok2;
        QString val1;
        QString val2;
        double p1I = p1.toDouble(&ok1);
        double p2I = p2.toDouble(&ok2);

        if(ok1 == false){
            val1 = encontrar(p1);
            if(ok2 == false){
                val2 = encontrar(p2);
                all << separarDouble(init+val1+opr+val2, opr);
            }
            else{
                all << separarDouble(init+val1+opr+p2, opr);
            }
        }
        else{
            if(ok2 == false){
                val2 = encontrar(p2);
                all << separarDouble(init+p1+opr+val2, opr);
            }
            else{
                all << separarDouble(init+p1+opr+p2, opr);
            }
        }
    }
    void econtrarChar(QString str){
        int largo = all.size();
        int largoStr = str.size();
        bool despuesIgual = false;
        QString p1;
        QString p2;

        //obtener p1 y p2
        for(int i=0; i<largoStr; i++){
            if(despuesIgual == true){
                p2 += str[i];
            }
            else if(str[i] == "="){
                p1 += str[i];
                despuesIgual = true;
            }
            else{
                p1 += str[i];
            }
        }
        //buscar en all
        for(int j=0; j<largo; j++){
            if(all[j].contains(p2)){
                bool listo = false;
                int largoAct = all[j].size();
                QString curr = all[j];
                QString p2A;
                for(int y=0; y<largoAct; y++){
                    if(listo == true){
                        p2A += curr[y];
                    }
                    else if(curr[y] == "="){
                        listo = true;
                    }
                }
                p2 = p2A;
                break;
            }
        }
        all << p1 + p2;
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
                verificaOperacion(curr, "+");
            }
            else if(curr.contains("-")){
                verificaOperacion(curr, "-");
            }
            else if(curr.contains("*")){
                verificaOperacion(curr, "*");
            }
            else if(curr.contains("/")){
                verificaOperacion(curr, "/");
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
                verificaOperacionFloat(curr, "+");
            }
            else if(curr.contains("-")){
                verificaOperacionFloat(curr, "-");
            }
            else if(curr.contains("*")){
                verificaOperacionFloat(curr, "*");
            }
            else if(curr.contains("/")){
                verificaOperacionFloat(curr, "/");
            }
            else{
                verificarEspeFloat(curr);
            }
        }
    }
    void realizarOperacionesLong(QStringList lista){
        int largo = lista.size();
        QString curr;
        QStringList vacia;
        //all = vacia; //se limpia

        for(int i=0; i<largo; i++){
            curr = lista[i];
            if(curr.contains("+")){
                verificaOperacionLong(curr, "+");
            }
            else if(curr.contains("-")){
                verificaOperacionLong(curr, "-");
            }
            else if(curr.contains("*")){
                verificaOperacionLong(curr, "*");
            }
            else if(curr.contains("/")){
                verificaOperacionLong(curr, "/");
            }
            else{
                verificarEspeLong(curr);
            }
        }
    }
    void realizarOperacionesDouble(QStringList lista){
        int largo = lista.size();
        QString curr;
        QStringList vacia;
        //all = vacia; //se limpia

        for(int i=0; i<largo; i++){
            curr = lista[i];
            if(curr.contains("+")){
                verificaOperacionDouble(curr, "+");
            }
            else if(curr.contains("-")){
                verificaOperacionDouble(curr, "-");
            }
            else if(curr.contains("*")){
                verificaOperacionDouble(curr, "*");
            }
            else if(curr.contains("/")){
                verificaOperacionDouble(curr, "/");
            }
            else{
                verificarEspeDouble(curr);
            }
        }
    }
    void realizarOperacionesChar(QStringList lista){
        int largo = lista.size();
        QString curr;

        for(int i=0; i<largo; i++){
            curr = lista[i];

            if(curr.contains('\"')){
                all << curr;
            }
            else{
                econtrarChar(curr);
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
    bool get2(){
        return pp2;
    }
};
