#include "heap.h"
#include "testing.h"
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "strutil.h"

#define CANT_ELEM_ARRAY_VOLUMEN 4321
#define CANT_ELEM_ARRAY_PRUEBAS 123
#define CANT_ELEM_RANDOM 344
#define CANT_ENTEROS_ORDENADOS 456
#define CANT_ELEM_PRUEBAS_DESTRUIR 19

int comparar_enteros(void* valor1, void* valor2) {
    int numero1 = *(int*) valor1;
    int numero2 = *(int*) valor2;
    return (numero1<numero2) ? -1 : (numero1 > numero2);
}

bool esta_ordenado(void** elementos, cmp_func_t cmp, size_t cant) {
    for (int i = 0; i < cant - 1; i++) {
        if (cmp(elementos[i], elementos[i + 1]) == 1) return false;
    }
    return true;
}

void shuffle(int *array, size_t n) {
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}


void pruebas_heap_vacio() {
    printf("\nINICIO DE PRUEBAS HEAP VACIO\n\n");

    heap_t* heap = heap_crear((cmp_func_t)comparar_enteros);

    print_test("El heap fue creado", heap != NULL);
    print_test("El heap esta vacio", heap_esta_vacio(heap));
    print_test("Heap_cantidad es cero en heap vacio", heap_cantidad(heap) == 0);
    print_test("Heap_ver_max es NULL en heap vacio", heap_ver_max(heap) == NULL);
    print_test("Heap desencolar en heap vacio es NULL", heap_desencolar(heap) == NULL);

    heap_destruir(heap, NULL);

    print_test("El heap fue destruido", true);
}

void pruebas_unitarias() {
    printf("\nINICIO DE PRUEBAS HEAP UNITARIAS\n\n");

    int entero_prueba_1 = 7541;

    heap_t* heap = heap_crear((cmp_func_t)comparar_enteros);

    print_test("El heap fue creado", heap != NULL);
    print_test("Se encolo un elemento", heap_encolar(heap, &entero_prueba_1));
    print_test("El heap no esta vacio", !heap_esta_vacio(heap));
    print_test("Heap_cantidad es uno en heap vacio", heap_cantidad(heap) == 1);
    print_test("Heap_ver_max es igual al unico valor encolado", heap_ver_max(heap) == &entero_prueba_1);
    print_test("Heap desencolar devuelve el unico valor encolado", heap_desencolar(heap) == &entero_prueba_1);
    print_test("Heap esta vacio es true luego de desencolar", heap_esta_vacio(heap));
    print_test("Heap cantidad es cero luego de desencolar", heap_cantidad(heap) == 0);
    print_test("Ver maximo es NULL luego de desencolar", heap_desencolar(heap) == NULL);

    heap_destruir(heap, NULL);

    print_test("El heap fue destruido", true);
}

void pruebas_pocos_elementos() {

    printf("\nINICIO DE PRUEBAS HEAP POCOS ELEMENTOS\n\n");

    int entero_prueba_2 = 542;
    int entero_prueba_3 = -23;
    int entero_prueba_4 = 0;
    int entero_prueba_5 = 1000;
    int entero_prueba_6 = 975;
    int entero_prueba_7 = 52;
    int entero_prueba_8 = 8;

    heap_t* heap = heap_crear((cmp_func_t)comparar_enteros);

    print_test("El heap fue creado", heap != NULL);
    print_test("Se encolo un elemento", heap_encolar(heap, &entero_prueba_2));
    print_test("Se encolo un elemento", heap_encolar(heap, &entero_prueba_3));
    print_test("Se encolo un elemento", heap_encolar(heap, &entero_prueba_4));
    print_test("Se encolo un elemento", heap_encolar(heap, &entero_prueba_5));
    print_test("Se encolo un elemento", heap_encolar(heap, &entero_prueba_6));
    print_test("Se encolo un elemento", heap_encolar(heap, &entero_prueba_7));
    print_test("Se encolo un elemento", heap_encolar(heap, &entero_prueba_8));

    print_test("El heap no esta vacio", !heap_esta_vacio(heap));
    print_test("Heap_cantidad es igual a la cantidad de elementos encolados", heap_cantidad(heap) == 7);
    print_test("Heap_ver_max es igual al valor maximo insertado", heap_ver_max(heap) == &entero_prueba_5);
    print_test("Heap desencolar devuelve el maximo valor encolado", heap_desencolar(heap) == &entero_prueba_5);
    print_test("Heap_ver_max es igual al valor maximo insertado", heap_ver_max(heap) == &entero_prueba_6);
    print_test("Heap desencolar devuelve el maximo valor encolado", heap_desencolar(heap) == &entero_prueba_6);
    print_test("Heap_ver_max es igual al valor maximo insertado", heap_ver_max(heap) == &entero_prueba_2);
    print_test("Heap desencolar devuelve el maximo valor encolado", heap_desencolar(heap) == &entero_prueba_2);
    print_test("Heap_ver_max es igual al valor maximo insertado", heap_ver_max(heap) == &entero_prueba_7);
    print_test("Heap desencolar devuelve el maximo valor encolado", heap_desencolar(heap) == &entero_prueba_7);
    print_test("Heap_ver_max es igual al valor maximo insertado", heap_ver_max(heap) == &entero_prueba_8);
    print_test("Heap desencolar devuelve el maximo valor encolado", heap_desencolar(heap) == &entero_prueba_8);
    print_test("Heap_ver_max es igual al valor maximo insertado", heap_ver_max(heap) == &entero_prueba_4);
    print_test("Heap desencolar devuelve el maximo valor encolado", heap_desencolar(heap) == &entero_prueba_4);
    print_test("Heap_ver_max es igual al valor maximo insertado", heap_ver_max(heap) == &entero_prueba_3);
    print_test("Heap desencolar devuelve el maximo valor encolado", heap_desencolar(heap) == &entero_prueba_3);

    print_test("El heap esta vacio luego de desencolar todo", heap_esta_vacio(heap));
    print_test("Heap cantidad es igual a 0 luego de desencolar todo", heap_cantidad(heap) == 0);
    print_test("Ver maximo es NULL luego de desencolar", heap_desencolar(heap) == NULL);

    heap_destruir(heap, NULL);

    print_test("El heap fue destruido", true);
}

