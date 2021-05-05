/**
  *@file operaciones.cpp
  * @title operaciones
  * @brief Clase que realiza las operaciones de todos los tipos de datos menos las estucturas
*/
#include <QString>
#include <QStringList>
#include <QApplication>
#include <iostream>

class operaciones{
private:
    QString num1g;
    bool num2g;
    QString Stringp1;
    bool pp2;
    QStringList all;
    QStringList results;
    /**
     * @brief operacionInt Realiza las operaciones para los ints
     * @param num1 Primer numero entero
     * @param num2 Segundo numero entero
     * @param simb Tipo de operacion a realizar
     * @return Resultado de la operacion
     */
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
    /**
     * @brief operacionDouble Realiza las operaciones para los doubles
     * @param num1 Primer numero entero
     * @param num2 Segundo numero entero
     * @param simb Tipo de operacion a realizar
     * @return Resultado de la operacion
     */
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
    /**
     * @brief operacionLong Realiza las operaciones para los longs
     * @param num1 Primer numero entero
     * @param num2 Segundo numero entero
     * @param simb Tipo de operacion a realizar
     * @return Resultado de la operacion
     */
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
    /**
     * @brief operacionFloat Realiza las operaciones para los floats
     * @param num1 Primer numero entero
     * @param num2 Segundo numero entero
     * @param simb Tipo de operacion a realizar
     * @return Resultado de la operacion
     */
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
    /**
     * @brief separar Metodo para separar las partes de una variable int
     * @param str Variable a separar
     * @param simb Operacion a realizar
     * @return Variable con resultado
     */
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
    /**
     * @brief separarDouble Metodo para separar las partes de una variable double
     * @param str Variable a separar
     * @param simb Operacion a realizar
     * @return Variable con resultado
     */
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
    /**
     * @brief separarLong Metodo para separar las partes de una variable long
     * @param str Variable a separar
     * @param simb Operacion a realizar
     * @return Variable con resultado
     */
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
    /**
     * @brief separarFloats Metodo para separar las partes de una variable floats
     * @param str Variable a separar
     * @param simb Operacion a realizar
     * @return Variable con resultado
     */
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
    /**
     * @brief separacionEspe metodo para encontrar el valor de una variable asignada de otro objeto
     * @param str Variable a encontrar el valor
     * @return variable con valor respectivo
     */
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
    /**
     * @brief verificarEspeInt Verificacione de las partes de una variable de tipo int
     * @param str Variable a analizar
     */
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
    /**
     * @brief verificarEspeLong Verificacione de las partes de una variable de tipo long
     * @param str Variable a analizar
     */
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
    /**
     * @brief verificarEspeDouble Verificacione de las partes de una variable de tipo double
     * @param str Variable a analizar
     */
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
    /**
     * @brief verificarEspeFloat Verificacione de las partes de una variable de tipo float
     * @param str Variable a analizar
     */
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
    /**
     * @brief encontrar Busca el valor de una variable en la lista all
     * @param str Variable a buscar
     * @return valor de la variable
     */
    QString encontrar(QString str){
        int largo = all.size();
        QString val;
        num1g = QString::number(largo);

        for(int i=0; i<largo; i++){
            if(all[i].contains(str) == true){
                Stringp1 = "SI ESTA ENTRANDO";
                bool despuesIgual = false;
                QString curr = all[i];
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
    /**
     * @brief verificaOperacion identifica operacion a realizar en el tipo de dato int
     * @param str Variable a anlizar
     * @param opr Operacion a realizar
     */
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
    /**
     * @brief verificaOperacionFloat identifica operacion a realizar en el tipo de dato float
     * @param str Variable a anlizar
     * @param opr Operacion a realizar
     */
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
    /**
     * @brief verificaOperacionLong identifica operacion a realizar en el tipo de dato long
     * @param str Variable a anlizar
     * @param opr Operacion a realizar
     */
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
    /**
     * @brief verificaOperacionDouble identifica operacion a realizar en el tipo de dato double
     * @param str Variable a anlizar
     * @param opr Operacion a realizar
     */
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
    /**
     * @brief econtrarChar Metodo para encontar una variable de tipo char
     * @param str Variable a encontar
     */
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
    /**
     * @brief realizarOperacionesInt realiza las operaciones de los tipos de datos int
     * @param lista Lista con las variables a analizar
     */
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
    /**
     * @brief realizarOperacionesFloats realiza las operaciones de los tipos de datos float
     * @param lista Lista con las variables a analizar
     */
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
    /**
     * @brief realizarOperacionesLong realiza las operaciones de los tipos de datos long
     * @param lista Lista con las variables a analizar
     */
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
    /**
     * @brief realizarOperacionesDouble realiza las operaciones de los tipos de datos double
     * @param lista Lista con las variables a analizar
     */
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
    /**
     * @brief realizarOperacionesChar realiza las operaciones de los tipos de datos char
     * @param lista Lista con las variables a analizar
     */
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
    /**
     * @brief Realizar calculos para reference
     * @param Lista con las variables
    **/
    void realizarOperacionesReference(QStringList lista){
        int largo = lista.size();
        QString curr;

        for(int i=0; i<largo; i++){
            curr = lista[i];
            curr.remove("reference<");
            curr.remove(">");
            curr.remove("&");
            curr.remove("int");
            curr.remove("long");
            curr.remove("double");
            curr.remove("float");
            curr.remove("char");
            curr.remove(" ");
            all << curr;
        }
    }
    /**
     * @brief setResults pone el valor a la QStringList "results"
     * @param lista Valor que se le asignara a "results"
     */
    void setResults(QStringList lista){
        results = lista;
    }
    /**
     * @brief getAll retorna todas las variables analizadas
     * @return variables analizadas
     */
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
