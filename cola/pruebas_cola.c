#include "cola.h"
#include "pila.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>

static void prueba_cola_vacia(void) {
    cola_t* cola = cola_crear();
    printf("---------PRUEBA CON COLA ESTADO INICIAL---------\n");

    print_test("Prueba cola vacia es true:", cola_esta_vacia(cola) == true);

    print_test("Prueba de ver el inicio de la cola es NULL:", cola_ver_primero(cola) == NULL);
    
    print_test("Verificar que desencolar una cola vacia no se puede: ", cola_desencolar(cola) == NULL);
    
    print_test("Verificar que se puede encolar el elemento NULL", cola_encolar(cola, NULL) == true);

    print_test("Verificar que al desapilar se encolar el elemento NULL", cola_desencolar(cola) == NULL);
    
    print_test("Verifico que la cola este vacia nuevamente: ", cola_esta_vacia(cola) == true);

    printf("---------FIN DE LAS PRUEBAS CON LA COLA EN ESTADO INICIAL---------\n");
    
    printf("---------PRUEBA DE ELIMINAR UNA COLA ENCOLADA---------\n");
    
    int numero_auxiliar = 45;
    int* puntero_auxiliar = &numero_auxiliar;

    print_test("Encolo un elemento de prueba(puntero int)", cola_encolar(cola, puntero_auxiliar) == true);
    print_test("Verifico que el primer elemento encolado sea el puntero: ", cola_ver_primero(cola) == puntero_auxiliar);
    print_test("Verifico el tope de la pila:", cola_ver_primero(cola) == puntero_auxiliar);

    cola_destruir(cola, NULL);
    print_test("La cola fue destruida",true);

    printf("---------FIN DE LA PRUEBA DE ELIMINAR UNA COLA ENCOLADA---------\n");
}

static void prueba_punteros_distintos(){
    cola_t* cola = cola_crear();
    size_t tam = 10000;
    printf("---------PRUEBA DE VOLUMEN CON PUNTEROS ESTATICOS(10.000 DE CADA UNO)---------\n");
    int vector_int[tam];
    char vector_char[tam];
    float vector_float[tam];
    for(size_t i = 0; i < tam; i++){
        vector_int[i] = (int)i;
        vector_char[i] = (char)(97 + i);
        vector_float[i] = (float)i;
    }

    bool ok = true;
    bool ok_2 = true; //Chequea el invariante de la cola

    printf("---------COMIENZO A ENCOLAR LOS PUNTEROS---------");
    for(size_t i = 0; i < tam; i++){
        ok &= cola_encolar(cola, &vector_int[i]);
    }

    print_test("El vector int fue encolado completamente: ", ok);
    print_test("El primer elemento de la cola coincide con el primer elemento del vector int", cola_ver_primero(cola) == &vector_int[0]);
    for(size_t i = 0; i < tam; i++){
        ok &= cola_encolar(cola, &vector_char[i]);
    }
    print_test("El vector char fue encolado completamente: ", ok);
    print_test("El primer elemento de la cola coincide con el primer elemento del vector int", cola_ver_primero(cola) == &vector_int[0]);

    for(size_t i = 0; i < tam; i++){
        ok &= cola_encolar(cola, &vector_float[i]);
    }

    print_test("El vector float fue encolado completamente: ", ok);
    print_test("El primer elemento de la cola coincide con el primer elemento del vector int", cola_ver_primero(cola) == &vector_int[0]);
    printf("---------TODOS LOS ELEMENTOS FUERON ENCOLADOS---------\n"); 

    printf("---------COMIENZO A DESENCOLAR ELEMENTOS:---------\n");

    for(size_t i = 0; i < tam; i++){
        ok &= cola_desencolar(cola) == &vector_int[i];
        if(i+1 != tam){
            ok_2 &= cola_ver_primero(cola) == &vector_int[i+1];
        }
    }
    print_test("Los primeros 10.000 elementos coinciden con los del vector_int", ok);
    print_test("Al desencolar se mantuvo el invariante: ", ok_2);
    print_test("Verifico que el primer elemento no sea NULL(siguen quedando punteros)", cola_ver_primero(cola) != NULL);
    for(size_t i = 0; i < tam; i++){
        ok &= cola_desencolar(cola) == &vector_char[i];
        if(i+1 != tam){
            ok_2 &= cola_ver_primero(cola) == &vector_char[i+1];
        }
    }
    print_test("Los primeros 10.000 elementos coinciden con los del vector_char", ok);
    print_test("Al desencolar se mantuvo el invariante: ", ok_2);
    print_test("Verifico que el primer elemento no sea NULL(siguen quedando punteros)", cola_ver_primero(cola) != NULL);
    for(size_t i = 0; i < tam; i++){
        ok &= cola_desencolar(cola) == &vector_float[i];
        if(i+1 != tam){
            ok_2 &= cola_ver_primero(cola) == &vector_float[i+1];
        }
    }
    print_test("Los primeros 10.000 elementos coinciden con los del vector_float", ok);
    print_test("Al desencolar se mantuvo el invariante: ", ok_2);
    print_test("Ahora la cola DEBE estar vacia: ", cola_ver_primero(cola) == NULL);
    
    printf("---------TODOS LOS ELEMENTOS FUERON DESENCOLADOS---------\n");
    cola_destruir(cola, NULL);
    print_test("La cola fue destruida", true);
}

