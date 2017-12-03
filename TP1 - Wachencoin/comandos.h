#ifndef COMANDOS_H
#define COMANDOS_H

#include "pagos.h"
#include "usuarios.h"
#include "lista.h"
#include "pila.h"
#include <stdlib.h>
#include <stdbool.h>

//Recibe un arreglo de usuarios, un monto y un ID.
//Chequea si el monto del usuario ubicado en la posicon Id
//del arreglo es el mismo al pasado por parametro.
//Devuelve un booleano.
bool validar_pago(usuario_t** arreglo_usuarios, double monto, size_t id_param);

//Recibe una arreglo de usuarios, unas coordenadas y un ID.
//Chequea si los datos del usuario ubicado en la posicion ID
//coinciden con los datos pasados por parametro.
//Devuelve un booleano.
bool validar_usuario(usuario_t** arreglo_usuarios, char* coord_param, size_t id_param);

//Recibe un arreglo de usuarios, 2 ID's y un monto.
//Chequea si el saldo del usuario con ID_1 es suficiente para transferirle
//el pago al usuario con ID_2.
//Devuelve un booleano.
bool pagar(usuario_t** arreglo_usuarios, size_t id_1, size_t id_2, double monto);

//Recibe la lista de procesamientos principal del programa, un ID, un monto y un codigo.
//Agrega a la lista el pago que se crea con los parametros usados.
//Si todo sale bien, imprime OK. Caso contrario, imprime ERROR.
bool agregar_pago(lista_t* lista_procesamientos, size_t id, double monto, char* codigo);

//Recibe la lista de procesamientos principal del programa.
//Cuenta cuantos pagos quedan sin procesar y el monto total de esos pagos.
//Los imprime.
bool mostrar_pagos_pendientes(lista_t* lista_procesamientos);

//Funcion similar al algoritmo de Calculadora Polaca Inversa.
//Recibe la lista de procesamientos principal del programa, el arreglo de usuarios
//y un entero "N". Procesa N pagos pendientes
bool procesar(int n, lista_t* lista_procesamientos, usuario_t** arreglo_usuarios);

//Funcion que recibe el nombre de un archivo y guarda el estado de las cuentas
//en ese archivo.
bool guardar_cuentas(char* archivo, usuario_t** arreglo_usuarios);


#endif //COMANDOS_H