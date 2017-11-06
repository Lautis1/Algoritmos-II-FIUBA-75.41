#include <stddef.h>
#include <stdbool.h>

#ifndef LISTA_H
#define LISTA_H

// Primitivas de la lista

typedef struct lista lista_t;

// Crea una lista.
// Post: devuelve una nueva lista vacía, NULL en caso de que algo falle.
lista_t *lista_crear(void);

// Devuelve verdadero o falso, según si la lista tiene o no elementos dentro de si.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento a la lista al final. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra al final
// de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento a la lista al principio. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del ultimo elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del ultimo, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el ultimo elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t* lista);

// Obtiene el largo de la lista, 0 si esta vacia.
// Pre: La lista fue creada
// Post: Se devolvio el largo de la lista.
size_t lista_largo(const lista_t *lista);


// Destruye la lista. Si se recibe la función destruir_dato por parámetro, entonces
// para cada uno de los nodos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));


// Primitiva del iterador interno

// Recorre uno a uno los elementos de la lista empezando por el primero,
// detiene su ejecucion al llegar al final o si visitar devuelve false.
// Si recibe la funcion visitar por parametro hace algo con cada elemento
// de la lista, no necesariamente la modifica. Si es NULL no hace nada.
// Pre: La lista fue creada.
// Post: Dependiendo de la funcion visitar se modifican o no ciertos elementos
// de la lista.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);


// Primitivas del iterador externo

typedef struct lista_iter lista_iter_t;

// Crea un iterador que esta asociado a la lista que se le pasa por
// parametro, el iterador se inicializa sobre la primera posicion.
// Pre: La lista que se le pasa por parametro fue creada
// Post: Devuelve un iterador parado sobre la primera posicion de la lista, devuelve NULL
// en caso de que algo falle
lista_iter_t *lista_iter_crear(lista_t *lista);

// Se mueve a la proxima posicion de la lista, devuelve false si esta
// al final de la lista
// Pre: iter fie creado
// Post: devuelve false si esta al final de la lista, true en otro caso
bool lista_iter_avanzar(lista_iter_t *iter);

// Devuelve el dato correspondiente al nodo sobre el que esta parado
// el iterador, devuelve NULL en caso de que el iterador este al final
// Pre: El iterador fue creado
// Post: Devuelve el dato sobre el que esta parado el iterador.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Chequea si el iterador esta al final de la lista
// Pre: El iterador fue creado
// Post: Devuelve true o false dependiendo de si iter esta al final o no
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador
// Pre: El iterador y la lista fueron creados.
void lista_iter_destruir(lista_iter_t *iter);

// Inserta un elemento en la posicion sobre la que esta parado el iterador,
// luego de insertar, el iterador se queda parado sobre el elemento que inserto
// Pre: El iterador fue creado
// Post: La lista a la que esta asociada el iterador tiene un elemento mas, devuelve
// false en caso de que falle de alguna manera, true si inserto correctamente
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Borra el elemento sobre el que esta parado el iterador, luego de eliminar
// el iterador queda parado sobre el elemento anterior al cual se elimino, devuelve
// el dato correspondiente al nodo eliminado, NULL en caso de que el iterador
// estuviese parado al final o la lista estuviese vacia.
// Pre: El iterador y la lista fueron creados.
// Post: Se devolvio el elemento eliminado, la lista tiene un elemento menos
// si la lista no estaba vacia, si el iterador esta al final devuelve NULL.
void *lista_iter_borrar(lista_iter_t *iter);

//Funcion de pruebas del alumno
void pruebas_lista_alumno(void);


#endif //LISTA_H
