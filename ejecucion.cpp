#ifndef materiales_cabecera
#define materiales_cabecera
#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include "materiales_ejecucion.cpp"
#include "nodoArbol.cpp"
#include "arbol.cpp"
struct valores{
    int es_entero;
    float valorEntero;
    string valorCadena;
    valores():es_entero(0),valorEntero(0),valorCadena(""){};
};
struct nodoPilaEjec{
    nodoArbol *nodo_actual;
    map<int,valores> parametros;
    map<string,int> tradParametros;
    nodoPilaEjec(nodoArbol *actual):nodo_actual(actual){};
};
map<string,valores> variablesPrograma;
stack<nodoPilaEjec> pilaPrograma;
bool termino;
int externo_esentero;
float externovalor;
string externostring;
void correr(nodoArbol *actual){
    //cout<<actual->token<<"\n";
    if(termino){
        //cout<<"/BT"<<actual->token<<"\n";
        return;
    }
    if(actual->token>=19 && actual->token<=22 || actual->token == 1){
        switch(actual->token){
            case 1:
                correr(actual->hijos[1]);
                return;
            case 19:
                if(actual->nuevo == 0){
                    correr(actual->hijos[0]);
                    actual->nuevo = 1;
                }
                correr(actual->hijos[1]);
                if(actual->hijos[1]->es_entero==1){
                    if(actual->hijos[1]->valor!=0){
                        correr(actual->hijos[3]);
                    }else return;
                    correr(actual->hijos[2]);
                    correr(actual);
                }else if(actual->hijos[1]->es_entero ==2){
                    if(actual->hijos[1]->vstring!=""){
                        correr(actual->hijos[3]);
                    }else return;
                    correr(actual->hijos[2]);
                    correr(actual);
                    return;
                }else{
                    correr(actual->hijos[3]);
                    correr(actual->hijos[2]);
                    correr(actual);
                }
                break;
            case 20:
                correr(actual->hijos[0]);
                if(actual->hijos[0]->es_entero){
                    if(actual->hijos[0]->valor!=0){
                        correr(actual->hijos[1]);
                    }else{
                        if(actual->hijos.size()>=3){
                            correr(actual->hijos[2]);
                        }
                    }
                }else if(actual->hijos[0]->vstring!=""){
                    correr(actual->hijos[1]);
                    if(actual->hijos.size()>=3){
                        correr(actual->hijos[2]);
                    }
                }else if(actual->hijos.size()>=3){
                    correr(actual->hijos[2]);
                }
                break;
            case 21:
                correr(actual->hijos[0]);
                if(actual->hijos[0]->es_entero){
                    if(actual->hijos[0]->valor!=0){
                        correr(actual->hijos[1]);
                        correr(actual);
                    }else return;
                }
                break;
            case 22:
                correr(actual->hijos[0]);
                pilaPrograma.push(actual);
                nodoArbol *paramReal = funciones[actual->nombre]->hijos[0];
                for(int i =0;i<actual->hijos[0]->hijos.size();i++){
                    //cout<<i<<";";
                    pilaPrograma.top().tradParametros[paramReal->hijos[i]->nombre] = i;
                    pilaPrograma.top().parametros[i].es_entero =actual->hijos[0]->hijos[i]->es_entero;
                    if(pilaPrograma.top().parametros[i].es_entero == 1){
                        pilaPrograma.top().parametros[i].valorEntero = actual->hijos[0]->hijos[i]->valor;
                    }else{
                        pilaPrograma.top().parametros[i].valorCadena = actual->hijos[0]->hijos[i]->vstring;
                    }
                }
                //cout<<actual->nombre<<"__";
                //cout<<funciones[actual->nombre]<<"\n";
                correr(funciones[actual->nombre]);
                //cout<<":)\n";
                //cout<<externo_esentero<<","<<externostring<<","<<externovalor<<"\n";
                actual->es_entero = externo_esentero;
                if(externo_esentero==1){
                    actual->valor = externovalor;
                }else{
                    actual->vstring = externostring;
                }
                termino = 0;
                pilaPrograma.pop();
                break;
        }
        //cout<<"BT\n";
        return;
    }
    for(int i=0;i<actual->hijos.size();i++){
        if(actual->hijos[i]->token==tradSimbolo.FUN)continue;
        correr(actual->hijos[i]);
        if(termino){
            //cout<<"-BT"<<actual->token<<"\n";
            return;
        }
    }
    switch(actual->token){
        case 2://ASIG
            //cout<<"Asignacion a variable: "<<actual->hijos[0]->nombre<<":: ";
            if(pilaPrograma.top().tradParametros.count(actual->hijos[0]->nombre)){
                //cout<<"Local:: ";
                actual->es_entero = actual->hijos[1]->es_entero;
                if(actual->hijos[1]->es_entero==1){
                    pilaPrograma.top().parametros[pilaPrograma.top().tradParametros[actual->hijos[0]->nombre]].es_entero=1;
                    pilaPrograma.top().parametros[pilaPrograma.top().tradParametros[actual->hijos[0]->nombre]].valorCadena="";
                    pilaPrograma.top().parametros[pilaPrograma.top().tradParametros[actual->hijos[0]->nombre]].valorEntero=actual->hijos[1]->valor;
                    //cout<<pilaPrograma.top().parametros[pilaPrograma.top().tradParametros[actual->hijos[0]->nombre]].valorEntero;
                }else{
                    pilaPrograma.top().parametros[pilaPrograma.top().tradParametros[actual->hijos[0]->nombre]].es_entero=2;
                    pilaPrograma.top().parametros[pilaPrograma.top().tradParametros[actual->hijos[0]->nombre]].valorEntero=0;
                    pilaPrograma.top().parametros[pilaPrograma.top().tradParametros[actual->hijos[0]->nombre]].valorCadena=actual->hijos[1]->vstring;
                    //cout<<pilaPrograma.top().parametros[pilaPrograma.top().tradParametros[actual->hijos[0]->nombre]].valorCadena;
                }
            }else{
                //cout<<"global:: ";
                variablesPrograma[actual->hijos[0]->nombre].es_entero = actual->hijos[1]->es_entero;
                if(actual->hijos[1]->es_entero==1){
                    variablesPrograma[actual->hijos[0]->nombre].valorEntero = actual->hijos[1]->valor;
                    //cout<<variablesPrograma[actual->hijos[0]->nombre].valorEntero;
                }else{
                    variablesPrograma[actual->hijos[0]->nombre].valorCadena = actual->hijos[1]->vstring;
                    //cout<<variablesPrograma[actual->hijos[0]->nombre].valorCadena;
                }
            }
            //cout<<"\n";
            break;
        case 3:
            if((actual->hijos[0]->es_entero==1 && actual->hijos[1]->es_entero==2 ) || (actual->hijos[0]->es_entero==2 && actual->hijos[1]->es_entero==1 )){
                cout<<"Error: No se puede hacer operacion suma entre numeros y cadenas\n";
                exit(0);
            }
            if(actual->hijos[0]->es_entero==1){
                actual->es_entero = 1;
                actual->valor = actual->hijos[0]->valor + actual->hijos[1]->valor;
            }else{
                actual->es_entero = 2;
                actual->vstring = actual->hijos[0]->vstring + actual->hijos[1]->vstring;
            }
            break;
        case 4:
            if(actual->hijos[1]->es_entero==2 || actual->hijos[0]->es_entero==2){
                cout<<"Error: No se puede hacer operacion resta a una cadena\n";
                exit(0);
            }
            if(actual->hijos[0]->es_entero==1){
                actual->es_entero = 1;
                actual->valor = actual->hijos[0]->valor - actual->hijos[1]->valor;
            }
            break;
        case 5:
            if(actual->hijos[1]->es_entero==2 || actual->hijos[0]->es_entero==2){
                cout<<"Error: No se puede hacer operacion producto a una cadena\n";
                exit(0);
            }
            if(actual->hijos[0]->es_entero==1){
                actual->es_entero = 1;
                actual->valor = actual->hijos[0]->valor * actual->hijos[1]->valor;
            }
            break;
        case 6:
            if(actual->hijos[1]->es_entero==2 || actual->hijos[0]->es_entero==2){
                cout<<"Error: No se puede hacer operacion division a una cadena\n";
                exit(0);
            }
            if(actual->hijos[0]->es_entero==1){
                actual->es_entero = 1;
                actual->valor = actual->hijos[0]->valor / actual->hijos[1]->valor;
            }
            break;
        case 7:
            if(actual->hijos[1]->es_entero==2 || actual->hijos[0]->es_entero==2){
                cout<<"Error: No se puede hacer operacion AND a una cadena\n";
                exit(0);
            }
            if(actual->hijos[0]->es_entero==1){
                actual->es_entero = 1;
                actual->valor = ((bool)actual->hijos[0]->valor & (bool)actual->hijos[1]->valor);
            }
            break;
        case 8:
            if(actual->hijos[1]->es_entero==2 || actual->hijos[0]->es_entero==2){
                cout<<"Error: No se puede hacer operacion OR a una cadena\n";
                exit(0);
            }
            if(actual->hijos[0]->es_entero==1){
                actual->es_entero = 1;
                actual->valor = ((bool)actual->hijos[0]->valor | (bool)actual->hijos[1]->valor);
            }
            break;
        case 9:
            if(actual->hijos[0]->es_entero==1){
                cout<<actual->hijos[0]->valor<<"\n";
            }else{
                cout<<actual->hijos[0]->vstring<<"\n";
            }
            break;
        case 10:
            termino = 1;
            externo_esentero = actual->hijos[0]->es_entero;
            if(actual->hijos[0]->es_entero==1){
                externovalor = actual->hijos[0]->valor;
            }else{
                externostring = actual->hijos[0]->vstring;
            }
            break;
        case 11:
            if(actual->hijos.size()==0)break;
            actual->es_entero = actual->hijos[0]->es_entero;
            if(actual->hijos[0]->es_entero==1){
                actual->valor = actual->hijos[0]->valor;
            }else{
                actual->vstring = actual->hijos[0]->vstring;
            }
            if(actual->negativo == -1){
                if(actual->es_entero == 2){
                    cout<<"Error: no se pueden hacer cadenas negativas\n";
                    exit(1);
                }
            }
            if(actual->negativo == -1)actual->valor*=-1;
            if(actual->negado)actual->valor = !((bool)actual->valor);
            if(actual->hijos[0]->token == tradSimbolo.VAR)actual->nombre = actual->hijos[0]->nombre;
            /*if(actual->es_entero==1)
            cout<<"Retorno de EXPR: "<<actual->valor<<"\n";
            else cout<<"Retorno de EXPR: "<<actual->vstring<<"\n";*/
            break;
        case 12:
            if((actual->hijos[0]->es_entero==1 && actual->hijos[1]->es_entero==2 ) || (actual->hijos[0]->es_entero==2 && actual->hijos[1]->es_entero==1 )){
                cout<<"Error: No se puede hacer operacion mayor entre numeros y cadenas\n";
                exit(0);
            }
            if(actual->hijos[0]->es_entero==1){
                actual->es_entero = 1;
                actual->valor = actual->hijos[0]->valor > actual->hijos[1]->valor;
            }else{
                actual->es_entero = 1;
                actual->valor = actual->hijos[0]->vstring > actual->hijos[1]->vstring;
            }
            break;
        case 13:
            if((actual->hijos[0]->es_entero==1 && actual->hijos[1]->es_entero==2 ) || (actual->hijos[0]->es_entero==2 && actual->hijos[1]->es_entero==1 )){
                cout<<"Error: No se puede hacer operacion booleana menor entre numeros y cadenas\n";
                exit(0);
            }
            if(actual->hijos[0]->es_entero==1){
                actual->es_entero = 1;
                actual->valor = actual->hijos[0]->valor < actual->hijos[1]->valor;
            }else{
                actual->es_entero = 1;
                actual->valor = actual->hijos[0]->vstring < actual->hijos[1]->vstring;
            }
            break;
        case 14:
            if((actual->hijos[0]->es_entero==1 && actual->hijos[1]->es_entero==2 ) || (actual->hijos[0]->es_entero==2 && actual->hijos[1]->es_entero==1 )){
                cout<<"Error: No se puede hacer operacion booleana igual entre numeros y cadenas\n";
                exit(0);
            }
            if(actual->hijos[0]->es_entero==1){
                actual->es_entero = 1;
                actual->valor = actual->hijos[0]->valor == actual->hijos[1]->valor;
            }else{
                actual->es_entero = 1;
                actual->valor = actual->hijos[0]->vstring == actual->hijos[1]->vstring;
            }
            break;
        case 15:
            if((actual->hijos[0]->es_entero==1 && actual->hijos[1]->es_entero==2 ) || (actual->hijos[0]->es_entero==2 && actual->hijos[1]->es_entero==1 )){
                cout<<"Error: No se puede hacer operacion booleana diferente entre numeros y cadenas\n";
                exit(0);
            }
            if(actual->hijos[0]->es_entero==1){
                actual->es_entero = 1;
                actual->valor = actual->hijos[0]->valor != actual->hijos[1]->valor;
            }else{
                actual->es_entero = 1;
                actual->valor = actual->hijos[0]->vstring != actual->hijos[1]->vstring;
            }
            break;
        case 16:
            actual->es_entero = 1;
            if(actual->hijos[0]->es_entero==1){
                if(actual->hijos[0]->valor==0)actual->valor=1;
                else actual->valor=0;
            }else{
                if(actual->hijos[0]->vstring=="")actual->valor=1;
                else actual->valor=0;
            }
            break;
        case 24:
            if(actual->nuevo){
                if(actual->funcion>0){
                    pilaPrograma.top().tradParametros[actual->nombre] = pilaPrograma.top().tradParametros.size();
                    pilaPrograma.top().parametros[pilaPrograma.top().tradParametros[actual->nombre]].es_entero=1;
                }else{
                    variablesPrograma[actual->nombre].es_entero = 1;
                }
            }else{
                //cout<<"Valor obtenido de variable: "<<actual->nombre<<":: ";
                if(pilaPrograma.top().tradParametros.count(actual->nombre)){
                    actual->es_entero = pilaPrograma.top().parametros[pilaPrograma.top().tradParametros[actual->nombre]].es_entero;
                    if(actual->es_entero==1){
                        actual->valor = pilaPrograma.top().parametros[pilaPrograma.top().tradParametros[actual->nombre]].valorEntero;
                        //cout<<"Numero:: "<<actual->valor<<"\n";
                    }else{
                        actual->vstring = pilaPrograma.top().parametros[pilaPrograma.top().tradParametros[actual->nombre]].valorCadena;
                        //cout<<"Cadena:: "<<actual->vstring<<"\n";
                    }
                }else{
                    actual->es_entero = variablesPrograma[actual->nombre].es_entero;
                    if(actual->es_entero == 1){
                        actual->valor = variablesPrograma[actual->nombre].valorEntero;
                        //cout<<actual->valor<<"\n";
                    }else{
                        actual->vstring = variablesPrograma[actual->nombre].valorCadena;
                        //cout<<actual->vstring<<"\n";
                    }
                }
            }
            break;
        case 25:
            if((actual->hijos[0]->es_entero==1 && actual->hijos[1]->es_entero==2 ) || (actual->hijos[0]->es_entero==2 && actual->hijos[1]->es_entero==1 )){
                cout<<"Error: No se puede hacer operacion mayor o igual entre numeros y cadenas\n";
                exit(0);
            }
            if(actual->hijos[0]->es_entero==1){
                actual->es_entero = 1;
                actual->valor = actual->hijos[0]->valor >= actual->hijos[1]->valor;
            }else{
                actual->es_entero = 1;
                actual->valor = actual->hijos[0]->vstring >= actual->hijos[1]->vstring;
            }
            break;
        case 26:
            if((actual->hijos[0]->es_entero==1 && actual->hijos[1]->es_entero==2 ) || (actual->hijos[0]->es_entero==2 && actual->hijos[1]->es_entero==1 )){
                cout<<"Error: No se puede hacer operacion menor o igual entre numeros y cadenas\n";
                exit(0);
            }
            if(actual->hijos[0]->es_entero==1){
                actual->es_entero = 1;
                actual->valor = actual->hijos[0]->valor <= actual->hijos[1]->valor;
            }else{
                actual->es_entero = 1;
                actual->valor = actual->hijos[0]->vstring <= actual->hijos[1]->vstring;
            }
            break;

    }
    //cout<<"BT\n";
}
void ejecutar(){
    pilaPrograma.push(arbolPrograma);
    correr(arbolPrograma);
}
#endif