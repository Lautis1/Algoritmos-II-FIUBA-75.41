#ifndef ALGOS_GITHUB_RECURSOS_H
#define ALGOS_GITHUB_RECURSOS_H

#include "tp2.h"


typedef struct recurso {
    char *clave;
    int cant_de_solicitudes;
}recurso_t;


/***************************************************************************/

// Crea un recurso_t con la cadena pasada por parametro.
// Post: Devuelve un elemento del tipo recurso_t si se
// creo sin problemas, NULL en caso de que algo halla fallado
recurso_t* crear_recurso(char* nombre_recurso);

// Pre: recursos_mas_solicitado fue creado
// Dado un hash, el cual contiene el nombre de un recurso como clave y un recurso_t,
// asociado a esa clave como valor. Aumenta en uno el numero de solicitudes de dicho recurso.
// Post: devuelve true o false dependiendo de si la operacion se efectuo correctamente. Se
// aumento en uno el contador de solicitudes de dicho recurso.
bool aumenta_cont_solicitudes_recurso(hash_t* recursos_mas_solicitados, char* recurso);

// Pre: recurso1 y recurso2 fueron creados
// Funcion de comparacion de recurso_t, hecho para un heap de minimos, por lo que los
// valores de retorno estan invertidos.
// Post: Devuelve -1 si el recurso1 es mayor a recurso2; 1 en viceversa; 0 si coinciden.
int comparar_recursos(recurso_t* recurso1, recurso_t* recurso2);

// Pre: recurso fue creado.
// Destruye el recurso recibido por parametro.
// Post: recurso fue destruido.
void destruir_recurso(recurso_t* recurso);

// Funcion que adapta la firma de la funcion de destruccion de recursos
// para que sea generica y pueda ser recibida como parametros por otras funciones.
void wrapper_destruir_recurso(void* dato);

//Dado un heap temporal de recursos y una cantidad "N", imprime por pantalla los
//N sitios mas visitados de la pagina.
void mostrar_n_recursos(heap_t* recursos_temp, int cantidad_de_recursos_a_mostrar);

// Pasa los top k elementos de hash hacia un heap de minimos con cantidad de elementos igual a k.
void pasar_top_k_de_hash_a_heap(hash_t* hash, heap_t* heap, int k);

#endif //ALGOS_GITHUB_RECURSOS_H
