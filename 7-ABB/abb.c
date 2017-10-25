#define _POSIX_C_SOURCE 200809L

#include "abb.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ******************************************************************
 *                CREACION DE LOS TIPOS DE DATOS                    *
 * *****************************************************************/

typedef struct nodo_abb{
	const char* clave;
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
	pila_t* pila;
	nodo_abb_t* actual;
};

/* ******************************************************************
 *                       FUNCIONES PRIVADAS                         *
 * *****************************************************************/
nodo_abb_t* crear_nodo(const char* clave, void* valor) {

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

// Nodo fue creado.
// Destruye la clave del nodo y el nodo.
// Post: Devuelve el dato.
void* destruir_nodo(nodo_abb_t* nodo) {

    void* dato_aux = nodo->valor;
    free((char*)nodo->clave);
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
            nodo_abb_t* nodo_aux = crear_nodo(clave, dato);
            if (!nodo_aux) return false;
            nodo->der = nodo_aux;
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
            nodo_abb_t* nodo_aux = crear_nodo(clave, dato);
            if (nodo_aux == NULL) return false;
            nodo->izq = nodo_aux;
            arbol->cantidad++;
        }
    }
    return true;
}


// Devuelve el nodo y no el valor asi despues puedo usar la funcion tanto para abb_borrar como para obtener.
nodo_abb_t* buscar_nodo_por_clave(const abb_t* arbol, nodo_abb_t* nodo, const char* clave) {

    if (arbol->cmp(clave, nodo->clave) == 0) return nodo;
    else if (arbol->cmp(clave, nodo->clave) > 0) {
        if (nodo->der != NULL) {
            return buscar_nodo_por_clave(arbol, nodo->der, clave);
        }
        else {
            return NULL;
        }
    }
    else {
        if (nodo->izq != NULL) {
            return buscar_nodo_por_clave(arbol, nodo->izq, clave);
        }
        else {
            return NULL;
        }
    }
}

void destruir_post_order(nodo_abb_t* nodo, abb_destruir_dato_t destruir_dato){
	
	if(!nodo) return;
	destruir_post_order(nodo->izq,destruir_dato);
	destruir_post_order(nodo->der,destruir_dato);
    void* dato = destruir_nodo(nodo);
	if(destruir_dato) destruir_dato(dato);

}
//Devuelve la cantidad de hijos de un nodo. 0 si es una "hoja"
size_t cantidad_de_hijos(nodo_abb_t* nodo) {

    if (nodo->izq && nodo->der) return 2;
    if (nodo->izq || nodo->der) return 1;
    return 0;
}

//Intercambia el nodo que va a tomar el lugar del borrado con el borrado.
void swapear_nodos(nodo_abb_t* nodo1, nodo_abb_t* nodo2){
	
	const char* clave_2 = nodo2->clave;
	void* valor_2 = nodo2->valor;
	nodo2->clave = nodo1->clave;
	nodo2->valor = nodo1->valor;
	nodo1->clave = clave_2;
	nodo1->valor = valor_2;
}

// Pre: nodo existe
nodo_abb_t* buscar_nodo_minimo(nodo_abb_t* nodo){
	
	if (nodo == NULL) return NULL;
    buscar_nodo_minimo(nodo->izq);
    return nodo;
}
// Pre: el nodo buscado existe y tiene un padre.
// Busca al padre del nodo correspondiente a la clave pasada por parametro.
nodo_abb_t* buscar_nodo_padre(abb_t* arbol, nodo_abb_t* nodo, const char* clave) {
    if (arbol->cmp(clave, nodo->clave) < 0) {
        if (arbol->cmp(clave, nodo->izq->clave) == 0) {
            return nodo;
        }
        return buscar_nodo_padre(arbol, nodo->izq, clave);
    }
    else {
        if (arbol->cmp(clave, nodo->der->clave) == 0) {
            return nodo;
        }
        return buscar_nodo_padre(arbol, nodo->der, clave);
    }
}

bool es_raiz(abb_t* arbol, nodo_abb_t* nodo) {
    return arbol->cmp(arbol->raiz->clave, nodo->clave) == 0;
}

// Funcion generica de borrar, se usa para todos los casos. (0,1 y 2 hojas)
void* borrar(abb_t* arbol, nodo_abb_t* nodo, const char* clave, nodo_abb_t* reemplazo, nodo_abb_t* padre) {
    if (padre != NULL) {
        if (padre->izq != NULL && arbol->cmp(padre->izq->clave, clave) == 0) {
            padre->izq = reemplazo;
        } else { // Si llega aca, padre->izq no existe o no es el nodo buscado, entonces padre->der debe ser el nodo buscado.
            padre->der = reemplazo;
        }
    } else { // Si es raiz
        arbol->raiz = reemplazo;
    }
    void* dato_aux = destruir_nodo(nodo);
    arbol->cantidad--;
    return dato_aux;
}

void* borrar_dos_hijos(abb_t* arbol, const char* clave, nodo_abb_t* nodo) {
    nodo_abb_t* nodo_sucesor = buscar_nodo_minimo(nodo->der);
    // Busco el padre ahora antes de swapear porque despues no lo voy a poder encontrar
    // debido a la alteracion momentanea del invariante del abb.
    nodo_abb_t* padre_sucesor = buscar_nodo_padre(arbol, arbol->raiz, nodo_sucesor->clave);
    swapear_nodos(nodo, nodo_sucesor);
    // Al ser minimo va a tener o 0 o 1 hijo, por lo que no tiene sentido chequear si tiene 2 hijos.
    if (nodo_sucesor->izq != NULL) {
        return borrar(arbol, nodo_sucesor, clave, nodo_sucesor->izq, padre_sucesor);
    } else if (nodo_sucesor->der != NULL){
        return borrar(arbol, nodo_sucesor, clave, nodo_sucesor->der, padre_sucesor);
    }
    return borrar(arbol, nodo_sucesor, clave, NULL, padre_sucesor);
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

    if (arbol == NULL) return false;
    if (arbol->cantidad == 0) {
        nodo_abb_t *nodo_aux = crear_nodo(clave, dato);
        if (nodo_aux == NULL) return false;
        arbol->raiz = nodo_aux;
        arbol->cantidad++;
        return true;
    } else {
        return abb_insertar(arbol, arbol->raiz, clave, dato);
    }
}

