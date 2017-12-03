#ifndef USUARIOS_H
#define USUARIOS_H

#define _POSIX_C_SOURCE 200809L
#include <stddef.h>
#include <stdbool.h>
#include "strutil.h"

typedef struct usuario{
	size_t id;
	char* coordenadas;
	double saldo;
}usuario_t;

//Crea un usuario a partir de un id, coordenadas y saldo.
//Devuelve ese usuario
usuario_t* usuario_crear(size_t id, char* coordenadas, double saldo);

void destruir_usuario(usuario_t* usuario);

//Lee la cantidad de lineas que hay en el archivo.
//Cada linea representa un usuario y su informacion.
size_t cantidad_de_usuarios_archivo(const char* archivo);

//Crea un arreglo de usuarios a partir de un archivo.
//Dentro de ese arreglo habra usuarios del tipo usuario_t*.
//Devuelve el arreglo.
usuario_t** estructura_usuarios(const char* archivo);

void destruir_array_usuarios(usuario_t** arreglo_usuarios);

//Funcion auxiliar "puede_pagar" que decide, segun el saldo del usuario,
//si esta en condiciones de realizar un pago.
bool puede_pagar(usuario_t* usuario, double monto);

#endif //USUARIOS_H

