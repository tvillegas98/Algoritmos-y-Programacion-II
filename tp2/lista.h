#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stdlib.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La lista está planteada como una lista de punteros genéricos. */

struct lista;
typedef struct lista lista_t;


/* Defino el iterador externo */

struct lista_iter;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista
//Post: Devuelve una nueva lista vacia.

lista_t *lista_crear(void);

// Devuelve verdadero si la lista no tiene elementos, false en caso contrario,
//Pre; La lista fue creada,
bool lista_esta_vacia(const lista_t *lista);

//Agrega un nuevo elemento al principio de la lista. Devuelve false en caso de error
//Pre: La lista fue creada.
//Post: Se agrego un nuevo elemento al principio de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

//Agrego un nuveo elemento al final de la lista. Devuelve false en caso de error
//Pre: La lista fue creada
//Post: Se agrego un nuevo elemento al final de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

//Borra el primer elemento de la lista. Si la lista tiene elementos,
//se devuelve su valor luego de quitarlo, si esta vacia, devuelve NULL.
//Pre: La lista fue creada.
//Post: Se devolvio el valor del primer elemento de la lista, la lista
//tiene un elemento menos en caso de no estar vacia.
void *lista_borrar_primero(lista_t *lista);

//Obtiene el valor del primer elemento de la lista. Si la lista tiene elementos, 
//se devuelve el valor del primer elemento, si esta vacia devuele NULL.
//Pre: La lista fue creada.
//Post: Se devolvio el valor del primer elemento de la lista, cuando no esta vacia.
void *lista_ver_primero(const lista_t *lista);

//Obtiene el valor del ultimo elemento de la lista. Si la lista tiene elementos, 
//se devuelve el valor del ultimo elemento, si esta vacia devuele NULL.
//Pre: La lista fue creada.
//Post: Se devolvio el valor del ultimo elemento de la lista, cuando no esta vacia.
void *lista_ver_ultimo(const lista_t *lista);

//Se obtiene el valor del largo de la lista.
//Pre: La lista fue creada.
//Post: Se devolvio el valor del largo de la lista.
size_t lista_largo(const lista_t *lista);

//Destruye la lista. Si se recibe la funcion destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
//En caso de que no se requiera destruir los datos se debe usar NULL.
// Pre: La lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista.
// Post: Se eliminaron todos los elementos de la cola.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void*));


/* *****************************************************************
 *                  PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/

//Para cada elemento de la lista se ejecuta la funcion visitar, que se le pasa por parametro.
//La funcion visitar devuelve true en caso de querer seguir iterando la lista, false en caso contrario.
//En caso de no querer usar el parametro extra se le debe pasar NULL como parametro.
//Pre: La lista fue creada
//Post: Se ejecuto la funcion visitar para cada elemento de la lista.
void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra);

/* *****************************************************************
 *                 PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

//Crea un iterador externo de la lista.
//Devuelve 
lista_iter_t *lista_iter_crear(lista_t *lista);

//Avanza un elemento de la lista, 
//Pre: La lista y su iterador fueron creadas.
//Post: Se avanzo un elemento en la lista, false en caso de que no haya mas elementos en la lista.
bool lista_iter_avanzar(lista_iter_t *iter);

//Obtiene el dato de la posicion actual en la que se encuentra el iterador.
//Pre: La lista y su iterador fueron creadas.
//Post: Se devolvio el dato de la posicion actual del iterador, si actual es invalido devuelve NULL;
void *lista_iter_ver_actual(const lista_iter_t *iter);

//Devuelve verdadero si el iterador se encuentra al final de la lista, false en caso contrario.
//Pre: La lista y su iterador fueron creados.
//Post: Devuelve verdadero si el iterador se encuentra al final de la lista, false en caso contrario.
bool lista_iter_al_final(const lista_iter_t *iter);

//Inserta un elemento entre el elemento anterior y el actual del iterador de la lista.
//Pre: La lista y su iterador fueron creadas.
//Post: Se inserto el elemento, false en caso de error.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

//Saca el elemento que se encuentra en la posicion actual del iterador de la lista,
//si la lista esta vacia, o si el iterador se encuentra al final, devuelve NULL.
//Pre: La lista y su iterador fueron creadas.
//Post: Se devolvio el valor del elemento quitado, la lista contiene un elemento menos, si no estaba vacia.
void *lista_iter_borrar(lista_iter_t *iter);

//Destruye el iterador
//Pre: El iterador de la lista fue creado
//Post: El iterador de la lista fue destruido
void lista_iter_destruir(lista_iter_t *iter);

/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

//Realiza pruebas sobre la implementacion realizada.
//
//Las pruebas se encuentran en el archivo ‘pruebas_lista.c’.

void pruebas_lista_estudiante(void);

#endif // LISTA_H