void *abb_obtener(const abb_t *arbol, const char *clave) {

    if (!arbol || arbol->cantidad == 0) return NULL;
    nodo_abb_t* nodo_aux = buscar_nodo_por_clave(arbol, arbol->raiz, clave);
    return (nodo_aux == NULL ? NULL : nodo_aux->valor);
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
    
    if (!arbol || arbol->cantidad == 0) return false;
    return buscar_nodo_por_clave(arbol, arbol->raiz, clave) != NULL;
}

size_t abb_cantidad(abb_t* arbol){
	
	return arbol->cantidad;
}

void abb_destruir(abb_t *arbol) {

    // Se destruye al arbol usando internamente un recorrido post-order para asi eliminar siempre hojas.
    if(!arbol) return;
    destruir_post_order(arbol->raiz, arbol->destruir_dato);
    free(arbol);
}

void *abb_borrar(abb_t *arbol, const char *clave) {
	
	if(!arbol || arbol->cantidad == 0) return NULL;
	nodo_abb_t* nodo_a_borrar = buscar_nodo_por_clave(arbol,arbol->raiz, clave);
    if (nodo_a_borrar == NULL) return NULL;
    nodo_abb_t* padre = NULL;
    size_t cantidad_hijos = cantidad_de_hijos(nodo_a_borrar);
	if(cantidad_hijos == 0) {
        if (!es_raiz(arbol, nodo_a_borrar)) {
            padre = buscar_nodo_padre(arbol, arbol->raiz, clave);
        }
        // Si es raiz entonces padre va a ser NULL.
        return borrar(arbol,nodo_a_borrar, clave, NULL, padre);
    }
	else if(cantidad_hijos == 1) {
        if (!es_raiz(arbol, nodo_a_borrar)) {
            padre = buscar_nodo_padre(arbol, arbol->raiz, clave);
        }
		if (nodo_a_borrar->izq == NULL) {
            return borrar(arbol,nodo_a_borrar, clave, nodo_a_borrar->der,padre);
        }
        return borrar(arbol,nodo_a_borrar, clave, nodo_a_borrar->izq, padre);
	}
    else {
        return borrar_dos_hijos(arbol, clave, nodo_a_borrar);
    }
}



/* ******************************************************************
 *                 IMPLEMENTACION ITERADOR INTERNO                  *
 * *****************************************************************/
/* FUNCION AUXILIAR RECURSIVA QUE ITERA INTERNAMENTE POR EL ARBOL CON RECORRIDO
INORDER*/

//Recorre el arbol con recorrido inorder, aplicando la funcion visitar a cada
//nodo.
void iterador_inorder(nodo_abb_t* nodo, bool visitar(const char *, void *, void *),void* extra){
	
	if(!nodo) return;
	iterador_inorder(nodo->izq,visitar,extra);
	if (!visitar(nodo->clave,nodo->valor,extra)) return;
	iterador_inorder(nodo->der,visitar,extra);
}


void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	
	if(!arbol) return;
	if(!visitar) return;
	iterador_inorder(arbol->raiz,visitar,extra);
}

/* ******************************************************************
 *                  IMPLEMENTACION ITERADOR EXTERNO                 *
 * *****************************************************************/
//FUNCION AUXILIAR ENCARGADA DE APILAR LOS HIJOS IZQUIERDOS DE UN NODO Y SU RAIZ

void apilar_nodos_izq(abb_iter_t* iter, nodo_abb_t* nodo){

	if(!nodo) return;
	pila_apilar(iter->pila, nodo);
	apilar_nodos_izq(iter,nodo->izq);

}

//Crea un iterador para el arbol.
//PRE : el arbol fue creado.
//POST: devuelve el iterador.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol){ //APILAR RAIZ Y TODOS LOS H.IZQ
	
	if(!arbol) return NULL;
	abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	if(!iter) return NULL;
	pila_t* pila = pila_crear();
    if (!pila) return NULL;
	iter->pila = pila;
	nodo_abb_t* nodo = arbol->raiz;
	apilar_nodos_izq(iter,nodo);
	iter->actual = pila_desapilar(iter->pila);
	return iter;
}

//Avanza una posicion sobre el arbol en recorrido inorder.
//PRE: el iterador fue creado.
//POST : devuelve un booleano.
bool abb_iter_in_avanzar(abb_iter_t *iter) {

	if(abb_iter_in_al_final(iter)) return false;
	if(iter->actual->der){
		apilar_nodos_izq(iter,iter->actual->der);
	}
    iter->actual = pila_desapilar(iter->pila);
	return true;
}

//Devuelve un booleano segun si el iterador esta al
//final del arbol o no.
bool abb_iter_in_al_final(const abb_iter_t *iter){
	
	return(pila_esta_vacia(iter->pila));
}

//Devuelve la clave a la que apunta el iterador.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    if (pila_esta_vacia(iter->pila)) return NULL;

	return((nodo_abb_t*)pila_ver_tope(iter->pila))->clave;
}

//Destruye el iterador y su pila.
void abb_iter_in_destruir(abb_iter_t* iter){
	
	pila_destruir(iter->pila);
	free(iter);
}


