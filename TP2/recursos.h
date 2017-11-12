#ifndef ALGOS_GITHUB_RECURSOS_H
#define ALGOS_GITHUB_RECURSOS_H

#include "tp2.h"

typedef struct recurso {
    char *clave;
    int cant_de_solicitudes;
}recurso_t;

void pasar_recursos_de_hash_a_heap(const char* clave, void* recurso, void* heap);

bool aumenta_cont_solicitudes_recurso(hash_t* temp_recursos_solicitados, char* recurso);

int comparar_recursos(recurso_t* recurso1, recurso_t* recurso2);

recurso_t* crear_recurso(char* nombre_recurso);

#endif //ALGOS_GITHUB_RECURSOS_H
