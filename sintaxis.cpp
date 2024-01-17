#ifndef sintaxis_cabecera
#define sintaxis_cabecera
#include <iostream>
#include <map>
#include "token.cpp"
#include "cabsintaxis.h"
using namespace std;
bool hayErrores;
decodificador inverso;

void match(token &tt,int &actual,int fin, int esperado,vector<string> &nombres){
    if(actual<=fin && tt.elementos[actual].token == esperado){
        actual++;
    }else{
        hayErrores = 1;
        cout<<"Error: Se esperaba "<<nombres[esperado]<<" pero se encontro "<<nombres[tt.elementos[actual].token]<<" "<<tt.elementos[actual].lexema<<"\n";
    }
}

void arguments(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.COMA){
        match(recorrido,actual,numTokens,inverso.COMA,nombres);
        expression(recorrido,actual,numTokens,nombres);
        arguments(recorrido,actual,numTokens,nombres);
    }
}

void arguments_opc(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        
        expression(recorrido,actual,numTokens,nombres);
        arguments(recorrido,actual,numTokens,nombres);
    }
}

void parameters_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.COMA){
        match(recorrido,actual,numTokens,inverso.COMA,nombres);
        match(recorrido,actual,numTokens,inverso.IDENTIFICADOR,nombres);
        parameters_2(recorrido,actual,numTokens,nombres);
    }
}

