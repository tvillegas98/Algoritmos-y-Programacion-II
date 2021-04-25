#include "cola.h"
#include "pila.h"
#include "lista.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>

static void prueba_lista_vacia(){
    lista_t* lista = lista_crear();
    printf("---------PRUEBAS CON LISTA EN ESTADO INICIAL---------\n");

    print_test("La lista esta vacia: ", lista_esta_vacia(lista) == true);

    print_test("Ver el inicio de la lista devuelve null", lista_ver_primero(lista) == NULL);

    print_test("Ver el final de la lista devuelve null", lista_ver_ultimo(lista) == NULL);

    print_test("Borrar un elemento en una lista vacia devuelve null", lista_borrar_primero(lista) == NULL);

    print_test("Enlistar el elemento al null principio de la lista devuelve true", lista_insertar_primero(lista, NULL) == true);

    print_test("Enlistar el elemento null al final de la lista devuelve true", lista_insertar_ultimo(lista, NULL) == true);

    print_test("Verifico que el largo de la lista sea 2", lista_largo(lista) == 2);

    print_test("Borro el primer elemento de la lista", lista_borrar_primero(lista) == NULL);
    print_test("Borro el 'segundo' elemento de la lista", lista_borrar_primero(lista) == NULL);
    print_test("Verifico que la lista este vacia y su largo sea 0", lista_esta_vacia(lista) == true && lista_largo(lista) == 0);

    printf("---------FIN DE LAS PRUEBAS CON LA LISTA EN ESTADO INICIAL---------\n");
    lista_destruir(lista, NULL);
    print_test("La lista fue destruida", true);
}

static void prueba_lista_con_iter(){
    lista_t* lista = lista_crear();
    lista_iter_t* iter = lista_iter_crear(lista);
    printf("---------PRUEBAS CON LISTA EN ESTADO INICIAL CON UN ITERADOR--------\n");
    print_test("Actual debe apuntar a null(porque la lista esta vacia)", lista_iter_ver_actual(iter) == NULL);
    
    print_test("Actual debe estar al final de la lista(porque la lista esta vacia)", lista_iter_al_final(iter) == true);

    print_test("Borrar un elemento con el iterador en una lista vacia debe devolver null", lista_iter_borrar(iter) == NULL);

    print_test("Enlistar el elemento null con el iterador devuelve true:", lista_iter_insertar(iter, NULL) == true);
    print_test("Actual debe ahora apuntar a null: ", lista_iter_ver_actual(iter) == NULL);
    print_test("Verifico que el largo de la lista sea uno:", lista_largo(lista) == 1);
    
    print_test("Borrar null de la lista devuelve null: ", lista_iter_borrar(iter) == NULL);
    print_test("Actual debe ahora apuntar a null: ", lista_iter_ver_actual(iter) == NULL);
    print_test("Verifico que el largo de la lista sea uno:", lista_largo(lista) == 0);

    print_test("Verifico que la lista esta vacia: ", lista_esta_vacia(lista) == true);
    printf("---------FIN DE LAS PRUEBAS CON EL ITERADOR Y LA LISTA EN ESTADO INICIAL-------\n");
    
    printf("---------INSERTAR MAS DE UN ELEMENTO CON EL ITERADOR-------\n");
    
    size_t tam = 10;
    int vector[tam];
    bool ok = true;
    bool ok_dos = true;
    for(size_t i = 0; i < tam; i++){
        vector[i] = (int)i;
        ok &= lista_iter_insertar(iter, &vector[i]);
        ok_dos &= lista_iter_ver_actual(iter) == &vector[i];
    }

    print_test("Todos los elementos fueron insertados correctamente: ", ok);
    print_test("En cada iteracion, actual apunto al nuevo elemento: ", ok_dos);
    print_test("El largo de la lista es 10(elementos insertados)", lista_largo(lista) == 10);

    int numero = 22;
    int* numero_p = &numero;
    print_test("Inserte un elemento al inicio de la lista correctamente: ", lista_iter_insertar(iter, numero_p) == true);
    print_test("Verifico que actual ahora apunte al nuevo elemento: ", (int*)lista_iter_ver_actual(iter) == numero_p);
    print_test("Verifico que actual sea igual al primer elemento de la lista: ", lista_ver_primero(lista) == lista_iter_ver_actual(iter));
    print_test("Borro el elemento actual en el iterador: ", lista_iter_borrar(iter) == numero_p);
    for(size_t i = 0; i < tam; i++){
        ok &= lista_iter_borrar(iter) == &vector[tam-1-i];
    }
    print_test("Borre la lista entera: ", ok);
    print_test("La lista debio quedar vacia: ", lista_esta_vacia(lista) == true);
    lista_destruir(lista, NULL);
    lista_iter_destruir(iter);
    print_test("La lista y el iterador fueron destruidos", true);
    printf("---------FIN DE LA INSERCION DE ELEMENTOS-------\n");
}

