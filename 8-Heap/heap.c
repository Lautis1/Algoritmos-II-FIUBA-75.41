#include "heap.h"

#define TAM_INICIAL 32

struct heap {
    cmp_func_t comparar;
    void** arreglo;
    size_t tamanio;
    size_t cantidad;
};

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

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)) {
    if (destruir_elemento) {
        for (int i = 0; i < heap->cantidad; ++i) {
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

void *heap_ver_max(const heap_t *heap) {
    if (!heap_esta_vacio(heap)) {
        return heap->arreglo[0];
    }
    return NULL;
}

