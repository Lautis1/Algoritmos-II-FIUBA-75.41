#define _POSIX_C_SOURCE 200809L
#include "wachencoin.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void destruir_pago(pago_t* pago){
	free(pago->codigo);
	free(pago);
}

void wrapper(void* dato){
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

//Crea un arreglo de usuario_t* usuarios con los datos leidos del archivo.
//Devuelve ese arreglo.
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
//#############################################################################
//IMPLEMENTACION DE OPERACIONES (CODIGOS DE PAGO)
//validar_usuario, validar_pago, pagar.
//Los usuarios estan guardados en un arreglo.


//Recibe una arreglo de usuarios, unas coordenadas y un ID.
//Chequea si los datos del usuario ubicado en la posicion ID
//coinciden con los datos pasados por parametro.
//Devuelve un booleano.
bool validar_usuario(usuario_t** arreglo_usuarios, char* coord_param, size_t id_param){

	usuario_t* usuario1 = arreglo_usuarios[id_param];
	if(!usuario1) return false;
	return (strcmp(usuario1->coordenadas,coord_param)==0 && usuario1->id == id_param);

}

//Recibe un arreglo de usuarios, un monto y un ID.
//Chequea si el monto del usuario ubicado en la posicon Id
//del arreglo es el mismo al pasado por parametro.
//Devuelve un booleano.
bool validar_pago(usuario_t** arreglo_usuarios, double monto, size_t id_param){

	usuario_t* usuario1 = arreglo_usuarios[id_param];
	if(!usuario1) return false;
	if(usuario1->saldo >= monto && usuario1->id == id_param) return true;
	return false;
}

//Recibe un arreglo de usuarios, 2 ID's y un monto.
//Chequea si el saldo del usuario con ID_1 es suficiente para transferirle
//el pago al usuario con ID_2.
//Devuelve un booleano.
bool pagar(usuario_t** arreglo_usuarios, size_t id_1, size_t id_2, double monto){

	usuario_t* usuario_pagador = arreglo_usuarios[id_1];
	usuario_t* usuario_cobrador = arreglo_usuarios[id_2];
	if(!usuario_pagador || !usuario_cobrador) return false;
	if(usuario_pagador->saldo >= monto){
		usuario_cobrador->saldo += monto;
		usuario_pagador->saldo -= monto;
		return true;
	}
	return false;
}

//#############################################################################
//IMPLEMENTACION DE COMANDOS PARA LA INTERFAZ
//agregar_pago, pagos_pendientes, procesar, guardar_cuentas, finalizar.


//Recibe la cola de procesamientos principal del programa, un ID, un monto y un codigo.
//Agrega a la cola el pago que se crea con los parametros usados.
//Si todo sale bien, imprime OK. Caso contrario, imprime ERROR.
bool agregar_pago(cola_t* cola_procesamientos, size_t id, double monto, char* codigo){

	pago_t* pago = pago_crear(id,codigo,monto);
	if(!pago) return false;
	if(!cola_encolar(cola_procesamientos,pago)) return false;
	return true;
}

//Recibe la cola de procesamientos principal del programa.
//Cuenta cuantos pagos quedan sin procesar y el monto total de esos pagos.
//Los imprime.
bool pagos_pendientes(cola_t* cola_procesamientos){

	lista_t* lista_auxiliar = lista_crear();
	if(!lista_auxiliar) return false;
	double monto_total = 0;
	int cantidad_pagos = 0;
	while(!cola_esta_vacia(cola_procesamientos)){
		pago_t* pago = cola_desencolar(cola_procesamientos);
		cantidad_pagos++;
		monto_total += pago->monto;
		lista_insertar_ultimo(lista_auxiliar,pago);
	}
	while(!lista_esta_vacia(lista_auxiliar)){
		cola_encolar(cola_procesamientos, lista_borrar_primero(lista_auxiliar));
	}
	fprintf(stdout,"%d,%.3f\n",cantidad_pagos,monto_total);
	lista_destruir(lista_auxiliar,NULL);
	return true;
}

//Funcion similar al algoritmo de Calculadora Polaca Inversa.
//Procesa "N" pagos pendientes.
bool procesar(int n, cola_t* cola_procesamientos, usuario_t** arreglo_usuarios){

	bool ok = true;
	for(int i=0; i<n && !cola_esta_vacia(cola_procesamientos);i++){
		pila_t* pila = pila_crear();
		if(!pila) return false;
		pago_t* pago = (pago_t*)cola_desencolar(cola_procesamientos);
		char** linea = split(pago->codigo,';');
		int j=0;
		while(linea[j]){
			if(strcmp(linea[j],"validar_usuario")==0){
				char* coordenadas = pila_desapilar(pila);
				size_t id = (size_t)atoi((char*)pila_desapilar(pila));
				if(!validar_usuario(arreglo_usuarios,coordenadas,id)){
					fprintf(stderr,"Error en pago %zu\n", pago->id);
					ok = false;
					break;					
				}
			}
			else if(strcmp(linea[j],"validar_pago")==0){
				double monto = atof((char*)pila_desapilar(pila));
				size_t id = (size_t)atoi((char*)pila_desapilar(pila));
				if(!validar_pago(arreglo_usuarios,monto,id)){
					fprintf(stderr,"Error en pago %zu\n", pago->id);
					ok = false;
					break;
				}
			}
			else if(strcmp(linea[j],"pagar")==0){
				size_t id_1 = (size_t)atoi((char*)pila_desapilar(pila));
				size_t id_2 = (size_t)atoi((char*)pila_desapilar(pila));
				double monto = atof((char*)pila_desapilar(pila));
				if(!pagar(arreglo_usuarios, id_1, id_2, monto)){
					fprintf(stderr,"Error en pago %zu\n", pago->id);
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
//#############################################################################
//Función que procesa lo que el usuario escribe a traves de la consola
int procesar_entrada_stdin(char* entrada, cola_t* cola_procesamientos,usuario_t** arreglo_usuarios){

	char** input = split(entrada,' ');
	char* com1 = "agregar_pago";
	char* com2 = "procesar";
	char* com3 = "pagos_pendientes";
	char* com4 = "guardar_cuentas";
	char* com5 = "finalizar";
	int indice_corte = 0;
	int cant_parametros = 0;
	while(input[cant_parametros]){
		cant_parametros++;
	}
	if(strcmp(input[0],com5)==0){
		indice_corte = 1;
	}
	else if(strcmp(input[0],com1)==0){
		if(cant_parametros==4) agregar_pago(cola_procesamientos,(size_t)atoi(input[1]),atof(input[2]),input[3]);
		else{ fprintf(stderr,"Error en comando %s\n",com1);
			indice_corte = -1;
		}
	}
	else if(strcmp(input[0],com2)==0){
		if(cant_parametros==2) procesar(atoi(input[1]),cola_procesamientos,arreglo_usuarios);
		else{ fprintf(stderr,"Error en comando %s\n",com2);
			indice_corte = -1;
		}
	}
	else if(strcmp(input[0],com3)==0){
		if(cant_parametros==1) pagos_pendientes(cola_procesamientos);
		else{ fprintf(stderr,"Error en comando %s\n",com3);
			indice_corte = -1;
		}
	}
	else if(strcmp(input[0],com4)==0){
		if(cant_parametros==2) guardar_cuentas(input[1],arreglo_usuarios);
		else{ fprintf(stderr,"Error en comando %s\n",com4);
			indice_corte = -1;
		}
	}
	else{
		fprintf(stderr,"Error en comando %s\n",input[0]);
		indice_corte = -1;
	}
	free_strv(input);
	if(indice_corte>=0) fputs("OK\n",stdout);
	return indice_corte;
}
		
		










