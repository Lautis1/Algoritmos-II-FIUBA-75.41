#define _XOPEN_SOURCE 700

#include "comandos.h"

#define TIME_FORMAT "%FT%T%z"
/***********************************************************************************************/

bool procesar_log(char* nombre_de_archivo, hash_t* recursos_mas_solicitados, abb_t* visitantes) {
    
    FILE* archivo = fopen(nombre_de_archivo, "r");
    if(archivo == NULL) return false;
    char *linea = NULL;
    size_t capacidad = 0;
    ssize_t leidos;

    hash_t* peticiones_por_ip = hash_crear((hash_destruir_dato_t)wrapper_destruir_hash_solicitudes);
    if (peticiones_por_ip == NULL) return false;

    while ((leidos = getline(&linea, &capacidad, archivo)) > 0) {
        quitar_caracter_new_line(linea);
        char **linea_a_procesar = split(linea, '\t');
        char *ip = linea_a_procesar[0];
        char *nombre_recurso = linea_a_procesar[3];
        time_t* instante = malloc(sizeof(time_t));
        *instante = iso8601_to_time(linea_a_procesar[1]);

        abb_guardar(visitantes, ip, NULL);
        agregar_fecha_de_solicitud(ip, instante, peticiones_por_ip);
        aumenta_cont_solicitudes_recurso(recursos_mas_solicitados, nombre_recurso);
        free_strv(linea_a_procesar);
    }
    abb_in_order(visitantes, detectar_DOS, peticiones_por_ip);
    hash_destruir(peticiones_por_ip);
    free(linea);
    fclose(archivo);

    return true;
}

void mostrar_mas_visitados(hash_t* recursos_mas_solicitados, int cantidad_de_recursos_a_mostrar){

    printf("Sitios más visitados:\n");
    heap_t* recursos_temp = heap_crear((cmp_func_t)comparar_recursos);
    if(!recursos_temp) return;
    pasar_top_k_de_hash_a_heap(recursos_mas_solicitados, recursos_temp, cantidad_de_recursos_a_mostrar);
    mostrar_n_recursos(recursos_temp, cantidad_de_recursos_a_mostrar);
    heap_destruir(recursos_temp, NULL);
}

void mostrar_visitantes(abb_t* visitantes, char* ip_inicio, char* ip_fin){

    if(abb_cantidad(visitantes) == 0) return;
    fprintf(stdout, "Visitantes:\n");
    recorrido_arbol(visitantes, imprimir_claves, NULL, ip_inicio, ip_fin);

}