void parameters(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.IDENTIFICADOR){
        match(recorrido,actual,numTokens,inverso.IDENTIFICADOR,nombres);
        parameters_2(recorrido,actual,numTokens,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void parameters_opc(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.IDENTIFICADOR){
        parameters(recorrido,actual,numTokens,nombres);
    }
}

void function(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;
    if(esteToken == inverso.IDENTIFICADOR){
        match(recorrido,actual,numTokens,inverso.IDENTIFICADOR,nombres);
        match(recorrido,actual,numTokens,inverso.PARENTESISABIERTO,nombres);
        parameters_opc(recorrido,actual,numTokens,nombres);
        match(recorrido,actual,numTokens,inverso.PARENTESISCERRADO,nombres);
        block(recorrido,actual,numTokens,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void primary(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE){
        match(recorrido,actual,numTokens,inverso.TRUE,nombres);
    }else if(esteToken == inverso.FALSE){
        match(recorrido,actual,numTokens,inverso.FALSE,nombres);
    }else if(esteToken == inverso.NULO){
        match(recorrido,actual,numTokens,inverso.NULO,nombres);
    }else if(esteToken == inverso.NUMERO){
        match(recorrido,actual,numTokens,inverso.NUMERO,nombres);
    }else if(esteToken == inverso.CADENA){
        match(recorrido,actual,numTokens,inverso.CADENA,nombres);
    }else if(esteToken == inverso.IDENTIFICADOR){
        match(recorrido,actual,numTokens,inverso.IDENTIFICADOR,nombres);
    }else if(esteToken == inverso.PARENTESISABIERTO){
        match(recorrido,actual,numTokens,inverso.PARENTESISABIERTO,nombres);
        expression(recorrido,actual,numTokens,nombres);
        match(recorrido,actual,numTokens,inverso.PARENTESISCERRADO,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void call_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.PARENTESISABIERTO){
        match(recorrido,actual,numTokens,inverso.PARENTESISABIERTO,nombres);
        arguments_opc(recorrido,actual,numTokens,nombres);
        match(recorrido,actual,numTokens,inverso.PARENTESISCERRADO,nombres);
        call_2(recorrido,actual,numTokens,nombres);
    }
}

void call(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO){
        primary(recorrido,actual,numTokens,nombres);
        call_2(recorrido,actual,numTokens,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void unary(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.BANG){
        match(recorrido,actual,numTokens,inverso.BANG,nombres);
        unary(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.RESTA){
        match(recorrido,actual,numTokens,inverso.RESTA,nombres);
        unary(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO){
        call(recorrido,actual,numTokens,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void factor_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.DIVIDIR){
        match(recorrido,actual,numTokens,inverso.DIVIDIR,nombres);
        unary(recorrido,actual,numTokens,nombres);
        factor_2(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.MULTIPLICAR){
        match(recorrido,actual,numTokens,inverso.MULTIPLICAR,nombres);
        unary(recorrido,actual,numTokens,nombres);
        factor_2(recorrido,actual,numTokens,nombres);
    }
}

void factor(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        unary(recorrido,actual,numTokens,nombres);
        factor_2(recorrido,actual,numTokens,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void term_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.RESTA){
        match(recorrido,actual,numTokens,inverso.RESTA,nombres);
        factor(recorrido,actual,numTokens,nombres);
        term_2(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.SUMA){
        match(recorrido,actual,numTokens,inverso.SUMA,nombres);
        factor(recorrido,actual,numTokens,nombres);
        term_2(recorrido,actual,numTokens,nombres);
    }
}

void term(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        factor(recorrido,actual,numTokens,nombres);
        term_2(recorrido,actual,numTokens,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void comparisson_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.MAYOR){
        match(recorrido,actual,numTokens,inverso.MAYOR,nombres);
        term(recorrido,actual,numTokens,nombres);
        comparisson_2(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.MAYORIGUAL){
        match(recorrido,actual,numTokens,inverso.MAYORIGUAL,nombres);
        term(recorrido,actual,numTokens,nombres);
        comparisson_2(recorrido,actual,numTokens,nombres);
    }if(esteToken == inverso.MENOR){
        match(recorrido,actual,numTokens,inverso.MENOR,nombres);
        term(recorrido,actual,numTokens,nombres);
        comparisson_2(recorrido,actual,numTokens,nombres);
    }if(esteToken == inverso.MENORIGUAL){
        match(recorrido,actual,numTokens,inverso.MENORIGUAL,nombres);
        term(recorrido,actual,numTokens,nombres);
        comparisson_2(recorrido,actual,numTokens,nombres);
    }
}

void comparisson(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        term(recorrido,actual,numTokens,nombres);
        comparisson_2(recorrido,actual,numTokens,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void equality_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.NOIGUAL){
        match(recorrido,actual,numTokens,inverso.NOIGUAL,nombres);
        comparisson(recorrido,actual,numTokens,nombres);
        equality_2(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.IGUAL){
        match(recorrido,actual,numTokens,inverso.IGUAL,nombres);
        comparisson(recorrido,actual,numTokens,nombres);
        equality_2(recorrido,actual,numTokens,nombres);
    }
}

void equality(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        comparisson(recorrido,actual,numTokens,nombres);
        equality_2(recorrido,actual,numTokens,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void logic_and_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.AND){
        match(recorrido,actual,numTokens,inverso.AND,nombres);
        equality(recorrido,actual,numTokens,nombres);
        logic_and_2(recorrido,actual,numTokens,nombres);
    }
}

void logic_and(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        equality(recorrido,actual,numTokens,nombres);
        logic_and_2(recorrido,actual,numTokens,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void logic_or_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.OR){
        match(recorrido,actual,numTokens,inverso.OR,nombres);
        logic_and(recorrido,actual,numTokens,nombres);
        logic_or_2(recorrido,actual,numTokens,nombres);
    }
}

void logic_or(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        logic_and(recorrido,actual,numTokens,nombres);
        logic_or_2(recorrido,actual,numTokens,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void assignment_opc(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.IGUAL){
        match(recorrido,actual,numTokens,inverso.IGUAL,nombres);
        expression(recorrido,actual,numTokens,nombres);
    }
}

void assignment(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        logic_or(recorrido,actual,numTokens,nombres);
        assignment_opc(recorrido,actual,numTokens,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void expression(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        assignment(recorrido,actual,numTokens,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void block(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.LLAVEABIERTA){
        match(recorrido,actual,numTokens,inverso.LLAVEABIERTA,nombres);
        declaration(recorrido,actual,numTokens,nombres);
        match(recorrido,actual,numTokens,inverso.LLAVECERRADA,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void while_stmt(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.WHILE){
        match(recorrido,actual,numTokens,inverso.WHILE,nombres);
        match(recorrido,actual,numTokens,inverso.PARENTESISABIERTO,nombres);
        expression(recorrido,actual,numTokens,nombres);
        match(recorrido,actual,numTokens,inverso.PARENTESISCERRADO,nombres);
        statement(recorrido,actual,numTokens,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void return_exp_opc(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        expression(recorrido,actual,numTokens,nombres);
    }
}

void return_stmt(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.RETURN){
        match(recorrido,actual,numTokens,inverso.RETURN,nombres);
        return_exp_opc(recorrido,actual,numTokens,nombres);
        match(recorrido,actual,numTokens,inverso.PUNTOCOMA,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void print_stmt(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.PRINT){
        match(recorrido,actual,numTokens,inverso.PRINT,nombres);
        expression(recorrido,actual,numTokens,nombres);
        match(recorrido,actual,numTokens,inverso.PUNTOCOMA,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void else_statement(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.ELSE){
        match(recorrido,actual,numTokens,inverso.ELSE,nombres);
        statement(recorrido,actual,numTokens,nombres);
    }
}

void if_stmt(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.IF){
        match(recorrido,actual,numTokens,inverso.IF,nombres);
        match(recorrido,actual,numTokens,inverso.PARENTESISABIERTO,nombres);
        expression(recorrido,actual,numTokens,nombres);
        match(recorrido,actual,numTokens,inverso.PARENTESISCERRADO,nombres);
        statement(recorrido,actual,numTokens,nombres);
        else_statement(recorrido,actual,numTokens,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void for_stmt_3(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        expression(recorrido,actual,numTokens,nombres);
    }
}

void for_stmt_2(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        expression(recorrido,actual,numTokens,nombres);
        match(recorrido,actual,numTokens,inverso.PUNTOCOMA,nombres);
    }else if(esteToken == inverso.PUNTOCOMA){
        match(recorrido,actual,numTokens,inverso.PUNTOCOMA,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void for_stmt_1(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.VAR){
        var_decl(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO || esteToken == inverso.NUMERO
            || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR || esteToken == inverso.PARENTESISABIERTO
            || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        expr_stmt(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.PUNTOCOMA){
        match(recorrido,actual,numTokens,inverso.PUNTOCOMA,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}


void for_stmt(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;

    if(esteToken == inverso.FOR){
        match(recorrido,actual,numTokens,inverso.FOR,nombres);
        match(recorrido,actual,numTokens,inverso.PARENTESISABIERTO,nombres);
        for_stmt_1(recorrido,actual,numTokens,nombres);
        for_stmt_2(recorrido,actual,numTokens,nombres);
        for_stmt_3(recorrido,actual,numTokens,nombres);
        match(recorrido,actual,numTokens,inverso.PARENTESISCERRADO,nombres);
        statement(recorrido,actual,numTokens,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void expr_stmt(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;
    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO
        ||esteToken == inverso.NUMERO || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR
        || esteToken == inverso.PARENTESISABIERTO || esteToken == inverso.BANG || esteToken == inverso.RESTA){

        expression(recorrido,actual,numTokens,nombres);
        match(recorrido,actual,numTokens,inverso.PUNTOCOMA,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void statement(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    int esteToken = recorrido.elementos[actual].token;
    if(hayErrores)return;
    if(esteToken == inverso.TRUE || esteToken == inverso.FALSE || esteToken == inverso.NULO
        || esteToken == inverso.NUMERO || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR
        || esteToken == inverso.PARENTESISABIERTO || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        
        expr_stmt(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.FOR){
        for_stmt(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.IF){
        if_stmt(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.PRINT){
        print_stmt(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.RETURN){
        return_stmt(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.WHILE){
        while_stmt(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.LLAVEABIERTA){
        block(recorrido,actual,numTokens,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void var_init(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    int esteToken = recorrido.elementos[actual].token;
    if(hayErrores)return;
    if(esteToken == inverso.IGUAL){
        match(recorrido,actual,numTokens,inverso.IGUAL,nombres);
        expression(recorrido,actual,numTokens,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void var_decl(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    int esteToken = recorrido.elementos[actual].token;
    if(hayErrores)return;
    if(esteToken == inverso.VAR){
        match(recorrido,actual,numTokens,inverso.VAR,nombres);
        match(recorrido,actual,numTokens,inverso.IDENTIFICADOR,nombres);
        var_init(recorrido,actual,numTokens,nombres);
        match(recorrido,actual,numTokens,inverso.PUNTOCOMA,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void fun_decl(token &recorrido, int &actual, int &numTokens,vector<string> &nombres){
    int esteToken = recorrido.elementos[actual].token;
    if(hayErrores)return;
    if(esteToken == inverso.FUN){
        match(recorrido,actual,numTokens,inverso.FUN,nombres);
        function(recorrido,actual,numTokens,nombres);
    }else{
        hayErrores = 1;
        cout<<"Error: no se esperaba "<<nombres[recorrido.elementos[actual].token]<<" "<<recorrido.elementos[actual].lexema;
    }
}

void declaration(token &recorrido,int &actual,int &numTokens,vector<string> &nombres){
    if(hayErrores)return;
    int esteToken = recorrido.elementos[actual].token;
    if(esteToken == inverso.FUN){
        fun_decl(recorrido,actual,numTokens,nombres);
        declaration(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.VAR){
        var_decl(recorrido,actual,numTokens,nombres);
        declaration(recorrido,actual,numTokens,nombres);
    }else if(esteToken == inverso.IF || esteToken== inverso.FOR || esteToken == inverso.PRINT || esteToken == inverso.RETURN
            || esteToken == inverso.WHILE || esteToken == inverso.LLAVEABIERTA || esteToken == inverso.TRUE || esteToken == inverso.FALSE
            || esteToken == inverso.NULO || esteToken == inverso.NUMERO || esteToken == inverso.CADENA || esteToken == inverso.IDENTIFICADOR
            || esteToken == inverso.PARENTESISABIERTO || esteToken == inverso.BANG || esteToken == inverso.RESTA){
        statement(recorrido,actual,numTokens,nombres);
        declaration(recorrido,actual,numTokens,nombres);
    }
}
void program(token &recorrido,int &actual, int &numTokens, vector<string> &nombres){
    
    declaration(recorrido,actual,numTokens,nombres);
    
}

void parsear(token &recorrido,int actual,int numTokens, vector<string> &nombres){
    program(recorrido,actual,numTokens,nombres);
    if(actual>=numTokens && !hayErrores){
        cout<<"sintaxis Correcta\n";
    }else{
        hayErrores = 1;
        cout<<"Error se esperaba fin de archivo pero se encontro: \n"<<nombres[recorrido.elementos[actual].token];
    }
}


#endif