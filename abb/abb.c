#define  _POSIX_C_SOURCE 200809L
#include "abb.h"
#include "pila.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*************************
 * Definicion estructuras*
 * del ABB               *
 * **********************/

typedef struct abb_nodo{
    struct abb_nodo* izq;
    struct abb_nodo* der;
    void* dato;
    char* clave;
}abb_nodo_t;

struct abb{
    abb_nodo_t* raiz;
    abb_comparar_clave_t comparador;
    abb_destruir_dato_t destructor;
    size_t cantidad;
};

struct abb_iter{
    pila_t* pila;
};

/********************
 * Funciones Aux    *
 *  de ABB_NODO     *
 * *****************/

abb_nodo_t* crear_nodo(const char* clave, void* dato){
    abb_nodo_t* abb_nodo = malloc(sizeof(abb_nodo_t));
    if(!abb_nodo) return NULL;
    
    abb_nodo->clave = strdup(clave);
    abb_nodo->dato = dato;
    abb_nodo->izq = NULL;
    abb_nodo->der = NULL;

    return abb_nodo;
}

void destruir_nodo(abb_nodo_t* actual, abb_destruir_dato_t destruir){
    if(destruir) destruir(actual->dato);
    free(actual->clave);
    free(actual);
}

/**********************
 * Funciones          *
 * Auxiliares del     *
 *  ABB               *
 * *******************/

abb_nodo_t** abb_buscar(abb_nodo_t** raiz, const char* clave, abb_comparar_clave_t cmp){
    if(!*raiz || cmp((*raiz)->clave, clave) == 0){
        return raiz;
    }else if(cmp((*raiz)->clave, clave) > 0){
        return abb_buscar(&(*raiz)->izq, clave, cmp);
    }else{
        return abb_buscar(&(*raiz)->der, clave, cmp);
    }
}


void _abb_destruir(abb_nodo_t *actual, abb_destruir_dato_t destruir){//Recorrido postorder para elimminar el arbol
    if(!actual) return;
    _abb_destruir(actual->izq, destruir);
    _abb_destruir(actual->der, destruir);
    destruir_nodo(actual, destruir);
}

abb_nodo_t* sig_inorder(abb_nodo_t* raiz){
    abb_nodo_t* nodo_actual = raiz;
    while(nodo_actual->izq){
        nodo_actual = nodo_actual->izq;
    }
    return nodo_actual;
}

void* _abb_borrar(abb_nodo_t** raiz, const char* clave, abb_comparar_clave_t cmp){
    abb_nodo_t** nodo_a = abb_buscar(raiz, clave, cmp);
    if(!*nodo_a) return NULL;
    abb_nodo_t* nodo = *nodo_a;
    abb_nodo_t* aux;
    void* p_gen = nodo->dato;
    if(nodo->izq && nodo->der){//Ambos hijos
        aux = sig_inorder(nodo->der);
        free(nodo->clave);
        nodo->clave = strdup(aux->clave);
        nodo->dato = _abb_borrar(&nodo->der, aux->clave, cmp);
        return p_gen;
    }else if(nodo->izq){//Solo hijo izq
        aux = nodo->izq;
    }else if(nodo->der){//Solo hijo derecho
        aux = nodo->der;
    }else{//Sin hijos
        aux = NULL;
    }
    destruir_nodo(nodo, NULL);
    *nodo_a = aux;
    return p_gen;
}

void apilar_sig_inorder(abb_nodo_t *nodo, pila_t *pila){
    while(nodo){
        pila_apilar(pila, nodo);
        nodo = nodo->izq;
    }
}

/**********************
 * Primitivas del     *
 *  ABB               *
 * *******************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* arbol = malloc(sizeof(abb_t));
    if(!arbol) return NULL;

    arbol->raiz = NULL;
    arbol->cantidad = 0;
    arbol->comparador = cmp;
    arbol->destructor = destruir_dato;

    return arbol;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    abb_nodo_t** a_nodo = abb_buscar(&arbol->raiz, clave, arbol->comparador);
    if(*a_nodo){//Caso en el que ya existia la clave en el abb
        if(arbol->destructor) arbol->destructor((*a_nodo)->dato);
        (*a_nodo)->dato = dato;
        return true;
    }
    (*a_nodo) = crear_nodo(clave, dato);
    if(!a_nodo) return false;
    arbol->cantidad++;
    return true;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
    abb_nodo_t* raiz_aux = arbol->raiz;//Evito problemas de vdistinto tipo de variable
    abb_nodo_t** a_nodo = abb_buscar(&raiz_aux, clave, arbol->comparador);
    if(!*a_nodo) return NULL;
    return (*a_nodo)->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    abb_nodo_t* raiz_aux = arbol->raiz;//Evitos problemas de distinto tipo de variable
    abb_nodo_t** a_nodo = abb_buscar(&raiz_aux, clave, arbol->comparador);
    if(!*a_nodo) return false;
    return true;
}


size_t abb_cantidad(abb_t *arbol){
    return arbol->cantidad;
}

void abb_destruir(abb_t *arbol){
    _abb_destruir(arbol->raiz, arbol->destructor);
    free(arbol);
}

void *abb_borrar(abb_t *arbol, const char *clave){
    if(!abb_pertenece(arbol, clave)) return NULL;
    void* dato = _abb_borrar(&arbol->raiz, clave, arbol->comparador);
    arbol->cantidad--;
    if(arbol->cantidad == 0) arbol->raiz = NULL;
    return dato;
}

/**********************
 * Primitivas         *
 * Iterador Interno   *
 * del ABB            *
 * *******************/

bool _abb_in_order(abb_nodo_t* raiz, bool visitar(const char* clave, void* dato, void*extra), void* extra){
    if(!raiz) return true;
    bool ok = _abb_in_order(raiz->izq, visitar, extra);
    if(!ok || !visitar(raiz->clave, raiz->dato, extra)) return false;
    return _abb_in_order(raiz->der, visitar, extra);
}

void abb_in_order(abb_t *arbol, bool visitar(const char* clave, void* dato, void* extra), void * extra){
    _abb_in_order(arbol->raiz, visitar, extra);
}

/**********************
 * Primitivas         *
 * Iterador Externo   *
 * del ABB            *
 * *******************/

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t *iter = malloc(sizeof(abb_iter_t));
    pila_t *pila = pila_crear();
    abb_nodo_t *nodo = arbol->raiz;
    apilar_sig_inorder(nodo, pila);
    iter->pila = pila;
    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    pila_t *pila = iter->pila;
    if(pila_esta_vacia(pila)) return false;
    abb_nodo_t *nodo = pila_desapilar(pila);
    if(nodo->der){
        nodo = nodo->der;
        apilar_sig_inorder(nodo, pila);
    }
    iter->pila = pila;
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    if(pila_esta_vacia(iter->pila)) return NULL;
    abb_nodo_t *nodo = pila_ver_tope(iter->pila);
    return nodo->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    if(pila_esta_vacia(iter->pila)) return true;
    return false;
}

void abb_iter_in_destruir(abb_iter_t *iter){
    pila_destruir(iter->pila);
    free(iter);
}