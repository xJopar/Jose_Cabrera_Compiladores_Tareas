 /*
 *	Traductor dirigido por sintaxis	
 *	Curso: Compiladores y Lenguajes de Bajo de Nivel
 *	Practica de Programacion Nro. 3
 *	Nombre: Jose Maria Cabrera Peralta
 *  CI: 5553957
 *	Descripcion:
 *		Implementación de un Traductor para el lenguaje JSON simplificado que pase a XML.
 */
#include "analizador_sintactico_trad.c"

int contador = 0;   // Para contar los espacios a insertar en output.xml
FILE *output;       // Archivo de salida XML

/************* Traductor ********************/

void jsonTrad()
{
    elementTrad();
}

void elementTrad()
{
    if(t.compLex == L_LLAVE)
    {
        objectTrad();
    }
    else if(t.compLex == L_CORCHETE)
    {
        arrayTrad();
    }
}

void arrayTrad()
{
    if(t.compLex == L_CORCHETE)
    {
        match(L_CORCHETE);
        arrayPTrad();
        insertEspacios();
    }
}

void arrayPTrad()
{
    if(t.compLex == L_CORCHETE || t.compLex == L_LLAVE)
    {
        fprintf(output, "\n" );
        insertEspacios();
        fprintf(output, "<item>\n" );
        contador += TAB;
        
        elementListTrad();
        match(R_CORCHETE);
        
        insertEspacios();
        fprintf(output, "</item>\n" );
        contador -= TAB;
    }
    else if(t.compLex == R_CORCHETE)
    {
        contador -= TAB;
        match(R_CORCHETE);
    }
}

void elementListTrad()
{
    if(t.compLex == L_CORCHETE || t.compLex == L_LLAVE)
    {
        elementTrad();
        elementListPTrad();
    }
}

void elementListPTrad()
{
    if(t.compLex == R_CORCHETE)
    {
        return;
    }
    else if(t.compLex == COMA)
    {
        match(COMA);
        
        insertEspacios();
        fprintf(output, "</item>\n" );
        insertEspacios();
        fprintf(output, "<item>\n" );
        contador += TAB;
        
        elementTrad();
        elementListPTrad();
    }
}

void objectTrad()
{
    if(t.compLex == L_LLAVE)
    {
        match(L_LLAVE);
        objectPTrad();
    }
}

void objectPTrad()
{
    if(t.compLex == LITERAL_CADENA)
    {
        attributesListTrad();
        match(R_LLAVE);
    }
    else if(t.compLex == R_LLAVE)
    {
        match(R_LLAVE);
    }
}

void attributesListTrad()
{
    if(t.compLex == LITERAL_CADENA)
    {
        attributeTrad();
        attributesListPTrad();
    }
}

void attributesListPTrad()
{
    if (t.compLex == R_LLAVE)
    {
        contador -= TAB;
        return;
    }
    else if(t.compLex == COMA)
    {
        match(COMA);
        attributeTrad();
        attributesListPTrad();
    }
}

void attributeTrad()
{
    if(t.compLex == LITERAL_CADENA)
    {
        insertEspacios();
        char lex[TAMLEX];
        strcpy(lex, t.lexema );
        
        fprintf(output, "<" );
        attributeNameTrad();
        fprintf(output, ">" );
        
        match(DOS_PUNTOS);
        attributeValueTrad();
        
        fprintf(output, "</" );
        stringSinComillas(lex);
        fprintf(output, ">\n" );
    }
}

void attributeNameTrad()
{
    if(t.compLex == LITERAL_CADENA)
    {    
        stringSinComillas(t.lexema);
        match(LITERAL_CADENA);
    }
}

void attributeValueTrad()
{
    if(t.compLex == L_CORCHETE || t.compLex == L_LLAVE)
    {
        contador += TAB;
        elementTrad();
    }
    else if(t.compLex == LITERAL_CADENA )
    {
        fprintf(output, "%s" , t.lexema );
        match(LITERAL_CADENA);
    }
    else if(t.compLex == LITERAL_NUM)
    {
        fprintf(output, "%s" , t.lexema );
        match(LITERAL_NUM);
    }
    else if(t.compLex == PR_TRUE)
    {
        fprintf(output, "%s" , t.lexema );
        match(PR_TRUE);
    }
    else if(t.compLex == PR_FALSE)
    {
        fprintf(output, "%s" , t.lexema );
        match(PR_FALSE);
    }
    else if(t.compLex == PR_NULL)
    {
        fprintf(output, "%s" , t.lexema );
        match(PR_NULL);
    }
}

void stringSinComillas(char* cadena)
{
    char aux[TAMLEX];
    strcpy(aux, cadena );
    int i = 1;
    
    while( aux[i] != '\"' )
    {
        fprintf(output, "%c", aux[i] );
        i++;
    }
}

void insertEspacios ()
{
    int i;
    for (i = 1; i <= contador; i++)
    {
        fprintf(output, " " );
    }
}

int main (int argc,char* args[])
{
    // Apertura del archivo de salida XML
    output = fopen ("output.xml", "w");
    if(argc > 1)
    {
        if (!(archivo = fopen(args[1],"rt")))
        {
            printf("Archivo no encontrado.\n");
            exit(1);
        }
        
        cargarTokens();
        parser();
        
        fclose(archivo);
        archivo = fopen(args[1],"rt");
        
        posicion = 0;
        tokenActual = arrayTokens[posicion];
        numLinea = 1;
        
        sigLex();
        jsonTrad();
            
        fclose(archivo);
        
    } else
    {
        printf("Debe pasar como parametro el path al archivo fuente.\n");
        exit(1);
    }
    
    fclose(output);
    return 0;
}
