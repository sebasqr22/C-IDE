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
            if(act.contains("{")){
                strcs << act;
            }
            else if(act.contains("int")){
                ints << act;
            }
            else if(act.contains("char")){
                chars << act;
            }
            else if(act.contains("long")){
                longs << act;
            }
            else if(act.contains("float")){
                floats << act;
            }
            else if(act.contains("double")){
                doubles << act;
            }
            else if(act.contains("print")){
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
        else if(name.contains("print")){
            return prints;
        }
        else{
            return refs;
        }
    }
};
