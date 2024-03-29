#ifndef arbol_cabecera
#define arbol_cabecera
#include <iostream>
#include <map>
#include <math.h>
#include "token.cpp"
#include "cabarbolsintaxis.h"
#include "sintaxis.cpp"
#include "nodoArbol.cpp"
using namespace std;
void arbolmatch(token &tt,int &actual,int fin, int esperado,vector<string> &nombres){
    if(actual<=fin && tt.elementos[actual].token == esperado){
        actual++;
    }else{
        hayErrores = 1;
        cout<<"Error: Se esperaba "<<nombres[esperado]<<" pero se encontro "<<nombres[tt.elementos[actual].token]<<" "<<tt.elementos[actual].lexema<<"\n";
    }
}

void arbolarguments(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;
    if(esteToken == inverso.COMA){
        arbolmatch(recorrido,actual,numTokens,inverso.COMA,nombres);
        nodoArbol *avatar = new nodoArbol(tradSimbolo.EXPR,0,1,nodo->funcion,nodo);
        nodo->hijos.push_back(avatar);
        arbolexpression(recorrido,actual,numTokens,nombres,avatar);
        arbolarguments(recorrido,actual,numTokens,nombres,nodo);
    }
}

void arbolarguments_opc(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        nodoArbol *avatar = new nodoArbol(tradSimbolo.EXPR,0,1,nodo->funcion,nodo);
        nodo->hijos.push_back(avatar);
        arbolexpression(recorrido,actual,numTokens,nombres,avatar);
        arbolarguments(recorrido,actual,numTokens,nombres,nodo);
    }
}

void arbolparameters_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;
    if(esteToken == inverso.COMA){
        arbolmatch(recorrido,actual,numTokens,inverso.COMA,nombres);
        nodoArbol *avatar = new nodoArbol(tradSimbolo.ELEM,1,0,nodo->funcion,nodo);
        nodo->hijos.push_back(avatar);
        avatar->nombre = recorrido.elementos[actual].lexema;
        if(!existencias[{nodo->funcion,recorrido.elementos[actual].lexema}]){
            existencias[{nodo->funcion,recorrido.elementos[actual].lexema}] = 1;
        }else{
            cout<<"Error: el parametro: "<<recorrido.elementos[actual].lexema<<" aparece multiples veces en la funcion\n";
            hayErrores = 1;
        }
        arbolmatch(recorrido,actual,numTokens,inverso.IDENTIFICADOR,nombres);
        arbolparameters_2(recorrido,actual,numTokens,nombres,avatar);
    }
}

void arbolparameters(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.IDENTIFICADOR){
        nodoArbol *avatar = new nodoArbol(tradSimbolo.ELEM,1,0,nodo->funcion,nodo);
        nodo->hijos.push_back(avatar);
        avatar->nombre = recorrido.elementos[actual].lexema;
        if(!existencias[{nodo->funcion,recorrido.elementos[actual].lexema}]){
            existencias[{nodo->funcion,recorrido.elementos[actual].lexema}] = 1;
        }else{
            cout<<"Error: el parametro: "<<recorrido.elementos[actual].lexema<<" aparece multiples veces en la funcion\n";
            hayErrores = 1;
        }
        arbolmatch(recorrido,actual,numTokens,inverso.IDENTIFICADOR,nombres);
        arbolparameters_2(recorrido,actual,numTokens,nombres,nodo);
    }
}

void arbolparameters_opc(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.IDENTIFICADOR){
        arbolparameters(recorrido,actual,numTokens,nombres,nodo);
    }
}

void arbolfunction(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.IDENTIFICADOR){
        if(!funciones.count(recorrido.elementos[actual].lexema)){
            funciones[recorrido.elementos[actual].lexema] = nodo;
        }else{
            cout<<"Error: la funcion: "<<recorrido.elementos[actual].lexema<<" ha sido declarada multiples veces\n";
            hayErrores = 1;
        }
        nodo->nombre = recorrido.elementos[actual].lexema;
        arbolmatch(recorrido,actual,numTokens,inverso.IDENTIFICADOR,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISABIERTO,nombres);
        nodoArbol *parametros = new nodoArbol(tradSimbolo.PARAM,1,0,nodo->funcion,nodo);
        nodo->hijos.push_back(parametros);
        arbolparameters_opc(recorrido,actual,numTokens,nombres,parametros);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISCERRADO,nombres);
        nodoArbol *acciones = new nodoArbol(tradSimbolo.PROG,1,0,nodo->funcion,nodo);
        nodo->hijos.push_back(acciones);

        arbolblock(recorrido,actual,numTokens,nombres,acciones);
    }
}

