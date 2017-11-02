#include "wachencoin.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

	if(argc!=2){
		fputs("FALTAN/SOBRAN COMANDOS\n", stderr);
		return 0;
	}
	usuario_t** arreglo_usuarios = estructura_usuarios(argv[1]);
	lista_t* lista_procesamientos = lista_crear();
	size_t largo = 0;
	char* linea = NULL;
	ssize_t len;
	while((len=getline(&linea,&largo,stdin))>0){
		if(linea[len-1]=='\n') linea[len-1]='\0';
		if(procesar_entrada_stdin(linea,lista_procesamientos,arreglo_usuarios)!=0) break;
	}
	free(linea);
	lista_destruir(lista_procesamientos,destruir_pagos_wrapper);
	destruir_array_usuarios(arreglo_usuarios);
	return 0;
}