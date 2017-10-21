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
		free(nodo);
		return NULL;
	}
	char* clave_auxiliar = strdup(clave);  //LAS CLAVES SE COPIAN DINAMICAMENTE ?????????
	if(!clave_auxiliar){
		free(clave_auxiliar);
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
	arbol->cantidad = 0;
	abb->cmp= cmp;
	abb->destruir_dato = destruir_dato;
	return abb;
}

