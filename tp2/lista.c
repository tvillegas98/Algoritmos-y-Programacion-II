#include "lista.h"
#include <stdlib.h>

typedef struct nodo {
    void* dato;
    struct nodo* siguiente;
} nodo_t;

nodo_t *nodo_crear(void *valor) {
    nodo_t *nodo = malloc(sizeof(nodo_t));
    if (nodo ==  NULL) {
        return NULL;
    }
    nodo->dato = valor;
    nodo->siguiente = NULL;
    return nodo;
}

struct lista {
    nodo_t* primero;
    nodo_t* ultimo;
    size_t largo;
};

struct lista_iter {
    nodo_t *anterior;
    nodo_t *actual;
    lista_t *lista;
};

lista_t *lista_crear(void) {
    lista_t *lista = malloc(sizeof(lista_t));
    if (lista == NULL) {
        return NULL;
    }
    lista->primero = NULL;
    lista->ultimo = NULL;
    lista->largo = 0;
    return lista;
}

bool lista_esta_vacia(const lista_t *lista) {
    if (lista->primero == NULL && lista->largo == 0) {
        return true;
    }
    return false;
}

bool lista_insertar_primero(lista_t *lista, void* dato) {
    nodo_t *nodo = nodo_crear(dato);
    if (nodo == NULL){
        return false;
    }
    if(lista_esta_vacia(lista)){
        lista->primero = nodo;
        lista->ultimo = nodo;
        lista->largo++;
        return true;
    }
    nodo->siguiente = lista->primero;
    lista->primero = nodo;
    lista->largo++;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void* dato) {
    nodo_t *nodo = nodo_crear(dato);
    if (nodo == NULL){
        return false;
    }
    if(lista_esta_vacia(lista)){
        lista->primero = nodo;
        lista->ultimo = nodo;
        lista->largo++;
        return true;
    }
    lista->ultimo->siguiente = nodo;
    lista->ultimo = nodo;
    lista->largo++;
    return true;
}

void *lista_borrar_primero(lista_t *lista) {
    if (lista_esta_vacia(lista)){
        return NULL;
    }
    void *devolver = lista->primero->dato;
    if (lista_largo(lista) == 1){
        free(lista->primero);
        lista->primero = NULL;
        lista->ultimo = NULL;
        lista->largo--;
        return devolver;
    }    
    nodo_t *nuevo_primero = lista->primero->siguiente;
    free(lista->primero);
    lista->primero = nuevo_primero;
    lista->largo--;
    return devolver;
}

void *lista_ver_primero(const lista_t *lista) {
    if (lista_esta_vacia(lista)){
        return NULL;
    }
    return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t *lista) {
    if (lista_esta_vacia(lista)){
        return NULL;
    }
    return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista) {
    return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)) {
    while (!lista_esta_vacia(lista)){
        if (destruir_dato){
            destruir_dato(lista->primero->dato);
        }
        lista_borrar_primero(lista);
    }
    free(lista);
}

/* Definicion del iterador interno */
void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra) {
    nodo_t *nodo_actual = lista->primero;
    bool continuar = true;
    while (nodo_actual && continuar) {
        continuar = visitar(nodo_actual->dato, extra);
        nodo_actual = nodo_actual->siguiente;
    }
}

/* Definiciones del iterador externo */

lista_iter_t *lista_iter_crear(lista_t *lista) {
    lista_iter_t *iter = malloc(sizeof(lista_iter_t));
    if (iter == NULL){
        return NULL;
    }
    iter->lista = lista;  
    iter->actual = lista->primero;
    iter->anterior = NULL;
    return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
    if(lista_iter_al_final(iter)){
        return false;
    }
    nodo_t *nuevo_actual = iter->actual->siguiente;
    iter->anterior = iter->actual;     
    iter->actual = nuevo_actual;
    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter) {
    if(lista_iter_al_final(iter)){
        return NULL;
    }
    return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter) {    
    if (iter->actual == NULL){
        return true;
    }
    return false;
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
    if(lista_esta_vacia(iter->lista)){
        lista_insertar_primero(iter->lista, dato);
        iter->actual = iter->lista->primero;
        iter->anterior = NULL;
        return true;
    }
    if(iter->anterior == NULL){
        lista_insertar_primero(iter->lista, dato);        
        iter->actual = iter->lista->primero;
        return true;
    }
     if(lista_iter_al_final(iter)){
        lista_insertar_ultimo(iter->lista, dato);
        iter->actual = iter->lista->ultimo;            
        return true;
    }
    nodo_t *nodo = nodo_crear(dato);
    if(nodo == NULL){
        return false;
    }    
    nodo->siguiente = iter->actual;
    iter->anterior->siguiente = nodo;
    iter->actual = nodo;
    iter->lista->largo++;
    return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
    if(lista_esta_vacia(iter->lista)){
        return NULL;
    }
    if(lista_iter_al_final(iter)){
        return NULL;
    }
    if(lista_largo(iter->lista) == 1){
        void *devolver = lista_borrar_primero(iter->lista);
        iter->actual = NULL;
        return devolver;
    }
    if (iter->anterior == NULL){
        void *devolver = iter->actual->dato;
        iter->lista->primero = iter->actual->siguiente;
        free(iter->actual);
        iter->actual = iter->lista->primero;
        iter->lista->largo--;
        return devolver;
    }   
    void *devolver = iter->actual->dato;   
    
    if(iter->actual == iter->lista->ultimo){
        free(iter->actual);
        iter->anterior->siguiente = NULL;
        iter->actual = NULL;
        iter->lista->ultimo = iter->anterior;
        iter->lista->largo--;
        return devolver;
    }
    nodo_t *nuevo_actual = iter->actual->siguiente;
    
    iter->anterior->siguiente = nuevo_actual;
    free(iter->actual);
    iter->actual = nuevo_actual;
    iter->lista->largo--;    
    return devolver;
}

void lista_iter_destruir(lista_iter_t *iter) {
    free(iter);
}