void arbolprimary(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE){
        nodo->valor = 1;
        nodo->es_entero =1;
        arbolmatch(recorrido,actual,numTokens,inverso.TRUE,nombres);
    }else if(esteToken == inverso.FALSE){
        nodo->valor = 0;
        nodo-> es_entero = 1;
        arbolmatch(recorrido,actual,numTokens,inverso.FALSE,nombres);
    }else if(esteToken == inverso.NULO){
        nodo->valor = 0;
        nodo-> es_entero = 1;
        arbolmatch(recorrido,actual,numTokens,inverso.NULO,nombres);
    }else if(esteToken == inverso.NUMERO){
        nodo->es_entero = 1;
        int i = 0;
        string aux = recorrido.elementos[actual].lexema;
        float valor = 0;
        float decimal = 0;
        float indecimal = 1;
        float exponente = 0;
        int negativo = 1;
        while(i<aux.size() && aux[i]>='0' && aux[i]<='9'){
            valor*=10;
            valor+=aux[i]-'0';
            i++;
        }
        if(i<aux.size() && aux[i]=='.'){
            i++;
            while(i<aux.size() && aux[i]>='0' && aux[i]<='9'){
                indecimal/=10;
                decimal+=(aux[i]-'0')*indecimal;
                i++;
            }
        }
        if(i<aux.size() && aux[i]=='E'){
            i++;
            if(aux[i]=='-'){
                negativo*=-1;
                i++;
            }
            exponente = 0;
            while(i<aux.size() && aux[i]>='0' && aux[i]<='9'){
                exponente*=10;
                exponente+=aux[i]-'0';
                i++;
            }
        }
        nodo->valor = (valor+decimal)*pow(10,negativo*exponente);
        nodo->es_entero = 1;
        arbolmatch(recorrido,actual,numTokens,inverso.NUMERO,nombres);
    }else if(esteToken == inverso.CADENA){
        nodo->es_entero = 2;
        nodo->vstring = recorrido.elementos[actual].lexema;
        arbolmatch(recorrido,actual,numTokens,inverso.CADENA,nombres);
    }else if(esteToken == inverso.IDENTIFICADOR){
        nodo->token = tradSimbolo.VAR;
        datoMapa uno,dos;
        uno.funcion = 0;
        dos.funcion = nodo->funcion;
        uno.nombre = recorrido.elementos[actual].lexema;
        dos.nombre = recorrido.elementos[actual].lexema;
        if(!(existencias[uno] || existencias[dos])){
            cout<<"Error: La variable: "<<recorrido.elementos[actual].lexema<<" no ha sido definida\n";
            hayErrores=1;
        }
        nodo->nombre = recorrido.elementos[actual].lexema;
        arbolmatch(recorrido,actual,numTokens,inverso.IDENTIFICADOR,nombres);
    }else if(esteToken == inverso.PARENTESISABIERTO){
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISABIERTO,nombres);
        nodoArbol *nuevo = new nodoArbol(tradSimbolo.EXPR,0,1,nodo->funcion,nodo);
        nodo->hijos.push_back(nodo);
        arbolexpression(recorrido,actual,numTokens,nombres,nuevo);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISCERRADO,nombres);
    }
}

void arbolcall_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.PARENTESISABIERTO){
        if(nodo->token!=tradSimbolo.CALL && nodo->hijos.size()==0){
            cout<<"Error, los parametros solamente se pueden pasar a llamadas de funciones y a funciones\n";
            hayErrores = 1;
        }
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISABIERTO,nombres);
        nodoArbol *parametros = new nodoArbol(tradSimbolo.PARAM,1,0,nodo->funcion,nodo);
        nodo->hijos.push_back(parametros);
        arbolarguments_opc(recorrido,actual,numTokens,nombres,parametros);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISCERRADO,nombres);
        arbolcall_2(recorrido,actual,numTokens,nombres,nodo);
    }
}

