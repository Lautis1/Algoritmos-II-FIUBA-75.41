#define _POSIX_C_SOURCE 200809L
#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lista.h"

#define TAMANIO_INICIAL 60
#define MAX_FACTOR_REDIM 2
#define MIN_FACTOR_REDIM 0.3
#define FACTOR_REDIM 2


/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 ******************************************************************/

typedef struct hash_item {  //Le cambiamos el nombre, antes era nodo_hash_t.
    char *clave;
    void *dato;
} hash_item_t;

struct hash {
    lista_t **tabla;
    size_t tamanio;
    size_t cantidad;
    hash_destruir_dato_t destruir_dato;
};

struct hash_iter {
    lista_iter_t *lista_iter;
    size_t pos_actual;
    size_t elem_recorridos;
    const hash_t *hash;
};

/*******************************************************************
*                        FUNCIONES PRIVADAS                        *
*******************************************************************/
//Realiza el hashing sobre la clave. Devuelve un indice
//ALGORITMO DE HASH BY DJB2

size_t funcion_hash(const char *str) {
    
    size_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + (size_t) c; /* hash * 33 + c */
    }

    return hash;
}

//Llama a la funcion destruir_dato y elimina y libera la memoria
//del item pasado por parametro.
void destruir(hash_item_t* item, hash_destruir_dato_t destruir_dato) {
    
    if (item != NULL) {
        if (destruir_dato) {
            destruir_dato(item->dato);
        }
        free(item->clave);
    }
    free(item);
}

//Pre: Hash fue creado
//Post: Devuelve el iterador sobre la lista contenida en cada item, NULL en caso de que no se haya encontrado.
lista_iter_t *busqueda_item_en_hash(const hash_t *hash, const char *clave) {
    
    size_t indice_busqueda = funcion_hash(clave) % hash->tamanio;
    lista_iter_t *iter = lista_iter_crear(hash->tabla[indice_busqueda]);
    if (iter == NULL) return NULL;
    while (!lista_iter_al_final(iter)) {
        hash_item_t* item = lista_iter_ver_actual(iter);
        if (strcmp(clave, item->clave) == 0) {
            break;
        }
        lista_iter_avanzar(iter);
    }
	return iter;
}

//Recibe un par clave-valor y crea un item de hash con los datos recibidos
//Post: Devuelve dicho item.
hash_item_t *crear_item(const char *clave, void *dato) {

    hash_item_t *item_nue = malloc(sizeof(hash_item_t));
    if (item_nue == NULL) return NULL;
    char* clave_aux = strdup(clave);
    if(!clave_aux) return NULL;
    item_nue->clave = clave_aux;
    item_nue->dato = dato;
    return item_nue;
}

//Encuentra la proxima lista que contiene al menos a un item de hash dentro
void encontrar_proxima_lista_no_vacia(hash_iter_t *hash_iter) {

    while (lista_esta_vacia(hash_iter->hash->tabla[hash_iter->pos_actual]) && !hash_iter_al_final(hash_iter)) {
        hash_iter->pos_actual += 1;
    }
}

//Crea una arreglo de tamaño 'tamanio_tabla' de elementos del tipo lista_t**
//Post: Devuelve dicha tabla, NULL en caso de que algun malloc haya fallado o el tamaño recibido sea 0
lista_t** hash_crear_tabla(size_t tamanio_tabla) {
    
    lista_t **tabla = malloc(sizeof(lista_t*) * tamanio_tabla);
    if (tabla == NULL) return NULL;
    for (int i = 0; i < tamanio_tabla; i++) {
        lista_t *lista_aux = lista_crear();
        if (lista_aux == NULL) {
            lista_destruir(tabla[i],NULL);
            return NULL;
        }
        tabla[i] = lista_aux;
    }
    return tabla;
}

