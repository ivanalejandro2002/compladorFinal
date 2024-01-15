#ifndef cabsintaxis_cabecera
#define cabsintaxis_cabecera
#include "token.cpp"
void expression(token &,int &,int &,vector<string> &);
void block(token &,int &,int &,vector<string> &);
void declaration(token &,int &,int &,vector<string> &);
void statement(token &,int &,int &,vector<string> &);
void var_decl(token &,int &,int &,vector<string> &);
void expr_stmt(token &,int &,int &,vector<string> &);


#endif