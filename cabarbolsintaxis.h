#ifndef cabarbolsintaxis_cabecera
#define cabarbolsintaxis_cabecera
#include "token.cpp"
#include "nodoArbol.cpp"
void arbolexpression(token &,int &,int &,vector<string> &,nodoArbol *);
void arbolblock(token &,int &,int &,vector<string> &,nodoArbol *);
void arboldeclaration(token &,int &,int &,vector<string> &,nodoArbol *);
void arbolstatement(token &,int &,int &,vector<string> &,nodoArbol *);
void arbolvar_decl(token &,int &,int &,vector<string> &,nodoArbol *);
void arbolexpr_stmt(token &,int &,int &,vector<string> &,nodoArbol *);


#endif