static void prueba_lista_ya_definida(){
    printf("---------PRUEBA DE ITERADOR CON LISTA YA DEFINIDA-------\n");
    lista_t* lista = lista_crear();
    size_t tam = 10;
    bool ok = true;
    int vector[tam];
    for(size_t i = 0; i < tam; i++){
        vector[i] = (int)i+1;
        ok &= lista_insertar_ultimo(lista, &vector[i]);
    }
    print_test("La nueva lista fue creada con exito: ", true);
    lista_iter_t* iter = lista_iter_crear(lista);
    for(size_t i = 0; i < tam/2; i++){
        ok &= lista_iter_avanzar(iter);
    }
    print_test("El iterador actualmente esta en la mitad de la lista: ", lista_iter_ver_actual(iter) == &vector[tam/2]);

    int numero = 22;
    int* numero_p = &numero;
    
    ok = lista_iter_insertar(iter, numero_p);

    print_test("Numero insertado en la mitad de la lista: ", ok);
    print_test("Verifico que actual sea igual al numero recien insertado: ", lista_iter_ver_actual(iter) == numero_p);
    print_test("El largo de la lista es 11: ", lista_largo(lista) == 11);
    print_test("Elimino el elemento recien insertado: ", lista_iter_borrar(iter) == numero_p);
    print_test("Actual debe apuntar al antiguo elemento en la mitad: ", lista_iter_ver_actual(iter) == &vector[tam/2]);
    print_test("El largo de la lista es 10 de nuevo: ", lista_largo(lista) == 10);
    lista_iter_destruir(iter);
    print_test("Se destruyo el iterador actual: ", true);
    lista_iter_t* iter_dos = lista_iter_crear(lista);

    int suma_total = 0;
    printf("A continuacion se sumaran todos los elementos de la lista\n");
    while(!lista_iter_al_final(iter_dos)){
        int* dato = lista_iter_ver_actual(iter_dos);
        suma_total += *dato;
        ok = lista_iter_avanzar(iter_dos);
    }
    print_test("Se sumaron todos los elementos (1-10) y la suma es 55:", ok && suma_total == 55);
    lista_iter_destruir(iter_dos);
    lista_destruir(lista, NULL);
    print_test("La lista y el iterador fueron destruidos", true);
    printf("---------FIN DE LAS PRUEBAS CON LA LISTA YA DEFINIDA-------\n");
}


static void prueba_lista_volumen(){
    printf("---------PRUEBA DE VOLUMEN-------\n");
    size_t tam = 10000;
    lista_t* lista = lista_crear();
    int vector_int[tam];
    char vector_char[tam];
    float vector_float[tam];
    bool ok = true;
    
    for(size_t i = 0; i < tam; i++){
        vector_int[i] = (int)i;
        vector_char[i] = (char)(97 + i);
        vector_float[i] = (float)i;
    }

    for(size_t i = 0; i < tam; i++){
        ok &= lista_insertar_ultimo(lista, &vector_int[i]);
    }
    print_test("10.000 punteros int enlistados(largo debe ser 10.000): ", ok && lista_largo(lista) == tam);

    for(size_t i = 0; i < tam; i++){
        ok &= lista_insertar_ultimo(lista, &vector_char[i]);
    }
    print_test("10.000 punteros char enlistados(largo debe ser 20.000)", ok && lista_largo(lista) == tam*2);
    for(size_t i = 0; i < tam; i++){
        ok &= lista_insertar_ultimo(lista, &vector_float[i]);
    }
    print_test("10.000 punteros float enlistados(largo debe ser 30.000): ", ok && lista_largo(lista) == tam*3);

    for(size_t i = 0; i < tam; i++){
        ok &= lista_borrar_primero(lista) == &vector_int[i];
    }
    print_test("Se eliminaron 10.000 punteros int: ", ok && lista_largo(lista) == tam*2);

    for(size_t i = 0; i < tam; i++){
        ok &= lista_borrar_primero(lista) == &vector_char[i];
    }
    print_test("Se eliminaron 10.000 punteros char: ", ok && lista_largo(lista) == tam);

    for(size_t i = 0; i < tam; i++){
        ok &= lista_borrar_primero(lista) == &vector_float[i];
    }
    print_test("Se eliminaron 10.000 punteros float: ", ok && lista_largo(lista) == 0);

    print_test("La lista debe estar vacia: ", lista_esta_vacia(lista));

    lista_destruir(lista, NULL);
    print_test("La lista fue destruida", true);
    printf("---------FIN DE LA PRUEBA DE VOLUMEN-------\n");
}

