#include <QStringList>

class creadorListas{
private:
    QStringList ints; //listo
    QStringList chars;//listo
    QStringList floats;//listo
    QStringList doubles;//listo
    QStringList refs;
    QStringList prints;//listo
    QStringList strcs;//listo
    QStringList longs;//listo

public:
    creadorListas(){

    }
    void organizar(QStringList lista){
        int largo = lista.size();
        for(int i=0; i < largo; i++){
            QString act = lista[i];
            if(act[0] == "{"){
                strcs << act;
            }
            else if(act[0] == "i"){
                ints << act;
            }
            else if(act[0] == "c"){
                chars << act;
            }
            else if(act[0] == "l"){
                longs << act;
            }
            else if(act[0] == "f"){
                floats << act;
            }
            else if(act[0] == "d"){
                doubles << act;
            }
            else if(act[0] == "p"){
                prints << act;
            }
            else{
                refs << act;
            }
        }
    }
    QStringList get(QString name){
        if(name.contains("{")){
            return strcs;
        }
        else if(name.contains("int")){
            return ints;
        }
        else if(name.contains("char")){
            return chars;
        }
        else if(name.contains("long")){
            return longs;
        }
        else if(name.contains("float")){
            return floats;
        }
        else if(name.contains("double")){
            return doubles;
        }
        else if(name.contains("pr")){
            return prints;
        }
        else{
            return refs;
        }
    }
};
