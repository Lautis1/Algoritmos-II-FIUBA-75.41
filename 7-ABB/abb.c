#define _POSIX_C_SOURCE 200809L

#include "abb.h"
#include <stdio.h>
#include <stdlib.h>

/* ******************************************************************
 *                CREACION DE LOS TIPOS DE DATOS                    *
 * *****************************************************************/

typedef struct nodo_abb{
	char* clave;
	void* valor;
	struct nodo_abb* izq;
	struct nodo_abb* der;
}nodo_abb_t;

struct abb{
	nodo_abb_t* raiz;
	abb_comparar_clave_t cmp;
	abb_destruir_dato_t destruir_dato;
	size_t cantidad;
};

struct abb_iter{
	abb_t* arbol;
	pila_t* pila;
	nodo_abb_t* actual;
};

/* ******************************************************************
 *                       FUNCIONES PRIVADAS                         *
 * *****************************************************************/
nodo_abb_t* crear_nodo(char* clave, void* valor){
	
	nodo_abb_t* nodo = malloc(sizeof(nodo_abb_t));
	if(!nodo){
		return NULL;
	}
    char* clave_auxiliar = strdup(clave);
    if(!clave_auxiliar){
        return NULL;
    }
    nodo->clave = clave_auxiliar;
	nodo->valor = valor;
	nodo->izq = NULL;
	nodo->der = NULL;
	return nodo;
}

//Quizas haya que modificar esto, porque despues se va a usar en abb_destruir y no esta bueno que devuelva el valor.
//Si yo llamo a la funcion y no hago nada con el valor que me devuelve la funcion, el compilador me va a gritar.
void* destruir_nodo(abb_t* arbol, nodo_abb_t* nodo) {
    
    void* dato_aux = nodo->valor;
    free(nodo->clave);
    if (arbol->destruir_dato) {
        arbol->destruir_dato(nodo->dato);
    }
    free(nodo);
    return dato_aux;
}

bool abb_insertar(abb_t* arbol, nodo_abb_t* nodo, const char* clave, void* dato) {
    
    // Ya se chequeo si el arbol existe en abb_guardar, no hace falta revisar de nuevo.
    if (arbol->cmp(clave, nodo->clave) == 0) {
        if (arbol->destruir_dato) {
            arbol->destruir_dato(nodo->valor);
        }
        nodo->valor = dato;
    }
        //Si me tengo que mover a la derecha es decir la clave que quiero insertar es mayor a la clave del nodo actual.
    else if (arbol->cmp(clave, nodo->clave) > 0) {
        if (nodo->der != NULL) {
            return abb_insertar(arbol, nodo->der, clave, dato);
        }
        else {
            nodo_abb_t* nodo_aux = crear_nodo(clave, valor);
            if (!nodo_aux) return false;
            nodo->der = nodo_aux;
            if (nodo_aux == NULL) return false;
            nodo->der = nodo_aux;
        }
    }
        // Si me tengo que mover a la izq.
    else {
        if (nodo->izq != NULL) {
            return abb_insertar(arbol, nodo->izq, clave, dato);
        }
        else {
            nodo_abb_t* nodo_aux = crear_nodo(clave, valor);
            if (nodo_aux == NULL) return false;
            nodo->izq = nodo_aux;  //Habias puesto nodo->der, cuando hay que crear un nodo->izq.
        }
    }
    arbol->cantidad++; //Pongo esto aca ya que se repetia en los 2 else.
    return true;
}


// Devuelve el nodo y no el valor asi despues puedo usar la funcion para el abb_borrar
nodo_abb_t* buscar_por_clave(abb_t* arbol, nodo_abb_t* nodo, char* clave) {
    
    if (arbol->cmp(clave, nodo->clave) == 0) return nodo;
    else if (arbol->cmp(clave, nodo->clave) > 0) {
        if (nodo->der != NULL) {
            return buscar_por_clave(arbol, nodo->der, clave);
        }
        else {
            return NULL;
        }
    }
    else {
        if (nodo->izq != NULL) {
            return buscar_por_clave(arbol, nodo->izq, clave);
        }
        else {
            return NULL;
        }
    }
}

//Destruye nodos en recorrido postorder.
void destruir_post_order(nodo_abb_t* nodo, abb_destruir_dato_t destruir_dato){
	
	if(!raiz) return;
	//Primero trato la rama izquierda, luego la derecha, y por ultimo la raiz.
	destruir_post_order(nodo->izq,destruir_dato) //Aca le podemos pasar destruir_nodo creo
	destruir_post_order(nodo->der,destruir_dato);
	if(destruir_dato) destruir_dato(nodo->valor);
	free(nodo->clave);
	free(nodo);
}
//Devuelve la cantidad de hijos de un nodo. 0 si es una "hoja"
int cantidad_de_hijos(nodo_abb_t* nodo){
	
	if(!nodo->der && !nodo->izq) return 0; //caso de nodo "hoja"
	if(nodo->der || nodo->izq) return 1;  //caso 1 hijo
	else return 2;
}

//Intercambia el nodo que va a tomar el lugar del borrado con el borrado.
void swapear_nodos(nodo_abb_t* nodo1, nodo_abb_t* nodo2){
	
	char* clave_2 = nodo2->clave;
	void* valor_2 = nodo2->valor;
	nodo2->clave = nodo1->clave;
	nodo2->valor = nodo1->valor;
	nodo1->clave = clave_2;
	nodo1->valor = valor_2;
}

