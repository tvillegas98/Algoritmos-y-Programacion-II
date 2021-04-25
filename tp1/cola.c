#include "cola.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct nodo{
    void* dato;
    struct nodo* prox;
}nodo_t;


static nodo_t* crear_nodo(void* valor){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    
    if(!nodo) return NULL;

    nodo->dato = valor;
    nodo->prox = NULL;

    return nodo;
}

typedef struct cola {
    nodo_t* prim;
    nodo_t* ult;
}cola_t;

/* *****************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/


cola_t *cola_crear(void){

    cola_t* cola = malloc(sizeof(cola_t));
    if(cola == NULL) return NULL;
    cola->prim = NULL;
    cola->ult = NULL;
    return cola;

}


bool cola_esta_vacia(const cola_t *cola){
    return cola->prim == NULL;
}

bool cola_encolar(cola_t *cola, void *valor){
    nodo_t* nuevo_nodo = crear_nodo(valor);
    if(!nuevo_nodo) return false;

    if(cola_esta_vacia(cola)){   
        cola->prim = nuevo_nodo;
        cola->ult = nuevo_nodo;
        return true;
    }
    cola->ult->prox = nuevo_nodo;
    cola->ult = nuevo_nodo;
    
    return true;
}

void *cola_ver_primero(const cola_t *cola){
    if(cola_esta_vacia(cola)) return NULL;
    void* puntero_auxiliar = cola->prim->dato;
    return puntero_auxiliar;
}

void *cola_desencolar(cola_t *cola){
    if(cola_esta_vacia(cola)) return NULL;

    nodo_t* nodo_auxiliar = cola->prim;
    void* puntero_auxiliar = nodo_auxiliar->dato;

    if(cola->prim == cola->ult){
        cola->ult = NULL;
    }

    cola->prim = cola->prim->prox;

    free(nodo_auxiliar);
    return puntero_auxiliar;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)){
    while(!cola_esta_vacia(cola)){
        void* dato = cola_desencolar(cola);
        if(destruir_dato) destruir_dato(dato);
    }
    free(cola);
}

