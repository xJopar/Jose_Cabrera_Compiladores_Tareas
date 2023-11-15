/*
 *	Analizador Léxico	
 *	Curso: Compiladores y Lenguajes de Bajo de Nivel
 *	Práctica de Programación Nro. 1
 *	Nombre: Jose Maria Cabrera Peralta
 *  CI: 5553957
 *	Descripcion:
 *		Implementacion de un analizador léxico para archivos JSON simplificado.
 */

//Header
#include "anlex.h"

/************* Variables globales **************/
int consumir;			/* 1 indica al analizador lexico que debe devolver
						el sgte componente lexico, 0 debe devolver el actual*/

char cad[5*TAMLEX];		// string utilizado para cargar mensajes de error
token t;				// token global para recibir componentes del Analizador Lexico

// variables para el analizador lexico
FILE *archivo;			// Fuente json
FILE *fd;               // Archivo para la salida esperada
char id[TAMLEX];		// Utilizado por el analizador lexico
int numLinea=1;			// Numero de Linea
char espacios[300];    // Arreglo que contendra los espacios a insertar
int contador=-1;         // Cantidad de espacios a cargar

/**************** Funciones **********************/
// Rutinas del analizador lexico
void error(const char* mensaje)
{
	printf("Lin %d: Error Lexico. %s.\n",numLinea,mensaje);
}

void sigLex()
{
	int i=0;
	char c=0;
	int acepto=0;
	int estado=0;
	char msg[41];
	char aux[10];
	int bandera=0;
	contador=-1;
	entrada e;

	while((c=fgetc(archivo))!=EOF)
	{
	    
		if (c==' ')
		{
		    do
            {
                // se cargan los espacios leidos
                contador++;
                espacios[contador]=' ';
                c=fgetc(archivo); 
            }while(c==' ');
            c=ungetc(c,archivo);
		}
		else if(c=='\t'){
		    //si es tabulador escribe tambien en output.txt
            while(c=='\t')
            { 
                fprintf(fd, "%c" , c);
	        	c=fgetc(archivo);
            }
		}
		else if(c=='\n')
		{
			//incrementar el numero de linea
			numLinea++;
			fprintf(fd, "\n");
			contador=-1;
			continue;
		}
		//  LITERAL_CADENA " .*"
		else if (c=='\"')
		{
			//es un string
		    i=0;
			id[i]=c;
			i++;
			int bandError = 0;
			do
			{
				c=fgetc(archivo);
				if (c=='\"')
				{
                    id[i]=c;
                    i++;
                    bandera=1;
                    break;
				}
                else if(c==EOF || c==',' || c=='\n' || c==':')
				{
				    sprintf(msg,"No se esperaba '%c', Se esperaba que finalize el literal",c);
					error(msg);
                    while(c!='\n')
                        c=fgetc(archivo);
                    bandError = 1;
                    ungetc(c,archivo);
                    break;
				}
				else
				{
					id[i]=c;
					i++;
				}
			}while(isascii(c) || bandera==0);
			if (bandError == 1){
			    contador=-1;
			    continue;
			}
			id[i]='\0';
			t.pe=buscar(id);
			t.compLex=t.pe->compLex;
	        if (t.pe->compLex==-1)
			{
				strcpy(e.lexema,id);
			    e.compLex=LITERAL_CADENA;
	            insertar(e);
				t.pe=buscar(id);
        	    t.compLex=LITERAL_CADENA;
			}
			break;
		}
		else if (isdigit(c))
		{
				//es un numero
				i=0;
				estado=0;
				acepto=0;
				id[i]=c;
				
				while(!acepto)
				{
					switch(estado){
					case 0: //una secuencia netamente de digitos, puede ocurrir . o e
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=0;
						}
						else if(c=='.'){
							id[++i]=c;
							estado=1;
						}
						else if(tolower(c)=='e'){
							id[++i]=c;
							estado=3;
						}
						else{
							estado=6;
						}
						break;
						
					case 1://un punto, debe seguir un digito
						c=fgetc(archivo);						
						if (isdigit(c))
						{
							id[++i]=c;
							estado=2;
						}
						else{
							sprintf(msg,"No se esperaba '%c'",c);    
							estado=-1;
						}
						break;
					case 2://la fraccion decimal, pueden seguir los digitos o e
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=2;
						}
						else if(tolower(c)=='e')
						{
							id[++i]=c;
							estado=3;
						}
						else
							estado=6;
						break;
					case 3://una e, puede seguir +, - o una secuencia de digitos
						c=fgetc(archivo);
						if (c=='+' || c=='-')
						{
							id[++i]=c;
							estado=4;
						}
						else if(isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else{
							sprintf(msg,"No se esperaba '%c'",c); 
							estado=-1;
						}
						break;
					case 4://necesariamente debe venir por lo menos un digito
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else{
							sprintf(msg,"No se esperaba '%c'",c);  
							estado=-1;
						}
						break;
					case 5://una secuencia de digitos correspondiente al exponente
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else{
							estado=6;
						}break;
					case 6://estado de aceptacion, devolver el caracter correspondiente a otro componente lexico
						if (c!=EOF){
						    if(isalpha(c)){    // Si viene una letra que no es E, es error
						        sprintf(msg,"No se esperaba '%c'",c); 
						        error(msg);
                                acepto=1;
                                t.compLex=1;
                                while(c!='\n')
                                    c=fgetc(archivo);
                                ungetc(c,archivo);
					            break;
						    }
							ungetc(c,archivo);
						}else
							c=0;
						id[++i]='\0';
						acepto=1;
						t.pe=buscar(id);
						if (t.pe->compLex==-1)
						{
							strcpy(e.lexema,id);
							e.compLex=LITERAL_NUM;
							insertar(e);
							t.pe=buscar(id);
						}
						t.compLex=LITERAL_NUM;
						break;
					case -1:
						if (c==EOF)
							error("No se esperaba el fin de archivo");
						else
						    error(msg);
                            acepto=1;
                            t.compLex=1;
                            while(c!='\n')
                                c=fgetc(archivo);
                            ungetc(c,archivo);
					        break;
					}
				}
			break;
		}
		else if (c==':')
		{
			t.compLex=DOS_PUNTOS;
		    t.pe=buscar(":");
			break;
		}
		else if (c==',')
		{
			t.compLex=COMA;
			t.pe=buscar(",");
			break;
		}
		else if (c=='[')
		{
			t.compLex=L_CORCHETE;
			t.pe=buscar("[");
			break;
		}
		else if (c==']')
		{
			t.compLex=R_CORCHETE;
			t.pe=buscar("]");
			break;
		}
		else if (isalpha(c))
		{
			//es una palabra reservada (true, false o null)
			i=0;
			do{
				id[i]=c;
				i++;
				c=fgetc(archivo);
			}while(isalpha(c));
			id[i]='\0';
			sprintf(aux,"%s",id);
			if(strcmp(aux,"true")==0 || strcmp(aux,"TRUE")==0 || strcmp(aux,"false")==0 || strcmp(aux,"FALSE")==0 
			                                                  || strcmp(aux,"null")==0 || strcmp(aux,"NULL")==0)
			{
			    t.pe=buscar(id);
			    t.compLex=t.pe->compLex;
			}
			else
			{
			    while(c!='\n'){
			        c=fgetc(archivo);
		        }
		        contador=-1;
			    error(aux);
			    ungetc(c,archivo);
			    continue;
			}
			if (c!=EOF)
				ungetc(c,archivo);
			else
				c=0;
			break;
		}
		
		else if (c=='{')
		{
			t.compLex=L_LLAVE;
	        t.pe=buscar("{");
		    break;
        }
		else if (c=='}')
	    {
			t.compLex=R_LLAVE;
		    t.pe=buscar("}");
		    break;
		}
		else if (c!=EOF)
		{
		    sprintf(msg,"%c no esperado",c);
			error(msg);
			t.compLex=1;
            while(c!='\n')
                c=fgetc(archivo);
            
            strcpy(espacios, " ");
            contador = -1;
            ungetc(c,archivo);
		}
	}
	if (c==EOF)
	{
		t.compLex=EOF;
		sprintf(e.lexema,"EOF");
		t.pe=&e;
	}
	
}

