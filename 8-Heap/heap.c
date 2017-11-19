#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "heap.h"


#define TAM_INICIAL 32
#define FACTOR_AUMENTAR_TAMANIO 2
#define FACTOR_DISIMINUIR_TAMANIO 4

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 ******************************************************************/
struct heap {
    cmp_func_t comparar;
    void** arreglo;
    size_t tamanio;
    size_t cantidad;
};

/*******************************************************************
*                        FUNCIONES AUXILIARES                      *
*******************************************************************/
//Intercambia dos punteros genericos dentro de un arreglo.
void swap(void* arreglo[], size_t posicion_1, size_t posicion_2){

	void* aux = arreglo[posicion_1];
	arreglo[posicion_1] = arreglo[posicion_2];
	arreglo[posicion_2] = aux;
}
//Busca la posicion del hijo izquierdo.
size_t posicion_hijo_izq(size_t posicion_padre){

	return ((2*posicion_padre)+1);
}

//Busca la posicion del hijo derecho.
size_t posicion_hijo_der(size_t posicion_padre){

	return posicion_hijo_izq(posicion_padre)+1;
}

//Busca la posicion del padre.
size_t posicion_padre(size_t posicion_hijo){

	return (posicion_hijo-1)/2;
}

//Redimensiona el heap.
//Recibe un heap y un tamanio nuevo.
//Redimensiona con Realloc.
bool heap_redimensionar(heap_t* heap, size_t tamanio_nuevo){

	void** arreglo_nuevo = realloc(heap->arreglo,sizeof(void*)*tamanio_nuevo);
	if(!arreglo_nuevo) return false;
	heap->arreglo = arreglo_nuevo;
	heap->tamanio = tamanio_nuevo;
	return true;
}

//Downheap basado en el pseudocodigo del Cormen.
//Mueve un elemento hacia indices mayores del arreglo hasta
//que se cumpla la invariante del heap.
void downheap(void* arreglo[], size_t indice, size_t cantidad, cmp_func_t comparar){

	size_t izquierdo = posicion_hijo_izq(indice);
	size_t derecho = posicion_hijo_der(indice);
    size_t hijo_mayor;

	if(izquierdo<cantidad && comparar(arreglo[izquierdo],arreglo[indice])>0){
		hijo_mayor = izquierdo;
	}
	else hijo_mayor = indice;
	if(derecho < cantidad && comparar(arreglo[derecho],arreglo[hijo_mayor])>0){
		hijo_mayor = derecho;
	}
	if(hijo_mayor != indice){
		swap(arreglo,indice,hijo_mayor);
		downheap(arreglo,hijo_mayor,cantidad,comparar);
	}
}

//Convierte el arreglo en un heap de maximos.
void heapify(void* arreglo[], size_t cantidad, cmp_func_t comparar){
	
	for(size_t indice = cantidad/2 ; indice > 0; indice--){
		downheap(arreglo,indice-1,cantidad,comparar);		
	}
}

//Funcion que ordena un arreglo de punteros.
//Recibe una funcion de comparacion.
//Modifica el arreglo in-situ.
void heap_sort(void* arreglo[], size_t cantidad, cmp_func_t cmp){

	heapify(arreglo,cantidad,cmp);
	for(size_t i = cantidad-1; i>0;i--){
		swap(arreglo,0,i);
		cantidad--;
		downheap(arreglo,0,cantidad,cmp);
	}
}

//Mueve el elemento en la posicion "indice" del arreglo
//hacia indices menores en el arreglo, hasta que se cumpla
//la invariante del heap.
void upheap(void* arreglo[], size_t indice, cmp_func_t comparar){

	if(indice==0) return;
	size_t indice_padre = posicion_padre(indice);
	if(comparar(arreglo[indice],arreglo[indice_padre])>0){
		swap(arreglo,indice,indice_padre);
		upheap(arreglo,indice_padre,comparar);
	}
}
/*******************************************************************
*                        IMPLEMENTACION HEAP                       *
*******************************************************************/

heap_t *heap_crear(cmp_func_t cmp) {
    
    heap_t* heap = malloc(sizeof(heap_t));
    if (heap == NULL) return NULL;
    heap->arreglo = malloc(sizeof(void*) * TAM_INICIAL);
    if (heap->arreglo == NULL) {
        free(heap);
        return NULL;
    }
    heap->comparar = cmp;
    heap->tamanio = TAM_INICIAL;
    heap->cantidad = 0;
    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
	
	heap_t* heap_nuevo = malloc(sizeof(heap_t));
	if(!heap_nuevo) return NULL;
	heap_nuevo->arreglo = malloc(n*sizeof(void*));
	if(!heap_nuevo->arreglo) {
		free(heap_nuevo);
		return NULL;
	}//Hay que copiar el arreglo recibido al heap nuevo.
	for(size_t i=0; i<n ; i++){
		heap_nuevo->arreglo[i] = arreglo[i];
	}
	heap_nuevo->comparar = cmp;
	heap_nuevo->cantidad = n;
	heap_nuevo->tamanio = n;
	heapify(heap_nuevo->arreglo,heap_nuevo->cantidad,heap_nuevo->comparar);
	return heap_nuevo;
}


void heap_destruir(heap_t *heap, void destruir_elemento(void *e)) {
    
    if (destruir_elemento) {
        for (size_t i = 0; i < heap->cantidad; ++i) {
            destruir_elemento(heap->arreglo[i]);
        }
    }
    free(heap->arreglo);
    free(heap);
}


size_t heap_cantidad(const heap_t *heap) {
    
    return heap->cantidad;
}


bool heap_esta_vacio(const heap_t *heap) {
    
    return heap->cantidad == 0;
}


bool heap_encolar(heap_t *heap, void *elem){

    if (elem == NULL) return false;

	if(heap->cantidad >= heap->tamanio){
		if(!heap_redimensionar(heap, heap->tamanio*FACTOR_AUMENTAR_TAMANIO)) return false;
	}
	if(heap_esta_vacio(heap)) heap->arreglo[0] = elem;
	else{
		heap->arreglo[heap->cantidad] = elem;
		upheap(heap->arreglo, heap->cantidad, heap->comparar);
	}
	heap->cantidad++;
	return true;
}

// Pre: Heap fue creado.
// Devuelve el valor con mayor prioridad del heap.
// Post: Devuelve el valor de maxima prioridad en caso de que el heap no este vacio,
// NULL en caso contrario.
void *heap_ver_max(const heap_t *heap) {
    
    if (!heap_esta_vacio(heap)) {
        return heap->arreglo[0];
    }
    return NULL;
}

// Pre: El heap fue creado.
// Desencola el elemento con mayor prioridad del heap.
// Post: Devuelve el valor de maxima prioridad en caso de que el heap no este vacio,
// NULL en caso contrario.
void *heap_desencolar(heap_t *heap){

	if(heap_esta_vacio(heap)) return NULL;
	void* dato_a_devolver = heap->arreglo[0];
	heap->arreglo[0] = heap->arreglo[heap->cantidad-1]; //Piso el primero por el ultimo
	downheap(heap->arreglo,0,heap->cantidad-1,heap->comparar);
	if(heap->cantidad <= heap->tamanio/FACTOR_DISIMINUIR_TAMANIO && heap->tamanio/FACTOR_AUMENTAR_TAMANIO >= TAM_INICIAL){
		if(!heap_redimensionar(heap, heap->tamanio/FACTOR_AUMENTAR_TAMANIO)) return NULL;
	}
	heap->cantidad--;
	return dato_a_devolver;
}

