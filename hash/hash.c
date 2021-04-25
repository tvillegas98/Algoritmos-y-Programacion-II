#define  _POSIX_C_SOURCE 200809L
#include "hash.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define TAM_INICIAL 13
#define F_CARGA_MAX 70
#define F_CARGA_MIN 20
#define DIMENSIONADOR 2
#define MULTIPLICADOR 100
/******************************
 * Definicion de              *
 * Estructuras                *
 * ***************************/
typedef enum estado{
    VACIO, OCUPADO, BORRADO
}estado_t;

typedef struct campo{
    estado_t estado;
    void* dato;
    char* clave;
}campo_t;

typedef struct hash{
    size_t capacidad;
    size_t cantidad;
    hash_destruir_dato_t funcion_destr;
    campo_t* tabla;
}hash_t;

struct hash_iter {
    const hash_t *hash;
    size_t indice;
};

/******************************
 * Funcion de Hashing         *
 * ***************************/
//Funcion Hash llamada Jenkins on at time
//Argumentos:
//  const char str: cadena a hashear
//  size_t strlen: longitud de la cadena
//  size_t len_hash: tamanio de la tabla hash
//Retorno: 
//  Un indice para la tabla hash en el rango 0~len(hash)-1
size_t hash_function(const char *str, size_t strlen, size_t len_hash)
{
    size_t hash, i;
    for(hash = i = 0; i < strlen; ++i)
    {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash % len_hash;
}


/******************************
 * Funciones de redimension   *
 * o wrappers                 *
 * ***************************/

hash_t* _hash_crear(hash_destruir_dato_t funcion_destruir, size_t tam){
    hash_t* hash = malloc(sizeof(hash_t));
    if(!hash) return NULL;
    campo_t* tabla_ini = malloc(sizeof(campo_t)*tam);
    if(!tabla_ini){
        free(hash);
        return NULL;
    }

    hash->capacidad = tam;
    hash->cantidad = 0;
    hash->funcion_destr = funcion_destruir;
    hash->tabla = tabla_ini;

    for(size_t i = 0; i < tam; i++) hash->tabla[i].estado = VACIO;

    return hash;
}

bool hallar_clave(const hash_t* hash, const char* clave, size_t* indice, estado_t estado_buscado){
    size_t indice_aux = hash_function(clave, strlen(clave), hash->capacidad);
    while(hash->tabla[indice_aux].estado != VACIO){
        if(estado_buscado == OCUPADO && hash->tabla[indice_aux].estado == OCUPADO && strcmp(hash->tabla[indice_aux].clave, clave) == 0){
            if(indice) *indice = indice_aux;
            return true;
        }
        if(estado_buscado == VACIO && hash->tabla[indice_aux].estado == OCUPADO && strcmp(hash->tabla[indice_aux].clave, clave) == 0){
            if(indice) *indice = indice_aux;
            return false;
        }
        if(indice_aux+1 >= hash->capacidad){
            indice_aux = 0;
        }else{
            indice_aux++;
        }
    }
    if(estado_buscado == VACIO){
        *indice = indice_aux;
        return true;
    }
    return false;
}

bool hash_redimensionar(hash_t* hash, size_t tam){
    campo_t *tabla_nueva = malloc((sizeof(campo_t)*tam));
    if(!tabla_nueva) return false;

    for(size_t i = 0; i < tam; i++) tabla_nueva[i].estado = VACIO;


    for(size_t i = 0; i < hash->capacidad; i++){
        if(hash->tabla[i].estado == OCUPADO){
            size_t j = hash_function(hash->tabla[i].clave,strlen(hash->tabla[i].clave),tam);
            while(tabla_nueva[j].estado == OCUPADO){
                if(j+1 >= tam){
                    j = 0;
                }else{
                    j++;
                }
            }
            tabla_nueva[j].clave = hash->tabla[i].clave;
            tabla_nueva[j].dato = hash->tabla[i].dato;
            tabla_nueva[j].estado = OCUPADO;
        }
    }
    
    free(hash->tabla);
    hash->capacidad = tam;
    hash->tabla = tabla_nueva;
    return true;
}

/******************************
 * Definicion de las          *
 * Primitivas del Hash        *
 * ***************************/


hash_t* hash_crear(hash_destruir_dato_t funcion_destruir){
    hash_t* hash = _hash_crear(funcion_destruir, TAM_INICIAL);
    return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
    if((MULTIPLICADOR*hash->cantidad/hash->capacidad) >= F_CARGA_MAX){
        if(!hash_redimensionar(hash, hash->capacidad*DIMENSIONADOR)) return false;
    }

    size_t indice;

    if(hallar_clave(hash, clave, &indice, VACIO)){
        char* copia = strdup(clave);
        if(!copia) return false;
        hash->tabla[indice].estado = OCUPADO;
        hash->tabla[indice].dato = dato;
        hash->tabla[indice].clave = copia;
        hash->cantidad++;
    }else{
        if(hash->funcion_destr) hash->funcion_destr(hash->tabla[indice].dato);
        hash->tabla[indice].dato = dato;
    }

    return true;
}



void *hash_borrar(hash_t *hash, const char *clave){
    if((MULTIPLICADOR*hash->cantidad/hash->capacidad) <= F_CARGA_MIN && hash->capacidad >= TAM_INICIAL){
        if(!hash_redimensionar(hash, hash->capacidad/DIMENSIONADOR)) return false;
    }
    size_t indice;
    void* dato = NULL;
    if(hallar_clave(hash, clave, &indice, OCUPADO)){
        free(hash->tabla[indice].clave);
        dato = hash->tabla[indice].dato;
        hash->tabla[indice].estado = BORRADO;
        hash->cantidad--;
    }
    return dato;
}

void *hash_obtener(const hash_t *hash, const char *clave){
    size_t indice;
    if(hallar_clave(hash, clave, &indice, OCUPADO)) return hash->tabla[indice].dato;
    return NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
    return hallar_clave(hash, clave, NULL, OCUPADO);
}

size_t hash_cantidad(const hash_t* hash){
    return hash->cantidad;
}

void hash_destruir(hash_t *hash){
    for(size_t i = 0; i < hash->capacidad; i++){
        if(hash->tabla[i].estado == OCUPADO){
            free(hash->tabla[i].clave);
            if(hash->funcion_destr) hash->funcion_destr(hash->tabla[i].dato);
        }
    }
    free(hash->tabla);
    free(hash);
}

/******************************
 * Definicion de las          *
 * Primitivas del Iterador    *
 * ***************************/

hash_iter_t *hash_iter_crear(const hash_t *hash){
    hash_iter_t *iter = malloc(sizeof(hash_iter_t));
    if (!iter){
        return NULL;
    }
    size_t indice = 0;
    iter->hash = hash;

    while ((indice < hash->capacidad) && hash->tabla[indice].estado != OCUPADO){
        indice++;
    }
    
    iter->indice = indice;
    return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){
    if(hash_iter_al_final(iter)){
        return false;
    }
    iter->indice++;
    while(!hash_iter_al_final(iter) && iter->hash->tabla[iter->indice].estado != OCUPADO) iter->indice++;
    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
    if(hash_iter_al_final(iter)){
        return NULL;
    }
    return iter->hash->tabla[iter->indice].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter){
    return !(iter->indice < iter->hash->capacidad);
}

void hash_iter_destruir(hash_iter_t *iter){
    free(iter);
}