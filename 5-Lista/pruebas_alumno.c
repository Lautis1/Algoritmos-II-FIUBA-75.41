#include "lista.h"
#include "testing.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
/* ******************************************************************
 *                   PRUEBAS UNITARIAS LISTA ALUMNO
 * *****************************************************************/

void pruebas_lista_vacia(){
	printf("### INICIO DE PRUEBAS CON LISTA VACIA ###\n");
	lista_t* lista1 = lista_crear();
	print_test("Crear una lista", lista1!=NULL);
	print_test("La lista esta vacia", lista_esta_vacia(lista1));
	print_test("Lista tiene 0 elementos",lista_largo(lista1)==0);
	print_test("Si borro el primero, es NULL", lista_borrar_primero(lista1)==NULL);
	print_test("El ultimo elemento es NULL", lista_ver_ultimo(lista1)==NULL);
	lista_destruir(lista1,NULL);
	print_test("La lista ha sido destruida", true);
	printf("\n");
}

void pruebas_elemento_null(){
	printf("### INICIO DE PRUEBAS CON ELEMENTO NULL ###\n");
	lista_t* lista2 = lista_crear();
	print_test("Insertar un elemento NULL", lista_insertar_primero(lista2,NULL));
	print_test("La lista NO esta vacia", !lista_esta_vacia(lista2));
	print_test("La lista tiene un solo elemento", lista_largo(lista2)==1);
	print_test("El primero de la lista es un NULL", lista_ver_primero(lista2)==NULL);
	print_test("Borro y devuelve NULL", lista_borrar_primero(lista2)==NULL);
	lista_destruir(lista2,NULL);
	print_test("Lista destruida",true);
	printf("\n");
}

void pruebas_algunos_elementos(){
	printf("### INICIO DE PRUEBAS CON ALGUNOS ENTEROS ###\n");
	lista_t* lista3 = lista_crear();
	int num1 = 5;
	int num2 = 4;
	print_test("Insertar el primer elemento", lista_insertar_primero(lista3,&num1));
	print_test("Insertar el segundo elemento", lista_insertar_ultimo(lista3,&num2));
	print_test("El largo de la lista es 2", lista_largo(lista3)==2);
	print_test("El primero es el primer elemento que agregue", (*((int*)lista_ver_primero(lista3)))==num1);
	print_test("El ultimo es el ultimo elemento que agregue", (*((int*)lista_ver_ultimo(lista3)))==num2);
	print_test("Primer elemento borrado", lista_borrar_primero(lista3)==&num1);
	print_test("El ultimo que agregue es el primero y fue borrado", (*((int*)lista_borrar_primero(lista3)))==num2);
	print_test("La lista quedo vacia", lista_esta_vacia(lista3));
	lista_destruir(lista3,NULL);
	print_test("La lista ha sido destruida", true);
	printf("\n");
}

void pruebas_muchos_elementos(){
	printf("### INICIO DE PRUEBAS CON MUCHOS ELEMENTOS ###\n");
	lista_t* lista4 = lista_crear();
	bool ok;
	int numero_volumen = 8;
	for(int i=0; i<1000; i++){
		ok = lista_insertar_ultimo(lista4,&numero_volumen);
	}
	int primero = 8;
	int ultimo = 8;
	print_test("Lista con 1000 elementos",ok);
	print_test("La lista no esta vacia", !lista_esta_vacia(lista4));
	print_test("El largo de la lista es 1000", lista_largo(lista4)==1000);
	print_test("El primero es el num 8", (*((int*)lista_ver_primero(lista4)))==primero);
	print_test("El ultimo es el num 8", (*((int*)lista_ver_ultimo(lista4)))==ultimo);
	while(!lista_esta_vacia(lista4)){
		lista_borrar_primero(lista4);
	}
	print_test("Los elementos fueron borrados", true);
	print_test("Se vacio la lista", lista_esta_vacia(lista4));
	lista_destruir(lista4,NULL);
	print_test("Lista destruida", true);
	printf("\n");
}


void pruebas_casos_borde(){
	printf("### INICIO DE PRUEBAS CON CASOS BORDE ###\n");
	//Si inserto en la ultima posicion en lista vacia, el primero es igual al ultimo
	lista_t* lista5 = lista_crear();
	int dato = 5;
	int dato2 = 10;
	print_test("Insertar al final dato", lista_insertar_ultimo(lista5,&dato));
	print_test("El primero es un 5",(*((int*)lista_ver_primero(lista5)))==dato);
	print_test("El ultimo tambien es un 5",(*((int*)lista_ver_ultimo(lista5)))==dato);
	print_test("Agrego un elemento en la ultima posicion", lista_insertar_ultimo(lista5,&dato2));
	print_test("El ultimo ahora es un 10",(*((int*)lista_ver_ultimo(lista5)))==dato2);
	print_test("El largo de la lista es 2", lista_largo(lista5)==2);
	lista_destruir(lista5,NULL);
	print_test("La lista se destruyo", true);
	printf("\n");
}

