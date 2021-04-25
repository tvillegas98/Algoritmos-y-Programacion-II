#include "cola.h"
#include <stdlib.h>


typedef struct nodo_cola {
    void* dato;
    struct nodo_cola* siguiente;
} nodo_cola_t;

nodo_cola_t *nodo_cola_crear(void *valor) {
    nodo_cola_t *nodo = malloc(sizeof(nodo_cola_t));
    if (nodo ==  NULL) {
        return NULL;
    }
    nodo->dato = valor;
    nodo->siguiente = NULL;
    return nodo;
}

struct cola {
    nodo_cola_t* primero;
    nodo_cola_t* ultimo;
};

cola_t *cola_crear(void) {

    cola_t *cola = malloc(sizeof(cola_t));
    if (cola == NULL) {
        return NULL;
    }
    cola->primero = NULL;
    cola->ultimo = NULL;
    return cola;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)) {
    while (!cola_esta_vacia(cola)) {        
        void *dato = cola_desencolar(cola);
        if (destruir_dato){
            destruir_dato(dato);
        }      
    }
    
    free(cola);
}

bool cola_esta_vacia(const cola_t *cola) {
    if (cola->primero == NULL){
        return true;
    }
    return false;
}

bool cola_encolar(cola_t *cola, void *valor) {
    nodo_cola_t *nodo = nodo_cola_crear(valor);
    if (nodo == NULL){
        return false;
    }
    if (cola_esta_vacia(cola)){
        cola->primero = nodo;
        cola->ultimo = nodo;
        return true;
    }
    cola->ultimo->siguiente = nodo;
    cola->ultimo = nodo;
    return true;
}

void *cola_ver_primero(const cola_t *cola) {
    if (cola_esta_vacia(cola)) {
        return NULL;
    }
    return cola->primero->dato;
}

void *cola_desencolar(cola_t *cola) {
    if (cola_esta_vacia(cola)) {
        return NULL;
    }
    void *devolver = cola->primero->dato;
    nodo_cola_t *nuevo_primero = cola->primero->siguiente;    
    free(cola->primero);
    cola->primero = nuevo_primero;
    return devolver;
}