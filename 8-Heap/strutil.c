#define _POSIX_C_SOURCE 200809L
#include "strutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Devuelve en un arreglo dinámico terminado en NULL con todos los subsegmentos
 * de 'str' separados por el carácter 'sep'. Tanto el arreglo devuelto como las
 * cadenas que contiene son allocadas dinámicamente.
 *
 * Quien llama a la función toma responsabilidad de la memoria dinámica del
 * arreglo devuelto. La función devuelve NULL si falló alguna llamada a
 * malloc(), o si 'sep' es '\0'.
 */

char** split(const char* str, char sep){
	if(sep == '\0') return NULL;
	size_t tamanio = 0;
	for(int i=0;str[i]!='\0';i++){
		if(str[i]==sep) tamanio++;
	}
	size_t longitud = strlen(str);
	char** array = malloc((tamanio+2)*sizeof(char*));
	if(!array) return NULL;
	size_t index=0;
	size_t cantidad_caracteres = 0;
	int pos = 0;
	while(index<=longitud){
		if(str[index] == sep || str[index] == '\0'){
			char* cadena = malloc(sizeof(char)*(cantidad_caracteres+1));
			strncpy(cadena,&str[index-cantidad_caracteres],cantidad_caracteres);
			cadena[cantidad_caracteres] = '\0';
			array[pos] = cadena;
			pos++;
			cantidad_caracteres = 0;
		}else{
			cantidad_caracteres++;
		}
		index++;
	}
	array[tamanio+1] = NULL;
	return array;
}


/*
 * Libera un arreglo dinámico de cadenas, y todas las cadenas que contiene.
 */
void free_strv(char *strv[]){
	int index = 0;
	while(strv[index]!=NULL){
		free(strv[index]);
		index++;
	}
	free(strv);
}



