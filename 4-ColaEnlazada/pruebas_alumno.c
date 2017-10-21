#include "cola.h"
#include "testing.h"
#include "vector_dinamico.h"
#include "pila.h"
#include <stdio.h>
#include <stddef.h>
/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/

void pruebas_cola_vacia(){
	printf("##INICIO DE PRUEBAS CON COLA DE TAMAÑO 0\n");
	cola_t* cola = cola_crear();
	print_test("Crear una cola", cola != NULL);
	print_test("La cola esta vacia", cola_esta_vacia(cola));
	print_test("No se puede desencolar en cola vacia", cola_desencolar(cola)==NULL);
	print_test("Ver_primero devuelve NULL", cola_ver_primero(cola)==NULL);
	cola_destruir(cola,NULL);
	print_test("La cola ha sido destruida", true);
	printf("\n");
}

void pruebas_null(){
	/*Pruebo que se permite encolar NULL*/
	printf("##INICIO DE PRUEBAS CON ELEMENTO NULL\n");
	cola_t* cola1 = cola_crear();
	print_test("Encolar un elemento NULL",cola_encolar(cola1,NULL));
	print_test("La cola no esta vacia", !cola_esta_vacia(cola1));
	print_test("El primero de la cola es NULL", cola_ver_primero(cola1)==NULL);
	print_test("Desencolar devuelve NULL", cola_desencolar(cola1)==NULL);
	print_test("Se vacio la cola", cola_esta_vacia(cola1));
	cola_destruir(cola1,NULL);
	print_test("La cola ha sido destruida", true);
	printf("\n");
}

void pruebas_cola_un_elemento(){
	printf("##INICIO DE PRUEBAS CON 1 ELEMENTO\n");
	cola_t* cola2 = cola_crear();
	print_test("Crear una cola", cola2!=NULL);
	int numero = 5;
	cola_encolar(cola2,&numero);
	print_test("Elemento encolado", !cola_esta_vacia(cola2));
	/*Chequeo que el primero sea el elemento que encole*/
	print_test("Ver primero correcto", (*((int*)cola_ver_primero(cola2)))==numero);
	print_test("Cola desencolada correctamente", (*((int*)cola_desencolar(cola2)))==numero);
	print_test("La cola esta vacia", cola_esta_vacia(cola2));
	cola_destruir(cola2,NULL);
	print_test("La cola ha sido destruida", true);
	printf("\n");
}
/* *****************************************************************
 *                    FUNCIONES AUXILIARES
 * *****************************************************************/

void pila_destruir_wrapper(void* dato){
	pila_destruir(dato);
}
void vector_destruir_wrapper(void* dato){
	vector_destruir(dato);
}

void pruebas_con_vector(){
	printf("##INICIO DE PRUEBAS CON UN VECTOR DINAMICO\n");
	/*Pruebo con un vector de 5 elementos*/
	vector_t* vec = vector_crear(5);
	cola_t* cola3 =  cola_crear();
	vector_guardar(vec,0,1);
	vector_guardar(vec,1,2);
	vector_guardar(vec,2,3);
	vector_guardar(vec,3,4);
	vector_guardar(vec,4,5);
	print_test("Encolar vector", cola_encolar(cola3,vec));
	print_test("El primero es el vector", cola_ver_primero(cola3)==vec);
	print_test("Cola NO vacia", !cola_esta_vacia(cola3));
	cola_destruir(cola3, vector_destruir_wrapper);
	print_test("La cola ha sido destruida", true);
	printf("\n");
}

void pruebas_con_pila(){
	printf("##INICIO DE PRUEBAS CON PILA\n");
	pila_t* pila = pila_crear();
	cola_t* cola4 = cola_crear();
	/*Apilo algunos elementos*/
	bool ok = true;
	for(int i=0;i<5;i++){
		ok &= pila_apilar(pila,&i);
	}
	print_test("Apilar 4 elementos", ok);
	print_test("Encolar la pila en la cola", cola_encolar(cola4, pila));
	print_test("El primero de la cola es la pila", cola_ver_primero(cola4)==pila);
	print_test("La cola NO esta vacia", !cola_esta_vacia(cola4));
	cola_destruir(cola4,pila_destruir_wrapper);
	print_test("Cola destruida",true);
	printf("\n");
}


void pruebas_cola_alumno(){
	/*Ejecuta todas las pruebas*/
	pruebas_cola_vacia();
	pruebas_null();
	pruebas_cola_un_elemento();
	pruebas_con_vector();
	pruebas_con_pila();
}
