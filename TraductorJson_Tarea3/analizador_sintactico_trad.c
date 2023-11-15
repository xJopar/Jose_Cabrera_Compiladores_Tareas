/*
 *	Analizador Sintactico
 *	Curso: Compiladores y Lenguajes de Bajo de Nivel
 *	Practica de Programacion Nro. 2
 *	Nombre: Jose Maria Cabrera Peralta
 *  CI: 5553957
 *	Descripcion:
 *		Implementacion de un analizador sintactico para archivos JSON simplificado.
 */



//Analizador Lexico
#include "analizador_sintactico_trad.h"


token t;
FILE *archivo;
token arrayTokens[TAMARRAY];
int numLinea = 1;
int tamActual = 0;
char id[TAMLEX];
char msg1[TAMMSG];


/**************** Funciones **********************/
// Rutinas del analizador lexico
void error(const char* mensaje)
{
	printf("Lin %d: Error Lexico. %s.\n",numLinea,mensaje);
}

void sigLex()
{
    int i = 0;
    int ban = 0;
    int acepto = 0;
    int estado = 0;
    char c = 0;
    char msg[TAMMSG];
    char aux[TAMAUX] = " ";
    token e;

    while ((c = fgetc(archivo)) != EOF) 
    {
        
        if (c == ' ') 
        {
            // Ignorar espacios en blanco
            do 
            {
                c = fgetc(archivo);
            } while (c == ' ');
            c = ungetc(c, archivo);
        }
        else if (c == '\t') 
        {
            // Ignorar los tabuladores
            while (c == '\t') 
            {
                c = fgetc(archivo);
            }
        } 
        else if (c == '\n') {
            // incrementar numero de linea
            numLinea++;
            continue;
        } 
        else if (c == '\"')
        {
            // es un LITERAL_CADENA
            i = 0;
            id[i] = c;
            i++;
            do 
            {
                c = fgetc(archivo);
                if (c == '\"') 
                {
                    id[i] = c;
                    i++;
                    ban = 1;
                    break;
                } else if (c == EOF || c == ',' || c == '\n' || c == ':') 
                {
                    while (c != '\n') 
                    {
                        c = fgetc(archivo);
                        if (c == '\"') 
                        {
                            ban = 1;
                        }
                    }
                    if (ban == 0) 
                    {
                        sprintf(msg,"No se esperaba '%c', Se esperaba que finalize el literal",c);
                        error(msg);
                    }
                    ungetc(c, archivo);
                    break;
                } else 
                {
                    id[i] = c;
                    i++;
                }
            } while (isascii(c) || ban == 0);
            id[i] = '\0';
            strcpy(t.lexema, id);
            t.compLex = LITERAL_CADENA;
            t.linea = numLinea;
            arrayTokens[tamActual] = t;
            tamActual++;
            break;
        }
        else if (isdigit(c)) 
        {
            // es un numero
            i = 0;
            estado = 0;
            acepto = 0;
            id[i] = c;
            while (!acepto) 
            {
                switch (estado) 
                {
                    case 0: // Una secuencia netamente de digitos, puede ocurrir . o e
                        c = fgetc(archivo);
                        if (isdigit(c)) 
                        {
                            id[++i] = c;
                            estado = 0;
                        } else if (c == '.') 
                        {
                            id[++i] = c;
                            estado = 1;
                        } else if (tolower(c) == 'e') 
                        {
                            id[++i] = c;
                            estado = 3;
                        } else
                            estado = 6;
                        break;
                    case 1: // Un punto, debe seguir un digito. 
                        c = fgetc(archivo);
                        if (isdigit(c)) 
                        {
                            id[++i] = c;
                            estado = 2;
                        } else 
                        {
                            sprintf(msg,"No se esperaba '%c'", c);
                            estado = -1;
                        }
                        break;
                    case 2: // La fraccion decimal, pueden seguir los digitos o e.
                        c = fgetc(archivo);
                        if (isdigit(c)) 
                        {
                            id[++i] = c;
                            estado = 2;
                        } else if (tolower(c) == 'e') 
                        {
                            id[++i] = c;
                            estado = 3;
                        } else
                            estado = 6;
                        break;
                    case 3: // Una e, puede seguir +, - o una secuencia de digitos.
                        c = fgetc(archivo);
                        if (c == '+' || c == '-') 
                        {
                            id[++i] = c;
                            estado = 4;
                        } else if (isdigit(c)) 
                        {
                            id[++i] = c;
                            estado = 5;
                        } else 
                        {
                            sprintf(msg,"No se esperaba '%c'", c);
                            estado = -1;
                        }
                        break;
                    case 4:// Necesariamente debe venir por lo menos un digito.
                        c = fgetc(archivo);
                        if (isdigit(c)) 
                        {
                            id[++i] = c;
                            estado = 5;
                        } else 
                        {
                            sprintf(msg,"No se esperaba '%c'", c);
                            estado = -1;
                        }
                        break;
                    case 5:// Una secuencia de digitos correspondiente al exponente.
                        c = fgetc(archivo);
                        if (isdigit(c)) 
                        {
                            id[++i] = c;
                            estado = 5;
                        } else
                            estado = 6;
                        break;
                    case 6: // Estado de aceptacion, devolver el caracter correspondiente a otro componente lexico.
                        if (c != EOF)
                            ungetc(c, archivo);
                        else
                            c = 0;
                        id[++i] = '\0';
                        acepto = 1;
                        t.compLex = LITERAL_NUM;
                        t.linea = numLinea;
                        strcpy(t.lexema, id);
                        arrayTokens[tamActual] = t;
                        tamActual++;
                        break;
                    case -1:
                        if (c == EOF)
                            error("No se esperaba el fin de archivo.\n");
                        else
                            error(msg);
                        acepto = 1;
                        t.compLex = VACIO;
                        t.linea=numLinea;
                        ungetc(c, archivo);
                        break;
                }
            }
            break;
        } else if (c == ':')
        {
            t.compLex = DOS_PUNTOS;
            t.linea = numLinea;
            strcpy(t.lexema, ":");
            arrayTokens[tamActual] = t;
            tamActual++;
            break;
        } else if (c == ',') 
        {
            t.compLex = COMA;
            t.linea = numLinea;
            strcpy(t.lexema, ",");
            arrayTokens[tamActual] = t;
            tamActual++;
            break;
        } else if (c == '[') 
        {
            t.compLex = L_CORCHETE;
            t.linea = numLinea;
            strcpy(t.lexema, "[");
            arrayTokens[tamActual] = t;
            tamActual++;
            break;
        } else if (c == ']') 
        {
            t.compLex = R_CORCHETE;
            t.linea = numLinea;
            strcpy(t.lexema, "]");
            arrayTokens[tamActual] = t;
            tamActual++;
            break;
        } else if (c == '{') 
        {
            t.compLex = L_LLAVE;
            t.linea = numLinea;
            strcpy(t.lexema, "{");
            arrayTokens[tamActual] = t;
            tamActual++;
            break;
        } else if (c == '}') 
        {
            t.compLex = R_LLAVE;
            t.linea = numLinea;
            strcpy(t.lexema, "}");
            arrayTokens[tamActual] = t;
            tamActual++;
            break;
        }
        //Palabras reservadas para true, false y null.
        else if (isalpha(c))
		{
			i = 0;
			do
			{
				id[i] = c;
				i++;
				c = fgetc(archivo);
			} while(isalpha(c));
			
			id[i] = '\0';
			sprintf(aux,"%s",id);
			
			if(strcmp(aux,"true") == 0 || strcmp(aux,"TRUE") == 0)                                
			{
			    t.compLex = PR_TRUE;
                t.linea = numLinea;
                strcpy(t.lexema, aux);
                arrayTokens[tamActual] = t;
                tamActual++;
			}
			else if(strcmp(aux,"false") == 0 || strcmp(aux,"FALSE") == 0)
			{
			    t.compLex = PR_FALSE;
                t.linea = numLinea;
                strcpy(t.lexema, aux);
                arrayTokens[tamActual] = t;
                tamActual++;
			}
			else if(strcmp(aux,"null") == 0 || strcmp(aux,"NULL") == 0)
			{
			    t.compLex = PR_NULL;
                t.linea = numLinea;
                strcpy(t.lexema, aux);
                arrayTokens[tamActual] = t;
                tamActual++;
			}
			else
			{
			    error(aux);
			    ungetc(c,archivo);
			    continue;
			}
			if (c != EOF)
				ungetc(c,archivo);
			else
				c = 0;
			break;
		}
         else if (c != EOF)
        {
            sprintf(msg, "%c no esperado", c);
            error(msg);
            while (c != '\n')
                c = fgetc(archivo);
            ungetc(c, archivo);
        }
    }
    if (c == EOF)
    {
        t.compLex = EOF;
        t.linea = numLinea;
        sprintf(t.lexema, "EOF");
        arrayTokens[tamActual] = t;
        tamActual++;
    }
}

