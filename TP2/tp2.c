#define _XOPEN_SOURCE

#include <time.h>
#include "tp2.h"
#include "hash.h"
#include "lista.h"
#include "heap.h"
#include "strutil.h"

#define TIME_FORMAT "%FT%T%z"

#define AGREGAR_ARCHIVO "agregar_archivo"
#define VISITANTES "ver_visitantes"
#define VISITADOS "ver_mas_visitados"

#define CANT_PARAM_AGREGAR 1
#define CANT_PARAM_VISITANTES 2
#define CANT_PARAM_VISITADOS 1


typedef struct recurso {
    char *clave;
    int cant_de_solicitudes;
} recurso_t;


/*
    Dada una cadena en formato ISO-8601 devuelve una variable de tipo time_t
    que representa un instante en el tiempo.
*/
time_t iso8601_to_time(const char* iso8601)
{
    struct tm bktime = { 0 };
    strptime(iso8601, TIME_FORMAT, &bktime);
    return mktime(&bktime);
}

size_t procesar_log(char* nombre_de_archivo, hash_t* peticiones_por_ip, heap_t* recursos_mas_solicitados, heap_t* solicitantes) {
    FILE* archivo = fopen(nombre_de_archivo, "r");
    char *linea = NULL;
    size_t capacidad = 0;
    ssize_t leidos;
    size_t cantidad_de_lineas = 0;

    hash_t* temp_recursos_solicitados = hash_crear(NULL);
    if (temp_recursos_solicitados == NULL) return NULL;

    while ((leidos = getline(&linea, &capacidad, archivo)) > 0) {
        char **log_a_procesar = split(linea, '\t');
        char *ip = log_a_procesar[0];
        char *nombre_recurso = log_a_procesar[3];

        time_t instante = iso8601_to_time(log_a_procesar[1]);

        agregar_fecha_de_solicitud(ip, instante, peticiones_por_ip);

        aumenta_cont_solicitudes_recurso(temp_recursos_solicitados, nombre_recurso);
    }

    pasar_recursos_de_hash_a_heap(temp_recursos_solicitados, recursos_mas_solicitados);

    free(linea);
    fclose(archivo);

    return cantidad_de_lineas;
}

bool pasar_recursos_de_hash_a_heap(hash_t* hash_temp_recursos, heap_t* heap_recursos) {
    hash_iter_t* iter_hash = iter_hash_iter_crear(hash_temp_recursos);
    if(iter_hash == NULL) return false;

    while (!hash_iter_al_final(iter_hash)) {
        heap_encolar(((recurso_t *) (hash_obtener(hash_temp_recursos, hash_iter_ver_actual(iter_hash)))));
        hash_iter_avanzar(iter_hash);
    }
    return true;
}

bool agregar_fecha_de_solicitud(char* ip, time_t fecha, hash_t* peticiones_por_ip) {
    if (!hash_pertenece(peticiones_por_ip, ip)) {
        lista_t *lista_aux = lista_crear();
        hash_guardar(peticiones_por_ip, ip, lista_aux);
    }
    if (!lista_insertar_ultimo((lista_t *) hash_obtener(peticiones_por_ip, ip), instante)) return false;
}

bool aumenta_cont_solicitudes_recurso(hash_t* temp_recursos_solicitados, char* recurso) {
    if (!hash_pertenece(temp_recursos_solicitados, recurso)) {
        recurso_t *recurso_aux = malloc(sizeof(recurso_t));
        if (recurso_aux == NULL) return false;

        recurso_aux->clave = recurso;
        recurso_aux->cant_de_solicitudes = 0;
        hash_guardar(temp_recursos_solicitados, recurso, recurso_aux);
    }

    ((recurso_t *) (hash_obtener(temp_recursos_solicitados, recurso)))->cant_de_solicitudes++;

    return true;
}

//Imprimir posibles ip con DoS

void imprimir_dos(char* direccion_ip){

	if(!direccion_ip) return;
	fprintf(stdout, "DoS: %s\n", direccion_ip);
}

//Funcion de comparacion de IPS
//Devuelve 0 si son iguales, 1 si el primer parametro es mayor al 2do,
//-1 si el segundo parametro es mayor al primero.

int comparar_ips(char* ip_1, char* ip_2){

	char** dir_ip1 = split(ip_1, '.');
	char** dir_ip2 = split(ip_2, '.');
	int retorno;
	for(int i=0; i<4; i++){
		if(atoi(dir_ip1[i]) > atoi(dir_ip2[i])){
			retorno = 1;
			break;
		}
		else if(atoi(dir_ip1[i]) < atoi(dir_ip2[i])){
			retorno = -1;
			break;
		}
		else retorno = 0;
	}
	return retorno;
}

//Funcion de comparacion de recurso_t

int comparar_recursos(recurso_t* recurso1, recurso_t* recurso2){
	//Comparo la cantidad de solicitudes de cada recurso

	int valor_retorno;
	int cantidad_1 = recurso1->cant_de_solicitudes;
	int cantidad_2 = recurso2->cant_de_solicitudes;
	if(cantidad1 > cantidad2) valor_retorno = 1;
	else if(cantidad1 < cantidad_2) valor_retorno = -1;
	else valor_retorno = 0;
	return valor_retorno;
}

//Imprimir error por stderr (por lo menos mi correctora exigia funcion auxiliar)
void imprimir_error(char* comando){

	fprintf(stderr, "Error en comando %c\n", comando);
}

//Funcion de comparacion de cadenas
int comparar_cadenas(char* cadena1, char* cadena2){

	return strcmp(cadena1, cadena2);
}

//Contar cantidad de parametros
int contar_cantidad_parametros(char** array){

	int cantidad = 0;
	while(array[cantidad]){
		cantidad++;
	}
	return cantidad;
}

//Hago algunas cosas para la interfaz

int procesar_entrada_stdin(char* linea_entrada){

	char** input = split(linea_entrada,' ');
	int indice_corte = 0;
	if(comparar_cadenas(input[0],AGREGAR_ARCHIVO)==0){
		if(contar_cantidad_parametros(input) == CANT_PARAM_AGREGAR){
		**funcion encargada, la hacemos juntos**
		}
		else{
			imprimir_error(AGREGAR_ARCHIVO);
			indice_corte = -1;
		}
	}
	else if(comparar_cadenas(input[0],VISITADOS)==0){
		if(contar_cantidad_parametros(input)==CANT_PARAM_VISITADOS){
			**funcion**
		}
		else{
			imprimir_error(VISITADOS);
			indice_corte = -1;
		}
	}
	else if(comparar_cadenas(input[0],VISITANTES)==0){
		if(contar_cantidad_parametros(input)==CANT_PARAM_VISITANTES){
			**FUNCION**
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
	if(indice_corte >= 0) fputs("OK\n",stdout);
	return indice_corte;

}




