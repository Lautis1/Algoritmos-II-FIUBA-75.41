#ifndef ALGOS_GITHUB_DOS_H
#define ALGOS_GITHUB_DOS_H

#include "tp2.h"

// Dada una cadena en formato ISO-8601 devuelve una variable de tipo time_t
// que representa un instante en el tiempo.
time_t iso8601_to_time(const char* iso8601);

void detectar_DOS(const char* ip, void* solicitudes_ip, void* arbol_visitantes);

bool usuario_hizo_mas_solicitudes_de_las_permitidas(lista_t* lista_solicitudes);

bool agregar_fecha_de_solicitud(char* ip, time_t* fecha, hash_t* peticiones_por_ip);

void imprimir_dos(const char* direccion_ip);

void wrapper_destruir_hash_solicitudes(void* lista);

#endif //ALGOS_GITHUB_DOS_H