/* 
 ==================================================================================================================
                                                    Analizador Sintáctico
 ==================================================================================================================
*/


token tokenActual;
int setSync[TAMCONJ];
int bandError = 0;
int posicion = -1;

void cargarTokens()
{
    while (t.compLex != EOF) 
    {
        sigLex();   
    }
}

void errorSintactico(const char* mensaje) 
{
    printf(" Error sintáctico. %s",  mensaje);
    bandError = 1;
}

void parser()
{
    getToken();
    int conjSig []={EOF,0,0,0,0,0,0,0};
    json(conjSig);
}

void getToken()
{
    posicion++;
    tokenActual = arrayTokens[posicion];
}

void json(int setSync[])
{
    if(tokenActual.compLex == L_LLAVE || tokenActual.compLex == L_CORCHETE)
    {
        element();
    } else
    {
        sprintf(msg1, "En la linea %d Se esperaba L_LLAVE o L_CORCHETE.\n" , tokenActual.linea);
        errorSintactico(msg1);
    }
}

void match(int t)
{
    if(t == tokenActual.compLex)
    {
        getToken();
    }else
    {
        sprintf(msg1, "En la linea %d No se esperaba  %s\n", tokenActual.linea, tokenActual.lexema );
        errorSintactico(msg1);
    }
}

