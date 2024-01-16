#ifndef arbolnodo_cabecera
#define arbolnodo_cabecera
#include <iostream>
#include <vector>
using namespace std;
int cantfunciones = 0;
struct nodoArbol{
    int token;
    int semantico;
    int funcion;
    bool isStatement;
    bool isExpression;
    double valor;
    string nombre;
    vector<nodoArbol *> hijos;
    nodoArbol *padre;

    nodoArbol(int v,bool esstat,bool esexp,int idfuncion, nodoArbol *papa):token(v),isStatement(esstat),isExpression(esexp),hijos(vector<nodoArbol *>(0)),funcion(idfuncion),padre(papa){}
};

#endif