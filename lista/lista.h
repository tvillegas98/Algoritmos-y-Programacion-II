#ifndef _LISTA_H
#define _LISTA_H
#include <stdlib.h>
#include <stdbool.h>

/**************************************
 *      PRIMITIVAS DE LA LISTA        *
 * ************************************/


typedef struct lista lista_t;


/*
Crea una lista.
Post: Devuelve una lista vacia.
*/
lista_t *lista_crear(void);


// Devuelve true si la lista esta vacia, en caso contrario, false.
// Pre: La lista fue creada.
bool lista_esta_vacia(const lista_t* lista);

// Devuelve un booleano true si no hubo problemas en la insercion
//del elemento, en caso algun fallo, devuelve NULL.
//Pre:La lista fue creada
//Post: Dato debe estar al inicio de la lista
bool lista_insertar_primero(lista_t* lista, void* dato);

//Devuelve un booleano en estado true si no hubo problemas en la insercion
//del dato, en caso de algun fallo, devuelve NULL.
//Pre:La lista fue creada
//Post: Dato debe ser el final de la lista
bool lista_insertar_ultimo(lista_t* lista, void* dato);

//Devuelve al usuario el dato que esta al inicio de la lista,
//si la lista esta vacia, devuelve NULL.
//Pre:La lista fue creada.
//Post:Se devolvio el primer elemento de la lista
// al usuario y La lista no debio ser modificada.
void* lista_ver_primero(const lista_t* lista);

//Devuelve al usuario el dato que esta al final de la lista,
//si la lista esta vacia, devuelve NULL.
//Pre:La lista fue creada.
//Post:Se devolvio el primer elemento de la lista
// al usuario y La lista no debio ser modificada.
void* lista_ver_ultimo(const lista_t* lista);


//Borra el primer elemento de la lista si solo si, esta tiene
//elementos, en caso de que este vacia devuelve NULL, si no estaba vacia
//devuelve el dato borrado.
//Pre:La lista fue creada.
//Post: Devolvio el dato borrado.
void* lista_borrar_primero(lista_t* lista);


// Destruye la lista de maneras distintas, dependiendo de que funcion
// el usuario le pase como argumento.
// Si la funcion es NULL, se destruira la lista y no los punteros asociados
// a los datos.
// Si es cualquier otra cosa distinta de NULL, los datos podran verse modificados
// o eliminados.
//Pre:La lista fue creada
//Post: La lista fue destruida y el estado de los datos depende del usuario.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void*));


//Pre:La lista fue creada
//Post:Devuelve el largo de la lista 
size_t lista_largo(const lista_t* lista);

/**************************************
 * PRIMITIVA DEL ITERADOR INTERNO     *
 * ************************************/

//Recorre la lista y para cada elemento de la lista
//se le aplica la funcion visitar, pasandole por parametro el
//elemento de la lista con nombre dato, y una variable llamada extra
//que puede tener distintos usos, en casos de no necesitarla, colocar NULL.
//La funcion visitar debe cumplir que el dato que devuelva sea bool, es decir,
//de ella dependera si la lista sigue iterando o no.
//Pre:La lista fue creada
//Post:Se llamo la funcion aplicar para cada elemento de la lista,
// o hasta determinado elemento.Ningun elemento de la lista debio ser modificado.
void lista_iterar(lista_t* lista, bool(*visitar)(void* dato, void *extra), void* extra);

/**************************************
 * PRIMITIVA DEL ITERADOR EXTERNO     *
 * ************************************/
typedef struct lista_iter lista_iter_t;

//Crea un iterador externo para el tda lista.
//Post: Devuelve el iterador externo para la lista
lista_iter_t *lista_iter_crear(lista_t *lista);

//Avanzo al siguiente elemento de la lista, true si avanzo correctamente
//Si llego al ultimo elemento de la lista, este ya no puede seguir avanzando
// y se devuelve false.
//Pre: El iterador fue creado
//Post: El iterador avanzo un elemento y se devolvio true, en caso contrario, false.
bool lista_iter_avanzar(lista_iter_t *iter);

//Se verifica donde esta apuntando actualmente el iterador y devuelve el
//dato de dicho elemento.
//Pre: El iterador fue creado.
//Post: Devuelve el dato apuntado por el iterador.
void *lista_iter_ver_actual(const lista_iter_t *iter);

//Verifica si el iterador esta en el final de la lista
//esto solo sucede si el elemento actual del iterador apunta a NULL
//Pre:El iterador fue creado
//Post: Devuelve true si la lista esta al final, false en caso contrario.
bool lista_iter_al_final(const lista_iter_t *iter);

//Destruye el iterador cuando el usuario lo requiera
//Pre:El iterador fue creado
//Post:El iterador fue destruido
void lista_iter_destruir(lista_iter_t *iter);



//Inserta un elemento en la posicion actual del iterador, 
//si el iterador esta al inicio, se comporta igual que la primitiva insertar_primero
//si el iterador esta al final, se comporta igual que la primitiva insertar_ultimo
//cualquier otro caso, el anterior elemento apuntara al nuevo el elemento
//y el iterador actual apuntara al nuevo elemento
//En caso de algun error, se devuelve false
//Pre:La lista fue creada
//Post:La lista tiene un elemento nuevo, se incrementa su largo
//y devuelve true
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

//Elimina un elemento en la posicion actual del iterador
//Si la lista esta vacia o esta al final de la lista, se devuelve null
//Si la lista tiene un solo elemento, la lista quedara vacia luego
//Si el elemento actual es el primer elemento de la lista, se comportara
//de la misma forma que la primitiva borrar_primero
//Si actual esta en el ultimo de la lista, actual debera apuntar a null
//Pre: La lista fue creada
//Post: La lista tiene un elemento menos y se incremento su largo
void *lista_iter_borrar(lista_iter_t *iter);

// Realiza pruebas sobre la implementación realizada.
void pruebas_lista_estudiante(void);



#endif