void element()
{ 
    int conjPrimero[] = {L_LLAVE, L_CORCHETE,0,0,0,0,0,0};
    int conjSig [] = {EOF,COMA, R_CORCHETE, R_LLAVE,0,0,0,0};
    checkInput(conjPrimero, conjSig);
    
    switch(tokenActual.compLex)
    {
        case L_LLAVE:
            object(conjSig);
            break;
        case L_CORCHETE:
            array(conjSig);
            break;
        default:
            sprintf(msg1, "En la linea %d Se esperaba L_LLAVE o L_CORCHETE.\n" , tokenActual.linea);
            errorSintactico(msg1);
            break;
    }
    
    checkInput(conjSig,conjPrimero);
}

void object(int setSync[])
{
    int conjPrimero[] = {L_LLAVE,0,0,0,0,0,0,0};
    int conjSig[] = {EOF,COMA, R_CORCHETE, R_LLAVE,0,0,0,0};
    checkInput(conjPrimero,conjSig);
    
    switch (tokenActual.compLex)
    {
        case L_LLAVE:
            match(L_LLAVE);        
            objectP(setSync);
            break;
        default:
            sprintf(msg1, "En la linea %d Se esperaba L_LLAVE.\n" , tokenActual.linea);
            errorSintactico(msg1);
            break;
    }
    
    checkInput(conjSig,conjPrimero);
}

void objectP(int setSync[])
{
    int conjPrimero[] = {LITERAL_CADENA,R_LLAVE,0,0,0,0,0,0};
    int conjSig[] = {EOF,COMA, R_CORCHETE, R_LLAVE,0,0,0,0};
    checkInput(conjPrimero,conjSig);
    
    switch (tokenActual.compLex)
    {
        case LITERAL_CADENA:
            atributeList();
            match(R_LLAVE);        
            break;
        case R_LLAVE:
            match(R_LLAVE);
            break;
        default:
            sprintf(msg1, "En la linea %d Se esperaba LITERAL_CADENA o R_LLAVE.\n" , tokenActual.linea);
            errorSintactico(msg1);

            break;
    }
    
    checkInput(conjSig,conjPrimero);
}

