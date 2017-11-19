#ifndef ALGOS_GITHUB_TP2_H
#define ALGOS_GITHUB_TP2_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include "hash.h"
#include "lista.h"
#include "heap.h"
#include "strutil.h"
#include "abb.h"
#include "DOS.h"
#include "recursos.h"
#include "visitantes.h"
#include "comandos.h"
/*****************************************************************************************************/
//Funcion que recibe un arbol de visitantes y un hash con los recursos mas solicitados del log.
//Lee por entrada standard lo que ingresa el usuario y llama a la funcion que procesa esos datos.
void recibir_comandos(abb_t* visitantes, hash_t* recursos);

//Funcion encargada de imprimir un error de comando por stderr.
void imprimir_error(char* comando);

//Funcion encargada de contar la cantidad de componentes de un arreglo.
//Devuelve esa cantidad.
int contar_cantidad_parametros(char** array);

//Funcion encargada de procesar lo que ingresa el usuario en entrada standard.
//Segun el comando que ingrese, efectua dicha operacion.
//Devuelve un entero que representa el estado de la ejecucion de la funcion.
int procesar_entrada_stdin(char* linea_entrada, abb_t* arbol_visitantes, hash_t* recursos_mas_solicitados);

//Recibe una cadena y reemplaza el caracter de salto de linea
//por el caracter de fin de cadena.
void quitar_caracter_new_line(char* cadena);

#endif //ALGOS_GITHUB_TP2_H
