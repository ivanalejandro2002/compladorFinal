#ifndef arbolnodo_cabecera
#define arbolnodo_cabecera
#include <iostream>
#include <vector>
#include <map>
using namespace std;
int cantfunciones = 0;
struct nodoArbol{
    int token;
    int semantico;
    int funcion;
    bool isStatement;
    bool negativo;
    bool isExpression;
    bool nuevo;
    double valor;
    bool declarado;
    int es_entero;
    bool negado;
    string nombre;
    string vstring;
    vector<nodoArbol *> hijos;
    nodoArbol *padre;

    nodoArbol(int v,bool esstat,bool esexp,int idfuncion, nodoArbol *papa):token(v),isStatement(esstat),isExpression(esexp),hijos(vector<nodoArbol *>(0)),funcion(idfuncion),padre(papa),semantico(0),valor(0),nombre(""),nuevo(0),declarado(0),negado(0),negativo(1){}
};

struct datoMapa{
    int funcion;
    string nombre;
};
map<datoMapa,bool> existencias;

struct tipoSimbolo{
    int PROG;
    int FUN;
    int VAR;
    int ASIG;
    int SUMA;
    int RESTA;
    int MULT;
    int DIV;
    int AND;
    int OR;
    int PRINT;
    int RETURN;
    int EXPR;
    int MAYOR;
    int MENOR;
    int IGUAL;
    int DIFERENTE;
    int NOT;
    int DOT;
    int PARAM;
    int FOR;
    int IF;
    int WHILE;
    int CALL;
    tipoSimbolo():PROG(0),FUN(1),ASIG(2),SUMA(3),RESTA(4),MULT(5),DIV(6),AND(7),OR(8),PRINT(9),RETURN(10),EXPR(11),MAYOR(12),MENOR(13),IGUAL(14),DIFERENTE(15),NOT(16),DOT(17),PARAM(18),FOR(19),IF(20),WHILE(21),CALL(22){};
};

map<string,nodoArbol> funciones;
#endif