void arbolcall(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;
    if(actual<numTokens &&esteToken == inverso.IDENTIFICADOR && recorrido.elementos[actual+1].token == inverso.PARENTESISABIERTO){
        if(funciones.count(recorrido.elementos[actual].lexema)==0){
            cout<<"Error: La funcion "<<recorrido.elementos[actual].lexema<<" no ha sido definida\n";
            hayErrores = 1;
        }
        nodoArbol *nuevo = new nodoArbol(tradSimbolo.CALL,1,0,nodo->funcion,nodo);
        nodo->hijos.push_back(nuevo);
        nuevo->nombre = recorrido.elementos[actual].lexema;
        arbolmatch(recorrido,actual,numTokens,inverso.IDENTIFICADOR,nombres);
        arbolcall_2(recorrido,actual,numTokens,nombres,nuevo);
        if(nuevo->hijos[0]->hijos.size()!=funciones[nuevo->nombre]->hijos[0]->hijos.size()){
            cout<<"Error, la funcion "<<nuevo->nombre<<" tiene "<<funciones[nuevo->nombre]->hijos[0]->hijos.size()<<" parametros, pero se llamo con "<<nuevo->hijos[0]->hijos.size()<<"\n";
            hayErrores = 1;
        }
    }else if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO){
        nodoArbol *nuevo = new nodoArbol(tradSimbolo.EXPR,0,1,nodo->funcion,nodo);
        nodo->hijos.push_back(nuevo);
        arbolprimary(recorrido,actual,numTokens,nombres,nuevo);
    }
}

void arbolunary(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.BANG){
        if(nodo->token!=tradSimbolo.EXPR){
            cout<<"Error: El operador ! solo puede ser usado con operadores logicos y variables\n";
            hayErrores = 1;
        }
        nodoArbol *nope = new nodoArbol(tradSimbolo.NOT,0,1,nodo->funcion,nodo);
        nodo->hijos.push_back(nope);
        arbolmatch(recorrido,actual,numTokens,inverso.BANG,nombres);
        arbolunary(recorrido,actual,numTokens,nombres,nope);
    }else if(esteToken == inverso.RESTA){
        cout<<nodo->token<<","<<nodo->padre->hijos[0]->nombre<<"\n";
        arbolmatch(recorrido,actual,numTokens,inverso.RESTA,nombres);
        nodo->negativo=-1;
        arbolunary(recorrido,actual,numTokens,nombres,nodo);
    }else if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO){
        arbolcall(recorrido,actual,numTokens,nombres,nodo);
    }
}

void arbolfactor_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.DIVIDIR){
        arbolmatch(recorrido,actual,numTokens,inverso.DIVIDIR,nombres);
        nodoArbol *diva = new nodoArbol(tradSimbolo.RESTA,0,1,nodo->funcion,nodo->padre);
        nodo->padre->hijos[nodo->padre->hijos.size()-1] = diva;
        diva->hijos.push_back(nodo);
        nodo->padre = diva;
        nodoArbol *otro = new nodoArbol(tradSimbolo.EXPR,0,1,diva->funcion,diva);
        diva->hijos.push_back(otro);
        arbolunary(recorrido,actual,numTokens,nombres,otro);
        arbolfactor_2(recorrido,actual,numTokens,nombres,otro);
    }else if(esteToken == inverso.MULTIPLICAR){
        nodoArbol *mula = new nodoArbol(tradSimbolo.MULT,0,1,nodo->funcion,nodo->padre);
        nodo->padre->hijos[nodo->padre->hijos.size()-1] = mula;
        mula->hijos.push_back(nodo);
        nodo->padre = mula;
        nodoArbol *otro = new nodoArbol(tradSimbolo.EXPR,0,1,mula->funcion,mula);
        mula->hijos.push_back(otro);
        arbolmatch(recorrido,actual,numTokens,inverso.MULTIPLICAR,nombres);
        arbolunary(recorrido,actual,numTokens,nombres,otro);
        arbolfactor_2(recorrido,actual,numTokens,nombres,otro);
    }
}

void arbolfactor(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbolunary(recorrido,actual,numTokens,nombres,nodo);
        arbolfactor_2(recorrido,actual,numTokens,nombres,nodo);
    }
}