void array(int setSync[])
{
    int conjPrimero[] = {L_CORCHETE,0,0,0,0,0,0,0};
    int conjSig[] = {EOF,COMA, R_CORCHETE, R_LLAVE,0,0,0,0};
    checkInput(conjPrimero,conjSig);
    
    switch (tokenActual.compLex)
    {
        case L_CORCHETE:
            match(L_CORCHETE);        
            arrayP(setSync);
            break;
        default:
            sprintf(msg1, "En la linea %d Se esperaba L_CORCHETE.\n" , tokenActual.linea);
            errorSintactico(msg1);
            break;
    }
    
    checkInput(conjSig,conjPrimero);
}

void arrayP(int setSync[])
{
    int conjPrimero[] = {L_LLAVE, L_CORCHETE, R_CORCHETE,0,0,0,0,0};
    int conSig[] = {EOF,COMA, R_CORCHETE, R_LLAVE,0,0,0,0};
    checkInput(conjPrimero,conSig);
    
    switch (tokenActual.compLex)
    {
        case L_LLAVE:
            elementList();
            match(R_CORCHETE);        
            break;
        case L_CORCHETE:
            elementList();
            match(R_CORCHETE);
            break;
        case R_CORCHETE:
            match(R_CORCHETE);
            break;
        default:
            sprintf(msg1, "En la linea %d Se esperaba L_LLAVE, L_CORCHETE o R_CORCHETE.\n" , tokenActual.linea);
            errorSintactico(msg1);
            break;
    }
    
    checkInput(conSig,conjPrimero);
}

void elementList()
{
    int conjPrimero[] = {L_LLAVE,L_CORCHETE,0,0,0,0,0,0};
    int conjSig[] = {R_CORCHETE,0,0,0,0,0,0,0};
    checkInput(conjPrimero,conjSig);
    
    if(tokenActual.compLex == L_LLAVE || tokenActual.compLex == L_CORCHETE){
        element();
        elementListP(conjSig);
    } else
    {
        sprintf(msg1, "En la linea %d Se esperaba L_LLAVE, L_CORCHETE.\n" , tokenActual.linea);
        errorSintactico(msg1);
        checkInput(conjSig,conjPrimero);
    }
}

void elementListP(int setSync[])
{
    int conjPrimero[] = {COMA,0,0,0,0,0,0,0};
    int conjSig[] = {R_CORCHETE};
    
    if(tokenActual.compLex == R_CORCHETE){
        checkInput(conjSig,conjPrimero);
    } else
    {
        checkInput(conjPrimero,conjSig);
    }
    
    switch(tokenActual.compLex)
    {
        case COMA:
            match(COMA);
            element();
            elementListP(conjSig);
            break;
    }
    
    checkInput(conjSig,conjPrimero);
}

void atribute()
{
    int conjPrimero[] = {LITERAL_CADENA,0,0,0,0,0,0,0};
    int conjSig[] = {COMA,R_LLAVE,0,0,0,0,0,0};
    checkInput(conjPrimero,conjSig);
    
    switch(tokenActual.compLex)
    {
        case LITERAL_CADENA:
            atributeName();
            match(DOS_PUNTOS);
            atributeValue(conjSig);
            break;
            
        default:
            sprintf(msg1, "En la linea %d Se esperaba LITERAL_CADENA.\n" , tokenActual.linea);
            errorSintactico(msg1);
            break;
    }
}

void atributeList()
{
    int conjPrimero[] = {LITERAL_CADENA,0,0,0,0,0,0,0};
    int conjSig[] = {R_LLAVE,0,0,0,0,0,0,0};
    checkInput(conjPrimero,conjSig);
    
    if(tokenActual.compLex == LITERAL_CADENA){
        atribute();
        atributeListP(conjSig);
    } else
    {
        sprintf(msg1, "En la linea %d Se esperaba LITERAL_CADENA.\n" , tokenActual.linea);
        errorSintactico(msg1);
    }
    
    checkInput(conjSig,conjPrimero);
}

