#include "lista.h"
#include <stdlib.h>

// Definiciones

typedef struct nodo_lista {
    void* dato;
    struct nodo_lista* proximo;
}nodo_lista_t;

typedef struct lista {
    nodo_lista_t* primero;
    nodo_lista_t* ultimo;
    size_t largo;
} lista_t;

/* ===================================== */

lista_t *lista_crear(void) {
    lista_t* lista = malloc(sizeof(lista_t));

    if (lista == NULL) return NULL;

    lista->primero = lista->ultimo = NULL;
    lista->largo = 0;

    return lista;
}

bool lista_esta_vacia(const lista_t *lista) {
    return lista_largo(lista) == 0 && (lista_ver_primero(lista) == NULL) && (lista_ver_ultimo(lista) == NULL);
}


nodo_lista_t* crear_nodo(void *dato) {
    nodo_lista_t* nodo = malloc(sizeof(nodo_lista_t));
    if (nodo == NULL) return NULL;
    nodo->dato = dato;
    return nodo;
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
    nodo_lista_t* nodo = crear_nodo(dato);
    if (nodo == NULL) return false;

    //Si esta vacia, lista->primero va a ser NULL, que es lo que queremos en este caso
    nodo->proximo = lista->primero;

    if (lista_esta_vacia(lista)) {
        lista->ultimo = nodo;
    }

    lista->primero = nodo;
    lista->largo++;

    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
    nodo_lista_t* nodo = crear_nodo(dato);

    if (nodo == NULL) return false;

    nodo->dato = dato;

    nodo->proximo = NULL;

    if (lista_esta_vacia(lista)) {
        lista->primero = nodo;
    } else {
        lista->ultimo->proximo = nodo;
    }

    lista->ultimo = nodo;

    lista->largo++;

    return true;
}

void *lista_borrar_primero(lista_t *lista) {
    if (lista_esta_vacia(lista)) {
        return NULL;
    }

    nodo_lista_t* nodo_aux = lista->primero;
    void* dato_aux = lista->primero->dato;

    if (lista->primero->proximo == NULL) {
        lista->ultimo = NULL;
    }

    //Funciona aun cuando la lista queda vacia, ya que en ese caso
    // lista_primero y lista_ultimo apuntaran a NULL
    lista->primero = lista->primero->proximo;

    free(nodo_aux);

    lista->largo--;

    return dato_aux;
}

void *lista_ver_primero(const lista_t *lista) {
    return (lista_largo(lista) == 0) ? NULL : lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t* lista) {
    return (lista_largo(lista) == 0) ? NULL : lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista) {
    return lista->largo;
}

void lista_destruir(lista_t *lista, void destruir_dato(void *)) {
    while (!lista_esta_vacia(lista)) {
        void* dato = lista_borrar_primero(lista);
        if (destruir_dato != NULL) {
            destruir_dato(dato);
        }
    }
    free(lista);
}



// Primitivas del iterador interno


void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
    if (visitar == NULL) return;

    nodo_lista_t* actual = lista->primero;

    while (actual != NULL) {
        if (!visitar(actual->dato, extra)) return;

        actual = actual->proximo;
    }
}


// Primitivas del iterador externo


typedef struct lista_iter {
    nodo_lista_t* anterior;
    nodo_lista_t* actual;
    lista_t* lista;
}lista_iter_t;

lista_iter_t *lista_iter_crear(lista_t *lista) {
    lista_iter_t* iterador = malloc(sizeof(lista_iter_t));

    if (iterador == NULL) return NULL;

    iterador->anterior = NULL;
    iterador->actual = lista->primero;
    iterador->lista = lista;
    return iterador;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
    if (lista_iter_al_final(iter)) return false;

    nodo_lista_t* nodo_aux = iter->actual;

    iter->actual = iter->actual->proximo;
    iter->anterior = nodo_aux;

    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter) {
    return (lista_iter_al_final(iter)) ? NULL : iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter) {
    return iter->actual == NULL;
}

void lista_iter_destruir(lista_iter_t *iter) {
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
    nodo_lista_t* nodo_nue = crear_nodo(dato);

    if (nodo_nue == NULL) return false;

    nodo_nue->dato = dato;

    nodo_nue->proximo = iter->actual;

    // Si estoy al final actualizo el puntero a ultimo, funciona
    // igualmente si la lista estaba vacia
    if (iter->actual == NULL) {
        iter->lista->ultimo = nodo_nue;
    }
    if (iter->anterior != NULL) {
        iter->anterior->proximo = nodo_nue;
    } else {
        iter->lista->primero = nodo_nue;
    }

    iter->actual = nodo_nue;

    iter->lista->largo++;

    return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
    // Si la lista esta vacia o iter esta al final(parado en NULL)
    if (iter->actual == NULL) return NULL;

    nodo_lista_t* nodo_aux = iter->actual;
    void* dato_aux = iter->actual->dato;

    if (iter->actual->proximo == NULL) {
        // Aun funciona en el caso de una lista de un elemento, anterior seria NULL, luego
        // de eliminado actual, quedaria una lista vacia con tanto primero como ultimo iguales a NULL
        iter->lista->ultimo = iter->anterior;
    }

    iter->actual = iter->actual->proximo;

    if (iter->anterior != NULL) {
        iter->anterior->proximo = iter->actual;
    } else {
        iter->lista->primero = iter->actual;
    }

    free(nodo_aux);

    iter->lista->largo--;

    return dato_aux;
}