//Pre: Recibe un hash y un tamaño que tiene que ser mayor igual que el tamaño inicial
//Post: La tabla correspondiente al hash recibido por parametro fue redimensionada a el tamaño recibido por parametro
bool hash_redimensionar(hash_t *hash, size_t nuevo_tamanio) {
    
    lista_t **tabla_nueva = hash_crear_tabla(nuevo_tamanio);
    if (tabla_nueva == NULL) return false;
    for (int j = 0; j < hash->tamanio; j++) {
        while (!lista_esta_vacia(hash->tabla[j])) {
            hash_item_t *item = lista_borrar_primero(hash->tabla[j]);
            size_t indice = funcion_hash(item->clave) % nuevo_tamanio;
            lista_insertar_ultimo(tabla_nueva[indice],item);
        }
        
        lista_destruir(hash->tabla[j], NULL);
    }
    free(hash->tabla);
    hash->tabla = tabla_nueva;
    hash->tamanio = nuevo_tamanio;
    return true;
}


/*******************************************************************
*                        IMPLEMENTACION HASH                       *
*******************************************************************/

//Crea un hash, recibiendo su funcion de destruccion.
//Pre: destruir_dato es capaz de destruir los datos del
//hash. Si no se la utiliza, esa funcion es NULL.
//Post: devuelve un hash vacio
hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
    
    hash_t *hash = malloc(sizeof(hash_t));
    if (hash == NULL) return NULL;
    lista_t** tabla = hash_crear_tabla(TAMANIO_INICIAL);
    if (tabla == NULL) {
        free(hash);
        return NULL;
    }
    hash->tabla = tabla;
    hash->tamanio = TAMANIO_INICIAL;
    hash->cantidad = 0;
    hash->destruir_dato = destruir_dato;
    return hash;
}

//El dato es guardado dentro del hash con su clave asociada.
//Si la clave ya esta en el hash, la reemplaza.
//Pre: el hash fue creado.
//Post: devuelve un booleano segun la condicion del guardado.
bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
    
    if (hash->cantidad/hash->tamanio >= MAX_FACTOR_REDIM) {
        if(!hash_redimensionar(hash, hash->tamanio * FACTOR_REDIM)) return false;
    }
    lista_iter_t* iter = busqueda_item_en_hash(hash, clave); //busco si existe el item en la tabla
    if (lista_iter_ver_actual(iter)!=NULL) {
    	hash_item_t* item_aux = lista_iter_ver_actual(iter); //DESTRUIR PORQUE HAY UNA LISTA EN ESA POSICION
        if (hash->destruir_dato != NULL) hash->destruir_dato(item_aux->dato);
        item_aux->dato = dato;
    } else {
        hash_item_t *item_a_insertar = crear_item(clave, dato);
        if (item_a_insertar == NULL) return false;
        // Tanto si la lista esta en la posicion esta vacia como si ya tiene
        // elementos no hay diferencia, lista insertar se encarga de manejar
        // la actualizacion de punteros y demas cosas
        lista_iter_insertar(iter, item_a_insertar);
        hash->cantidad++;
        
    }
    lista_iter_destruir(iter);
    return true;
}
//Busca el item en el hash y lo borra, devolviendo su dato.
//Pre: el hash fue creado.
//Post: se devuelve el dato del item borrado.
void *hash_borrar(hash_t *hash, const char *clave) {
    
    if (hash->cantidad/hash->tamanio <= MIN_FACTOR_REDIM && (hash->tamanio / FACTOR_REDIM) > TAMANIO_INICIAL) {
        if(!hash_redimensionar(hash, hash->tamanio / FACTOR_REDIM)) return NULL;
    }
    lista_iter_t* iter = busqueda_item_en_hash(hash, clave);
    void* dato;
    if(lista_iter_al_final(iter)) dato = NULL;
    else{
    	hash_item_t* aux = lista_iter_borrar(iter);
    	dato = aux->dato;
    	destruir(aux,NULL);
    	hash->cantidad--;
    }
    lista_iter_destruir(iter);
    return dato;
}

//Chequea si la clave pertenece al hash.
//Pre: el hash fue creado.
//Post: devuelve un booleano dependiendo de la existencia
//de la clave en el hash.
bool hash_pertenece(const hash_t *hash, const char *clave) {

	lista_iter_t* iter = busqueda_item_en_hash(hash,clave);
	hash_item_t* item = lista_iter_ver_actual(iter);
	lista_iter_destruir(iter);
	return item!=NULL;
}

