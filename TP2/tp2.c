
#include "tp2.h"
#include "hash.h"
#include "lista.h"
#include "heap.h"
#include "strutil.h"
#include "abb.h"

#define AGREGAR_ARCHIVO "agregar_archivo"
#define VISITANTES "ver_visitantes"
#define VISITADOS "ver_mas_visitados"

#define CANT_PARAM_AGREGAR 2
#define CANT_PARAM_VISITANTES 3
#define CANT_PARAM_VISITADOS 2

#define CANT_POS_ARRAY_IP 4

#define TAM_BUFFER 300

/**************************************************************************************/
//Funcion que recibe un arbol de visitantes y un hash con los recursos mas solicitados del log.
//Lee por entrada standard lo que ingresa el usuario y llama a la funcion que procesa esos datos.
void recibir_comandos(abb_t* visitantes, hash_t* recursos_mas_solicitados) {
    
    char str[TAM_BUFFER];
    void* estado;
    do {
        estado = fgets(str, TAM_BUFFER, stdin);
        quitar_caracter_new_line(str);
    }
    while (estado != NULL && procesar_entrada_stdin(str, visitantes, recursos_mas_solicitados) == 0);
}

//Funcion que itera un hash, aplicandole la funcion visitar a cada item.
void iterar_hash(hash_t* hash, void visitar(const char* clave, void *dato, void *extra), void *extra) {
    
    hash_iter_t* iter_hash = hash_iter_crear(hash);
    if(iter_hash == NULL) return;
    while (!hash_iter_al_final(iter_hash)) {
        char* clave_actual = (char*)hash_iter_ver_actual(iter_hash);
        void* dato_actual = hash_obtener(hash, clave_actual);
        visitar(clave_actual, dato_actual, extra);
        hash_iter_avanzar(iter_hash);
    }
    hash_iter_destruir(iter_hash);
}

/*FUNCION AUXILIAR*/
//Funcion encargada de imprimir un error de comando por stderr.
void imprimir_error(char* comando){

	fprintf(stderr, "Error en comando %s\n", comando);
}

/*FUNCION AUXILIAR*/
//Funcion encargada de contar la cantidad de componentes de un arreglo.
//Devuelve esa cantidad.
int contar_cantidad_parametros(char** array){

	int cantidad = 0;
	while(array[cantidad]) {
		cantidad++;
	}
	return cantidad;
}

//Funcion encargada de procesar lo que ingresa el usuario en entrada standard.
//Segun el comando que ingrese, efectua dicha operacion.
//Devuelve un entero que representa el estado de la ejecucion de la funcion.
int procesar_entrada_stdin(char* linea_entrada, abb_t* visitantes, hash_t* recursos_mas_solicitados){

	char** input = split(linea_entrada,' ');
	int indice_corte = 0;
	if(strcmp(input[0],AGREGAR_ARCHIVO) == 0){
		if(contar_cantidad_parametros(input) == CANT_PARAM_AGREGAR){
			procesar_log(input[1],recursos_mas_solicitados, visitantes);
		}
		else{
			imprimir_error(AGREGAR_ARCHIVO);
			indice_corte = -1;
		}
	}
	else if(strcmp(input[0],VISITADOS)==0){
		if(contar_cantidad_parametros(input)==CANT_PARAM_VISITADOS){
			mostrar_mas_visitados(recursos_mas_solicitados, atoi(input[1]));
		}
		else{
			imprimir_error(VISITADOS);
			indice_corte = -1;
		}
	}
	else if(strcmp(input[0],VISITANTES)==0){
		if(contar_cantidad_parametros(input) == CANT_PARAM_VISITANTES){
			mostrar_visitantes(visitantes, input[1], input[2]);
		}
		else {
			imprimir_error(VISITANTES);
			indice_corte = -1;
		}
	}
	else{
		imprimir_error(input[0]);
		indice_corte = -1;
	}
	free_strv(input);
	if(indice_corte >= 0) fprintf(stdout, "OK\n");
	return indice_corte;

}