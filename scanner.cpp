#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cassert>
#include <fstream>
#include "TRIE.cpp"
#include "automata.cpp"
#include "token.cpp"
#include "sintaxis.cpp"
#include "arbol.cpp"
#include "ejecucion.cpp"
#include "ejecucion.cpp"
using namespace std;

int main(int argc,char *argv[]){
    nodo_TRIE *arbol = new nodo_TRIE();
    token registro_Tokens = token();
    nodo_TRIE *it = arbol;
    int cantidadTokens = 0;
    vector< string > reservadas = {"and","else","false","for","fun","if","null","or","print","return","true","var","while"};
    vector<string> simbolos = {"<","<=",">",">=", "!", "!=", "=", "==", "+","-","*","/","{","}","(",")",",",".",";"};
    vector<string> nombres = {"","and","else","false","for","fun","if","null","or","print","return","true","var","while","LT","LE","GT","GE","BANG","NE","EQ","EQD","SUM","SUB","MUL","DIV","LLAVEABIERTA","LLAVECERRADA","PARENTESISABIERTO","PARENTESISCERRADO","COMA","PUNTO","PUNTOCOMA"};
    nombres.push_back("NUMERO");
    nombres.push_back("IDENTIFICADOR");
    nombres.push_back("CADENA");

    arbol->inicializa(cantidadTokens , reservadas);
    arbol -> inicializa(cantidadTokens , simbolos);
    /*for(int i = 1; i < argc ; i++){
        cout << argv[i] << "\n";
    }*/
    arbol->pasito('/')->pasito('/')->trabado=1;
    arbol->pasito('/')->pasito('*')->trabado=2;
    arbol->pasito('*')->pasito('/')->trabado=3;
    cantidadTokens++;
    nodo_TRIE actual;
    //arbol -> saca_Tokens();
    bool existe_Error=0;
    bool comentariote = 0;
    ifstream fin;
    fin.open(argv[1]);
    if(argc>1){
        bool errores_general=0;
        string lecturas;
        while(getline(fin,lecturas)){
            existe_Error = evalua(lecturas,arbol,cantidadTokens,nombres,comentariote,registro_Tokens);
            errores_general|=existe_Error;
            if(existe_Error){
                cout<<"El programa tiene errores\n";
                return 0;
            };
            //registro_Tokens.recorre();
        }
        hayErrores = 0;
        parsear(registro_Tokens,registro_Tokens.indice_inicial,registro_Tokens.indice_final,nombres);
        arbolparsear(registro_Tokens,registro_Tokens.indice_inicial,registro_Tokens.indice_final,nombres);
        //arbolchecar(arbolPrograma);
        ejecutar();
        if(errores_general)cout<<"El programa termino con errores\n";
        if(hayErrores)return 0;
        
        else cout<<"El programa termino exitosamente\n";
    }else{
        string lectura;
        while(1){
            getline(cin,lectura);
            existe_Error = evalua(lectura,arbol,cantidadTokens,nombres,comentariote,registro_Tokens);
            if(existe_Error)continue;
            registro_Tokens.recorre();
        }
    }
}