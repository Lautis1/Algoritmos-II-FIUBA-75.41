#ifndef ALGOS_GITHUB_TP2_H
#define ALGOS_GITHUB_TP2_H

#include "hash.h"
#include "lista.h"
#include "heap.h"
#include "strutil.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

time_t iso8601_to_time(const char* iso8601);

bool procesar_log(char* nombre_de_archivo, heap_t* recursos_mas_solicitados, heap_t* solicitantes);

bool pasar_recursos_de_hash_a_heap(char* clave, void* recurso, void* heap);

bool agregar_fecha_de_request(char* ip, time_t fecha, hash_t* peticiones_por_ip);

bool aumenta_cont_solicitudes_recurso(hash_t* temp_recursos_solicitados, char* recurso);

bool agregar_fecha_de_solicitud(char* ip, time_t fecha, hash_t* peticiones_por_ip);

void imprimir_dos(char* direccion_ip);

bool usuario_hizo_mas_solicitudes_de_las_permitidas(lista_t* lista_solicitudes);

void iterar_hash(hash_t* hash, void visitar(char* clave, void *dato, void *extra), void *extra);


#endif //ALGOS_GITHUB_TP2_H
