#define _XOPEN_SOURCE 700

#include "recursos.h"
/***************************************************************************************/
//Crea un recurso a partir de un nombre.
//Devuelve ese recurso.
recurso_t* crear_recurso(char* nombre_recurso) {
    recurso_t *recurso = malloc(sizeof(recurso_t));
    if (recurso== NULL) return NULL;

    recurso->clave = strdup(nombre_recurso);
    recurso->cant_de_solicitudes = 0;

    return recurso;
}

bool aumenta_cont_solicitudes_recurso(hash_t* recursos_mas_solicitados, char* nombre_recurso) {

    if (!hash_pertenece(recursos_mas_solicitados, nombre_recurso)) {

        recurso_t* recurso = crear_recurso(nombre_recurso);
        if (recurso == NULL) return false;
        hash_guardar(recursos_mas_solicitados, nombre_recurso, recurso);
    }
    ((recurso_t *) (hash_obtener(recursos_mas_solicitados, nombre_recurso)))->cant_de_solicitudes++;

    return true;
}

int comparar_recursos(recurso_t* recurso1, recurso_t* recurso2){

    //Comparo la cantidad de solicitudes de cada recurso
    int valor_retorno;
    int cantidad_1 = recurso1->cant_de_solicitudes;
    int cantidad_2 = recurso2->cant_de_solicitudes;
    if(cantidad_1 < cantidad_2) valor_retorno = 1;
    else if(cantidad_1 > cantidad_2) valor_retorno = -1;
    else valor_retorno = 0;
    return valor_retorno;
}

//Funcion encargada de destruir un recurso.
void destruir_recurso(recurso_t* recurso) {
    free(recurso->clave);
    free(recurso);
}

//Funcion que modifica la firma de la funcion de destruiccion de recursos
//para que sea generica y pueda ser recibida como parametros por otras funciones.
void wrapper_destruir_recurso(void* dato) {
    recurso_t* recurso = dato;
    destruir_recurso(recurso);
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


void pasar_top_k_de_hash_a_heap(hash_t* hash, heap_t* heap, int k) {

    hash_iter_t* iter_hash = hash_iter_crear(hash);
    if(iter_hash == NULL) return;

    for (int i = 0; i < k && !hash_iter_al_final(iter_hash); i++) {
        char* clave_actual = (char*)hash_iter_ver_actual(iter_hash);
        recurso_t* recurso_actual = hash_obtener(hash, clave_actual);
        heap_encolar(heap, recurso_actual);
        hash_iter_avanzar(iter_hash);
    }
    while (!hash_iter_al_final(iter_hash)) {
        char* clave_actual = (char*)hash_iter_ver_actual(iter_hash);
        recurso_t* recurso_actual = hash_obtener(hash, clave_actual);
        if (comparar_recursos((recurso_t*)heap_ver_max(heap), recurso_actual) > 0) {
            heap_desencolar(heap);
            heap_encolar(heap, recurso_actual);
        }
        hash_iter_avanzar(iter_hash);
    }
    hash_iter_destruir(iter_hash);
}