int main(int argc,char* args[])
{
	// inicializar analizador lexico
	initTabla();
	initTablaSimbolos();
	
	// Se crea un archivo tokenStream.txt donde se escribiran los resultados
	fd = fopen(ARCHIVO , "w");
	
	if(argc > 1)
	{
		if (!(archivo=fopen(args[1],"rt")))
		{
			printf("Archivo no encontrado.\n");
			exit(1);
		}
		
        // Escribir los componentes lexicos en el archivo de salida hasta que se llegue a EOF
	    while(t.compLex!=EOF){
		    sigLex();
			
	        // Escribir los espacios en blanco del archivo fuente al archivo de salida
	        if(contador > -1){
                int i = 0;
                for(i=0; i<=contador;i++)
                    fprintf(fd, "%c" , espacios[i]);
            }

			switch(t.compLex){
			
                case L_CORCHETE:
                    fprintf(fd,"%s","L_CORCHETE ");
                    break;
                case R_CORCHETE:
                    fprintf(fd,"%s","R_CORCHETE ");
                    break;
                case L_LLAVE:
                    fprintf(fd,"%s","L_LLAVE ");
                    break;
                case R_LLAVE:
                    fprintf(fd,"%s","R_LLAVE ");
                    break;
                case COMA:
                    fprintf(fd,"%s","COMA ");
                    break;
                case DOS_PUNTOS:
                    fprintf(fd,"%s","DOS_PUNTOS ");
                    break;
                case LITERAL_CADENA:
                    fprintf(fd,"%s","LITERAL_CADENA ");
                    break;
                case LITERAL_NUM:
                    fprintf(fd,"%s","LITERAL_NUM ");
                    break;
                case PR_TRUE:
                    fprintf(fd,"%s","PR_TRUE ");
                    break;
                case PR_FALSE:
                    fprintf(fd,"%s","PR_FALSE ");
                    break;
                case PR_NULL:
                    fprintf(fd,"%s","PR_NULL ");
                    break;
                case EOF:
                    break;
            }
		}
		fclose(archivo);
		fclose(fd);
	}else{
		printf("Debe pasar como parametro el path al archivo fuente.\n");
		exit(1);
	}

	return 0;
}
