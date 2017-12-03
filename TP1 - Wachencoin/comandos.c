#include "comandos.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define VALIDAR_USER "validar_usuario"
#define VALIDAR_PAGO "validar_pago"
#define PAGAR "pagar"

//Recibe un arreglo de usuarios, un monto y un ID.
//Chequea si el monto del usuario ubicado en la posicon Id
//del arreglo es el mismo al pasado por parametro.
//Devuelve un booleano.
bool validar_pago(usuario_t** arreglo_usuarios, double monto, size_t id_param){

	usuario_t* usuario1 = arreglo_usuarios[id_param];
	if(!usuario1) return false;
	return (puede_pagar(usuario1,monto) && usuario1->id == id_param);
}

//Recibe una arreglo de usuarios, unas coordenadas y un ID.
//Chequea si los datos del usuario ubicado en la posicion ID
//coinciden con los datos pasados por parametro.
//Devuelve un booleano.
bool validar_usuario(usuario_t** arreglo_usuarios, char* coord_param, size_t id_param){

	usuario_t* usuario1 = arreglo_usuarios[id_param];
	if(!usuario1) return false;
	return (strcmp(usuario1->coordenadas,coord_param)==0 && usuario1->id == id_param);

}

//Recibe un arreglo de usuarios, 2 ID's y un monto.
//Chequea si el saldo del usuario con ID_1 es suficiente para transferirle
//el pago al usuario con ID_2.
//Devuelve un booleano.
bool pagar(usuario_t** arreglo_usuarios, size_t id_1, size_t id_2, double monto){

	usuario_t* usuario_pagador = arreglo_usuarios[id_1];
	usuario_t* usuario_cobrador = arreglo_usuarios[id_2];
	if(!usuario_pagador || !usuario_cobrador) return false;
	if(puede_pagar(usuario_pagador,monto)){
		usuario_cobrador->saldo += monto;
		usuario_pagador->saldo -= monto;
		return true;
	}
	return false;
}

//#############################################################################
//IMPLEMENTACION DE COMANDOS PARA LA INTERFAZ
//agregar_pago, pagos_pendientes, procesar, guardar_cuentas, finalizar.


//Recibe la lista de procesamientos principal del programa, un ID, un monto y un codigo.
//Agrega a la lista el pago que se crea con los parametros usados.
//Si todo sale bien, imprime OK. Caso contrario, imprime ERROR.
bool agregar_pago(lista_t* lista_procesamientos, size_t id, double monto, char* codigo){

	pago_t* pago = pago_crear(id,codigo,monto);
	if(!pago) return false;
	if(!lista_insertar_ultimo(lista_procesamientos,pago)) return false;
	return true;
}

//Recibe la lista de procesamientos principal del programa.
//Cuenta cuantos pagos quedan sin procesar y el monto total de esos pagos.
//Los imprime.
bool mostrar_pagos_pendientes(lista_t* lista_procesamientos){

	double monto_total = 0;
	int cantidad_pagos = 0;
	size_t largo = lista_largo(lista_procesamientos);
	for(size_t i=0; i<largo;i++){
		pago_t* pago = lista_borrar_primero(lista_procesamientos);
		cantidad_pagos++;
		monto_total += pago->monto;
		lista_insertar_ultimo(lista_procesamientos,pago);
	}
	fprintf(stdout,"%d,%.3f\n",cantidad_pagos,monto_total);
	return true;
}

/*FUNCION AUXILIAR*/
//Funcion que imprime un error de pago por STDERR.
//La utilizo en la funcion PROCESAR para no repetir codigo.
void imprimir_error_pago_stderr(size_t id_pago){
	fprintf(stderr, "Error en pago %zu\n", id_pago);
}

//Funcion similar al algoritmo de Calculadora Polaca Inversa.
//Recibe la lista de procesamientos principal del programa, el arreglo de usuarios
//y un entero "N". Procesa N pagos pendientes
bool procesar(int n, lista_t* lista_procesamientos, usuario_t** arreglo_usuarios){

	bool ok = true;
	for(int i=0; i<n && !lista_esta_vacia(lista_procesamientos);i++){
		pila_t* pila = pila_crear();
		if(!pila) return false;
		pago_t* pago = (pago_t*)lista_borrar_primero(lista_procesamientos);
		char** linea = split(pago->codigo,';');
		int j=0;
		while(linea[j]){
			if(strcmp(linea[j], VALIDAR_USER)==0){
				char* coordenadas = pila_desapilar(pila);
				size_t id = (size_t)atoi((char*)pila_desapilar(pila));
				if(!validar_usuario(arreglo_usuarios,coordenadas,id)){
					imprimir_error_pago_stderr(pago->id);
					ok = false;
					break;					
				}
			}
			else if(strcmp(linea[j],VALIDAR_PAGO)==0){
				double monto = atof((char*)pila_desapilar(pila));
				size_t id = (size_t)atoi((char*)pila_desapilar(pila));
				if(!validar_pago(arreglo_usuarios,monto,id)){
					imprimir_error_pago_stderr(pago->id);
					ok = false;
					break;
				}
			}
			else if(strcmp(linea[j],PAGAR)==0){
				size_t id_1 = (size_t)atoi((char*)pila_desapilar(pila));
				size_t id_2 = (size_t)atoi((char*)pila_desapilar(pila));
				double monto = atof((char*)pila_desapilar(pila));
				if(!pagar(arreglo_usuarios, id_1, id_2, monto)){
					imprimir_error_pago_stderr(pago->id);
					ok = false;
					break;
				}
			}
			else{
				pila_apilar(pila,linea[j]);
			}
			j++;
		}
		free_strv(linea);
		destruir_pago(pago);
		pila_destruir(pila);
	}
	return ok;
}

//Funcion que recibe el nombre de un archivo y guarda el estado de las cuentas
//en ese archivo.
bool guardar_cuentas(char* archivo, usuario_t** arreglo_usuarios){

	FILE* archivo_a_guardar = fopen(archivo,"w");
	if(!archivo_a_guardar) return false;
	for(int i=0;arreglo_usuarios[i]!=NULL;i++){
		usuario_t* usuario_a_guardar = arreglo_usuarios[i];
		size_t id = usuario_a_guardar->id;
		double saldo = usuario_a_guardar->saldo;
		char* coord = usuario_a_guardar->coordenadas;
		fprintf(archivo_a_guardar,"%zu,%.3f,%s\n",id,saldo,coord);
	}
	fclose(archivo_a_guardar);
	return true;
}