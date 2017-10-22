#define _POSIX_C_SOURCE 200809L

#include "abb.h"
#include <stdio.h>
#include <stdlib.h>

/* ******************************************************************
 *                CREACION DE LOS TIPOS DE DATOS                    *
 * *****************************************************************/

typedef struct nodo_abb{
	char* clave;
	void* valor;
	struct nodo_abb* izq;
	struct nodo_abb* der;
}nodo_abb_t;

struct abb{
	nodo_abb_t* raiz;
	abb_comparar_clave_t cmp;
	abb_destruir_dato_t destruir_dato;
	size_t cantidad;
};

struct abb_iter{
	abb_t* arbol;
};

/* ******************************************************************
 *                       FUNCIONES AUXILIARES                       *
 * *****************************************************************/
nodo_abb_t crear_nodo(char* clave, void* valor){
	nodo_abb_t* nodo = malloc(sizeof(nodo_abb_t));
	if(!nodo){
		return NULL;
	}
    char* clave_auxiliar = strdup(clave);
    if(!clave_auxiliar){
        return NULL;
    }
    nodo->clave = clave_auxiliar;
	nodo->valor = valor;
	nodo->izq = NULL;
	nodo->der = NULL;
	return nodo;
}
/* ******************************************************************
 *                       PRIMITIVAS DEL ABB                         *
 * *****************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t* abb = malloc(sizeof(abb_t));
	if(!abb) return NULL;
	abb->raiz = NULL;
	abb->cantidad = 0;
	abb->cmp= cmp;
	abb->destruir_dato = destruir_dato;
	return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
    if (arbol == NULL) return false;

    if (arbol->cantidad == 0) {
        nodo_abb_t* nodo_aux = crear_nodo(clave, dato);
        if (nodo_aux == NULL) return false;

        arbol->raiz = nodo_aux;
        arbol->cantidad++;

        return true;
    }
    return abb_insertar(arbol, arbol->raiz, clave, dato);
}

bool abb_insertar(abb_t* arbol, nodo_abb_t* nodo, const char* clave, void* dato) {
    // Ya se chequeo si el arbol existe en abb_guardar, no hace revisar de nuevo.
    if (arbol->cmp(clave, nodo->clave) == 0) {
        if (arbol->destruir_dato) {
            arbol->destruir_dato(nodo->valor);
        }
        nodo->valor = dato;
    }
    //Si me tengo que mover a la derecha es decir la clave que quiero insertar es mayor a la clave del nodo actual.
    else if (arbol->cmp(clave, nodo->clave) > 0) {
        if (nodo->der != NULL) {
            return abb_insertar(arbol, nodo->der, clave, dato);
        }
        else {
            nodo_abb_t* nodo_aux = crear_nodo(clave, valor);
            if (nodo_aux == NULL) return false;

            nodo->der = nodo_aux;
            arbol->cantidad++;
        }
    }
    // Si me tengo que mover a la izq.
    else {
        if (nodo->izq != NULL) {
            return abb_insertar(arbol, nodo->izq, clave, dato);
        }
        else {
            nodo_abb_t* nodo_aux = crear_nodo(clave, valor);
            if (nodo_aux == NULL) return false;

            nodo->der = nodo_aux;
            arbol->cantidad++;
        }
    }
}