void arbolterm_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.RESTA){
        while(nodo->padre->isExpression)nodo = nodo->padre;
        nodoArbol *rasta = new nodoArbol(tradSimbolo.RESTA,0,1,nodo->funcion,nodo->padre);
        nodo->padre->hijos[nodo->padre->hijos.size()-1] = rasta;
        rasta->hijos.push_back(nodo);
        nodo->padre = rasta;
        nodoArbol *otro = new nodoArbol(tradSimbolo.EXPR,0,1,rasta->funcion,rasta);
        rasta->hijos.push_back(otro);
        arbolmatch(recorrido,actual,numTokens,inverso.RESTA,nombres);
        arbolfactor(recorrido,actual,numTokens,nombres,otro);
        arbolterm_2(recorrido,actual,numTokens,nombres,otro);
    }else if(esteToken == inverso.SUMA){
        while(nodo->padre->isExpression)nodo = nodo->padre;
        nodoArbol *sumo = new nodoArbol(tradSimbolo.SUMA,0,1,nodo->funcion,nodo->padre);
        nodo->padre->hijos[nodo->padre->hijos.size()-1] = sumo;
        sumo->hijos.push_back(nodo);
        nodo->padre = sumo;
        nodoArbol *otro = new nodoArbol(tradSimbolo.EXPR,0,1,sumo->funcion,sumo);
        sumo->hijos.push_back(otro);
        arbolmatch(recorrido,actual,numTokens,inverso.SUMA,nombres);
        arbolfactor(recorrido,actual,numTokens,nombres,otro);
        arbolterm_2(recorrido,actual,numTokens,nombres,otro);
    }
}

void arbolterm(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbolfactor(recorrido,actual,numTokens,nombres,nodo);
        arbolterm_2(recorrido,actual,numTokens,nombres,nodo);
    }
}

void arbolcomparisson_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.MAYOR){
        arbolmatch(recorrido,actual,numTokens,inverso.MAYOR,nombres);
        nodoArbol *mayordomo = new nodoArbol(tradSimbolo.MAYOR,0,1,nodo->funcion,nodo->padre);
        nodo->padre->hijos[nodo->padre->hijos.size()-1] = mayordomo;
        mayordomo->hijos.push_back(nodo);
        nodo->padre = mayordomo;
        nodoArbol *otro = new nodoArbol(tradSimbolo.EXPR,0,1,mayordomo->funcion,mayordomo);
        mayordomo->hijos.push_back(otro);
        arbolterm(recorrido,actual,numTokens,nombres,otro);
        arbolcomparisson_2(recorrido,actual,numTokens,nombres,otro);
    }else if(esteToken == inverso.MAYORIGUAL){
        nodoArbol *comodo = new nodoArbol(tradSimbolo.MAYORIGUAL,0,1,nodo->funcion,nodo->padre);
        nodo->padre->hijos[nodo->padre->hijos.size()-1] = comodo;
        comodo->hijos.push_back(nodo);
        nodo->padre = comodo;
        nodoArbol *otro = new nodoArbol(tradSimbolo.EXPR,0,1,comodo->funcion,comodo);
        comodo->hijos.push_back(otro);
        arbolmatch(recorrido,actual,numTokens,inverso.MAYORIGUAL,nombres);
        arbolterm(recorrido,actual,numTokens,nombres,otro);
        arbolcomparisson_2(recorrido,actual,numTokens,nombres,otro);
    }if(esteToken == inverso.MENOR){
        nodoArbol *chamaco = new nodoArbol(tradSimbolo.MENOR,0,1,nodo->funcion,nodo->padre);
        nodo->padre->hijos[nodo->padre->hijos.size()-1] = chamaco;
        chamaco->hijos.push_back(nodo);
        nodo->padre = chamaco;
        nodoArbol *otro = new nodoArbol(tradSimbolo.EXPR,0,1,chamaco->funcion,chamaco);
        chamaco->hijos.push_back(otro);
        arbolmatch(recorrido,actual,numTokens,inverso.MENOR,nombres);
        arbolterm(recorrido,actual,numTokens,nombres,otro);
        arbolcomparisson_2(recorrido,actual,numTokens,nombres,otro);
    }if(esteToken == inverso.MENORIGUAL){
        nodoArbol *ligartija = new nodoArbol(tradSimbolo.MENORIGUAL,0,1,nodo->funcion,nodo->padre);
        nodo->padre->hijos[nodo->padre->hijos.size()-1] = ligartija;
        ligartija->hijos.push_back(nodo);
        nodo->padre = ligartija;
        nodoArbol *otro = new nodoArbol(tradSimbolo.EXPR,0,1,ligartija->funcion,ligartija);
        ligartija->hijos.push_back(otro);
        arbolmatch(recorrido,actual,numTokens,inverso.MENORIGUAL,nombres);
        arbolterm(recorrido,actual,numTokens,nombres,otro);
        arbolcomparisson_2(recorrido,actual,numTokens,nombres,otro);
    }
}

void arbolcomparisson(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbolterm(recorrido,actual,numTokens,nombres,nodo);
        arbolcomparisson_2(recorrido,actual,numTokens,nombres,nodo);
    }
}

