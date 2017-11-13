#ifndef ALGOS_GITHUB_COMANDOS_H
#define ALGOS_GITHUB_COMANDOS_H

#include "tp2.h"
#include <unistd.h>
/************************************************************************************************/
//Funcion que recibe un archivo y lo procesa, agregando las direcciones IP de los visitantes
//del log a un ABB.
bool procesar_log(char* nombre_de_archivo, hash_t* recursos_mas_solicitados, abb_t* solicitantes);

//Funcion que funciona de manera levemente similar al "top_K": recibe un hash y un entero a procesar.
//Crea un heap de minimos y va encolando N elementos del heap
void procesar_n_a_mostrar(heap_t* min_heap, hash_t* recursos_mas_solicitados, int n);

//Obtiene los "N" sitios mas visitados de la pagina, los cuales se encuentran en un hash,
//pero son trasladados a un heap de minimos.
void mostrar_mas_visitados(hash_t* recursos_mas_solicitados,  int n);

//Recibe el arbol que contiene a los visitantes de la pagina y dos direcciones IP.
//Llama a una funcion que recorre el arbol y, a medida que va comparando si las
//direcciones de los visitantes pertenecen al rango conformado entre las 2 ip's 
//recibidas por parametro, va imprimiendolas por pantalla.
void mostrar_visitantes(abb_t* visitantes, char* ip_inicio, char* ip_fin);

//Recibe una cadena y reemplaza el caracter de salto de linea
//por el caracter de fin de cadena.
void quitar_caracter_new_line(char* cadena);

//Dado un heap temporal de recursos y una cantidad "N", imprime por pantalla los
//N sitios mas visitados de la pagina.
void mostrar_n_recursos(heap_t* recursos_temp, int cantidad_de_recursos_a_mostrar);

#endif //ALGOS_GITHUB_COMANDOS_H
