#ifndef PAGOS_H
#define PAGOS_H

#include <stddef.h>
#include <stdbool.h>
#include "usuarios.h"

typedef struct pago{
	size_t id;
	char* codigo;
	double monto;
}pago_t;

//Crea un pago a partir de un id, codigo y monto de pago.
//Devuelve ese pago
pago_t* pago_crear(size_t id, char* codigo, double monto);

//FUNCIONES AUXILIARES

void destruir_pago(pago_t* pago);
void destruir_pagos_wrapper(void* dato);

#endif //PAGOS_H