void arbolequality_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.NOIGUAL){
        nodoArbol *nogal = new nodoArbol(tradSimbolo.DIFERENTE,0,1,nodo->funcion,nodo->padre);
        nodo->padre->hijos[nodo->padre->hijos.size()-1] = nogal;
        nogal->hijos.push_back(nodo);
        nodo->padre = nogal;
        nodoArbol *otro = new nodoArbol(tradSimbolo.EXPR,0,1,nogal->funcion,nogal);
        nogal->hijos.push_back(otro);
        arbolmatch(recorrido,actual,numTokens,inverso.NOIGUAL,nombres);
        arbolcomparisson(recorrido,actual,numTokens,nombres,otro);
        arbolequality_2(recorrido,actual,numTokens,nombres,otro);
    }else if(esteToken == inverso.IGUALCOMP){
        nodoArbol *iguanas = new nodoArbol(tradSimbolo.DIFERENTE,0,1,nodo->funcion,nodo->padre);
        nodo->padre->hijos[nodo->padre->hijos.size()-1] = iguanas;
        iguanas->hijos.push_back(nodo);
        nodo->padre = iguanas;
        nodoArbol *otro = new nodoArbol(tradSimbolo.EXPR,0,1,iguanas->funcion,iguanas);
        iguanas->hijos.push_back(otro);
        arbolmatch(recorrido,actual,numTokens,inverso.IGUALCOMP,nombres);
        arbolcomparisson(recorrido,actual,numTokens,nombres,otro);
        arbolequality_2(recorrido,actual,numTokens,nombres,otro);
    }
}

void arbolequality(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbolcomparisson(recorrido,actual,numTokens,nombres,nodo);
        arbolequality_2(recorrido,actual,numTokens,nombres,nodo);
    }
}

void arbollogic_and_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.AND){
        arbolmatch(recorrido,actual,numTokens,inverso.AND,nombres);
        while(nodo->padre->token!=tradSimbolo.IF && nodo->padre->token!=tradSimbolo.WHILE && nodo->padre->token!=tradSimbolo.FOR)nodo = nodo->padre;
        nodoArbol *andy = new nodoArbol(tradSimbolo.AND,0,1,nodo->funcion,nodo->padre);
        nodo->padre->hijos[nodo->padre->hijos.size()-1] = andy;
        andy->hijos.push_back(nodo);
        nodo->padre = andy;
        nodoArbol *otro = new nodoArbol(tradSimbolo.EXPR,0,1,andy->funcion,andy);
        andy->hijos.push_back(otro);
        arbolequality(recorrido,actual,numTokens,nombres,otro);
        arbollogic_and_2(recorrido,actual,numTokens,nombres,otro);
    }
}

void arbollogic_and(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbolequality(recorrido,actual,numTokens,nombres,nodo);
        arbollogic_and_2(recorrido,actual,numTokens,nombres,nodo);
    }
}

void arbollogic_or_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.OR){
        arbolmatch(recorrido,actual,numTokens,inverso.OR,nombres);
        while(nodo->padre->token!=tradSimbolo.IF && nodo->padre->token!=tradSimbolo.WHILE && nodo->padre->token!=tradSimbolo.FOR)nodo = nodo->padre;
        nodoArbol *oreo = new nodoArbol(tradSimbolo.OR,0,1,nodo->funcion,nodo->padre);
        nodo->padre->hijos[nodo->padre->hijos.size()-1] = oreo;
        oreo->hijos.push_back(nodo);
        nodo->padre = oreo;
        nodoArbol *otro = new nodoArbol(tradSimbolo.EXPR,0,1,oreo->funcion,oreo);
        oreo->hijos.push_back(otro);
        arbollogic_and(recorrido,actual,numTokens,nombres,otro);
        arbollogic_or_2(recorrido,actual,numTokens,nombres,otro);
    }
}

void arbollogic_or(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbollogic_and(recorrido,actual,numTokens,nombres,nodo);
        arbollogic_or_2(recorrido,actual,numTokens,nombres,nodo);
    }
}