nodo_abb_t* buscar_nodo_minimo(nodo_abb_t* nodo){
	
	nodo_abb_t* nodo_actual = nodo;
	while(nodo_actual->izq){
		nodo_actual = nodo_actual->izq;
	}
	return nodo_actual;
}


/* ******************************************************************
 *                       PRIMITIVAS DEL ABB                         *
 * *****************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	
	abb_t* arbol = malloc(sizeof(abb_t));
	if(!arbol) return NULL;
	arbol->raiz = NULL;
	arbol->cantidad = 0;
	arbol->cmp= cmp;
	arbol->destruir_dato = destruir_dato;
	return arbol;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
    
    //abb_guardar estaria funcionando como una especie de wrapper para abb_insertar
    if (arbol == NULL) return false;
    if (arbol->cantidad == 0) {
        nodo_abb_t* nodo_aux = crear_nodo(clave, dato);
        if (nodo_aux == NULL) return false;
        arbol->raiz = nodo_aux;
        arbol->cantidad++;
        return true;
    }
    else{  //Supongo que esto es cuando arbol->cantidad != 0, sino estarias incrementado 2 veces la cantidad.
    	return abb_insertar(arbol, arbol->raiz, clave, dato);
    }


void *abb_obtener(const abb_t *arbol, const char *clave) {
    
    if (!arbol || arbol->cantidad == 0) return NULL;
    return buscar_por_clave(arbol, arbol->raiz, clave);
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
    
    if (!arbol || arbol->cantidad == 0) return false;
    return buscar_por_clave(arbol, arbol->raiz, clave) != NULL;
}

size_t abb_cantidad(abb_t* arbol){
	
	return arbol->cantidad;
}

void abb_destruir(abb_t *arbol) {
    
    // Se destruye al arbol usando internamente un recorrido post-order para asi eliminar siempre hojas y no tener que
    // lidiar con problemas de nodos con hijos y cosas asi.
    if(!arbol) return;
    destruir_post_order(arbol->raiz, arbol->destruir_dato);
    free(arbol);
}

void *abb_borrar(abb_t *arbol, const char *clave){
	
	if(!arbol) return NULL;
	//nodo_abb_t* padre = arbol->raiz;
	nodo_abb_t* nodo_a_borrar = abb_obtener(arbol,clave);
	if(cantidad_de_hijos(nodo_a_borrar)==0) destruir_nodo(arbol,nodo_a_borrar);
	else if(cantidad_de_hijos(nodo_a_borrar)==1){
		if(nodo_a_borrar->izq || !nodo_a_borrar->der){
			swapear_nodos(nodo_a_borrar,nodo_a_borrar->izq); //Los swapeo y el nodo a borrar

		}

	}
	

}

/* ******************************************************************
 *                 IMPLEMENTACION ITERADOR INTERNO                  *
 * *****************************************************************/
/* FUNCION AUXILIAR RECURSIVA QUE ITERA INTERNAMENTE POR EL ARBOL CON RECORRIDO
INORDER*/

//Recorre el arbol con recorrido inorder, aplicando la funcion visitar a cada
//nodo.
void iterador_inorder(nodo_abb_t* nodo, bool visitar(const char *, void *, void *),void* extra){
	
	if(!nodo) return;
	if(nodo->izq) iterador_inorder(nodo->izq,visitar,extra);
	visitar(nodo->clave,nodo->valor,extra);
	if(nodo->der) iterador_inorder(nodo->der,visitar,extra);
}


void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	
	if(!arbol) return;
	if(!visitar) return;
	iterador_inorder(arbol->raiz,visitar,extra);
}

/* ******************************************************************
 *                  IMPLEMENTACION ITERADOR EXTERNO                 *
 * *****************************************************************/
//FUNCION AUXILIAR ENCARGADA DE APILAR LOS HIJOS IZQUIERDOS DE UN NODO Y SU RAIZ

void apilar_nodos_izq(abb_iter_t* iter, nodo_abb_t* nodo){
	if(!nodo) return;
	pila_apilar(iter->pila, nodo);
	apilar_nodos_izq(iter,nodo->izq);

}

//Crea un iterador para el arbol.
//PRE : el arbol fue creado.
//POST: devuelve el iterador.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol){ //APILAR RAIZ Y TODOS LOS H.IZQ
	
	if(!arbol) return NULL;
	abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	if(!iter) return NULL;
	pila_t* pila = pila_crear();
	iter->pila = pila;
	nodo_abb_t* nodo = arbol->raiz;
	apilar_nodos_izq(iter,nodo);
	iter->actual = nodo;
	return iter;
}

//Avanza una posicion sobre el arbol en recorrido inorder.
//PRE: el iterador fue creado.
//POST : devuelve un booleano.
bool abb_iter_in_avanzar(abb_iter_t *iter){

	if(abb_iter_in_al_final(iter)) return false;
	iter->actual = pila_desapilar(iter->pila);
	if(iter->actual->der){
		apilar_nodos_izq(iter,iter->actual);
		iter->actual = iter->actual->der;
	}
	return true;

}

//Devuelve un booleano segun si el iterador esta al
//final del arbol o no.
bool abb_iter_in_al_final(const abb_iter_t *iter){
	
	return(pila_esta_vacia(iter->pila));
}

//Devuelve la clave a la que apunta el iterador.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter){

	return(pila_ver_tope(iter->pila));
}

//Destruye el iterador y su pila.
void abb_iter_in_destruir(abb_iter_t* iter){
	
	pila_destruir(iter->pila);
	free(iter);
}