/* ******************************************************************
 *                   PRUEBAS UNITARIAS ITER INTERNO                 *
 * *****************************************************************/
//Funcion auxiliar para hacer algo con los datos de cada nodo de la lista

bool sumar_datos(void* dato, void* extra){
	int* numero = dato;
	int* suma = extra;
	*suma += *numero;
	return true;
}
 void pruebas_iterador_interno(){
 	printf("### INICIO DE PRUEBAS CON ITERADOR INTERNO ###\n");
 	lista_t* lista6 = lista_crear();
 	int suma = 0;
 	int arreglo [] = {1,2,3};
 	for(int i=0; i<3; i++){
 		lista_insertar_ultimo(lista6,&arreglo[i]);
 	}
 	print_test("Se insertaron 3 datos en la lista", lista_largo(lista6)==3);
 	lista_iterar(lista6,sumar_datos,&suma);
 	print_test("La funcion iterar funciona correctamente",true);
 	print_test("El resultado de la funcion sumar es 6", suma==6);
 	lista_destruir(lista6,NULL);
 	print_test("Lista ha sido destruida", true);
 	printf("\n");
}

void pruebas_iterador_externo(){
	printf("### INICIO DE PRUEBAS CON ITERADOR EXTERNO ###\n");
	lista_t* lista7 = lista_crear();
	int num1 = 5;
	int num2 = 10;
	print_test("Insertar numero al principio", lista_insertar_ultimo(lista7,&num1));
	print_test("Insertar otro numero al final", lista_insertar_ultimo(lista7,&num2));
	print_test("Lista no vacia", lista_largo(lista7)==2);
	lista_iter_t* iterador = lista_iter_crear(lista7);
	print_test("Iterador creado con exito", iterador!=NULL);
	print_test("Chequeo que el iterador este en la primer pos",lista_iter_ver_actual(iterador)==lista_ver_primero(lista7));
	print_test("Avanzo una posicion con el iterador", lista_iter_avanzar(iterador));
	print_test("El iterador esta en la ultima posicion", lista_iter_ver_actual(iterador)==lista_ver_ultimo(lista7));
	int num3 = 15;
	print_test("Inserto un numero con el iterador", lista_iter_insertar(iterador,&num3));
	print_test("El iterador esta apuntando al num 15",(*((int*)lista_iter_ver_actual(iterador)))==num3);
	print_test("La lista tiene largo 3", lista_largo(lista7)==3);
	print_test("El iterador no esta al final", !lista_iter_al_final(iterador));
	print_test("Borro el elemento insertado", (*((int*)lista_iter_borrar(iterador)))==num3);
	print_test("Chequeo que el iterador este en la ultima posicion", lista_iter_ver_actual(iterador)==lista_ver_ultimo(lista7));
	lista_iter_destruir(iterador);
	print_test("Iterador destruido", true);
	print_test("El primer de la lista es el num 5",(*((int*)lista_ver_primero(lista7)))==num1);
	lista_borrar_primero(lista7);
	lista_borrar_primero(lista7);
	print_test("Se borraron los elementos y la lista quedo vacia", lista_esta_vacia(lista7));
	lista_destruir(lista7,NULL);
	print_test("Lista destruida",true);
	printf("\n");

}

