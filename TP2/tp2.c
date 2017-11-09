#define _XOPEN_SOURCE 700

#include <time.h>
#include "tp2.h"
#include "hash.h"
#include "lista.h"
#include "heap.h"
#include "strutil.h"
#include "abb.h"

#define TIME_FORMAT "%FT%T%z"

#define AGREGAR_ARCHIVO "agregar_archivo"
#define VISITANTES "ver_visitantes"
#define VISITADOS "ver_mas_visitados"

#define CANT_PARAM_AGREGAR 1
#define CANT_PARAM_VISITANTES 2
#define CANT_PARAM_VISITADOS 1

#define N_SOL_CONSIDERADAS_DDOS 5
#define RANGO_DE_TIEMPO_CONSIDERADO 2



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

bool procesar_log(char* nombre_de_archivo, heap_t* recursos_mas_solicitados, heap_t* solicitantes) {
    FILE* archivo = fopen(nombre_de_archivo, "r");
    char *linea = NULL;
    size_t capacidad = 0;
    ssize_t leidos;

    // Va a haber que crear un wrapper para el lista destruir, para que coincidan las firmas.
    hash_t* peticiones_por_ip = hash_crear(lista_destruir);
    if (peticiones_por_ip == NULL) return false;

    hash_t* temp_recursos_solicitados = hash_crear(NULL);
    if (temp_recursos_solicitados == NULL) return false;

    while ((leidos = getline(&linea, &capacidad, archivo)) > 0) {
        char **log_a_procesar = split(linea, '\t');
        char *ip = log_a_procesar[0];
        char *nombre_recurso = log_a_procesar[3];

        time_t instante = iso8601_to_time(log_a_procesar[1]);

        agregar_fecha_de_solicitud(ip, instante, peticiones_por_ip);

        aumenta_cont_solicitudes_recurso(temp_recursos_solicitados, nombre_recurso);
    }

    // Aca tambien faltaria un wrapper para pasar_recursoso_de_hash_a_heap
    iterar_hash(temp_recursos_solicitados, pasar_recursos_de_hash_a_heap, recursos_mas_solicitados);

    free(linea);
    fclose(archivo);

    return true;
}

bool pasar_recursos_de_hash_a_heap(char* clave, void* recurso, void* heap) {
    return heap_encolar((heap_t*)heap, recurso);
}

void detectar_DOS_guardar_ips_en_abb(char* ip, void* solicitudes_ip, void* abb) {
    lista_t* lista_solicitudes = solicitudes_ip;
    if(usuario_hizo_mas_solicitudes_de_las_permitidas(lista_solicitudes)) {
        imprimir_dos(ip);
    }
    // Aprovecho para agregar las ip al abb, ya que en el hash no se repiten, lo que ahorra algo de
    // operaciones ya que en ningun momento voy a agregar dos veces la mismas ip.
    abb_guardar((abb_t*)abb, ip, NULL);

}

bool usuario_hizo_mas_solicitudes_de_las_permitidas(lista_t* lista_solicitudes) {
    if (lista_largo(lista_solicitudes) < 5) return false;

    lista_iter_t* iter_izq = lista_iter_crear(lista_solicitudes);
    if (iter_izq == NULL) return false;

    lista_iter_t* iter_der = lista_iter_crear(lista_solicitudes);
    if (iter_der == NULL) return false;

    // Muevo el iter_der de tal forma que quede el iter_izq sobre la primera solicitud y iter_der sobre la quinta solicitud
    for (int i = 1; i < N_SOL_CONSIDERADAS_DDOS; i++) {
        lista_iter_avanzar(iter_der);
    }

    while (!lista_iter_al_final(iter_der)) {
        if (difftime(*(time_t*)lista_iter_ver_actual(iter_der), *(time_t*)lista_iter_ver_actual(iter_izq)) <= 2)
            return true;
        lista_iter_avanzar(iter_izq);
        lista_iter_avanzar(iter_der);
    }
    return false;
}

bool agregar_fecha_de_solicitud(char* ip, time_t fecha, hash_t* peticiones_por_ip) {
    if (!hash_pertenece(peticiones_por_ip, ip)) {
        lista_t *lista_aux = lista_crear();
        hash_guardar(peticiones_por_ip, ip, lista_aux);
    }
    return (lista_insertar_ultimo((lista_t *) hash_obtener(peticiones_por_ip, ip), &fecha));
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

void iterar_hash(hash_t* hash, void visitar(char* clave, void *dato, void *extra), void *extra) {
    hash_iter_t* iter_hash = hash_iter_crear(hash);
    if(iter_hash == NULL) return;

    while (!hash_iter_al_final(iter_hash)) {
        char* clave_actual = (char*)hash_iter_ver_actual(iter_hash);
        void* dato_actual = hash_obtener(hash, clave_actual);
        visitar(clave_actual, dato_actual, extra);
        hash_iter_avanzar(iter_hash);
    }
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
	if(cantidad_1 > cantidad_2) valor_retorno = 1;
	else if(cantidad_1 < cantidad_2) valor_retorno = -1;
	else valor_retorno = 0;
	return valor_retorno;
}

//Imprimir error por stderr (por lo menos mi correctora exigia funcion auxiliar)
void imprimir_error(char* comando){

	fprintf(stderr, "Error en comando %s\n", comando);
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
	if(comparar_cadenas(input[0],AGREGAR_ARCHIVO) == 0){
		if(contar_cantidad_parametros(input) == CANT_PARAM_AGREGAR){
		procesar_log();
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
	if(indice_corte >= 0) fprintf(stdout, "OK\n");
	return indice_corte;

}






