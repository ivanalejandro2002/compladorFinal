#ifndef arbol_cabecera
#define arbol_cabecera
#include <iostream>
#include <map>
#include "token.cpp"
#include "cabarbolsintaxis.h"
#include "nodoArbol.cpp"
using namespace std;
bool hayErrores;
decodificador inverso;
tipoSimbolo tradSimbolo;
void arbolmatch(token &tt,int &actual,int fin, int esperado,vector<string> &nombres){
    if(actual<=fin && tt.elementos[actual].token == esperado){
        actual++;
    }else{
        hayErrores = 1;
        cout<<"Error: Se esperaba "<<nombres[esperado]<<" pero se encontro "<<nombres[tt.elementos[actual].token]<<" "<<tt.elementos[actual].lexema<<"\n";
    }
}

void arbolarguments(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.COMA){
        arbolmatch(recorrido,actual,numTokens,inverso.COMA,nombres);
        arbolexpression(recorrido,actual,numTokens,nombres);
        arbolarguments(recorrido,actual,numTokens,nombres);
    }
}

void arbolarguments_opc(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        
        arbolexpression(recorrido,actual,numTokens,nombres);
        arbolarguments(recorrido,actual,numTokens,nombres);
    }
}

void arbolparameters_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.COMA){
        arbolmatch(recorrido,actual,numTokens,inverso.COMA,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.IDENTIFICADOR,nombres);
        arbolparameters_2(recorrido,actual,numTokens,nombres);
    }
}

void arbolparameters(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.IDENTIFICADOR){
        arbolmatch(recorrido,actual,numTokens,inverso.IDENTIFICADOR,nombres);
        arbolparameters_2(recorrido,actual,numTokens,nombres);
    }
}

void arbolparameters_opc(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.IDENTIFICADOR){
        arbolparameters(recorrido,actual,numTokens,nombres);
    }
}

void arbolfunction(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.IDENTIFICADOR){
        arbolmatch(recorrido,actual,numTokens,inverso.IDENTIFICADOR,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISABIERTO,nombres);
        arbolparameters_opc(recorrido,actual,numTokens,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISCERRADO,nombres);
        arbolblock(recorrido,actual,numTokens,nombres);
    }
}

void arbolprimary(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE){
        arbolmatch(recorrido,actual,numTokens,inverso.TRUE,nombres);
    }else if(esteToken == inverso.FALSE){
        arbolmatch(recorrido,actual,numTokens,inverso.FALSE,nombres);
    }else if(esteToken == inverso.NULO){
        arbolmatch(recorrido,actual,numTokens,inverso.NULO,nombres);
    }else if(esteToken == inverso.NUMERO){
        arbolmatch(recorrido,actual,numTokens,inverso.NUMERO,nombres);
    }else if(esteToken == inverso.CADENA){
        arbolmatch(recorrido,actual,numTokens,inverso.CADENA,nombres);
    }else if(esteToken == inverso.IDENTIFICADOR){
        arbolmatch(recorrido,actual,numTokens,inverso.IDENTIFICADOR,nombres);
    }else if(esteToken == inverso.PARENTESISABIERTO){
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISABIERTO,nombres);
        arbolexpression(recorrido,actual,numTokens,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISCERRADO,nombres);
    }
}

void arbolcall_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.PARENTESISABIERTO){
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISABIERTO,nombres);
        arbolarguments_opc(recorrido,actual,numTokens,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISCERRADO,nombres);
        arbolcall_2(recorrido,actual,numTokens,nombres);
    }
}

void arbolcall(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO){
        arbolprimary(recorrido,actual,numTokens,nombres);
        arbolcall_2(recorrido,actual,numTokens,nombres);
    }
}

void arbolunary(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.BANG){
        arbolmatch(recorrido,actual,numTokens,inverso.BANG,nombres);
        arbolunary(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.RESTA){
        arbolmatch(recorrido,actual,numTokens,inverso.RESTA,nombres);
        arbolunary(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO){
        arbolcall(recorrido,actual,numTokens,nombres);
    }
}

void arbolfactor_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.DIVIDIR){
        arbolmatch(recorrido,actual,numTokens,inverso.DIVIDIR,nombres);
        arbolunary(recorrido,actual,numTokens,nombres);
        arbolfactor_2(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.MULTIPLICAR){
        arbolmatch(recorrido,actual,numTokens,inverso.MULTIPLICAR,nombres);
        arbolunary(recorrido,actual,numTokens,nombres);
        arbolfactor_2(recorrido,actual,numTokens,nombres);
    }
}

void arbolfactor(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbolunary(recorrido,actual,numTokens,nombres);
        arbolfactor_2(recorrido,actual,numTokens,nombres);
    }
}

void arbolterm_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.RESTA){
        arbolmatch(recorrido,actual,numTokens,inverso.RESTA,nombres);
        arbolfactor(recorrido,actual,numTokens,nombres);
        arbolterm_2(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.SUMA){
        arbolmatch(recorrido,actual,numTokens,inverso.SUMA,nombres);
        arbolfactor(recorrido,actual,numTokens,nombres);
        arbolterm_2(recorrido,actual,numTokens,nombres);
    }
}

void arbolterm(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbolfactor(recorrido,actual,numTokens,nombres);
        arbolterm_2(recorrido,actual,numTokens,nombres);
    }
}

