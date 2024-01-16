#ifndef cabarbolsintaxis_cabecera
#define cabarbolsintaxis_cabecera
#include "token.cpp"
void arbolexpression(token &,int &,int &,vector<string> &);
void arbolblock(token &,int &,int &,vector<string> &);
void arboldeclaration(token &,int &,int &,vector<string> &);
void arbolstatement(token &,int &,int &,vector<string> &);
void arbolvar_decl(token &,int &,int &,vector<string> &);
void arbolexpr_stmt(token &,int &,int &,vector<string> &);


#endif