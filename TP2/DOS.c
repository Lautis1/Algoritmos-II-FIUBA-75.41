#define _XOPEN_SOURCE 700

#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include "lista.h"
#include "hash.h"
#include "DOS.h"

#define TIME_FORMAT "%FT%T%z"

#define N_SOL_CONSIDERADAS_DDOS 5
#define RANGO_DE_TIEMPO_CONSIDERADO 2



time_t iso8601_to_time(const char* iso8601) {
    struct tm bktime = { 0 };
    strptime(iso8601, TIME_FORMAT, &bktime);
    return mktime(&bktime);
}

void detectar_DOS(const char* ip, void* solicitudes_ip, void* arbol_visitantes) {
    lista_t* lista_solicitudes = solicitudes_ip;
    if(usuario_hizo_mas_solicitudes_de_las_permitidas(lista_solicitudes)) {
        imprimir_dos(ip);
    }
}

bool usuario_hizo_mas_solicitudes_de_las_permitidas(lista_t* lista_solicitudes) {

    if (lista_largo(lista_solicitudes) < N_SOL_CONSIDERADAS_DDOS) return false;
    lista_iter_t* iter_izq = lista_iter_crear(lista_solicitudes);
    if (iter_izq == NULL) return false;
    lista_iter_t* iter_der = lista_iter_crear(lista_solicitudes);
    if (iter_der == NULL) return false;

    // Muevo el iter_der de tal forma que quede el iter_izq sobre la
    // primera solicitud y iter_der sobre la quinta solicitud
    for (int i = 0; i < N_SOL_CONSIDERADAS_DDOS; i++) {
        lista_iter_avanzar(iter_der);
    }
    bool estado = false;
    while (!lista_iter_al_final(iter_der)) {
        double lapso = difftime(*((time_t *) lista_iter_ver_actual(iter_der)), *((time_t *) lista_iter_ver_actual(iter_izq)));
        if (lapso < RANGO_DE_TIEMPO_CONSIDERADO) {
            estado = true;
            break;
        }
        lista_iter_avanzar(iter_izq);
        lista_iter_avanzar(iter_der);
    }
    lista_iter_destruir(iter_izq);
    lista_iter_destruir(iter_der);
    return estado;
}

bool agregar_fecha_de_solicitud(char* ip, time_t* fecha, hash_t* peticiones_por_ip) {

    if (!hash_pertenece(peticiones_por_ip, ip)) {
        lista_t *lista_aux = lista_crear();
        hash_guardar(peticiones_por_ip, ip, lista_aux);
    }
    lista_t* lista_asociada = (lista_t*)hash_obtener(peticiones_por_ip, ip);
    return (lista_insertar_ultimo(lista_asociada, fecha));
}

//Imprimir posibles ip con DoS

void imprimir_dos(const char* direccion_ip){

    fprintf(stdout, "DoS: %s\n", direccion_ip);
}

void wrapper_destruir_hash_solicitudes(void* lista) {
    lista_t* lista_solicitudes = lista;
    lista_destruir(lista_solicitudes, free);
}

