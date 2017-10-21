#include "pila.h"
#include "testing.h"
#include <stdio.h>
#include <stddef.h>


/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/

void pruebas_pila_vacia() {
    
    printf("##INICIO DE PRUEBAS CON PILA DE TAMAÑO 0\n");
	pila_t* pila = pila_crear();
	print_test("Crear una pila", pila != NULL);
	
	/*Chequear si efectivamente la pila esta vacia*/
	print_test("La pila esta vacia", pila_esta_vacia(pila));
	
	/*Desapilar en una pila vacia es invalido*/
	print_test("No se puede desapilar pila vacia", pila_desapilar(pila)==NULL);
	
	/*Ver tope de una pila vacia es invalido*/
	print_test("No se puede ver tope en pila vacia", pila_ver_tope(pila)==NULL);
	
	/*Destruyo la pila*/
	pila_destruir(pila);
	print_test("La pila fue destruida", true);
	
}
void pruebas_pila_null(){
	
	printf("##INICIO DE PRUEBAS CON ELEMENTO NULL\n");
	pila_t* pila1 = pila_crear();
	/*Chequeo que sea valido apilar un NULL*/
	print_test("Apila un elemento NULL", pila_apilar(pila1,NULL));
	/*La pila no debe estar vacia*/
	print_test("Pila no esta vacia", !pila_esta_vacia(pila1));
	/*El tope debe ser NULL*/
	print_test("El tope de la pila es un NULL", pila_ver_tope(pila1)==NULL);
	/*Desapilo el NULL*/
	print_test("Desapila y devuelve NULL", pila_desapilar(pila1)==NULL);
	print_test("Se vacio la pila", pila_esta_vacia(pila1));
	/*Destruyo la pila*/
	pila_destruir(pila1);
	print_test("La pila fue destruida", true);
}
	
void pruebas_pila_un_elemento(){
	printf("##INICIO DE PRUEBAS CON 1 ELEMENTO\n");
	pila_t* pila2 = pila_crear();
	print_test("Crear una pila", pila2 != NULL);
	
	/*Apilo un elemento a la pila*/
	int numero = 5;
	pila_apilar(pila2,&numero);
	print_test("Apila un elemento", !pila_esta_vacia(pila2));
	
	/*Chequeo si el tope es igual al numero apilado*/
	print_test("Ver tope correcto", (*((int*)pila_ver_tope(pila2)))==numero);
	
	/*Desapilo y chequeo que el elemento desapilado sea el elemento que apile*/
	print_test("Desapilada correctamente", (*((int*)pila_desapilar(pila2)))==numero);
	print_test("La pila quedo vacia", pila_esta_vacia(pila2));
	
	/*Destruyo la pila*/
	pila_destruir(pila2);
	print_test("Pila destruida correctamente", true);
	}

	
void pruebas_pila_muchos_elementos(){
	
	printf("##INICIO DE PRUEBAS CON MUCHOS ELEMENTOS\n");
	pila_t* pila4 = pila_crear();
	/*Apilo 500 elementos*/
	bool ok = true;
	int numero2 = 10;
	for(int i=0;i<500;i++){
		ok &= pila_apilar(pila4,&numero2);
	}
	print_test("Apilar 500 elementos", ok);
	
	/*Chequeo que la pila no esta vacia*/
	print_test("Pila no esta vacia", !pila_esta_vacia(pila4));
	
	/*Chequeo que el tope sea 500*/
	int tope = 10;
	print_test("Ver tope correcto", (*((int*)pila_ver_tope(pila4)))==tope);
	
	/*Desapilo, la pila tiene que quedar vacia*/
	for(int i=0;i<500;i++){
		pila_desapilar(pila4);
	}
	print_test("La pila quedo vacia, desapilada totalmente", pila_esta_vacia(pila4));
	
	/*Destruyo la pila*/
	pila_destruir(pila4);
	print_test("Pila destruida correctamente", true);
	}
	
		

void pruebas_pila_alumno(){
	/*Ejecuta todas las pruebas*/
	pruebas_pila_vacia();
	pruebas_pila_null();
	pruebas_pila_un_elemento();
	pruebas_pila_muchos_elementos();
	
}
