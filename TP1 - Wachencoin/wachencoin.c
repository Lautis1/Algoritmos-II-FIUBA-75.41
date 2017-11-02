#define _POSIX_C_SOURCE 200809L
#include "wachencoin.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//CONSTANTES DE COMANDOS Y OPERACIONES
#define PROCESAR "procesar"
#define NUEVO_PAGO "agregar_pago"
#define PAGOS_PENDIENTES "pagos_pendientes"
#define FINALIZAR "finalizar"
#define GUARDAR "guardar_cuentas"

#define VALIDAR_USER "validar_usuario"
#define VALIDAR_PAGO "validar_pago"
#define PAGAR "pagar"


struct usuario{
	size_t id;
	char* coordenadas;
	double saldo;
};

struct pago{
	size_t id;
	char* codigo;
	double monto;
};
//###########################################################################
//CREACION DEL USUARIO
usuario_t* usuario_crear(size_t id, char* coordenadas, double saldo){
	
	usuario_t* usuario = malloc(sizeof(usuario_t));
	if(!usuario) return NULL;
	usuario->id = id;
	usuario->saldo = saldo;
	usuario->coordenadas = strdup(coordenadas);
	return usuario;
}

void destruir_usuario(usuario_t* usuario){
	free(usuario->coordenadas);
	free(usuario);
}

//#############################################################################
//CREACION DEL PAGO
pago_t* pago_crear(size_t id, char* codigo, double monto){
	
	pago_t* pago = malloc(sizeof(pago_t));
	if(!pago) return NULL;
	pago->id = id;
	pago->codigo = strdup(codigo);
	pago->monto = monto;
	return pago;
}

//Destruye el pago recibido por parametro.
//Se destruye tambien el codigo ya que es 
//reservado dinamicamente en memoria.
void destruir_pago(pago_t* pago){
	free(pago->codigo);
	free(pago);
}

//Funcion wrapper que "enmascara el comportamiento de una funcion", modificando
//su firma para poder usar dentro, otra funcion con distinta firma.
//Destruye el pago.
void destruir_pagos_wrapper(void* dato){
	destruir_pago(dato);
}
//#############################################################################
//MANEJO DEL ARCHIVO CSV
//Leo el archivo 2 veces: una para contar la cantidad de lineas (usuarios)
//que hay, asi creo el struct usuario_t* con tamaño cantidad_de_lineas.
//La 2da vez que lo recorro es para ir creando con split cada usuario
//e ir almacenandolo en el struct.
size_t cantidad_de_usuarios_archivo(const char* archivo){
	
	size_t cantidad_lineas = 0;
	size_t leidos = 0;
	char* linea = NULL;
	FILE* archivo_usuarios = fopen(archivo,"r");
	while(getline(&linea, &leidos, archivo_usuarios)>0){
		cantidad_lineas++;
	}
	fclose(archivo_usuarios);
	free(linea);
	return cantidad_lineas;
}

//Crea un arreglo de usuarios a partir de un archivo.
//Dentro de ese arreglo habra usuarios del tipo usuario_t*.
//Devuelve el arreglo.
usuario_t** estructura_usuarios(const char* archivo){

	FILE* archivo_usuarios = fopen(archivo,"r");
	if(!archivo_usuarios){
		fputs("Error al leer el archivo\n",stdout);
		return NULL;
	}
	size_t lineas = cantidad_de_usuarios_archivo(archivo);
	usuario_t** arreglo_usuarios = malloc((lineas+1)*sizeof(usuario_t*));
	if(!arreglo_usuarios){
		return NULL;
	}
	size_t leidos = 0;
	char* linea = NULL;
	ssize_t len;
	while((len=getline(&linea, &leidos, archivo_usuarios))>0){
		if(linea[len-1]=='\n') linea[len-1]='\0';
		char** info = split(linea,',');
		usuario_t* usuario = usuario_crear((size_t)atoi(info[0]),info[2],atof(info[1]));
		arreglo_usuarios[usuario->id] = usuario;
		free_strv(info);
	}
	arreglo_usuarios[lineas] = NULL;
	free(linea);
	fclose(archivo_usuarios);
	return arreglo_usuarios;
}