void atributeListP(int setSync[])
{
    int conjPrimero[] = {COMA,0,0,0,0,0,0,0};
    int conjSig[] = {R_LLAVE,0,0,0,0,0,0,0};
    
    if(tokenActual.compLex == R_LLAVE){
        checkInput(conjSig,conjPrimero);
    } else
    {        
        checkInput(conjPrimero,conjSig);
    }
    
    switch(tokenActual.compLex)
    {
        case COMA:
            match(COMA);
            atribute();
            atributeListP(conjSig);
            break;
    }
    
    checkInput(conjSig,conjPrimero);
}

void atributeName()
{
    int conjPrimero[] = {LITERAL_CADENA,0,0,0,0,0,0,0};
    int conjSig[] = {DOS_PUNTOS,0,0,0,0,0,0,0};
    checkInput(conjPrimero,conjSig);
    
    switch(tokenActual.compLex)
    {
        case LITERAL_CADENA:
            match(LITERAL_CADENA);
            break;
        default:
            sprintf(msg1, "En la linea %d Se esperaba LITERAL_CADENA.\n" , tokenActual.linea);
            errorSintactico(msg1);
            break;
    }
}

void atributeValue(int setSync[])
{
    int conjPrimero[] = {L_LLAVE, L_CORCHETE, LITERAL_CADENA, LITERAL_NUM, PR_TRUE, PR_FALSE, PR_NULL, 0};
    int conjSig[] = {COMA,R_LLAVE,0,0,0,0,0,0};
    checkInput(conjPrimero,conjSig);
    
    switch(tokenActual.compLex)
    {
        case L_LLAVE:
            element();
            break;
        case L_CORCHETE:
            element();
            break;
        case LITERAL_CADENA:
            match(LITERAL_CADENA);
            break;
        case LITERAL_NUM:
            match(LITERAL_NUM);
            break;
        case PR_TRUE:
            match(PR_TRUE);
            break;
        case PR_FALSE:
            match(PR_FALSE);
            break;
        case PR_NULL:
            match(PR_NULL);
            break;
        default:
            errorSintactico("Se esperaba L_LLAVE, L_CORCHETE, LITERAL_CADENA, LITERAL_NUM, TRUE, FALSE O NULL.\n");
            break;
    }
}

/* 
 ==================================================================================================================
                                     Panic Mode para sincronizacion de errores
 ==================================================================================================================
*/


void scan(int setSync1[], int setSync2[])
{
    setSync[16];
    int i = 0;
    int bandera = 0;
    int tam = 8;
    int tam1 = 16;
    int j;
    
    // setSync y setSync1 para sincronizar 
    for(j = 0; j < tam; j++)
    {
        setSync[j] = setSync1[j];
    }
    
    int k;
    int y = 0;
    for(k = tam; k < tam1; k++)
    {
        setSync[k] = setSync2[y];
        y++;
    }
    
    do{
        while(i < tam1)
        { 
            if(tokenActual.compLex != setSync[i] && tokenActual.compLex != EOF)
            {
                bandera = 1;
                i++;
            }else{
                bandera = 0;
                i = tam1;
                break;
            }
       }
       if(bandera == 1)
       {
           getToken();
           i = 0;
       }
    }while(i < tam1);
}

void checkInput(int conjPrimero[], int conjSig[])
{
    int tam = 8;
    int i;
    int bandera = 0;
    
    for (i = 0; i < tam; i++) 
    {
        if(tokenActual.compLex == conjPrimero[i])
        {
            bandera = 0;
            break;
        }else{
            bandera = 1;
        }
    }
    
    if(bandera == 1)
    {
        sprintf(msg1, "En la linea %d No se esperaba %s\n", tokenActual.linea, tokenActual.lexema);
        errorSintactico(msg1);
        scan(conjPrimero, conjSig);
    }
}



