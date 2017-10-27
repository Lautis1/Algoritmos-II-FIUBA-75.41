#include "lista.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

//ALUMNO = STROIA, LAUTARO E. // PADRON 100901 // CORRECTORA ANA CZARNITZKI

/* DEFINICION DEL STRUCT NODO */
typedef struct nodo{
	void* dato;
	struct nodo* proximo;
}nodo_t;

/* DEFINICION DEL STRUCT LISTA */
struct lista{
	nodo_t* primero;
	nodo_t* ultimo;
	size_t largo;
};

/* DEFINICION DEL STRUCT LISTA_ITER */
struct lista_iter{
	nodo_t* actual;
	nodo_t* anterior;
	lista_t* lista;
};

/********************************************************************
 *                    PRIMITIVAS DE LA LISTA                        *
 *******************************************************************/

 // Crea una lista.
// Post: devuelve una nueva lista vacia.
lista_t *lista_crear(void){
	lista_t* lista = malloc(sizeof(lista_t));
	if(!lista) return NULL;
	lista->primero = NULL;
	lista->ultimo = NULL;
	lista->largo = 0;
	return lista;
}

// Devuelve true en caso de que la lista tenga elementos; false en
// caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista){
	return (!lista->largo);
}

// Agrega un nuevo elemento en la primera posicion de la lista. 
// Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: Se agrego un nuevo elemento a la lista; dato se encuentra al principio
// de la lista. Se aumenta en 1 unidad el largo de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(!nodo) return false;
	nodo->dato = dato;
	if(lista_esta_vacia(lista)){
		lista->ultimo = nodo;
		nodo->proximo = NULL;
	}
	else{
		nodo->proximo = lista->primero;
	}
	lista->primero = nodo;
	lista->largo++;
	return true;
}

// Agrega un nuevo elemento en la ultima posicion de la lista. 
// Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: Se agrego un nuevo elemento a la lista, dato se encuentra al final
// de la lista. Se aumenta en 1 unidad el largo de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(!nodo) return false;
	nodo->dato = dato;
	nodo->proximo = NULL;
	if(lista_esta_vacia(lista)) lista->primero = nodo;
	else{
		lista->ultimo->proximo = nodo;
	}
	lista->ultimo = nodo;
	lista->largo++;
	return true;
}

// Borra el primer elemento de la lista y lo devuelve. Si esta vacia, devuelve NULL.
// Pre: la lista fue creada.
// Post: Se borra devuelve el primer elemento de la lista cuando no esta vacia.
// Se disminuye en 1 unidad el largo de la lista.
void *lista_borrar_primero(lista_t *lista){
	if(lista_esta_vacia(lista)) return NULL;
	nodo_t* proximo = lista->primero->proximo;
	void* dato = lista->primero->dato;
	free(lista->primero);
	lista->primero = proximo;
	lista->largo--;
	return dato;
}

// Devuelve el valor del primer elemento de la lista. Si esta vacia, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvio el primer elemento de la lista. 
void *lista_ver_primero(const lista_t *lista){
	if(lista_esta_vacia(lista)) return NULL;
	return lista->primero->dato;
}

// Devueve el valor del ultimo elemento de la lista. Si esta vacia, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvio el ultimo elemento de la lista.
void *lista_ver_ultimo(const lista_t* lista){
	if(lista_esta_vacia(lista)) return NULL;
	return lista->ultimo->dato;
}

// Devuelve la cantidad de elementos que contiene la lista.
// Pre: la lista fue creada.
// Post: devuelve el largo de la lista.
size_t lista_largo(const lista_t *lista){
	return lista->largo;
}

// Destruye la lista. Si se recibe la funcion destruir_dato por parametro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. Destruir_dato es una funcion capaz de destruir
// los datos de la lista,o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *)){
	while(lista->primero){
		void* dato = lista->primero->dato;
		if(destruir_dato!=NULL)
			destruir_dato(dato);
		nodo_t* prox = lista->primero->proximo;
		free(lista->primero);
		lista->primero = prox;
	}
	free(lista);
}