void arbolassignment_opc(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.IGUAL){
        if(nodo->token != tradSimbolo.VAR && !(nodo->token==tradSimbolo.EXPR && nodo->hijos.size()==1 && nodo->hijos[0]->token==tradSimbolo.VAR)){
            cout<<"Error: La asignacion solo se puede hacer a variables\n";
            hayErrores = 1;
        }
        nodoArbol *igualar = new nodoArbol(tradSimbolo.ASIG,1,0,nodo->padre->funcion,nodo->padre);
        nodo->padre->hijos[nodo->padre->hijos.size()-1] = igualar;
        igualar->hijos.push_back(nodo);
        nodo->padre = igualar;
        arbolmatch(recorrido,actual,numTokens,inverso.IGUAL,nombres);
        nodoArbol *termino = new nodoArbol(tradSimbolo.EXPR,0,1,igualar->funcion,igualar);
        igualar->hijos.push_back(termino);
        arbolexpression(recorrido,actual,numTokens,nombres,termino);
    }
}

void arbolassignment(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbollogic_or(recorrido,actual,numTokens,nombres,nodo);
        arbolassignment_opc(recorrido,actual,numTokens,nombres,nodo);
    }
}

void arbolexpression(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbolassignment(recorrido,actual,numTokens,nombres,nodo);
    }
}

void arbolblock(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.LLAVEABIERTA){
        arbolmatch(recorrido,actual,numTokens,inverso.LLAVEABIERTA,nombres);
        arboldeclaration(recorrido,actual,numTokens,nombres,nodo);
        arbolmatch(recorrido,actual,numTokens,inverso.LLAVECERRADA,nombres);
    }
}

void arbolwhile_stmt(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.WHILE){
        arbolmatch(recorrido,actual,numTokens,inverso.WHILE,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISABIERTO,nombres);
        nodoArbol *condicion = new nodoArbol(tradSimbolo.EXPR,0,1,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(condicion);
        arbolexpression(recorrido,actual,numTokens,nombres,condicion);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISCERRADO,nombres);
        nodoArbol *acciones = new nodoArbol(tradSimbolo.PROG,1,0,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(acciones);
        arbolstatement(recorrido,actual,numTokens,nombres,acciones);
    }
}

void arbolreturn_exp_opc(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        nodoArbol *objeto = new nodoArbol(tradSimbolo.EXPR,0,1,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(objeto);
        arbolexpression(recorrido,actual,numTokens,nombres,objeto);
    }
}

void arbolreturn_stmt(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.RETURN){
        arbolmatch(recorrido,actual,numTokens,inverso.RETURN,nombres);
        arbolreturn_exp_opc(recorrido,actual,numTokens,nombres,nodo);
        arbolmatch(recorrido,actual,numTokens,inverso.PUNTOCOMA,nombres);
    }
}

void arbolprint_stmt(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.PRINT){
        arbolmatch(recorrido,actual,numTokens,inverso.PRINT,nombres);
        nodoArbol *cosa = new nodoArbol(tradSimbolo.EXPR,0,1,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(cosa);
        arbolexpression(recorrido,actual,numTokens,nombres,cosa);
        arbolmatch(recorrido,actual,numTokens,inverso.PUNTOCOMA,nombres);
    }
}

void arbolelse_statement(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.ELSE){
        arbolmatch(recorrido,actual,numTokens,inverso.ELSE,nombres);
        nodoArbol *sino = new nodoArbol(tradSimbolo.PROG,1,0,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(sino);
        arbolstatement(recorrido,actual,numTokens,nombres,sino);
    }
}

void arbolif_stmt(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.IF){
        arbolmatch(recorrido,actual,numTokens,inverso.IF,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISABIERTO,nombres);
        nodoArbol *condicional = new nodoArbol(tradSimbolo.EXPR,0,1,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(condicional);
        arbolexpression(recorrido,actual,numTokens,nombres,condicional);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISCERRADO,nombres);
        nodoArbol *acciones = new nodoArbol(tradSimbolo.PROG,1,0,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(acciones);
        arbolstatement(recorrido,actual,numTokens,nombres,acciones);
        arbolelse_statement(recorrido,actual,numTokens,nombres,nodo);
    }
}

void arbolfor_stmt_3(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        nodoArbol *acciones = new nodoArbol(tradSimbolo.EXPR,0,1,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(acciones);
        arbolexpression(recorrido,actual,numTokens,nombres,acciones);
    }
}

void arbolfor_stmt_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbolexpression(recorrido,actual,numTokens,nombres,nodo);
        arbolmatch(recorrido,actual,numTokens,inverso.PUNTOCOMA,nombres);
    }else if(esteToken == inverso.PUNTOCOMA){
        arbolmatch(recorrido,actual,numTokens,inverso.PUNTOCOMA,nombres);
    }
}

