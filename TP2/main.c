#define _XOPEN_SOURCE 700

#include <stdio.h>
#include "heap.h"
#include "abb.h"
#include "visitantes.h"
#include "recursos.h"
#include "tp2.h"

int main() {

    abb_t* visitantes = abb_crear((abb_comparar_clave_t)comparar_ips, NULL);

    hash_t* recursos = hash_crear(wrapper_destruir_recurso);

    recibir_comandos(visitantes, recursos);

    hash_destruir(recursos);
    abb_destruir(visitantes);

    return 0;
}