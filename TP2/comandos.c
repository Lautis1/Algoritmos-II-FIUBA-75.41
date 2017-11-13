#define _XOPEN_SOURCE 700

#include "comandos.h"

#define TIME_FORMAT "%FT%T%z"
/***********************************************************************************************/
//Funcion que recibe un archivo y lo procesa, agregando las direcciones IP de los visitantes
//del log a un ABB.
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
    iterar_hash(peticiones_por_ip, detectar_DOS, NULL);
    hash_destruir(peticiones_por_ip);
    free(linea);
    fclose(archivo);

    return true;
}

/*FUNCION AUXILIAR*/
//Funcion auxiliar que actua como "visitar" en iterar_hash.
//Compara dos recursos, los cuales se encuentran en un heap de MINIMOS.
void func_visitar(const char* nombre_recurso, void* dato, void* heap){

    if(comparar_recursos(dato, heap_ver_max(heap)) < 0){
        heap_desencolar(heap);
        heap_encolar(heap, (recurso_t*)dato);
    }
}

//Funcion que funciona de manera levemente similar al "top_K": recibe un hash y un entero a procesar.
//Crea un heap de minimos y va encolando N elementos del heap
void procesar_n_a_mostrar(heap_t* min_heap, hash_t* recursos_mas_solicitados, int n){

    hash_iter_t* iter = hash_iter_crear(recursos_mas_solicitados);
    if(!iter) return;
    while(!hash_iter_al_final(iter) && n > 0){
        const char* clave = hash_iter_ver_actual(iter);
        void* dato = hash_obtener(recursos_mas_solicitados, clave);
        heap_encolar(min_heap, dato);
        hash_iter_avanzar(iter);
        n--;
    }
    hash_iter_destruir(iter);
}

//Obtiene los "N" sitios mas visitados de la pagina, los cuales se encuentran en un hash,
//pero son trasladados a un heap de minimos.
void mostrar_mas_visitados(hash_t* recursos_mas_solicitados, int cantidad_de_recursos_a_mostrar){

    printf("Sitios mas visitados:\n");
    heap_t* recursos_temp = heap_crear((cmp_func_t)comparar_recursos);
    if(!recursos_temp) return;
    procesar_n_a_mostrar(recursos_temp, recursos_mas_solicitados, cantidad_de_recursos_a_mostrar);
    iterar_hash(recursos_mas_solicitados,func_visitar,recursos_temp);
    mostrar_n_recursos(recursos_temp, cantidad_de_recursos_a_mostrar);
    heap_destruir(recursos_temp, NULL);
}

//Dado un heap temporal de recursos y una cantidad "N", imprime por pantalla los
//N sitios mas visitados de la pagina.
void mostrar_n_recursos(heap_t* recursos_temp, int cantidad_de_recursos_a_mostrar) {
    
    if(cantidad_de_recursos_a_mostrar == 0) return;
    recurso_t* sitio_visitado = (recurso_t*)heap_desencolar(recursos_temp);
    if (sitio_visitado == NULL) return;
    mostrar_n_recursos(recursos_temp,cantidad_de_recursos_a_mostrar-1);
    printf("\t%s - %d\n", sitio_visitado->clave, sitio_visitado->cant_de_solicitudes);
}


//Recibe el arbol que contiene a los visitantes de la pagina y dos direcciones IP.
//Llama a una funcion que recorre el arbol y, a medida que va comparando si las
//direcciones de los visitantes pertenecen al rango conformado entre las 2 ip's 
//recibidas por parametro, va imprimiendolas por pantalla.
void mostrar_visitantes(abb_t* visitantes, char* ip_inicio, char* ip_fin){

    if(abb_cantidad(visitantes) == 0) return;
    fprintf(stdout, "Visitantes:\n");
    recorrido_arbol(visitantes, imprimir_claves, NULL, ip_inicio, ip_fin);

}

//Recibe una cadena y reemplaza el caracter de salto de linea
//por el caracter de fin de cadena.
void quitar_caracter_new_line(char* cadena) {
    
    int i = 0;
    while (cadena[i] != '\0') {
        if (cadena[i] == '\n') {
            cadena[i] = '\0';
        }
        i++;
    }
}

