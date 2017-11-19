#ifndef ALGOS_GITHUB_DOS_H
#define ALGOS_GITHUB_DOS_H

#include "tp2.h"

// Dada una cadena en formato ISO-8601 devuelve una variable de tipo time_t
// que representa un instante en el tiempo.
time_t iso8601_to_time(const char* iso8601);

//Dada una ip, un puntero a una lista y un puntero a un ABB, detecta si
//un usuario hizo mas solicitudes de las permitidas en menos de 2 segundos.
bool detectar_DOS(const char* ip, void* dato1, void* hash);

//Recorre la lista de solicitudes y detecta si una direccion ip es sospechosa de DoS
//Devuelve true o false dependiendo del estado de la operacion.
bool usuario_hizo_mas_solicitudes_de_las_permitidas(lista_t* lista_solicitudes);

//Agrega la fecha en que se hizo la solicitud a un recurso.
//Devuelve true o false dependiendo del estado de la operacion.
bool agregar_fecha_de_solicitud(char* ip, time_t* fecha, hash_t* peticiones_por_ip);

//Imprimir posibles ip con DoS
void imprimir_dos(const char* direccion_ip);

//Funcion que modifica la firma de lista_destruir para ser utilizada en otra funcion.
void wrapper_destruir_hash_solicitudes(void* lista);

#endif //ALGOS_GITHUB_DOS_H


