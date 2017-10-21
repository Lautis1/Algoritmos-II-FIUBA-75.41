#include "tp0.h"
#include <stdio.h>

/* ******************************************************************
 *                     FUNCIONES A COMPLETAR
 * *****************************************************************/

/* swap() intercambia dos valores enteros.
 */
void swap (int* x, int* y) {
	int entero_auxiliar = *y;
	*y = *x;
	*x = entero_auxiliar;
}
/* maximo() busca el mayor elemento del arreglo y devuelve su posicion.
 * Si el vector es de largo 0, devuelve -1.
 */
int maximo(int vector[], int n) {
	if(n==0) return -1;
	int posicion_maximo = 0;
	for(int i=0;i<n;i++){
		if(vector[i]>vector[posicion_maximo]) posicion_maximo = i;
	}
	return posicion_maximo;
}

/* La función comparar recibe dos vectores y sus respectivas longitudes
 * y devuelve -1 si el primer vector es menor que el segundo; 0 si son
 * iguales; y 1 si el segundo es menor.
 *
 * Un vector es menor a otro cuando al compararlos elemento a elemento,
 * el primer elemento en el que difieren no existe o es menor.
 */
int comparar(int vector1[], int n1, int vector2[], int n2) {
	if(n1 == 0 && n2!=0) return -1;
	if(n1!=0 && n2==0) return 1;
	int vector_corto; 
	if(n1<n2) vector_corto = n1;
	else{
		vector_corto = n2;}
		
	for(int i=0;i<vector_corto;i++){
		if(vector1[i] < vector2[i]) return -1;
		if(vector1[i] > vector2[i]) return 1;
	}
	if(n1 < n2) return -1;
	if(n1 > n2) return 1;
	return 0;
}


/* selection_sort() ordena el arreglo recibido mediante el algoritmo de
 * selección.
 */
void seleccion(int vector[], int n) {
	while(n>0){
		int posicion = maximo(vector,n);
		swap(&vector[posicion],&vector[n-1]);
		n-=1;
	}
}