bool _pares(int* numero, int* suma){
    if(*numero%2 == 0){
        *suma += *numero;
    }
    return true;
}
bool pares(void* numero, void* suma){
    return _pares((int*)numero, (int*)suma);
}

static void prueba_iterador_interno(){
    printf("---------PRUEBA DEL ITERADOR INTERNO-------\n");
    lista_t* lista = lista_crear();
    size_t tam = 10;
    bool ok = true;
    int vector[tam];
    for(size_t i = 0; i < tam; i++){
        vector[i] = (int)i+1;
        ok &= lista_insertar_ultimo(lista, &vector[i]);
    }
    print_test("Se creo una lista de enteros con numeros del 1 al 10", ok);
    int suma = 0;
    lista_iterar(lista, pares, &suma);
    print_test("La suma de los numeros pares entre 1 y 10 es 30:", suma == 30);
    lista_destruir(lista, NULL);
    print_test("La lista fue destruida", true);
    printf("---------FIN DE LA PRUEBA DEL ITERADOR INTERNO-------\n");

}

void destruir_dato_cola(void* cola){
    cola_destruir(cola, NULL);
}

void destruir_dato_pila(void* pila){
    pila_destruir(pila);
}

static void prueba_listas_con_pila(){
    printf("---------PRUEBAS DE ENLISTAR UNA PILA-------\n");
    pila_t* pila = pila_crear();
    lista_t* lista = lista_crear();
    bool ok = true; 
    ok &= lista_insertar_primero(lista,pila);
    print_test("Se enlisto una pila: ", ok);
    ok &= lista_borrar_primero(lista) == pila;
    print_test("Se elimino la pila de la lista y la lista esta vacia:", ok && lista_esta_vacia(lista));
    
    size_t tam = 10;
    int vector[tam];
    for(size_t i = 0; i < tam; i++){
        vector[i] = (int)i;
        ok &= pila_apilar(pila, &vector[i]);
    }
    lista_insertar_primero(lista, pila);
    print_test("Pila con elementos: ", ok);
    lista_destruir(lista, destruir_dato_pila);
    print_test("Se destruyo la pila desde la lista y la lista: ", true);
    printf("---------FIN PRUEBAS DE ENLISTAR UNA PILA------\n");
}


static void prueba_listas_con_cola(){
    printf("---------PRUEBAS DE ENLISTAR UNA COLA-------\n");
    cola_t* cola = cola_crear();
    lista_t* lista = lista_crear();
    bool ok = true; 
    ok &= lista_insertar_primero(lista,cola);
    print_test("Se enlisto una cola: ", ok);
    ok &= lista_borrar_primero(lista) == cola;
    print_test("Se elimino la cola de la lista y la lista esta vacia:", ok && lista_esta_vacia(lista));
    
    size_t tam = 10;
    int vector[tam];
    for(size_t i = 0; i < tam; i++){
        vector[i] = (int)i;
        ok &= cola_encolar(cola, &vector[i]);
    }
    print_test("Cola con elementos: ", ok);
    ok = lista_insertar_primero(lista, cola);
    print_test("Se agrego la cola elementos a la lista:", ok);
    lista_destruir(lista, destruir_dato_cola);
    print_test("Se destruyo la cola desde la lista y la lista: ", true);
    printf("---------FIN PRUEBAS DE ENLISTAR UNA COLA-------\n");
}
static void prueba_lista_con_puntero_dinamico(){
    printf("---------PRUEBA ELIMINAR LISTA CON PUNTEROS DINAMICOS-------\n");
    lista_t* lista = lista_crear();
    bool ok = true;
    int* int_d = malloc(sizeof(int));
    float* float_d = malloc(sizeof(float));
    char* char_d = malloc(sizeof(char));
    ok &= lista_insertar_ultimo(lista, int_d);
    print_test("Puntero dinamico int, enlistado: ", ok);
    ok &= lista_insertar_ultimo(lista, float_d);
    print_test("Puntero dinamico float, enlistado: ", ok);
    ok &= lista_insertar_ultimo(lista, char_d);
    print_test("Puntero dinamico char, enlistado: ", ok);

    lista_destruir(lista, free);
    print_test("La lista fue destruida junto con los punteros dinamicos:", true);
    printf("---------FIN DE PRUEBA ELIMINAR LISTA CON PUNTEROS DINAMICOS-------\n");

}
void pruebas_lista_estudiante() {
    prueba_lista_vacia();
    prueba_lista_con_iter();
    prueba_lista_ya_definida();
    prueba_lista_volumen();
    prueba_iterador_interno();
    prueba_listas_con_cola();
    prueba_listas_con_pila();
    prueba_lista_con_puntero_dinamico();
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_lista_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
