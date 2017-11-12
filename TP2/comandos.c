#define _XOPEN_SOURCE 700

#include "comandos.h"

#define TIME_FORMAT "%FT%T%z"

bool procesar_log(char* nombre_de_archivo, heap_t* recursos_mas_solicitados, abb_t* solicitantes) {
    FILE* archivo = fopen(nombre_de_archivo, "r");
    char *linea = NULL;
    size_t capacidad = 0;
    ssize_t leidos;

    hash_t* peticiones_por_ip = hash_crear((hash_destruir_dato_t)lista_destruir);
    if (peticiones_por_ip == NULL) return false;
    hash_t* temp_recursos_solicitados = hash_crear(NULL);
    if (temp_recursos_solicitados == NULL) return false;


    while ((leidos = getline(&linea, &capacidad, archivo)) > 0) {
        quitar_caracter_new_line(linea);
        char **linea_a_procesar = split(linea, '\t');
        char *ip = linea_a_procesar[0];
        char *nombre_recurso = linea_a_procesar[3];
        time_t* instante = malloc(sizeof(time_t));
        *instante = iso8601_to_time(linea_a_procesar[1]);

        abb_guardar(solicitantes, ip, NULL);
        agregar_fecha_de_solicitud(ip, instante, peticiones_por_ip);
        aumenta_cont_solicitudes_recurso(temp_recursos_solicitados, nombre_recurso);
    }
    // Aca tambien faltaria un wrapper para pasar_recursos_de_hash_a_heap
    iterar_hash(temp_recursos_solicitados, pasar_recursos_de_hash_a_heap, recursos_mas_solicitados);
    iterar_hash(peticiones_por_ip, detectar_DOS, NULL);
    free(linea);
    fclose(archivo);

    return true;
}

//Muestra los "N" sitios mas visitados de la pagina.
void mostrar_mas_visitados(heap_t* recursos_mas_solicitados, int cantidad_de_recursos_a_mostrar){

    printf("Sitios mas visitados:\n");

    mostrar_n_recursos(recursos_mas_solicitados, cantidad_de_recursos_a_mostrar);
}

void mostrar_n_recursos(heap_t* recursos_mas_solicitados, int cantidad_de_recursos_a_mostrar) {
    if(cantidad_de_recursos_a_mostrar == 0) return;

    recurso_t* sitio_visitado = (recurso_t*)heap_desencolar(recursos_mas_solicitados);
    if (sitio_visitado == NULL) return;

    printf("\t%s - %d\n", sitio_visitado->clave, sitio_visitado->cant_de_solicitudes);
    mostrar_n_recursos(recursos_mas_solicitados,cantidad_de_recursos_a_mostrar-1);
    heap_encolar(recursos_mas_solicitados, sitio_visitado);
}


//Itera a traves del arbol, imprimiendo los visitantes en el rango
//que vaa desde "ip_desde" a "ip_hasta". (MODIFICAR, HAY QUE APILAR SOLO
//LOS NODOS PERTENECIENTES AL RANGO.)

void mostrar_visitantes(abb_t* arbol_visitantes, char* ip_inicio, char* ip_fin){

    if(abb_cantidad(arbol_visitantes) == 0) return;
    fprintf(stdout, "Visitantes: \n");
    recorrido_arbol(arbol_visitantes, imprimir_claves, NULL, ip_inicio, ip_fin);

}

void quitar_caracter_new_line(char* cadena) {
    int i = 0;
    while (cadena[i] != '\0') {
        if (cadena[i] == '\n') {
            cadena[i] = '\0';
        }
        i++;
    }
}