void destruir_dato_de_pila(void* pila){
    pila_destruir(pila);
}

static void prueba_encolar_pila(){
    cola_t* cola = cola_crear();
    pila_t* pila_vacia = pila_crear();
    pila_t* pila_con_elementos = pila_crear();
    
    bool ok = true;
    size_t tam = 10000;
    int vector[tam];
    
    printf("---------COMIENZO PRUEBAS PARA ENCOLAR PILAS---------\n");
    ok &= cola_encolar(cola, pila_vacia);
    print_test("Prueba de encolar una pila vacia: ", ok);
    
    for(size_t i = 0; i < tam; i++){
        vector[i] = (int)i;
        ok &= pila_apilar(pila_con_elementos, &vector[i]);
    }
    ok &= cola_encolar(cola, pila_con_elementos);
    print_test("Prueba de encolar una pila con elementos: ", ok);
    print_test("Verifico que el primer elemento coincida con la pila vacia: ", cola_ver_primero(cola) == pila_vacia);
    cola_destruir(cola, destruir_dato_de_pila);
    print_test("Verifico que puedo destruir la pila con una funcion auxiliar(pila_destruir): ",true);
    printf("---------FIN DE LA PRUEBA DE COLA CON PILAS---------\n");
}

static void prueba_encolar_puntero_dinamico(){
    cola_t* cola = cola_crear();
    size_t tam = 10;
    bool ok = true;
    void** puntero_generico = malloc(sizeof(void*)*tam);
    int** puntero_dinamico_int = malloc(sizeof(int*)*tam);
    char** puntero_dinamico_char = malloc(sizeof(char*)*tam);

    printf("---------COMIENZO PRUEBAS PARA ENCOLAR DATOS DINAMICOS---------\n");
    ok &= cola_encolar(cola, puntero_generico);
    print_test("Encolar un vector void** dinamico: ", ok);

    ok &= cola_encolar(cola, puntero_dinamico_int);
    print_test("Encolar un vector int** dinamico:", ok );

    ok &= cola_encolar(cola, puntero_dinamico_char);
    print_test("Encolar un vector char** dinamico: ", ok);

    printf("Verifico que la cola se destruya con funcion auxiliar free()\n");
    cola_destruir(cola, free);
    print_test("La cola fue destruida", true);
    printf("---------FIN DE LA PRUEBA DE DATOS DINAMICOS---------");
}

void pruebas_cola_estudiante() {
    prueba_cola_vacia();
    prueba_punteros_distintos();
    prueba_encolar_puntero_dinamico();
    prueba_encolar_pila();
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_cola_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
