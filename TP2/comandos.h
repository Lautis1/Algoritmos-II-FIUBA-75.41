#ifndef ALGOS_GITHUB_COMANDOS_H
#define ALGOS_GITHUB_COMANDOS_H

#include "tp2.h"
#include <unistd.h>
/************************************************************************************************/
//Funcion que recibe un archivo y lo procesa, detectando posibles DoS y guardando ips y recursos donde corresponda.
bool procesar_log(char* nombre_de_archivo, hash_t* recursos_mas_solicitados, abb_t* solicitantes);

//Obtiene los "N" sitios mas visitados de la pagina.
void mostrar_mas_visitados(hash_t* recursos_mas_solicitados,  int n);

//Recibe el arbol que contiene a los visitantes de la pagina y dos direcciones IP.
//Llama a una funcion que recorre el arbol y, a medida que va comparando si las
//direcciones de los visitantes pertenecen al rango conformado entre las 2 ip's 
//recibidas por parametro, va imprimiendolas por pantalla.
void mostrar_visitantes(abb_t* visitantes, char* ip_inicio, char* ip_fin);

#endif //ALGOS_GITHUB_COMANDOS_H
