#include "heap.h"
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#define TAM_INICIAL 10
#define DIMENSIONADOR 2

/********************
 * Definicion       *
 * Estructuras del  *
 * Heap             *
 * *****************/
struct heap{
    void** datos;
    cmp_func_t cmp;
    size_t cant;
    size_t tam;
};


/*********************
 * Funciones Aux     *
 * del Heap          *
 * ******************/


void swap(void* arr[], size_t indice_uno, size_t indice_dos){
    void* aux = arr[indice_uno];
    arr[indice_uno] = arr[indice_dos];
    arr[indice_dos] = aux;
}

bool heap_redimensionar(heap_t* heap, size_t tam_nuevo){
    if(tam_nuevo < TAM_INICIAL){
        tam_nuevo = TAM_INICIAL;
    }
    void** datos_nuevos;

    datos_nuevos = realloc(heap->datos, tam_nuevo * sizeof(void*));
    if(!datos_nuevos) return false;

    heap->datos = datos_nuevos;
    heap->tam = tam_nuevo;
    return true;
}

void upheap(void* arr[], size_t i_hijo, cmp_func_t cmp){
    if(i_hijo == 0) return;
    size_t i_padre = (i_hijo - 1) / 2;
    if(cmp(arr[i_hijo], arr[i_padre]) > 0){
        swap(arr, i_padre, i_hijo);
        upheap(arr, i_padre, cmp);
    }
}

size_t determinar_maximo(void* arr[], size_t tam , size_t i_padre, size_t i_izq, size_t i_der, cmp_func_t cmp){
    if(i_izq < tam && cmp(arr[i_izq], arr[i_padre]) > 0){
        i_padre = i_izq;
    }
    if(i_der < tam && cmp(arr[i_der], arr[i_padre]) > 0){
        i_padre = i_der;
    }
    return i_padre;
}

void downheap(void* arr[], size_t tam, size_t i_padre, cmp_func_t cmp){
    if(i_padre == tam) return;
    size_t i_izq = (2 * i_padre) + 1;
    size_t i_der  = (2 * i_padre) + 2;
    size_t i_max = determinar_maximo(arr, tam, i_padre, i_izq, i_der, cmp);
    if(i_max != i_padre){
        swap(arr, i_padre, i_max);
        downheap(arr, tam, i_max, cmp);
    }
}

void heapify(void* arr[], size_t tam, cmp_func_t cmp){

    for(int i = (int)tam/2; i >= 0; i--){
        downheap(arr, tam, i, cmp);
    }

}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
    heapify(elementos, cant, cmp);
    for(int i = (int)cant-1; i > 0; i--){
        swap(elementos, (size_t) i, 0);
        downheap(elementos, (size_t )i, 0, cmp);
    }
}
/*******************
 * Primitivas del  *
 * Heap            *
 * ****************/
heap_t *heap_crear(cmp_func_t cmp){
    heap_t* heap = malloc(sizeof(heap_t));
    if(!heap) return NULL;

    void** datos = malloc(sizeof(void*) * TAM_INICIAL);
    if(!datos){
        free(heap);
        return NULL;
    }
    heap->cmp = cmp;
    heap->cant = 0;
    heap->tam = TAM_INICIAL;
    heap->datos = datos;
    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
    heap_t *heap = malloc(sizeof(heap_t));
    if(!heap) return NULL;
    void** datos = malloc(sizeof(void*) * n);

    if(!datos){
        free(heap);
        return NULL;
    }

    heap->datos = datos;

    memcpy(heap->datos, arreglo, sizeof(void*) * n);
    
    heapify(heap->datos, n, cmp);
    
    heap->tam = n;
    heap->cant = n;
    heap->datos = datos;
    heap->cmp = cmp;
    return heap;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
    if(destruir_elemento){
        for(size_t i = 0; i < heap->cant; i++){
            destruir_elemento(heap->datos[i]);
        }
    }
    free(heap->datos);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap){
    return heap->cant;
}

bool heap_esta_vacio(const heap_t *heap){
    if(heap->cant == 0) return true;
    return false;
}

void *heap_ver_max(const heap_t *heap){
    if(heap_esta_vacio(heap)) return NULL;
    return heap->datos[0];
}

bool heap_encolar(heap_t *heap, void *elem){
    if(heap->cant == heap->tam){
        if(!heap_redimensionar(heap, heap->tam * DIMENSIONADOR)) return false;
    }
    heap->datos[heap->cant] = elem;    
    upheap(heap->datos, heap->cant, heap->cmp);
    heap->cant++;
    return true;
}

void *heap_desencolar(heap_t *heap){
    if(heap_esta_vacio(heap)) return NULL;
    swap(heap->datos, 0, heap->cant-1);
    void *elemento = heap->datos[heap->cant-1];
    heap->cant--;
    downheap(heap->datos, heap->cant, 0, heap->cmp);
    if((heap->tam > TAM_INICIAL) && (heap->cant/4 >= heap->tam)){
        if(!heap_redimensionar(heap, heap->tam / DIMENSIONADOR)) return NULL;
    }
    return elemento;
}