void arbolcomparisson_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.MAYOR){
        arbolmatch(recorrido,actual,numTokens,inverso.MAYOR,nombres);
        arbolterm(recorrido,actual,numTokens,nombres);
        arbolcomparisson_2(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.MAYORIGUAL){
        arbolmatch(recorrido,actual,numTokens,inverso.MAYORIGUAL,nombres);
        arbolterm(recorrido,actual,numTokens,nombres);
        arbolcomparisson_2(recorrido,actual,numTokens,nombres);
    }if(esteToken == inverso.MENOR){
        arbolmatch(recorrido,actual,numTokens,inverso.MENOR,nombres);
        arbolterm(recorrido,actual,numTokens,nombres);
        arbolcomparisson_2(recorrido,actual,numTokens,nombres);
    }if(esteToken == inverso.MENORIGUAL){
        arbolmatch(recorrido,actual,numTokens,inverso.MENORIGUAL,nombres);
        arbolterm(recorrido,actual,numTokens,nombres);
        arbolcomparisson_2(recorrido,actual,numTokens,nombres);
    }
}

void arbolcomparisson(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbolterm(recorrido,actual,numTokens,nombres);
        arbolcomparisson_2(recorrido,actual,numTokens,nombres);
    }
}

void arbolequality_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.NOIGUAL){
        arbolmatch(recorrido,actual,numTokens,inverso.NOIGUAL,nombres);
        arbolcomparisson(recorrido,actual,numTokens,nombres);
        arbolequality_2(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.IGUALCOMP){
        arbolmatch(recorrido,actual,numTokens,inverso.IGUALCOMP,nombres);
        arbolcomparisson(recorrido,actual,numTokens,nombres);
        arbolequality_2(recorrido,actual,numTokens,nombres);
    }
}

void arbolequality(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbolcomparisson(recorrido,actual,numTokens,nombres);
        arbolequality_2(recorrido,actual,numTokens,nombres);
    }
}

void arbollogic_and_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.AND){
        arbolmatch(recorrido,actual,numTokens,inverso.AND,nombres);
        arbolequality(recorrido,actual,numTokens,nombres);
        arbollogic_and_2(recorrido,actual,numTokens,nombres);
    }
}

void arbollogic_and(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbolequality(recorrido,actual,numTokens,nombres);
        arbollogic_and_2(recorrido,actual,numTokens,nombres);
    }
}

void arbollogic_or_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres,nodoArbol *nodo){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.OR){
        arbolmatch(recorrido,actual,numTokens,inverso.OR,nombres);
        nodoArbol *oreo = new nodoArbol(tradSimbolo.OR,0,1,nodo->funcion,nodo->padre);
        nodo->padre->hijos.push_back(nodo);
        oreo->hijos.push_back(nodo);
        nodoArbol *otro = new nodoArbol(tradSimbolo.EXPR,0,1,oreo->funcion,oreo);
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
        if(nodo->token != tradSimbolo.VAR){
            cout<<"Error: La asignación solo se puede hacer a variables\n";
            hayErrores = 1;
        }
        nodoArbol *igualar = new nodoArbol(tradSimbolo.IGUAL,0,1,nodo->padre->funcion,nodo->padre);
        nodo->padre->hijos[nodo->padre->hijos.size()-1] = igualar;
        igualar->hijos.push_back(nodo);
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
        nodoArbol *acciones = new nodoArbol(tradSimbolo.PROG,0,1,nodo->padre->funcion,nodo);
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
        nodoArbol *segundo = new nodoArbol(tradSimbolo.PROG,1,0,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(segundo);
        arbolfor_stmt_2(recorrido,actual,numTokens,nombres,segundo);
        nodoArbol *tercero = new nodoArbol(tradSimbolo.EXPR,1,0,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(tercero);
        arbolfor_stmt_3(recorrido,actual,numTokens,nombres,tercero);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISCERRADO,nombres);
        nodoArbol *funciones = new nodoArbol(tradSimbolo.PROG,1,0,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(funciones);
        arbolstatement(recorrido,actual,numTokens,nombres,funcion);
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
        nodoArbol *impro = new nodoArbol(tradSimbolo.EXPR,1,0,nodo->padre->funcion,nodo);
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
        if(existencias[{nodo->funcion,nodo->nombre}]){
            hayErrores = 1;
            cout<<"Error: la variable : "<<nodo->nombre<<" ha sido declarada multiples veces en un espacio\n";
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
            return;
        }
        nodoArbol *nuevo = new nodoArbol(tradSimbolo.FUN,1,0,++cantfunciones,nodo);
        nodo->hijos.push_back(nuevo);
        arbolfun_decl(recorrido,actual,numTokens,nombres,nuevo);
        arboldeclaration(recorrido,actual,numTokens,nombres,nodo);
    }else if(esteToken == inverso.VAR){
        nodoArbol *nuevo = new nodoArbol(tradSimbolo.VAR,1,0,nodo->padre->funcion,nodo);
        nodo->hijos.push_back(nuevo);
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
    arboldeclaration(recorrido,actual,numTokens,nombres,nuevo);
    
}

nodoArbol *arbolPrograma;

void arbolparsear(token &recorrido,int actual,int numTokens, vector<string> &nombres){
    arbolPrograma = new nodoArbol(tradSimbolo.PROG,0,0,0,nullptr);
    nodoArbol *nodoPrograma =arbolPrograma;
    arbolprogram(recorrido,actual,numTokens,nombres,nodoPrograma);
    if(actual>=numTokens && !hayErrores){
        cout<<"sintaxis Correcta\n";
    }else{
        hayErrores = 1;
        cout<<"Error se esperaba fin de archivo pero se encontro: \n"<<nombres[recorrido.elementos[actual].token];
    }
}


#endif