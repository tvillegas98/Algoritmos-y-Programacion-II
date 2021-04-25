#include "lista.h"
#include <stdlib.h>
#include <stdbool.h>

/**************************************
 *      ESTRUCTURAS                   *
 * ************************************/
typedef struct nodo{
    void* dato;
    struct nodo* prox;
}nodo_t;


typedef struct lista{
    nodo_t* prim;
    nodo_t* ult;
    size_t largo;
}lista_t;

typedef struct lista_iter{
    lista_t* lista;
    nodo_t* anterior;
    nodo_t* actual;
}lista_iter_t;

/**************************************
 *      PRIMITIVAS DEL NODO           *
 * ************************************/

static nodo_t* crear_nodo(void* valor){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    
    if(!nodo) return NULL;

    nodo->dato = valor;
    nodo->prox = NULL;

    return nodo;
}


/**************************************
 *      PRIMITIVAS DE LA LISTA        *
 * ************************************/

lista_t *lista_crear(void){
    lista_t* lista = malloc(sizeof(lista_t));
    if(!lista) return NULL;
    lista->prim = NULL;
    lista->ult = NULL;
    lista->largo = 0;
    return lista;
}

bool lista_esta_vacia(const lista_t* lista){
    return lista->largo == 0;
}

bool lista_insertar_ultimo(lista_t* lista, void* dato){
    nodo_t* nodo = crear_nodo(dato);
    if(!nodo) return false;

    if(lista_esta_vacia(lista)){
        lista->prim = nodo;
        lista->ult = lista->prim;
    }else{
        lista->ult->prox = nodo;
        lista->ult = nodo;
    }

    lista->largo++;
    return true;
}

bool lista_insertar_primero(lista_t* lista, void* dato){
    nodo_t* nodo = crear_nodo(dato);
    if(!nodo) return false;
    
    if(lista_esta_vacia(lista)){
        lista->prim = nodo;
        lista->ult = lista->prim;
    }else{
        nodo->prox = lista->prim;
        lista->prim = nodo;
    }

    lista->largo++;
    return true;
}



void* lista_ver_primero(const lista_t* lista){
    if(lista_esta_vacia(lista)) return NULL;
    return lista->prim->dato;
}

void* lista_ver_ultimo(const lista_t* lista){
    if(lista_esta_vacia(lista)) return NULL;
    return lista->ult->dato;
}

void* lista_borrar_primero(lista_t* lista){
    if(lista_esta_vacia(lista)) return NULL;
    nodo_t* nodo_aux = lista->prim;
    void* puntero_aux = nodo_aux->dato;

    if(lista->prim == lista->ult){
        lista->prim = NULL;
        lista->ult = NULL;
    }else{
        lista->prim = lista->prim->prox;
    }

    free(nodo_aux);
    lista->largo--;
    return puntero_aux;
}

void lista_destruir(lista_t* lista, void (*destruir_dato)(void *)){
    while(!lista_esta_vacia(lista)){
        void* dato = lista_borrar_primero(lista);
        if(destruir_dato) destruir_dato(dato);
    }
    free(lista);
}

size_t lista_largo(const lista_t* lista){
    return lista->largo;
}

/**************************************
 * PRIMITIVA DEL ITERADOR INTERNO     *
 * ************************************/

void lista_iterar(lista_t* lista, bool(*visitar)(void* dato, void *extra), void* extra){
    bool iterar = true;
    nodo_t* nodo_auxiliar = lista->prim;
    while(nodo_auxiliar && iterar){
        iterar = visitar(nodo_auxiliar->dato, extra);
        nodo_auxiliar = nodo_auxiliar->prox;
    }
}


/**************************************
 * PRIMITIVA DEL ITERADOR EXTERNO     *
 * ************************************/

lista_iter_t *lista_iter_crear(lista_t *lista){
    lista_iter_t* nuevo_iter = malloc(sizeof(lista_iter_t));
    if(!nuevo_iter) return NULL;
    nuevo_iter->lista = lista;
    nuevo_iter->anterior = NULL;
    nuevo_iter->actual = lista->prim;

    return nuevo_iter;
}


void *lista_iter_ver_actual(const lista_iter_t *iter){
    if(lista_iter_al_final(iter)) return NULL;
    return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){
    if(iter->actual == NULL) return true;
    return false;
}

bool lista_iter_avanzar(lista_iter_t *iter){
    if(lista_iter_al_final(iter)) return false;
    iter->anterior = iter->actual;
    iter->actual = iter->actual->prox;
    return true;
}

void lista_iter_destruir(lista_iter_t *iter){
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){

    if(lista_esta_vacia(iter->lista) || iter->actual == iter->lista->prim){
        if(!lista_insertar_primero(iter->lista, dato)) return false;
        iter->actual = iter->lista->prim;
    }else if(lista_iter_al_final(iter)){
        if(!lista_insertar_ultimo(iter->lista, dato)) return false;
        iter->actual = iter->lista->ult;
    }else{
        nodo_t* nuevo_nodo = crear_nodo(dato);
        if(!nuevo_nodo) return false;

        iter->anterior->prox = nuevo_nodo;
        nuevo_nodo->prox = iter->actual;
        iter->actual = nuevo_nodo;
        iter->lista->largo++; 
    }
    return true;
}

void *lista_iter_borrar(lista_iter_t *iter){
    void* puntero_auxiliar = NULL;
    if(lista_esta_vacia(iter->lista) || lista_iter_al_final(iter)) return puntero_auxiliar;

    if(!iter->anterior){
        puntero_auxiliar = lista_borrar_primero(iter->lista);
        iter->actual = iter->lista->prim;
    }else{
        nodo_t* auxiliar = iter->actual;
        iter->anterior->prox = auxiliar->prox;
        iter->actual = auxiliar->prox;
        if(iter->lista->ult == auxiliar) iter->lista->ult = iter->anterior;
        puntero_auxiliar = auxiliar->dato;
        iter->lista->largo--;
        free(auxiliar);
    }

    return puntero_auxiliar;
}
