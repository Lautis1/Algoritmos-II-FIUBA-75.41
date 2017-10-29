#ifndef ALGOS_HEAP_H
#define ALGOS_HEAP_H


typedef struct heap heap_t;
typedef int (*cmp_func_t) (const void *a, const void *b);

// Pre: cmp es una funcion de comparacion valida.
// Crea un heap con la funcion de comparacion pasada por parametro.
// Post: El heap fue creado, devuelve el heap, NULL en caso de que algo haya fallado.
heap_t *heap_crear(cmp_func_t cmp);

// Pre: cmp es una funcion de comparacion valida, arreglo, fue creado.
// Crea un heap a partir del arreglo recibido con la funcion de comparacion pasada por parametro.
// Post: El heap fue creado, devuelve el heap, NULL en caso de que algo haya fallado.
heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp);

// Pre: Heap fue creado
// Destruye el heap, si se le pasa destruir dato aplica dicha
// funcion al dato de cada elemento del heap.
// Post: El heap fue destruido.
void heap_destruir(heap_t *heap, void destruir_elemento(void *e));

// Pre: Heap fue creado.
// Devuelve la cantidad de elementos que hay en el heap.
size_t heap_cantidad(const heap_t *heap);

// Pre: Heap fue creado.
// Devuelve un booleano que indica si el heap esta vacio o no.
bool heap_esta_vacio(const heap_t *heap);

// Pre: El heap fue creado.
// Agrega un elemento al heap.
// Post: Devuelve un booleano indicando si se encolo el elemento correctamente.
bool heap_encolar(heap_t *heap, void *elem);

// Pre: Heap fue creado.
// Devuelve el valor con mayor prioridad del heap.
// Post: Devuelve el valor de maxima prioridad en caso de que el heap no este vacio,
// NULL en caso contrario.
void *heap_ver_max(const heap_t *heap);

// Pre: El heap fue creado.
// Desencola el elemento con mayor prioridad del heap.
// Post: Devuelve el valor de maxima prioridad en caso de que el heap no este vacio,
// NULL en caso contrario.
void *heap_desencolar(heap_t *heap);

// Pre: elementos fue creado, cmp es una funcion de comparacion valida.
// Ordena un arreglo mediante heaps.
// Post: El arreglo pasado por parametro fue ordenado.
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp);

#endif //ALGOS_HEAP_H
