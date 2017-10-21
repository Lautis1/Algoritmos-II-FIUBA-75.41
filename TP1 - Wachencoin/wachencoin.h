#ifndef WACHENCOIN_H
#define WACHENCOIN_H
#define _POSIX_C_SOURCE 200809L
#include <stddef.h>
#include <stdbool.h>
#include "pila.h"
#include "lista.h"
#include "cola.h"
#include "strutil.h"
#include <string.h>


typedef struct usuario usuario_t;
typedef struct pago pago_t;

//CREACION DEL TIPO DE DATO
usuario_t* usuario_crear(size_t id, char* coordenadas, double saldo);
pago_t* pago_crear(size_t id, char* codigo, double monto);
usuario_t** estructura_usuarios(const char* archivo);

//OPERACIONES
bool validar_usuario(usuario_t** arreglo_usuarios,char* coordenadas_param,size_t id);
bool validar_pago(usuario_t** arreglo_usuarios,double monto,size_t id);
bool pagar(usuario_t** arreglo_usuarios,size_t id_1,size_t id_2,double monto);

//COMANDOS
bool agregar_pago(cola_t* cola_procesamientos,size_t id,double monto,char* codigo);
bool pagos_pendientes(cola_t* cola_procesamientos);
bool procesar(int n,cola_t* cola_procesamientos,usuario_t** arreglo_usuarios);
bool guardar_cuentas(char* archivo, usuario_t** arreglo_usuarios);

//FUNCIONES AUXILIARES
void destruir_usuario(usuario_t* usuario);
void destruir_pago(pago_t* pago);
void wrapper(void* dato);

//FUNCIONES PRINCIPALES;
int procesar_entrada_stdin(char* entrada,cola_t* cola_procesamientos,usuario_t** arreglo_usuarios);


#endif //WACHENCOIN_H