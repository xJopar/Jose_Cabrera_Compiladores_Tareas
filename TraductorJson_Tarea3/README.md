# Compiladores y Lenguajes de Bajo de Nivel

## Práctica de Programación Nro. 3
 *	Nombre: José María Cabrera Peralta
 *  CI: 5553957
 *	Descripción: Implementación de un analizador léxico para archivos JSON simplificado.
>Implementar un traductor para el lenguaje Json simplificado. Recibe un archivo fuente Json, y
debe retornar un archivo traducido con su traducción a XML o los errores encontrados en
caso de existir (se adjuntan un ejemplo de un fuente con su salida esperada). En caso de error
se deberá implementar la estrategia de manejo de errores Panic Mode con sincronización y
continuar el análisis.
>*En lenguaje elegido fue C*

## Windows

 1. Generación de ejecutable:
	`gcc traductorJSON.c -o traductor`

 2. Ejecución del programa:
	`traductor.exe TUPRUEBA.txt`

## Linux

 1. Generación de ejecutable:
	`gcc traductorJSON.c  -o traductor`

 2. Ejecución del programa:
	`./traductor.exe TUPRUEBA.txt`
