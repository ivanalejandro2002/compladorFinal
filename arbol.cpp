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
    }else if(esteToken == inverso.IGUAL){
        arbolmatch(recorrido,actual,numTokens,inverso.IGUAL,nombres);
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

void arbollogic_or_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.OR){
        arbolmatch(recorrido,actual,numTokens,inverso.OR,nombres);
        arbollogic_and(recorrido,actual,numTokens,nombres);
        arbollogic_or_2(recorrido,actual,numTokens,nombres);
    }
}

void arbollogic_or(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbollogic_and(recorrido,actual,numTokens,nombres);
        arbollogic_or_2(recorrido,actual,numTokens,nombres);
    }
}

void arbolassignment_opc(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.IGUAL){
        arbolmatch(recorrido,actual,numTokens,inverso.IGUAL,nombres);
        arbolexpression(recorrido,actual,numTokens,nombres);
    }
}

void arbolassignment(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbollogic_or(recorrido,actual,numTokens,nombres);
        arbolassignment_opc(recorrido,actual,numTokens,nombres);
    }
}

void arbolexpression(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbolassignment(recorrido,actual,numTokens,nombres);
    }
}

void arbolblock(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.LLAVEABIERTA){
        arbolmatch(recorrido,actual,numTokens,inverso.LLAVEABIERTA,nombres);
        arboldeclaration(recorrido,actual,numTokens,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.LLAVECERRADA,nombres);
    }
}

void arbolwhile_stmt(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.WHILE){
        arbolmatch(recorrido,actual,numTokens,inverso.WHILE,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISABIERTO,nombres);
        arbolexpression(recorrido,actual,numTokens,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISCERRADO,nombres);
        arbolstatement(recorrido,actual,numTokens,nombres);
    }
}

void arbolreturn_exp_opc(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbolexpression(recorrido,actual,numTokens,nombres);
    }
}

void arbolreturn_stmt(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.RETURN){
        arbolmatch(recorrido,actual,numTokens,inverso.RETURN,nombres);
        arbolreturn_exp_opc(recorrido,actual,numTokens,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.PUNTOCOMA,nombres);
    }
}

void arbolprint_stmt(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.PRINT){
        arbolmatch(recorrido,actual,numTokens,inverso.PRINT,nombres);
        arbolexpression(recorrido,actual,numTokens,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.PUNTOCOMA,nombres);
    }
}

void arbolelse_statement(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.ELSE){
        arbolmatch(recorrido,actual,numTokens,inverso.ELSE,nombres);
        arbolstatement(recorrido,actual,numTokens,nombres);
    }
}

void arbolif_stmt(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.IF){
        arbolmatch(recorrido,actual,numTokens,inverso.IF,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISABIERTO,nombres);
        arbolexpression(recorrido,actual,numTokens,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISCERRADO,nombres);
        arbolstatement(recorrido,actual,numTokens,nombres);
        arbolelse_statement(recorrido,actual,numTokens,nombres);
    }
}

void arbolfor_stmt_3(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbolexpression(recorrido,actual,numTokens,nombres);
    }
}

void arbolfor_stmt_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbolexpression(recorrido,actual,numTokens,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.PUNTOCOMA,nombres);
    }else if(esteToken == inverso.PUNTOCOMA){
        arbolmatch(recorrido,actual,numTokens,inverso.PUNTOCOMA,nombres);
    }
}

void arbolfor_stmt_1(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.VAR){
        arbolvar_decl(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbolexpr_stmt(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.PUNTOCOMA){
        arbolmatch(recorrido,actual,numTokens,inverso.PUNTOCOMA,nombres);
    }
}


void arbolfor_stmt(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.FOR){
        arbolmatch(recorrido,actual,numTokens,inverso.FOR,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISABIERTO,nombres);
        arbolfor_stmt_1(recorrido,actual,numTokens,nombres);
        arbolfor_stmt_2(recorrido,actual,numTokens,nombres);
        arbolfor_stmt_3(recorrido,actual,numTokens,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.PARENTESISCERRADO,nombres);
        arbolstatement(recorrido,actual,numTokens,nombres);
    }
}

void arbolexpr_stmt(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;
    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO
        ||esteToken == inverso.NUMERO || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR
        || esteToken == inverso.PARENTESISABIERTO || esteToken == inverso.BANG || esteToken == inverso.RESTA){

        arbolexpression(recorrido,actual,numTokens,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.PUNTOCOMA,nombres);
    }
}

void arbolstatement(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    int esteToken = recorrido.elementos[actual].token;
    if(hayErrores)return;
    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO
        || esteToken == inverso.NUMERO || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR
        || esteToken == inverso.PARENTESISABIERTO || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        
        arbolexpr_stmt(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.FOR){
        arbolfor_stmt(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.IF){
        arbolif_stmt(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.PRINT){
        arbolprint_stmt(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.RETURN){
        arbolreturn_stmt(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.WHILE){
        arbolwhile_stmt(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.LLAVEABIERTA){
        arbolblock(recorrido,actual,numTokens,nombres);
    }
}

void var_init(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    int esteToken = recorrido.elementos[actual].token;
    if(hayErrores)return;
    if(esteToken == inverso.IGUAL){
        arbolmatch(recorrido,actual,numTokens,inverso.IGUAL,nombres);
        arbolexpression(recorrido,actual,numTokens,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void arbolvar_decl(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    int esteToken = recorrido.elementos[actual].token;
    if(hayErrores)return;
    if(esteToken == inverso.VAR){
        arbolmatch(recorrido,actual,numTokens,inverso.VAR,nombres);
        arbolmatch(recorrido,actual,numTokens,inverso.IDENTIFICADOR,nombres);
        arbolvar_init(recorrido,actual,numTokens,nombres);
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
        nodoArbol *nuevo = new nodoArbol(inverso.FUN,1,0,++cantfunciones,nodo);
        nodo->hijos.push_back(nuevo);
        arbolfun_decl(recorrido,actual,numTokens,nombres,nuevo);
        arboldeclaration(recorrido,actual,numTokens,nombres,nodo);
    }else if(esteToken == inverso.VAR){
        nodoArbol *nuevo = new nodoArbol(inverso.VAR,1,0,nodo->padre->funcion,nodo);
        nodo = nuevo;
        arbolvar_decl(recorrido,actual,numTokens,nombres);
        arboldeclaration(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.IF || esteToken== inverso.FOR || esteToken == inverso.PRINT || esteToken == inverso.RETURN
            || esteToken == inverso.WHILE || esteToken == inverso.LLAVEABIERTA || esteToken == inverso.TRUE || esteToken == inverso.FALSE
            || esteToken == inverso.NULO || esteToken == inverso.NUMERO || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR
            || esteToken == inverso.PARENTESISABIERTO || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        arbolstatement(recorrido,actual,numTokens,nombres);
        arboldeclaration(recorrido,actual,numTokens,nombres);
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