/********************************************************************
 *                     PRIMITIVAS DEL ITERADOR                      *
 *******************************************************************/
//ITERADOR INTERNO

// Itera la lista de principio a fin.Si visitar no es NULL, se la aplica
// a cada elemento.
// Pre: la lista fue creada.
// Post: Itera la lista de principio a fin o hasta que visitar lo detiene.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
	if(lista_esta_vacia(lista)) return;
	nodo_t* actual = lista->primero;
	while(actual && visitar(actual->dato,extra)){
		actual = actual->proximo;
	}
}

//ITERADOR EXTERNO

// Crea un iterador.
// Pre: la lista fue creada. 
// Post: devuelve un iterador con actual apuntando al primer nodo de la lista.
lista_iter_t *lista_iter_crear(lista_t *lista){
	lista_iter_t* iter =  malloc(sizeof(lista_iter_t));
	if(!iter) return NULL;
	iter->lista = lista;
	iter->anterior = NULL;
	if(lista_esta_vacia(lista)){
		iter->actual = NULL;
	}
	iter->actual = lista->primero;
	return iter;
}

// Avanza una posicion sobre la lista. 
// Pre: el iterador fue creado.
// Post: Devuele true si la posicion siguiente a actual no es NULL.
// En caso contrario, false.
bool lista_iter_avanzar(lista_iter_t *iter){
	if(!iter || !iter->actual) return false;
	iter->anterior = iter->actual;
	iter->actual = iter->actual->proximo;
	return true;
}

// Devuelve el elemento actual en donde el iterador esta parado.
// Pre: el iterador fue creado.
// Post: devuelve el elemento actual del iterador.
void *lista_iter_ver_actual(const lista_iter_t *iter){
	if(!iter->actual) return NULL;
	return(iter->actual->dato);
}

// Devuelve un booleano segun si el iterador esta al final de la lista.
// Pre: el iterador fue creado.
// Post: devuelve true si esta al final de la lista, false en caso contrario.
bool lista_iter_al_final(const lista_iter_t *iter){
	return(iter->actual == NULL);
}

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: se elimina el iterador.
void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
}

// Inserta un elemento en la posicion actual del iterador.
// Pre: el iterador fue creado.
// Post: dato esta en la posicion actual del iterador.
// Devuelve true si dato se inserto correctamente; false en caso contrario.
bool lista_iter_insertar(lista_iter_t *iter, void *dato){
	if(!iter) return false;
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(!nodo) return false;
	nodo->dato = dato;
	nodo->proximo = iter->actual;
	if(lista_esta_vacia(iter->lista)){
		iter->lista->primero = nodo;
		iter->lista->ultimo = nodo;
	}
	if(!iter->anterior && iter->actual) iter->lista->primero = nodo;
	if(iter->anterior && iter->actual) iter->anterior->proximo = nodo;
	if(!iter->actual && iter->anterior){
		iter->lista->ultimo = nodo;
		iter->anterior->proximo = nodo;}
	 
	iter->actual = nodo;
	iter->lista->largo++;
	return true;
}

// Borra el elemento de la posicion actual del iterador.
// Pre: el iterador fue creado.
// Post: Borra y devuelve un elemento de la lista, actual pasa a ser
// el siguiente del elemento borrado. Devuelve NULL si paso el final o esta vacia.
void *lista_iter_borrar(lista_iter_t *iter){
	if(iter->actual==NULL) return NULL;
	void* dato = iter->actual->dato;
	nodo_t* auxiliar = iter->actual;
	if(!iter->anterior) iter->lista->primero = iter->actual->proximo;
	if(!iter->actual->proximo) iter->lista->ultimo = iter->anterior;
	if(iter->anterior) iter->anterior->proximo = iter->actual->proximo;
	iter->actual = iter->actual->proximo;
	free(auxiliar);
	iter->lista->largo--;
	return dato;
}