void pruebas_iterador_externo2(){
	printf("### PRUEBAS ITERADOR LISTA VACIA ###\n");
	lista_t* lista8 = lista_crear();
	lista_iter_t* iterador2 = lista_iter_crear(lista8);
	print_test("La lista esta vacia", lista_esta_vacia(lista8));
	print_test("Se creo un iterador", iterador2!=NULL);
	print_test("Iterador esta al final", lista_iter_al_final(iterador2));
	print_test("Iter ver actual es NULL", lista_iter_ver_actual(iterador2)==NULL);
	print_test("No se puede avanzar en lista vacia", !lista_iter_avanzar(iterador2));
	lista_iter_destruir(iterador2);
	print_test("Iterador destruido",true);
	lista_iter_t* iterador3 = lista_iter_crear(lista8);
	print_test("Se creo otro iterador",iterador3!=NULL);
	int numero = 5;
	print_test("Inserto un elemento con iterador", lista_iter_insertar(iterador3,&numero));
	print_test("El largo de la lista ahora es 1", lista_largo(lista8)==1);
	print_test("El actual esta en el numero 5",(*((int*)lista_iter_ver_actual(iterador3)))==numero);
	print_test("El actual es igual al primero de la lista", lista_iter_ver_actual(iterador3)==lista_ver_primero(lista8));
	print_test("Avanzo una posicion", lista_iter_avanzar(iterador3));
	print_test("El iterador esta al final", lista_iter_al_final(iterador3));
	int numero2 = 10;
	bool ok1;
	for(int i=0;i<100;i++){
		ok1=lista_insertar_ultimo(lista8,&numero2);
	}
	print_test("Agrego 100 elementos en ultima posicion",ok1);
	print_test("El largo de la lista es 101", lista_largo(lista8)==101);
	print_test("El iterador sigue al final", lista_iter_al_final(iterador3));
	lista_iter_destruir(iterador3);
	print_test("Iterador destruido", true);
	lista_iter_t* iterador4 = lista_iter_crear(lista8);
	print_test("Se creo otro iterador", iterador4!=NULL);
	print_test("Actual es el primero de la lista", lista_iter_ver_actual(iterador4)==lista_ver_primero(lista8));
	int num = 95;
	print_test("Inserto en primera posicion", lista_iter_insertar(iterador4,&num));
	print_test("El largo de la lista es 102", lista_largo(lista8)==102);
	print_test("El actual es el numero que inserte", (*((int*)lista_iter_ver_actual(iterador4)))==num);
	print_test("Actual sigue siendo el primero de la lista", lista_iter_ver_actual(iterador4)==lista_ver_primero(lista8));
	bool avance=true;
	for(int i=0;i<(lista_largo(lista8))/2;i++){
		avance = lista_iter_avanzar(iterador4);
	}
	print_test("El iterador avanzo a la mitad de la lista", avance);
	print_test("Inserto en la posicion actual del iter",lista_iter_insertar(iterador4,&num));
	print_test("Se aumento el largo de la lista", lista_largo(lista8)==103);
	lista_iter_destruir(iterador4);
	print_test("Iterador4 destruido",true);
	while(!lista_esta_vacia(lista8)){
		lista_borrar_primero(lista8);
	}
	print_test("Se vacio la lista", lista_esta_vacia(lista8));
	lista_destruir(lista8,NULL);
	print_test("Lista destruida",true);
	printf("\n");
	printf("\n");
	printf("\n");
}

void pruebas_con_2_iteradores(){
	printf("### PRUEBAS CON 2 ITERADORES ###\n");
	lista_t* lista9 = lista_crear();
	lista_iter_t* iterador5 = lista_iter_crear(lista9);
	print_test("Se creo un iter", iterador5);
	print_test("El primer iterador apunta a NULL", lista_iter_ver_actual(iterador5)==NULL);
	int prueba = 10;
	bool ok;
	for(int i=0;i<10;i++){
		ok = lista_iter_insertar(iterador5,&prueba);
	}
	print_test("Inserto con el primer iterador", ok);
	print_test("El largo es 10",lista_largo(lista9)==10);
	lista_iter_t* iterador6 = lista_iter_crear(lista9);
	print_test("Hay dos iteradores en la lista", iterador5&&iterador6);
	print_test("El segundo iter es el primero de lalista", lista_iter_ver_actual(iterador6)==lista_ver_primero(lista9));
	print_test("Avanzo con segundo iter", lista_iter_avanzar(iterador6));
	for(int i=0;i<8;i++){
		lista_iter_avanzar(iterador6);
	}
	print_test("Segundo iter esta en el ultimo e la lista", lista_iter_ver_actual(iterador6)==lista_ver_ultimo(lista9));
	lista_iter_avanzar(iterador6);
	print_test("segundo iter al final", lista_iter_al_final(iterador6));
	print_test("No puedo avanzar", !lista_iter_avanzar(iterador6));
	lista_iter_destruir(iterador6);
	print_test("Destruido el 2do iterador", true);
	lista_iter_t* iterador7 = lista_iter_crear(lista9);
	print_test("Creo otro iterador", iterador7);
	for(int i=0;i<10;i++){
		lista_iter_avanzar(iterador5);
	}
	print_test("avanzo con primer iter al final", lista_iter_al_final(iterador5));
	print_test("Inserto uno al final con el primer iter", lista_iter_insertar(iterador5,&prueba));
	print_test("Largo 11", lista_largo(lista9)==11);
	print_test("Avanzo con segundo iter", lista_iter_avanzar(iterador7));
	for(int i=0;i<lista_largo(lista9);i++){
		lista_borrar_primero(lista9);
	}
	lista_iter_destruir(iterador5);
	lista_iter_destruir(iterador7);
	lista_destruir(lista9,NULL);


}

void pruebas_lista_alumno(){
	/*Ejecuta todas las pruebas*/
	pruebas_lista_vacia();
	pruebas_elemento_null();
	pruebas_algunos_elementos();
	pruebas_muchos_elementos();
	pruebas_casos_borde();
	pruebas_iterador_interno();
	pruebas_iterador_externo();
	pruebas_iterador_externo2();
	pruebas_con_2_iteradores();
}