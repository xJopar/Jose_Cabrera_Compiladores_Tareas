/*********** Librerias utilizadas **************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

/************* Definiciones ********************/
#define L_CORCHETE 	      91
#define R_CORCHETE	      93
#define L_LLAVE 	      123
#define R_LLAVE 	      125
#define COMA		      44
#define DOS_PUNTOS	      58
#define LITERAL_CADENA    262
#define LITERAL_NUM  	  263
#define PR_TRUE		      264
#define PR_FALSE	      265
#define PR_NULL           266
#define TAMAUX            5
#define TAMMSG            41
#define TAMLEX            50
#define VACIO             1
#define TAMCONJ           10
#define TAMARRAY          500
#define TAB               4

/************* Estructuras ********************/
typedef struct {
	int compLex;
    char lexema[TAMLEX];
    int linea;
} token;

/************* Prototipos del analizador sintactico ********************/
void sigLex();
void parser();
void getToken();
void json(int setSync[]);
void match(int t);
void element();
void object(int setSync[]);
void objectP(int setSync[]);
void array(int setSync[]);
void arrayP(int setSync[]);
void elementList();
void elementListP(int setSync[]);
void atribute();
void atributeList();
void atributeListP(int setSync[]);
void atributeName();
void atributeValue(int setSync[]);
void scan(int setSync1[], int setSync2[]);
void checkInput(int conjPrimero[], int conjSig[]);


/******** Prototipos del traductor *************/
void jsonTrad();
void elementTrad();
void arrayTrad();
void arrayPTrad();
void elementListTrad();
void elementListPTrad();
void objectTrad();
void objectPTrad();
void attributeTrad();
void attributesListTrad();
void attributesListPTrad();
void attributeNameTrad();
void attributeValueTrad();
void stringSinComillas(char* cadena);
void insertEspacios();
