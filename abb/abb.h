#ifndef ABB_H
#define ABB_H
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>


typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

//Crea un ABB vacio
//Args: 
//      cmp(abb_comparar_clave_t):Funcion de comparacion para el ABB
//      destruir_dato(abb_destruir_dato_t):Funcion de destruccion para los datos del ABB
//Return:
//      arbol(abb_t*):ABB vacio
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

//Guarda un elemento en el ABB, en caso de algun error se retorna false
//Pre:El arbol fue creado
//Args:
//      arbol(abb_t*):ABB
//      clave(const char*):Clave del dato a guardar
//      dato(void*):Dato a guardar
//Return:
//      bool: En caso de cualquier error se retorna false, sino, true
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

//Busca y elimina la clave pasada por parametro en el ABB, en caso de no encontrarse, retorna NULL.
//Args:
//  arbol(abb_t*):ABB
//  clave(const char*):Clave del dato a borrar
//
//Return:
//      dato:Si se encontro el dato, se borra del ABB y se retorna, sino, retorna NULL.
void *abb_borrar(abb_t *arbol, const char *clave);

//Busca y retorna el dato de la clave pasada por parametro, si no se encuentra en el ABB, se retorna NULL.
//El arbol no sera modificado.
//Args:
//      arbol(const abb_t*):ABB
//      clave(const char*):Clave del dato a obtener
//Return:
//      dato:Si se encontro el dato, se devuelve.
void *abb_obtener(const abb_t *arbol, const char *clave);

//Verifica si la clave pasada por parametro se encuentra en el ABB.
//El ABB no sera modificado.
//Args:
//      arbol(const abb_t*):ABB
//      clave(const char*):Clave a verificar
//Return:
//      bool:Retorna true si la clave se encuentra en el ABB, sino false.
bool abb_pertenece(const abb_t *arbol, const char *clave);

//Retorna la cantidad de elementos que se encuentran en el arbol
//Args:
//      arbol(abb_t*):ABB
//Return:
//      size_t: Cantidad de elementos en el arbol
size_t abb_cantidad(abb_t *arbol);

//Destruye el abb y todos sus elementos adentro.
//Args:
//      arbol(abb_t*):ABB
void abb_destruir(abb_t *arbol);

//Recorre el arbol con un recorrido inorder, aplicando la funcion visitar en cada par clave,dato.
//Args:
//      arbol(abb_t*):ABB
//      visitar(bool)(const char*, void*,void*):Funcion de callback utilizada en cada par clave,dato.
//      extra(void*):Dato extra en caso de que el usuario quiera guardar datos
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);


//Crea un iterador para el ABB
//El iterador y el ABB no seran modificados
//Pre:El arbol fue creado
//Args:
//      arbol(abb_t*):ABB
//Return:
//      iter(abb_iter_t*):Iterador para el ABB
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

//Avanza al siguiente elemento del ABB en un recorrido inorder
//Pre:El iterador fue creado
//Args:
//      iter(abb_iter_t*):Iterador del ABB
//Return:
//      bool:Si el iterador no esta al final del ABB, avanza un elemento y retorna true
//          Si esta al final devuelve false
bool abb_iter_in_avanzar(abb_iter_t *iter);


//Verifica donde esta parado el iterador y devuelve la clave de ese elemento
//El iterador y el ABB no seran modificados
//Pre:El iterador fue creado
//Args:
//     iter(abb_iter_t*):Iterador del ABB
//Return:
//      char*:Si el iterador no esta al final, devolvera la clave actual, en caso contrario
//            se retornara NULL.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

//Verifica si el iterador esta al final
//El iterador y el ABB no seran modificados
//Pre: el iterador fue creado
//Args:
//      iter(abb_iter_t*):Iterador del ABB
//Return:
//      bool:True si el iterador esta al final
//          false si no esta al final.
bool abb_iter_in_al_final(const abb_iter_t *iter);

//Destruye el iterador pero no el arbol.
//Pre:El iterador fue creado
//Args:
//      iter(abb_iter_t*):Iterador del ABB.
void abb_iter_in_destruir(abb_iter_t* iter);



/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación realizada del ABB.
void pruebas_abb_alumno(void);

#endif