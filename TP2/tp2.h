#ifndef ALGOS_GITHUB_TP2_H
#define ALGOS_GITHUB_TP2_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include "hash.h"
#include "lista.h"
#include "heap.h"
#include "strutil.h"
#include "abb.h"
#include "DOS.h"
#include "recursos.h"
#include "visitantes.h"
#include "comandos.h"


void iterar_hash(hash_t* hash, void visitar(const char* clave, void *dato, void *extra), void *extra);

void imprimir_error(char* comando);

int contar_cantidad_parametros(char** array);

int procesar_entrada_stdin(char* linea_entrada, abb_t* arbol_visitantes, heap_t* recursos_mas_solicitados);

void recibir_comandos(abb_t* visitantes, heap_t* recursos);

#endif //ALGOS_GITHUB_TP2_H