void arbolfor_stmt_1(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.VAR){
        nodoArbol *nuevo = new nodoArbol(tradSimbolo.VAR,1,0,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(nuevo);
        nuevo->nuevo = 1;
        arbolvar_decl(recorrido,actual,numTokens,nombres,nuevo);
    }else if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        
        arbolexpr_stmt(recorrido,actual,numTokens,nombres,nodo);
    }else if(esteToken == inverso.PUNTOCOMA){
        arbolmatch(recorrido,actual,numTokens,inverso.PUNTOCOMA,nombres);
    }
}


void arbolfor_stmt(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.FOR){
        arbolmatch(recorrido,actual,numTokens,inverso.FOR,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISABIERTO,nombres);
        nodoArbol *primero = new nodoArbol(tradSimbolo.PROG,1,0,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(primero);
        arbolfor_stmt_1(recorrido,actual,numTokens,nombres,primero);
        nodoArbol *segundo = new nodoArbol(tradSimbolo.EXPR,1,0,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(segundo);
        arbolfor_stmt_2(recorrido,actual,numTokens,nombres,segundo);
        nodoArbol *tercero = new nodoArbol(tradSimbolo.EXPR,1,0,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(tercero);
        arbolfor_stmt_3(recorrido,actual,numTokens,nombres,tercero);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISCERRADO,nombres);
        nodoArbol *funciones = new nodoArbol(tradSimbolo.PROG,1,0,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(funciones);
        arbolstatement(recorrido,actual,numTokens,nombres,funciones);
    }
}

void arbolexpr_stmt(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;
    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO
        ||esteToken == inverso.NUMERO || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR
        || esteToken == inverso.PARENTESISABIERTO || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        nodoArbol *nuevo = new nodoArbol(tradSimbolo.EXPR,0,1,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(nuevo);
        arbolexpression(recorrido,actual,numTokens,nombres,nuevo);
        arbolmatch(recorrido,actual,numTokens,inverso.PUNTOCOMA,nombres);
    }
}

void arbolstatement(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    int esteToken = recorrido.elementos[actual].token;
    if(hayErrores)return;
    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO
        || esteToken == inverso.NUMERO || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR
        || esteToken == inverso.PARENTESISABIERTO || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        
        arbolexpr_stmt(recorrido,actual,numTokens,nombres,nodo);
    }else if(esteToken == inverso.FOR){
        nodoArbol *foraneo = new nodoArbol(tradSimbolo.FOR,1,0,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(foraneo);
        arbolfor_stmt(recorrido,actual,numTokens,nombres,foraneo);
    }else if(esteToken == inverso.IF){
        nodoArbol *elife = new nodoArbol(tradSimbolo.IF,1,0,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(elife);
        arbolif_stmt(recorrido,actual,numTokens,nombres,elife);
    }else if(esteToken == inverso.PRINT){
        nodoArbol *impro = new nodoArbol(tradSimbolo.PRINT,1,0,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(impro);
        arbolprint_stmt(recorrido,actual,numTokens,nombres,impro);
    }else if(esteToken == inverso.RETURN){
        nodoArbol *regreso = new nodoArbol(tradSimbolo.RETURN,1,0,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(regreso);
        arbolreturn_stmt(recorrido,actual,numTokens,nombres,regreso);
    }else if(esteToken == inverso.WHILE){
        nodoArbol *william = new nodoArbol(tradSimbolo.WHILE,1,0,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(william);
        arbolwhile_stmt(recorrido,actual,numTokens,nombres,william);
    }else if(esteToken == inverso.LLAVEABIERTA){
        arbolblock(recorrido,actual,numTokens,nombres,nodo);
    }
}

void arbolvar_init(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    int esteToken = recorrido.elementos[actual].token;
    if(hayErrores)return;
    if(esteToken == inverso.IGUAL){
        nodoArbol *nuevo = new nodoArbol(tradSimbolo.ASIG,1,0,nodo->padre->funcion,nodo->padre);
        nodo->padre->hijos[nodo->padre->hijos.size()-1] = nuevo;
        nuevo->hijos.push_back(nodo);
        nodo->padre = nuevo;
        nuevo->declarado = 1;
        arbolmatch(recorrido,actual,numTokens,inverso.IGUAL,nombres);
        nodoArbol *asignacion = new nodoArbol(tradSimbolo.EXPR,0,1,nodo->padre->funcion,nuevo);
        nuevo->hijos.push_back(asignacion);
        arbolexpression(recorrido,actual,numTokens,nombres,asignacion);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void arbolvar_decl(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    int esteToken = recorrido.elementos[actual].token;
    if(hayErrores)return;
    if(esteToken == inverso.VAR){
        arbolmatch(recorrido,actual,numTokens,inverso.VAR,nombres);
        nodo->nombre = recorrido.elementos[actual].lexema;
        datoMapa ayuda;
        ayuda.funcion = nodo->funcion;
        ayuda.nombre = nodo->nombre;
        if(existencias[ayuda]){
            hayErrores = 1;
            cout<<"Error: la variable: "<<nodo->nombre<<" ha sido declarada multiples veces en un espacio\n";
        }else{
            existencias[ayuda]=1;
        }
        arbolmatch(recorrido,actual,numTokens,inverso.IDENTIFICADOR,nombres);
        arbolvar_init(recorrido,actual,numTokens,nombres,nodo);
        arbolmatch(recorrido,actual,numTokens,inverso.PUNTOCOMA,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void arbolfun_decl(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    int esteToken = recorrido.elementos[actual].token;
    if(hayErrores)return;
    if(esteToken == inverso.FUN){
        arbolmatch(recorrido,actual,numTokens,inverso.FUN,nombres);
        arbolfunction(recorrido,actual,numTokens,nombres,nodo);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void arboldeclaration(token &recorrido,int &actual,int &numTokens,vector<string> &nombres, nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;
    if(esteToken == inverso.FUN){
        if(nodo->funcion!=0){
            cout<<"Error: No se puede declarar una funcion dentro de otra\n";
            hayErrores=1;
        }
        nodoArbol *nuevo = new nodoArbol(tradSimbolo.FUN,1,0,++cantfunciones,nodo);
        nodo->hijos.push_back(nuevo);
        arbolfun_decl(recorrido,actual,numTokens,nombres,nuevo);
        arboldeclaration(recorrido,actual,numTokens,nombres,nodo);
    }else if(esteToken == inverso.VAR){
        nodoArbol *nuevo = new nodoArbol(tradSimbolo.VAR,1,0,nodo->padre->funcion,nodo);
        nodo->nuevo = 1;
        nodo->hijos.push_back(nuevo);
        nuevo->token = tradSimbolo.VAR;
        arbolvar_decl(recorrido,actual,numTokens,nombres,nuevo);
        arboldeclaration(recorrido,actual,numTokens,nombres,nodo);
    }else if(esteToken == inverso.IF || esteToken== inverso.FOR || esteToken == inverso.PRINT || esteToken == inverso.RETURN
            || esteToken == inverso.WHILE || esteToken == inverso.LLAVEABIERTA || esteToken == inverso.TRUE || esteToken == inverso.FALSE
            || esteToken == inverso.NULO || esteToken == inverso.NUMERO || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR
            || esteToken == inverso.PARENTESISABIERTO || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbolstatement(recorrido,actual,numTokens,nombres,nodo);
        arboldeclaration(recorrido,actual,numTokens,nombres,nodo);
    }
}

void arbolprogram(token &recorrido,int &actual, int &numTokens, vector<string> &nombres, nodoArbol *nodo){
    nodoArbol *nuevo = new nodoArbol(tradSimbolo.PROG,0,0,0,nodo);
    nodo->hijos.push_back(nuevo);
    arboldeclaration(recorrido,actual,numTokens,nombres,nuevo);
    
}
int z;
nodoArbol *arbolPrograma;
nodoArbol *realidad;
void arbolparsear(token &recorrido,int actual,int numTokens, vector<string> &nombres){
    arbolPrograma = new nodoArbol(tradSimbolo.PROG,0,0,0,nullptr);
    realidad = arbolPrograma;
    nodoArbol *nodoPrograma =arbolPrograma;
    arbolprogram(recorrido,actual,numTokens,nombres,nodoPrograma);
    if(actual>=numTokens && !hayErrores){
        //cout<<"sintaxis Correcta\n";
    }else{
        hayErrores = 1;
        cout<<"Error se esperaba fin de archivo pero se encontro: "<<nombres[recorrido.elementos[actual].token]<<"\n";
    }
}

void arbolchecar(nodoArbol *nodo){
    cout<<nodo->token<<","<<nodo->funcion<<","<<nodo->nombre<<", ["<<nodo->es_entero<<","<<nodo->negativo<<"]: "<<nodo->valor<<",'"<<nodo->vstring<<"'\n";
    for(int i=0;i<nodo->hijos.size();i++){
        arbolchecar(nodo->hijos[i]);
    }
    cout<<"BT\n";
}


#endif