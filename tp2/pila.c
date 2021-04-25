#include "pila.h"
#include <stdlib.h>
#include <stdio.h>
#define CANTIDAD_INICIAL 10
#define DIMENSIONADOR 2

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t *pila_crear(void){
    pila_t* pila = malloc(sizeof(pila_t));
    if(pila == NULL) return NULL;
    
    pila->datos = malloc(CANTIDAD_INICIAL * (sizeof(void*)));

    if(pila->datos == NULL){
        free(pila);
        return NULL;
    }

    pila->capacidad = 10;
    pila->cantidad = 0;
    
    return pila;
}

bool pila_redimensionar(pila_t *pila, size_t capacidad_nueva){
    void** datos_nuevo;

    datos_nuevo = realloc(pila->datos, capacidad_nueva * sizeof(void*));
    if(datos_nuevo == NULL) return false;

    pila->capacidad = capacidad_nueva;
    pila->datos = datos_nuevo;
    return true;
}

void pila_destruir(pila_t *pila){
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
    return pila->cantidad == 0;
}

bool pila_apilar(pila_t *pila, void *valor){

    if(pila->cantidad == pila->capacidad){ 
        if(!pila_redimensionar(pila, pila->capacidad * DIMENSIONADOR)) return false;
    }
    
    pila->cantidad++;
    pila->datos[pila->cantidad - 1] = valor;

    return true; 
}

void *pila_ver_tope(const pila_t *pila){

    if(pila_esta_vacia(pila)){
        return NULL;
    }

    void *puntero_aux = pila->datos[pila->cantidad - 1];
    
    return puntero_aux;
}

void *pila_desapilar(pila_t *pila){
    if(pila_esta_vacia(pila)) return NULL;
    
    void *puntero_aux = pila->datos[pila->cantidad - 1];

    if(pila->cantidad <= pila->capacidad/4 && pila->capacidad > CANTIDAD_INICIAL){
        if(!pila_redimensionar(pila, pila->capacidad / DIMENSIONADOR)) return NULL;
    }
    
    pila->cantidad--;

    return puntero_aux;
}


// ...