//Devuelve el valor asociado a una clave.
//Pre: el hash fue creado.
//Post: devuelve el dato asociado a la clave; NULL si esta
//no existe.
void *hash_obtener(const hash_t *hash, const char *clave) {
    
    lista_iter_t* iter = busqueda_item_en_hash(hash, clave);
    void* dato;
    if(lista_iter_al_final(iter)) dato = NULL;
    else dato = ((hash_item_t*)lista_iter_ver_actual(iter))->dato;
    lista_iter_destruir(iter);
    return dato;
}

//Devuelve la cantidad de elementos del hash.
//Pre: el hash fue creado.
//Post: Devuelve cuantos elementos hay; 0 si esta vacio.
size_t hash_cantidad(const hash_t *hash) {
    
    return hash->cantidad;
}

//Destruye el hash liberando la memoria y llamando a la
//funcion destruir para cada par (clave,dato).
//Pre: el hash fue creado.
//Post: se destruyen los datos del hash y el hash.
void hash_destruir(hash_t *hash) {
    
    for (int i = 0; i < hash->tamanio; i++) {
        while (!lista_esta_vacia(hash->tabla[i])) {
            hash_item_t *item_aux = lista_borrar_primero(hash->tabla[i]);
            destruir(item_aux, hash->destruir_dato);
        }
        lista_destruir(hash->tabla[i],NULL);
    }
    free(hash->tabla);
    free(hash);
}

/*******************************************************************
 *                    PRIMITIVAS DEL ITERADOR                      *
 ******************************************************************/
//Crea un iterador
//Pre: el hash fue creado
//Post: devuelve un iterador para el hash.
hash_iter_t *hash_iter_crear(const hash_t *hash) {
    
    hash_iter_t *iter = malloc(sizeof(hash_iter_t));
    if (!iter) return NULL;

    iter->hash = hash;
    iter->pos_actual = 0;
    iter->elem_recorridos = 0;
    if (hash->cantidad == 0) {
        iter->lista_iter = NULL;
    } else {
        encontrar_proxima_lista_no_vacia(iter);
        iter->lista_iter = lista_iter_crear(hash->tabla[iter->pos_actual]);
    }
    return iter;
}

//Chequea la posicion del iterador.
//Pre: el hash y el iterador fueron creados;
//Post: devuelve true si esta en la ultima posicion;
//false en caso contrario.
bool hash_iter_al_final(const hash_iter_t *iter) {
    
    return (iter->elem_recorridos == iter->hash->cantidad);
}

//Avanza a la siguiente posicion del hash.
//Pre: el hash y el iterador fueron creados.
//Post: devuelve un booleano segun si pudo avanzar o no.
bool hash_iter_avanzar(hash_iter_t *iter) {
    
    if (hash_iter_al_final(iter)) return false;
    if(!lista_iter_avanzar(iter->lista_iter)) return false;
    iter->elem_recorridos++;
    //Luego de avanzar puede quedar al final del hash el iterador
    if (lista_iter_al_final(iter->lista_iter) && !hash_iter_al_final(iter)) {
        lista_iter_destruir(iter->lista_iter);
        iter->pos_actual++;
        encontrar_proxima_lista_no_vacia(iter);
        iter->lista_iter = lista_iter_crear(iter->hash->tabla[iter->pos_actual]);
    }

    return true;


}

//Devuelve la clave donde esta posicionado el iter.
//Pre: el hash y el iterador fueron creados.
//Post: devuelve la clave a la cual apunta el iterador.
const char *hash_iter_ver_actual(const hash_iter_t *iter) {
    
    if (hash_iter_al_final(iter)) return NULL;
    hash_item_t *item_act = lista_iter_ver_actual(iter->lista_iter);
    return item_act->clave;
}

//Destruye el iterador.
//Pre: el hash y el iterador fueron creados.
//Post: destruye el iterador y libera la memoria.
void hash_iter_destruir(hash_iter_t *iter) {
    
    lista_iter_destruir(iter->lista_iter);
    free(iter);
}


