#define _POSIX_C_SOURCE 200809L
#include "pagos.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "usuarios.h"

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
