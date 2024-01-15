#ifndef token_Cabecera
#define token_Cabecera
#include <iostream>
#include <vector>
using namespace std;

struct decodificador{
    int AND;
    int ELSE;
    int FALSE;
    int FOR;
    int FUN;
    int IF;
    int NULO;
    int OR;
    int PRINT;
    int RETURN;
    int TRUE;
    int VAR;
    int WHILE;
    int MENOR;
    int MENORIGUAL;
    int MAYOR;
    int MAYORIGUAL;
    int BANG;
    int NOIGUAL;
    int IGUAL;
    int IGUALASIG;
    int SUMA;
    int RESTA;
    int MULTIPLICAR;
    int DIVIDIR;
    int LLAVEABIERTA;
    int LLAVECERRADA;
    int PARENTESISABIERTO;
    int PARENTESISCERRADO;
    int COMA;
    int PUNTO;
    int PUNTOCOMA;
    int NUMERO;
    int IDENTIFICADOR;
    int CADENA;
    decodificador():AND(1),ELSE(2),FALSE(3),FOR(4),FUN(5),IF(6),NULO(7),OR(8),PRINT(9),RETURN(10),
                    TRUE(11),VAR(12),WHILE(13),MENOR(14),MENORIGUAL(15),MAYOR(16),MAYORIGUAL(17),
                    BANG(18),NOIGUAL(19),IGUAL(20),IGUALASIG(21),SUMA(22),RESTA(23),MULTIPLICAR(24),
                    DIVIDIR(25),LLAVEABIERTA(26),LLAVECERRADA(27),PARENTESISABIERTO(28),PARENTESISCERRADO(29),
                    COMA(30),PUNTO(31),PUNTOCOMA(32),NUMERO(33),IDENTIFICADOR(34),CADENA(35){};
};
struct nodo_Token{
    int token;
    string tipo_Token;
    string lexema;
};
struct token{
    int indice_inicial,indice_final;
    vector<nodo_Token> elementos;
    token():indice_inicial(0),indice_final(0){};
    void agrega(int &numero_Token,string &tipoToken,string &lexema){
        elementos.push_back({numero_Token,tipoToken,lexema});
        indice_final++;
    }
    void recorre(){
        for(indice_inicial;indice_inicial<indice_final;indice_inicial++){
            cout<<elementos[indice_inicial].tipo_Token<<","<<elementos[indice_inicial].token<<"::"<<elementos[indice_inicial].lexema<<"\n";
        }
    }
    void elimina(){
        elementos.pop_back();
        indice_final--;
    }
};
#endif