//Funcion de destruccion del arreglo de usuarios.
void destruir_array_usuarios(usuario_t** arreglo_usuarios){

	size_t indice=0;
	while(arreglo_usuarios[indice]!=NULL){
		destruir_usuario(arreglo_usuarios[indice]);
		indice++;
	}
	free(arreglo_usuarios);
}
//#############################################################################
//IMPLEMENTACION DE OPERACIONES (CODIGOS DE PAGO)
//validar_usuario, validar_pago, pagar.
//Los usuarios estan guardados en un arreglo.

/*FUNCION AUXILIAR*/
//Funcion de comparacion. Recibe dos char* y los compara:
//-devuelve 0 si son iguales
//-menor a 0 si el 2do char* es menor al primero
//-mayor a 0 si el 2do char* es mayor al primero.
int comparacion_cadenas(char* cadena1, char* cadena2){
	return strcmp(cadena1,cadena2);
}

/*FUNCION AUXILIAR*/
//Funcion auxiliar "puede_pagar" que decide, segun el saldo del usuario,
//si esta en condiciones de realizar un pago.
bool puede_pagar(usuario_t* usuario, double monto){
	return (usuario->saldo >= monto);
}
//Recibe una arreglo de usuarios, unas coordenadas y un ID.
//Chequea si los datos del usuario ubicado en la posicion ID
//coinciden con los datos pasados por parametro.
//Devuelve un booleano.
bool validar_usuario(usuario_t** arreglo_usuarios, char* coord_param, size_t id_param){

	usuario_t* usuario1 = arreglo_usuarios[id_param];
	if(!usuario1) return false;
	return (comparacion_cadenas(usuario1->coordenadas,coord_param)==0 && usuario1->id == id_param);

}

//Recibe un arreglo de usuarios, un monto y un ID.
//Chequea si el monto del usuario ubicado en la posicon Id
//del arreglo es el mismo al pasado por parametro.
//Devuelve un booleano.
bool validar_pago(usuario_t** arreglo_usuarios, double monto, size_t id_param){

	usuario_t* usuario1 = arreglo_usuarios[id_param];
	if(!usuario1) return false;
	return (puede_pagar(usuario1,monto) && usuario1->id == id_param);
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
			if(comparacion_cadenas(linea[j], VALIDAR_USER)==0){
				char* coordenadas = pila_desapilar(pila);
				size_t id = (size_t)atoi((char*)pila_desapilar(pila));
				if(!validar_usuario(arreglo_usuarios,coordenadas,id)){
					imprimir_error_pago_stderr(pago->id);
					ok = false;
					break;					
				}
			}
			else if(comparacion_cadenas(linea[j],VALIDAR_PAGO)==0){
				double monto = atof((char*)pila_desapilar(pila));
				size_t id = (size_t)atoi((char*)pila_desapilar(pila));
				if(!validar_pago(arreglo_usuarios,monto,id)){
					imprimir_error_pago_stderr(pago->id);
					ok = false;
					break;
				}
			}
			else if(comparacion_cadenas(linea[j],PAGAR)==0){
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
	if(comparacion_cadenas(input[0],FINALIZAR)==0){
		indice_corte = 1;
	}
	else if(comparacion_cadenas(input[0],NUEVO_PAGO)==0){
		if(contar_cantidad_parametros(input)==4) agregar_pago(lista_procesamientos,(size_t)atoi(input[1]),atof(input[2]),input[3]);
		else{imprimir_error_comando_stderr(NUEVO_PAGO);
			indice_corte = -1;
		}
	}
	else if(comparacion_cadenas(input[0],PROCESAR)==0){
		if(contar_cantidad_parametros(input)==2) procesar(atoi(input[1]),lista_procesamientos,arreglo_usuarios);
		else{imprimir_error_comando_stderr(PROCESAR);
			indice_corte = -1;
		}
	}
	else if(comparacion_cadenas(input[0],PAGOS_PENDIENTES)==0){
		if(contar_cantidad_parametros(input)==1) mostrar_pagos_pendientes(lista_procesamientos);
		else{imprimir_error_comando_stderr(PAGOS_PENDIENTES);
			indice_corte = -1;
		}
	}
	else if(comparacion_cadenas(input[0],GUARDAR)==0){
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
		
		










