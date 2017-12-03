#include "usuarios.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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


//##############################################################################
//Lee la cantidad de lineas que hay en el archivo.
//Cada linea representa un usuario y su informacion.
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

/*FUNCION AUXILIAR*/
//Funcion auxiliar "puede_pagar" que decide, segun el saldo del usuario,
//si esta en condiciones de realizar un pago.
bool puede_pagar(usuario_t* usuario, double monto){
	return (usuario->saldo >= monto);
}