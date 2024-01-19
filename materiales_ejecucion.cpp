/*#ifndef materiales_cabecera
#define materiales_cabecera
#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include "nodoArbol.cpp"
struct valores{
    int es_entero;
    int valorEntero;
    string valorCadena;
    valores():es_entero(0),valorEntero(0),valorCadena(""){};
};
struct nodoPilaEjec{
    nodoArbol *nodo_actual;
    vector<valores> parametros;
    map<string,int> tradParametros;
    nodoPilaEjec(nodoArbol *actual):nodo_actual(actual),parametros(vector<valores>(0)){};
};

#endif*/