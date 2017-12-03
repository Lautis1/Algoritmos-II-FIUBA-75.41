#define _POSIX_C_SOURCE 200809L
#include "wachencoin.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "comandos.h"


//CONSTANTES DE COMANDOS Y OPERACIONES
#define PROCESAR "procesar"
#define NUEVO_PAGO "agregar_pago"
#define PAGOS_PENDIENTES "pagos_pendientes"
#define FINALIZAR "finalizar"
#define GUARDAR "guardar_cuentas"

//Funcion auxiliar que recibe una cadena y cuenta la cantidad de palabras que tiene
//La uso para contar la cantidad de parametros que ingresa el usuario por stdin.
int contar_cantidad_parametros(char** cadena){

	int cantidad = 0;
	while(cadena[cantidad]){
		cantidad++;
	}
	return cantidad;
}
//#############################################################################
/*FUNCION AUXILIAR*/
//Funcion que imprime un error de comando por STDERR.
//La utilizo en la funcion PROCESAR_ENTRADA_STDIN para no repetir codigo.
void imprimir_error_comando_stderr(char* comando){
	
	fprintf(stderr, "Error en comando %s\n", comando);
}

//Función que procesa lo que el usuario escribe a traves de la consola
int procesar_entrada_stdin(char* entrada, lista_t* lista_procesamientos,usuario_t** arreglo_usuarios){

	char** input = split(entrada,' ');
	int indice_corte = 0;
	if(strcmp(input[0],FINALIZAR)==0){
		indice_corte = 1;
	}
	else if(strcmp(input[0],NUEVO_PAGO)==0){
		if(contar_cantidad_parametros(input)==4) agregar_pago(lista_procesamientos,(size_t)atoi(input[1]),atof(input[2]),input[3]);
		else{imprimir_error_comando_stderr(NUEVO_PAGO);
			indice_corte = -1;
		}
	}
	else if(strcmp(input[0],PROCESAR)==0){
		if(contar_cantidad_parametros(input)==2) procesar(atoi(input[1]),lista_procesamientos,arreglo_usuarios);
		else{imprimir_error_comando_stderr(PROCESAR);
			indice_corte = -1;
		}
	}
	else if(strcmp(input[0],PAGOS_PENDIENTES)==0){
		if(contar_cantidad_parametros(input)==1) mostrar_pagos_pendientes(lista_procesamientos);
		else{imprimir_error_comando_stderr(PAGOS_PENDIENTES);
			indice_corte = -1;
		}
	}
	else if(strcmp(input[0],GUARDAR)==0){
		if(contar_cantidad_parametros(input)==2) guardar_cuentas(input[1],arreglo_usuarios);
		else{imprimir_error_comando_stderr(GUARDAR);
			indice_corte = -1;
		}
	}
	else{
		imprimir_error_comando_stderr(input[0]);
		indice_corte = -1;
	}
	free_strv(input);
	if(indice_corte>=0) fputs("OK\n",stdout);
	return indice_corte;
}
		
		