void pruebas_heap_volumen() {
    printf("\nINICIO DE PRUEBAS DE VOLUMEN DE HEAP\n\n");

    int array_prueba[CANT_ELEM_ARRAY_VOLUMEN];
    for (int i = 0; i < CANT_ELEM_ARRAY_VOLUMEN; i++) {
        array_prueba[i] = i;
    }

    shuffle(array_prueba, CANT_ELEM_ARRAY_VOLUMEN);

    heap_t* heap = heap_crear((cmp_func_t)comparar_enteros);

    print_test("El heap fue creado", heap != NULL);

    int contador_errores = 0;
    for (int i = 0; i < CANT_ELEM_ARRAY_VOLUMEN; i++) {
        if(!heap_encolar(heap,&array_prueba[i])) {
            contador_errores++;
        }
    }

    print_test("Se encolaron 4321 elementos correctamente", contador_errores == 0);
    print_test("Heap esta vacio es falso", !heap_esta_vacio(heap));
    print_test("Heap cantidad es igual a 4321", heap_cantidad(heap) == CANT_ELEM_ARRAY_VOLUMEN);
    printf("%d\n", (*(int*)heap_ver_max(heap)));
    print_test("Ver max es igual al maximo elemento encolado", (*(int*)heap_ver_max(heap)) == CANT_ELEM_ARRAY_VOLUMEN-1);

    contador_errores = 0;
    for (int i = CANT_ELEM_ARRAY_VOLUMEN-1; i >= 0; i--) {
        if((*(int*)heap_desencolar(heap)) != i) {
            contador_errores++;
        }
    }

    print_test("Se desencolaron todos los elementos correctamente, manteniendo el invariante de heap", contador_errores == 0);
    print_test("Heap esta vacio es true", heap_esta_vacio(heap));
    print_test("Heap cantidad es igual a 0", heap_esta_vacio(heap));
    print_test("Ver max en heap que ha sido vaciado es igual a NULL", heap_ver_max(heap) == NULL);
    print_test("Heap desencolar en heap vaciado es NULL", heap_desencolar(heap) == NULL);

    heap_destruir(heap, NULL);

    print_test("El heap ha sido destruido", true);
}

