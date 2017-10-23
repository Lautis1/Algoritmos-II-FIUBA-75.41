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
 *                       FUNCIONES PRIVADAS                         *
 * *****************************************************************/
nodo_abb_t* crear_nodo(char* clave, void* valor){
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

//Quizas haya que modificar esto, porque despues se va a usar en abb_destruir y no esta bueno que devuelva el valor.
//Si yo llamo a la funcion y no hago nada con el valor que me devuelve la funcion, el compilador me va a gritar.
void* destruir_nodo(abb_t* arbol, nodo_abb_t* nodo) {
    void* dato_aux = nodo->valor;
    free(nodo->clave);
    if (arbol->destruir_dato) {
        arbol->destruir_dato(nodo->dato);
    }
    free(nodo);
    return dato_aux;
}

bool abb_insertar(abb_t* arbol, nodo_abb_t* nodo, const char* clave, void* dato) {
    // Ya se chequeo si el arbol existe en abb_guardar, no hace falta revisar de nuevo.
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

// Devuelve el nodo y no el valor asi despues puedo usar la funcion para el abb_borrar
nodo_abb_t* buscar_por_clave(abb_t* arbol, nodo_abb_t* nodo, char* clave) {
    if (arbol->cmp(clave, nodo->clave) == 0) return nodo;

    else if (arbol->cmp(clave, nodo->clave) > 0) {
        if (nodo->der != NULL) {
            return buscar_por_clave(arbol, nodo->der, clave);
        }
        else {
            return NULL;
        }
    }
    else {
        if (nodo->izq != NULL) {
            return buscar_por_clave(arbol, nodo->izq, clave);
        }
        else {
            return NULL;
        }
    }
}

/* ******************************************************************
 *                       PRIMITIVAS DEL ABB                         *
 * *****************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t* arbol = malloc(sizeof(abb_t));
	if(!arbol) return NULL;
	arbol->raiz = NULL;
	arbol->cantidad = 0;
	arbol->cmp= cmp;
	arbol->destruir_dato = destruir_dato;
	return arbol;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
    //abb_guardar estaria funcionando como una especie de wrapper para abb_insertar
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

void *abb_obtener(const abb_t *arbol, const char *clave) {
    if (!arbol || arbol->cantidad == 0) return NULL;

    return buscar_por_clave(arbol, arbol->raiz, clave);
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
    if (!arbol || arbol->cantidad == 0) return false;

    return buscar_por_clave(arbol, arbol->raiz, clave) != NULL;
}

void abb_destruir(abb_t *arbol) {
    // Se destruye al arbol usando internamente un recorrido post-order para asi eliminar siempre hojas y no tener que
    // lidiar con problemas de nodos con hijos y cosas asi.

}
