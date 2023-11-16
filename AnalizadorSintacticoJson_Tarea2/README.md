# Compiladores y Lenguajes de Bajo de Nivel

## Práctica de Programación Nro. 2
 *	Nombre: José María Cabrera Peralta
 *  CI: 5553957
 *	Descripción: Implementación de un analizador léxico para archivos JSON simplificado.
>Implementar un analizador sintáctico descendente recursivo o LL(1) para el lenguaje Json
simplificado. Recibe un archivo fuente Json, y debe imprimir en pantalla si el fuente es
sintácticamente correcto o los errores encontrados en caso de existir (se adjuntan ejemplos
de fuentes correctos). En caso de error se deberá implementar la estrategia de manejo de
errores Panic Mode con sincronización y continuar el análisis.
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
