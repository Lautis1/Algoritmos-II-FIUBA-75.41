#define _XOPEN_SOURCE 700

#include <stdio.h>
#include "heap.h"
#include "abb.h"
#include "visitantes.h"
#include "recursos.h"
#include "tp2.h"

int main() {

    abb_t* visitantes = abb_crear((abb_comparar_clave_t)comparar_ips, NULL);

    heap_t* recursos = heap_crear((cmp_func_t)comparar_recursos);

    recibir_comandos(visitantes, recursos);

    // Destruir TODO

    return 0;
}