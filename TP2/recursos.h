#ifndef ALGOS_GITHUB_RECURSOS_H
#define ALGOS_GITHUB_RECURSOS_H
#include "tp2.h"

/* DEFINICION DEL TIPO DE DATO RECURSO_T */
typedef struct recurso {
    char *clave;
    int cant_de_solicitudes;
}recurso_t;
/***************************************************************************/
//Crea un recurso a partir de un nombre.
//Devuelve ese recurso.
recurso_t* crear_recurso(char* nombre_recurso);

//Dado un hash, el cual contiene los recursos mas solicitados del log,
//devuelve true o false dependiendo de si la operacion de aumentar la cantidad
//de solicitudes de cierto recurso se efectuo correctamente.
bool aumenta_cont_solicitudes_recurso(hash_t* recursos_mas_solicitados, char* recurso);

//Funcion de comparacion de recurso_t.
//Devuelve 1 si el recurso1 es mayor a recurso2; -1 en viceversa; 0 si coinciden.
int comparar_recursos(recurso_t* recurso1, recurso_t* recurso2);

//Funcion encargada de destruir un recurso.
void destruir_recurso(recurso_t* recurso);

//Funcion que modifica la firma de la funcion de destruiccion de recursos
//para que sea generica y pueda ser recibida como parametros por otras funciones.
void wrapper_destruir_recurso(void* dato);

#endif //ALGOS_GITHUB_RECURSOS_H