void pruebas_heap_desde_arreglo() {
    printf("\nINICIO DE PRUEBAS DE CREACION DE HEAP A PARTIR DE ARREGLO\n\n");

    int array_prueba[CANT_ELEM_ARRAY_PRUEBAS];
    for (int i = 0; i < CANT_ELEM_ARRAY_PRUEBAS; i++) {
        array_prueba[i] = i;
    }

    shuffle(array_prueba, CANT_ELEM_ARRAY_PRUEBAS);

    void* array_punteros[CANT_ELEM_ARRAY_PRUEBAS];
    for (int i = 0; i < CANT_ELEM_ARRAY_PRUEBAS; i++) {
        array_punteros[i] = &array_prueba[i];
    }

    heap_t* heap = heap_crear_arr(array_punteros, CANT_ELEM_ARRAY_PRUEBAS, (cmp_func_t)comparar_enteros);

    print_test("Se ha creado el heap a partir de un arreglo predefinido", heap != NULL);
    print_test("Heap esta vacio es false", !heap_esta_vacio(heap));
    print_test("La cantidad de elementos es igual a la proporcionada a la hora de crear la funcion", heap_cantidad(heap) == CANT_ELEM_ARRAY_PRUEBAS);
    print_test("Ver max es igual al mayor elemento del arreglo original", *(int*)heap_ver_max(heap) == CANT_ELEM_ARRAY_PRUEBAS-1);

    int contador_errores = 0;
    for (int i = CANT_ELEM_ARRAY_PRUEBAS-1; i >= 0; i--) {
        if((*(int*)heap_desencolar(heap)) != i) {
            contador_errores++;
        }
    }

    print_test("Se desencolaron todos los elementos y se quitaron en el orden correcto", contador_errores == 0);
    print_test("Heap esta vacio es true", heap_esta_vacio(heap));
    print_test("Cantidad es igual a 0 en heap vaciado", heap_cantidad(heap) == 0);
    print_test("Desencolar en heap que ha sido vaciado es NULL", heap_desencolar(heap) == NULL);
    print_test("Ver maximo en heap vaciado es NULL", heap_ver_max(heap) == NULL);

    heap_destruir(heap, NULL);

    print_test("El heap ha sido destruido", true);
}

void pruebas_heapsort() {

    int enteros_orden_random[CANT_ELEM_RANDOM];

    for (int i = 0; i < CANT_ELEM_RANDOM; i++) {
        enteros_orden_random[i] = i;
    }

    void* array_punteros_random[CANT_ELEM_RANDOM];
    for (int i = 0; i < CANT_ELEM_RANDOM; i++) {
        array_punteros_random[i] = &enteros_orden_random[i];
    }

    char array_chars[] = {"martineselmasgrande"};

    void* array_punteros_char[sizeof(array_chars)];
    for (int i = 0; i < sizeof(array_chars); i++) {
        array_punteros_char[i] = &array_chars[i];
    }

    int enteros_ordenados_al_reves[CANT_ENTEROS_ORDENADOS];

    for (int i = 0; i < CANT_ENTEROS_ORDENADOS; i++) {
        enteros_ordenados_al_reves[i] = 455-i;
    }

    void* array_punteros_al_reves[CANT_ENTEROS_ORDENADOS];
    for (int i = 0; i < CANT_ENTEROS_ORDENADOS; i++) {
        array_punteros_al_reves[i] = &enteros_ordenados_al_reves[i];
    }

    // Hago un shuffle de los numeros del array para que no queden en orden.
    shuffle(enteros_orden_random, CANT_ELEM_RANDOM);

    printf("\nINICIO DE PRUEBAS DE HEAPSORT\n\n");

    heap_sort(array_punteros_random, CANT_ELEM_RANDOM, (cmp_func_t)comparar_enteros);

    print_test("Se ordena un arreglo de enteros en orden aleatorio correctamente", esta_ordenado(array_punteros_random, (cmp_func_t )comparar_enteros, CANT_ELEM_RANDOM-1));

    heap_sort(array_punteros_al_reves, CANT_ENTEROS_ORDENADOS, (cmp_func_t)comparar_enteros);

    print_test("Se ordeno un arreglo de enteros originalmente ordenado de mayor a menor", esta_ordenado(array_punteros_al_reves, (cmp_func_t)comparar_enteros, CANT_ENTEROS_ORDENADOS));

    heap_sort(array_punteros_char, sizeof(array_chars), (cmp_func_t)strcmp);

    print_test("Se ordena un arreglo de chars correctamente", esta_ordenado(array_punteros_char, (cmp_func_t)strcmp, sizeof(array_chars)));
}



void pruebas_destruccion() {

    printf("\nINICIO DE PRUEBAS DE DESTRUCCION\n\n");

    char** array_char_pruebas = split("hola hola cosa la la la ala d ac ac kekd eadkaed ea dea d kaedk eakd kaedkaek dkaed", ' ');

    heap_t* heap = heap_crear_arr((void**)array_char_pruebas, CANT_ELEM_PRUEBAS_DESTRUIR, (cmp_func_t)strcmp);

    print_test("Se creo un heap con los valores pasados", heap != NULL);

    heap_destruir(heap, free);

    print_test("Se destruyo el heap y sus datos con la funcion pasada correctamente", true);

    free(array_char_pruebas);

}

void pruebas_heap_alumno() {
    pruebas_heap_vacio();
    pruebas_unitarias();
    pruebas_pocos_elementos();
    pruebas_heap_volumen();
    pruebas_heap_desde_arreglo();
    pruebas_heapsort();
    pruebas_destruccion();
}