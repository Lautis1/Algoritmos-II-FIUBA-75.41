#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La lista está planteada como una lista de punteros genéricos. */

typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

/********************************************************************
 *                    PRIMITIVAS DE LA LISTA                        *
 *******************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacia.
lista_t *lista_crear(void);

// Devuelve true en caso de que la lista tenga elementos; false en
// caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento en la primera posicion de la lista. 
// Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: Se agrego un nuevo elemento a la lista; dato se encuentra al principio
// de la lista. Se aumenta en 1 unidad el largo de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento en la ultima posicion de la lista. 
// Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: Se agrego un nuevo elemento a la lista, dato se encuentra al final
// de la lista. Se aumenta en 1 unidad el largo de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Borra el primer elemento de la lista y lo devuelve. Si esta vacia, devuelve NULL.
// Pre: la lista fue creada.
// Post: Se borra devuelve el primer elemento de la lista cuando no esta vacia.
// Se disminuye en 1 unidad el largo de la lista.
void *lista_borrar_primero(lista_t *lista);

// Devuelve el valor del primer elemento de la lista. Si esta vacia, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvio el primer elemento de la lista. 
void *lista_ver_primero(const lista_t *lista);

// Devueve el valor del ultimo elemento de la lista. Si esta vacia, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvio el ultimo elemento de la lista.
void *lista_ver_ultimo(const lista_t* lista);

// Devuelve la cantidad de elementos que contiene la lista.
// Pre: la lista fue creada.
// Post: devuelve el largo de la lista.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la funcion destruir_dato por parametro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. Destruir_dato es una funcion capaz de destruir
// los datos de la lista,o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));

/********************************************************************
 *                    PRIMITIVAS DEL ITERADOR              *
 *******************************************************************/
//ITERADOR INTERNO

// Itera la lista de principio a fin.Si visitar no es NULL, se la aplica
// a cada elemento.
// Pre: la lista fue creada.
// Post: Itera la lista de principio a fin o hasta que visitar lo detiene.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

//ITERADOR EXTERNO

// Crea un iterador.
// Pre: la lista fue creada. 
// Post: devuelve un iterador con actual apuntando al primer nodo de la lista.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza una posicion sobre la lista. 
// Pre: el iterador fue creado.
// Post: Devuele true si la posicion siguiente a actual no es NULL.
// En caso contrario, false.
bool lista_iter_avanzar(lista_iter_t *iter);

// Devuelve el elemento actual en donde el iterador esta parado.
// Pre: el iterador fue creado.
// Post: devuelve el elemento actual del iterador.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve un booleano segun si el iterador esta al final de la lista.
// Pre: el iterador fue creado.
// Post: devuelve true si esta al final de la lista, false en caso contrario.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: se elimina el iterador.
void lista_iter_destruir(lista_iter_t *iter);

// Inserta un elemento en la posicion actual del iterador.
// Pre: el iterador fue creado.
// Post: dato esta en la posicion actual del iterador.
// Devuelve true si dato se inserto correctamente; false en caso contrario.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Borra el elemento de la posicion actual del iterador.
// Pre: el iterador fue creado.
// Post: Borra y devuelve un elemento de la lista, actual pasa a ser
// el siguiente del elemento borrado. Devuelve NULL si paso el final o esta vacia.
void *lista_iter_borrar(lista_iter_t *iter);

/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación del alumno.
//
// Las pruebas deben emplazarse en el archivo ‘pruebas_alumno.c’, y
// solamente pueden emplear la interfaz pública tal y como aparece en cola.h
// (esto es, las pruebas no pueden acceder a los miembros del struct cola).
//
// Para la implementación de las pruebas se debe emplear la función
// print_test(), como se ha visto en TPs anteriores.
void pruebas_lista_alumno(void);

#endif // LISTA_H









