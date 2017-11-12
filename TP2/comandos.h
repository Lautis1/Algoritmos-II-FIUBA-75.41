#ifndef ALGOS_GITHUB_COMANDOS_H
#define ALGOS_GITHUB_COMANDOS_H

#include "tp2.h"
#include <unistd.h>

bool procesar_log(char* nombre_de_archivo, hash_t* recursos_mas_solicitados, abb_t* solicitantes);

void mostrar_mas_visitados(hash_t* recursos_mas_solicitados,  int n);

void mostrar_visitantes(abb_t* visitantes, char* ip_inicio, char* ip_fin);

void quitar_caracter_new_line(char* cadena);

void mostrar_n_recursos(heap_t* recursos_temp, int cantidad_de_recursos_a_mostrar);

#endif //ALGOS_GITHUB_COMANDOS_H
