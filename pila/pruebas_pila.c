#include "pila.h"
#include "testing.h"
#include <stdio.h>


static void prueba_pila_vacia(void) {
    pila_t *pila = pila_crear();

    print_test("Prueba pila vacia es true:", pila_esta_vacia(pila) == true);

    print_test("Prueba de ver el tope con la pila vacia:", pila_ver_tope(pila) == NULL);
    
    print_test("Desapilar una pila vacia devuelve null", pila_desapilar(pila) == NULL);

    print_test("Verificar que se puede apilar el elemento NULL", pila_apilar(pila, NULL) == true);

    print_test("Verificar que al desapilar se desapile el elemento NULL", pila_desapilar(pila) == NULL);

    int numero_auxiliar = 10;
    int *puntero_auxiliar = &numero_auxiliar;
    
    print_test("Apilo un valor de prueba(Puntero a int):", pila_apilar(pila, puntero_auxiliar) == true);

    print_test("Desapilo el valor anterior:", pila_desapilar(pila) == puntero_auxiliar);

    print_test("Verifico tope al vaciar de nuevo la pila:", pila_ver_tope(pila) == NULL);

    print_test("Desapilo la pila vacia nuevamente: ", pila_desapilar(pila) == NULL);


    pila_destruir(pila);
    print_test("La pila fue destruida", true);
}

static void prueba_volumen_pila_int(){
    pila_t* pila = pila_crear();
    
    size_t tam = 10000;

    /*Prueba con punteros int */
    int vector_prueba[tam];
    size_t i;
    /*Lleno el vector de valores int*/
    for(i = 0; i < tam; i++){
        vector_prueba[i] = (int)i;
    }

    bool ok = true;
    /*Apilo elemento por elemento en la pila. */
    for(i = 0; i < tam; i++){
        ok &= pila_apilar(pila, &vector_prueba[i]);
        ok &= pila_ver_tope(pila) == &vector_prueba[i];
    }
    print_test("Se apilaron 10000 elementos correctamente(punteros a int):", ok);

    /*Desapilo elemento por elemento hasta vaciar la pila*/
    for(i = 0; i < tam; i++){
        ok &= pila_desapilar(pila) == &vector_prueba[tam-i-1];
        if(tam-i-1 != 0){
            ok &= pila_ver_tope(pila) == &vector_prueba[tam-i-2]; //tam-i-2 deberia poder ver el tope actual
        }
    }

    /* Verifico que la pila se comporte como recien creada */
    print_test("Se desapilaron los 10000 elementos correctamente:", ok);
    ok &= pila_ver_tope(pila) == NULL;
    ok &= pila_desapilar(pila) == NULL;
    print_test("Verificacion que funcione como recien creada: ", ok);
    pila_destruir(pila);
    print_test("La pila fue destruida", true);
}

static void prueba_punteros_distintos(){
    pila_t* pila = pila_crear();

    bool ok  = true;
    size_t i;
    size_t tam = 10000;

    char vector_char_prueba[tam];
    int vector_int_prueba[tam];
    float vector_float_prueba[tam];

    for(i = 0; i < tam; i++){
        vector_char_prueba[i] = (char)(97+i);
        vector_int_prueba[i] = (int)i;
        vector_float_prueba[i] = (float)i;
    }
    /* APILO 3 TIPOS DISTINTOS DE PUNTEROS EN ORDEN CHAR -> INT -> FLOAT */
    for(i = 0; i < tam; i++){
        ok &= pila_apilar(pila, &vector_char_prueba[i]);
        ok &= pila_ver_tope(pila) == &vector_char_prueba[i];
    }
    print_test("Se apilaron los punteros char correctamente", ok);

    for(i = 0; i < tam; i++){
        ok &= pila_apilar(pila, &vector_int_prueba[i]);
        ok &= pila_ver_tope(pila) == &vector_int_prueba[i];
    }
    print_test("Se apilaron los punteros int correctamente", ok);

    for(i = 0; i < tam; i++){
        ok &= pila_apilar(pila, &vector_float_prueba[i]);
        ok &= pila_ver_tope(pila) == &vector_float_prueba[i];
    }
    print_test("Se apilaron los punteros float correctamente", ok);

    /*DESAPILO LOS PUNTEROS RECIEN AGREGADOS, EN EL ORDEN FLOAT -> INT -> CHAR*/

    for(i = 0; i < tam; i++){
        ok &= pila_desapilar(pila) == &vector_float_prueba[tam-i-1];
        if(tam-i-1 != 0){
            ok &= pila_ver_tope(pila) == &vector_float_prueba[tam-i-2]; //tam-i-2 deberia poder ver el tope actual
        }
    }

    print_test("Se desapilaron los punteros float correctamente", ok);

    for(i = 0; i < tam; i++){
        ok &= pila_desapilar(pila) == &vector_int_prueba[tam-i-1];
        if(tam-i-1 != 0){
            ok &= pila_ver_tope(pila) == &vector_int_prueba[tam-i-2];
        }
    }

    print_test("Se desapilaron los punteros int correctamente", ok);


    for(i = 0; i < tam; i++){
        ok &= pila_desapilar(pila) == &vector_char_prueba[tam-i-1];
        if(tam-i-1 != 0){
            ok &= pila_ver_tope(pila) == &vector_char_prueba[tam-i-2]; 
        }
    }

    print_test("Se desapilaron los punteros chat correctamente", ok);

    ok &= pila_ver_tope(pila) == NULL;
    ok &= pila_desapilar(pila) == NULL;
    print_test("Verificacion que funcione como recien creada: ", ok);
    pila_destruir(pila);
    print_test("La pila fue destruida", true);

}
void pruebas_pila_estudiante() {
    prueba_pila_vacia();
    prueba_volumen_pila_int();
    prueba_punteros_distintos();
    // ...
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_pila_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
