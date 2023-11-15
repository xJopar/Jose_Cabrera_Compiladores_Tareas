/*********** HEADER para analizador lexico ***********/
/*********** Librerias utilizadas **************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

/************* Definiciones ********************/
#define L_CORCHETE 	      2
#define R_CORCHETE	      4
#define L_LLAVE 	      6
#define R_LLAVE 	      8
#define COMA		      10
#define DOS_PUNTOS	      14
#define LITERAL_CADENA    128
#define LITERAL_NUM       129
#define PR_TRUE		      130
#define PR_FALSE	      131
#define PR_NULL           132
// Fin Codigos
#define TAMBUFF 		  5
#define TAMLEX 		      50
#define TAMHASH 	      101
#define ARCHIVO           "tokenStream.txt"

/************* Estructuras ********************/

typedef struct entrada{
	//definir los campos de 1 entrada de la tabla de simbolos
	int compLex;
	char lexema[TAMLEX];	
	struct entrada *tipoDato; 
	
} entrada;

typedef struct {
    int compLex;
	entrada *pe;
} token;

/************* Prototipos ********************/
void insertar(entrada e);
entrada* buscar(const char *clave);
void initTabla();
void initTablaSimbolos();
void sigLex();
