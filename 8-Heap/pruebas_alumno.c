#include "heap.h"
#include "testing.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int comparar(const void* dato1, const void* dato2){
	int resultado;
	
	if (*(int*)dato1 > *(int*)dato2){
		resultado = 1;	
	}else if(*(int*)dato1 < *(int*)dato2){
		resultado = -1;		
	}else{
		resultado = 0;
	}
	
	return resultado;
}

void pruebas_basicas(){
	puts("PRUEBAS BÁSICAS");
	heap_t* heap = heap_crear(comparar);
	print_test("heap fue creado", heap);
	
	int dato1 = 3;  
    int dato2 = 4;
    int dato3 = 2;
    int dato4 = 7;
    int dato5 = -5;
    int dato6 = 10;
    int dato7 = 15;
       
    print_test("encolar dato1", heap_encolar(heap, &dato1));
	print_test("encolar dato2", heap_encolar(heap, &dato2));
	print_test("encolar dato3", heap_encolar(heap, &dato3));
	print_test("encolar dato4", heap_encolar(heap, &dato4));
	print_test("encolar dato5", heap_encolar(heap, &dato5));
	print_test("encolar dato6", heap_encolar(heap, &dato6));
	print_test("encolar dato7", heap_encolar(heap, &dato7));
		
	print_test("desencolar", heap_desencolar(heap) == &dato7);
	print_test("desencolar", heap_desencolar(heap) == &dato6);
	print_test("desencolar", heap_desencolar(heap) == &dato4);
	print_test("desencolar", heap_desencolar(heap) == &dato2);
	print_test("desencolar", heap_desencolar(heap) == &dato1);
	print_test("desencolar", heap_desencolar(heap) == &dato3);
	print_test("desencolar", heap_desencolar(heap) == &dato5);
	print_test("desencolar heap vacio", heap_desencolar(heap) == NULL);
		
	heap_destruir(heap, NULL);
	
	puts("PRUEBAS heapsort");
	
	int* a = malloc(sizeof(int) * 10);
	int* b = malloc(sizeof(int) * 10);
	int* c = malloc(sizeof(int) * 10);
	int* d = malloc(sizeof(int) * 10);
	int* e = malloc(sizeof(int) * 10);
	int* f = malloc(sizeof(int) * 10);
	int* g = malloc(sizeof(int) * 10);
	
	*a = 8;
	*b = 70;
	*c = 70;
	*d = -5;
	*e = 40;
	*f = 0;
	*g = 8;
	
	void* arreglo[7] = {a, b, c, d, e, f, g};
	heap_sort(arreglo, 7, comparar);
	
	puts("Después de ordenar");
	for (size_t i = 0; i < 7; i++){
		printf("posición %zu: %d \n", i, *(int*)arreglo[i]);
	}	
	
	for (int i = 0; i < 7; i++){
		free(arreglo[i]);		
	}
	
	puts("PRUEBAS heap_crear_arr");
	
	void* arreglo2[3] = {&dato1, &dato2, &dato4};
	//void* arreglo3[] = {};
	void* arreglo4[1] = {&dato1};

	heap_t* heap2 = heap_crear_arr(arreglo2, 3, comparar);
	print_test("Crear heap con arr", heap2);
	print_test("desencolar", heap_desencolar(heap2) == &dato4);
	print_test("desencolar", heap_desencolar(heap2) == &dato2);
	print_test("desencolar", heap_desencolar(heap2) == &dato1);
	print_test("Esta vacio", heap_esta_vacio(heap2));
	heap_destruir(heap2, NULL);
	
	/*heap_t* heap3 = heap_crear_arr(arreglo3, 0, comparar);
	print_test("Crear heap con arr vacio", heap3);
	print_test("desencolar heap vacio", heap_desencolar(heap3) == NULL);
	print_test("Esta vacio", heap_esta_vacio(heap3));
	heap_destruir(heap3, NULL);*/
	
	heap_t* heap4 = heap_crear_arr(arreglo4, 1, comparar);
	print_test("Crear heap con arr de un elemento", heap4);
	print_test("desencolar", heap_desencolar(heap4) == &dato1);
	print_test("Esta vacio", heap_esta_vacio(heap4));
	heap_destruir(heap4,NULL);
}

void prueba_volumen(size_t largo){
	puts("PRUEBA VOLUMEN");
	heap_t* heap = heap_crear(comparar);
	
	size_t* numero;
	bool ok = true;
	size_t i;
		
	for(i = 0; i < largo ; i++){
		numero = malloc(sizeof(size_t));
		*numero = (size_t)rand () % largo;
		ok = heap_encolar(heap, numero);
		if (!ok) break;
	}
	
	print_test("Prueba heap encolar muchos elementos", ok);
	print_test("Cantidad elementos igual al largo", heap_cantidad(heap) == largo);
	
	void *ult_dato = heap_desencolar(heap);
	for(i = 1; i < largo; i++){
		void *dato = heap_desencolar(heap);
		if(comparar(dato, ult_dato) > 0){ // dato > ult_dato - Esto es para verificar que desencola en orden.
			ok = false;
			break;
		}
		void *a_destruir = ult_dato;
		ult_dato = dato;
		free(a_destruir);
	}
	
	print_test("Prueba heap desencolar muchos elementos", ok);
	print_test("Cantidad elementos igual a 0", heap_cantidad(heap) == 0);
	if(ult_dato){free(ult_dato);}
	
	heap_destruir(heap, free);	
	
	
	void *arreglo[largo];
	for(i = 0; i < largo; i++){
		size_t *dato = malloc(sizeof(size_t));
		*dato = (size_t)rand () % largo;
		arreglo[i] = dato;
	}
	heap_t *heap_arr = heap_crear_arr(arreglo, largo, comparar);
	
	//Verifica que el heap fue creado correctamente.
	ult_dato = heap_desencolar(heap_arr);
	for(i = 1; i < largo; i++){
		void *dato = heap_desencolar(heap_arr);
		if(comparar(dato, ult_dato) > 0){ 
			ok = false;
			break;
		}
		void *a_destruir = ult_dato;
		ult_dato = dato;
		free(a_destruir);
	}
	
	print_test("Heap crear arr muchos elementos", ok);
	if(ult_dato){free(ult_dato);}
	heap_destruir(heap_arr, free);	
}

void pruebas_heap_alumno(void){
	pruebas_basicas();
	prueba_volumen(1000);		
}
