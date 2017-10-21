#include "wachencoin.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

	if(argc!=2){
		fputs("FALTAN/SOBRAN COMANDOS\n", stderr);
		return 0;
	}
	else{
		
		usuario_t** arreglo_usuarios = estructura_usuarios(argv[1]);
		cola_t* cola_procesamientos = cola_crear();
		size_t largo = 0;
		char* linea = NULL;
		ssize_t len;
		while((len=getline(&linea,&largo,stdin))>0){
			if(linea[len-1]=='\n') linea[len-1]='\0';
			if(procesar_entrada_stdin(linea,cola_procesamientos,arreglo_usuarios)!=0) break;
			
		}
		free(linea);
		cola_destruir(cola_procesamientos,wrapper);
		for(int i=0;arreglo_usuarios[i];i++){
			destruir_usuario(arreglo_usuarios[i]);
		}	
		free(arreglo_usuarios);
	
	}
	return 0;
}