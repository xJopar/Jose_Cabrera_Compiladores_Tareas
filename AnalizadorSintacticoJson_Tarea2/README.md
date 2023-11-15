# Compiladores y Lenguajes de Bajo de Nivel

## Práctica de Programación Nro. 2
 *	Nombre: José María Cabrera Peralta
 *  CI: 5553957
 *	Descripción: Implementación de un analizador léxico para archivos JSON simplificado.
>Implementar un analizador léxico para el lenguaje Json simplificado. Recibe un archivo fuente Json, debe realizar un análisis léxico reconociendo tokens, y luego por cada línea del fuente imprimir una línea en un archivo de salida con la secuencia de componentes léxicos encontrados separados por espacios (se adjuntan ejemplos de un fuente y su respectivo archivo de salida). En caso de error léxico deberá imprimir un mensaje de error y continuar con la siguiente línea del archivo fuente.
>*En lenguaje elegido fue C*

## Windows

 1. Generación de ejecutable:
	`gcc analizador_sintactico.c -o ansin`

 2. Ejecución del programa:
	`ansin.exe TUPRUEBA.txt`

## Linux

 1. Generación de ejecutable:
	`gcc analizador_sintactico.c  -o ansin`

 2. Ejecución del programa:
	`./ansin.exe TUPRUEBA.txt`
