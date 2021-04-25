#include "abb.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void prueba_abb_vacio(){
    printf("PRUEBAS ABB VACIO\n");
    abb_t *abb = abb_crear(NULL, NULL);

    print_test("Prueba abb crear abb vacio", abb);
    print_test("Prueba abb cantidad es 0", abb_cantidad(abb) == 0);
    print_test("Prueba abb borrar F es NULL", !abb_borrar(abb, "F"));
    print_test("Prueba abb obtener F es NULL", !abb_obtener(abb, "F"));
    print_test("Prueba abb pertenece F es false", !abb_pertenece(abb, "F"));
    abb_destruir(abb);

}

static void pruebas_iter_abb_vacio(){
    printf("\nPRUEBAS ITER ABB VACIO\n");
    abb_t *abb = abb_crear(NULL, NULL);
    abb_iter_t *iter = abb_iter_in_crear(abb);

    print_test("Prueba abb iter crear con abb vacio", iter);
    print_test("Prueba abb iter esta al final", abb_iter_in_al_final(iter));
    print_test("Prueba abb iter avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iter ver actual es NULL", !abb_iter_in_ver_actual(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

static void pruebas_abb_guardar(){
    printf("\nPRUEBAS ABB GUARDAR\n");
    abb_t *abb = abb_crear(strcmp, NULL);

    char *clave1 = "V", *valor1 = "1";
    char *clave2 = "B", *valor2 = "2";
    char *clave3 = "Y", *valor3 = "3";

    print_test("Prueba abb guardar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb cantidad es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb pertenece clave1 es true", abb_pertenece(abb, clave1));
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb pertenece clave1 sigue siendo true", abb_pertenece(abb, clave1));
    print_test("Prueba abb guardar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb cantidad es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb pertenece clave 2 es true", abb_pertenece(abb, clave2));
    print_test("Prueba abb obtener clave2 es valor 2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb pertenece clave 2 sigue siendo true", abb_pertenece(abb, clave2));
    print_test("Prueba abb guardar clave3", abb_guardar(abb, clave3, valor3));
    print_test("Prueba abb cantidad es 3", abb_cantidad(abb) == 3);
    print_test("Prueba abb pertenece clave3 es true", abb_pertenece(abb, clave3));
    print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba abb pertenece clave3 sigue siendo true", abb_pertenece(abb, clave3));

    abb_destruir(abb);
}

static void pruebas_abb_reemplazar(){
    printf("\nPRUEBAS ABB GUARDAR EN CLAVE YA EXISTENTE\n");
    abb_t *abb = abb_crear(strcmp, NULL);

    char *clave1 = "V", *valor1a = "1a", *valor1b = "1b";
    char *clave2 = "B", *valor2a = "2a", *valor2b = "2b";

    print_test("Prueba abb guardar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb cantidad es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb guardar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb cantidad es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb guardar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb cantidad es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb guardar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb cantidad es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    abb_destruir(abb);   
}

static void pruebas_abb_reemplazar_con_destruir(){
    printf("\nPRUEBAS ABB REEMPLAZAR CON VALORES EN MEMORIA DINAMICA\n");
    abb_t *abb = abb_crear(strcmp, free);

    char *clave1 = "V", *valor1a, *valor1b;
    char *clave2 = "B", *valor2a, *valor2b;

    valor1a = malloc(sizeof(char));
    valor1b = malloc(sizeof(char));
    valor2a = malloc(sizeof(char));
    valor2b = malloc(sizeof(char));  

    print_test("Prueba abb guardar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb cantidad es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb guardar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb cantidad es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb guardar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb cantidad es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb guardar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb cantidad es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    abb_destruir(abb);
}

static void  pruebas_abb_borrar(){
    printf("\nPRUEBAS ABB BORRAR\n");
    abb_t *abb = abb_crear(strcmp, NULL);

    char *clave1 = "V", *valor1 = "1";
    char *clave2 = "B", *valor2 = "2";
    char *clave3 = "Y", *valor3 = "3";

    print_test("Prueba abb guardar clave1",abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb guardar clave1",abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb guardar clave3",abb_guardar(abb, clave3, valor3));
    print_test("Prueba abb pertenece clave1 es true",abb_pertenece(abb, clave1));
    print_test("Prueba abb borrar clave1 es valor3",abb_borrar(abb, clave1) == valor1);
    print_test("Prueba abb cantidad es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb borrar clave1 es NULL", !abb_borrar(abb, clave1));
    print_test("Prueba abb pertenece clave1 es false", !abb_pertenece(abb, clave1));
    print_test("Prueba abb pertenece clave2 es true",abb_pertenece(abb, clave2));
    print_test("Prueba abb borrar clave2 es valor2",abb_borrar(abb, clave2) == valor2);
    print_test("Prueba abb cantidad es 1", abb_cantidad(abb ) == 1);
    print_test("Prueba abb borrar clave2 es NULL", !abb_borrar(abb, clave2));
    print_test("Prueba abb pertenece clave2 es false", !abb_pertenece(abb, clave2));
    print_test("Prueba abb borrar clave3 es valor1", abb_borrar(abb, clave3) == valor3);
    print_test("Prueba abb cantidad es 0",abb_cantidad(abb) == 0);
    print_test("Prueba abb borrar clave3 es NULL", !abb_borrar(abb, clave3));
    print_test("Prueba abb pertenece clave3 es false", !abb_pertenece(abb, clave3));
    abb_destruir(abb);
}

static void pruebas_abb_clave_vacia(){
    printf("\nPRUEBAS GUARDAR CLAVE VACIA\n");
    abb_t *abb = abb_crear(strcmp, NULL);

    char *clave = "", *valor = "";

    print_test("Prueba abb guardar clave vacia", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad es 1",abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor", abb_obtener(abb, clave));
    print_test("Prueba abb pertenece clave vacia es true",abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia es valor",abb_borrar(abb, clave) == valor);
    print_test("Prueba abb cantidad es 0",abb_cantidad(abb) == 0);
    abb_destruir(abb);
}

static void pruebas_abb_valor_null(){
    printf("\nPRUEBAS ABB GUARDAR CON VALOR NULL\n");
    abb_t *abb = abb_crear(strcmp, NULL);

    char *clave = "A", *valor = NULL;

    print_test("Prueba abb guardar clave con valor NULL", abb_guardar(abb, clave, valor));
    print_test("Prueba abb cantidad es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave es NULL",abb_obtener(abb, clave) == valor);
    print_test("Prueba abb borrar clave devuelve valor NULL",abb_borrar(abb, clave) == valor);
    print_test("Prueba abb cantidad es 0",abb_cantidad(abb)  == 0);
    abb_destruir(abb);
}

static void pruebas_iter_algunos_elementos(){

    printf("\nPRUEBA ITERADOR EXTERNO CON ALGUNOS ELEMENTOS\n");
    abb_t *abb = abb_crear(strcmp, free);
    char* claves[] = {"M","J","F","D","Z","G",NULL};
    char* claves_ordenadas[] = {"D","F","G","J","M","Z",NULL};
    size_t *valores[6];
    for(size_t i = 0; claves[i] != NULL; i++){
        valores[i] = malloc(sizeof(size_t));
        *valores[i] = i;
        abb_guardar(abb, claves[i], valores[i]);
    }

    abb_iter_t *iter = abb_iter_in_crear(abb);

    print_test("Prueba abb iter crear", iter);
    print_test("Prueba iter no esta al final", !abb_iter_in_al_final(iter));
    bool ok = true;
    size_t i = 0;
    while(!abb_iter_in_al_final(iter)){
        ok = (*abb_iter_in_ver_actual(iter) == *claves_ordenadas[i]);
        abb_iter_in_avanzar(iter);
        i++;
    }
    print_test("Prueba abb iter itera en el orden esperado", ok);
    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

/*Esta funcion verifica el orden en que se iteran las claves, en caso de que el orden sea 
incorrecto, devuelve false y corta la iteracion antes de recorrerlo por completo*/
bool verificar_orden(const char* clave, void* dato, void* extra){
    int* contador = extra;
    *contador +=1;
    return true;
}

static void pruebas_iter_interno(){
    printf("\nPRUEBAS ITERADOR INTERNO\n");
    int tam = 26;
    abb_t* abb = abb_crear(strcmp, NULL);
    int vector_abc[26] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};
    char* unsorted_abc[] = {"M","J","U","F","D","W","Z","G","T","S","R","A","B","V","C","P","Q","X","Y","E","N","O","H","I","L","K",NULL};
   
    bool ok = true;
    for(size_t i = 0; unsorted_abc[i] != NULL; i++){
        ok = abb_guardar(abb, unsorted_abc[i], &vector_abc[i]);
        if(!ok) break;
    }

    print_test("Se guardo el abecedario desordenado: ", ok);    
    int contador = 0;
    int* p_contador = &contador;
    abb_in_order(abb, verificar_orden, p_contador);
    print_test("Se verifico que itere en el orden correcto", contador == tam); 
    abb_destruir(abb);    
}


bool verificar_orden_corte(const char* clave, void* dato, void* extra){
    int* contador = extra;
    *contador +=1;
    if(*(int *)contador == 10) return false;
    return true;
}

static void pruebas_iter_interno_con_corte(){
    printf("\nPRUEBAS ITER INTERNO CON CORTE\n");
    abb_t *abb = abb_crear(strcmp, NULL);
    int vector_abc[26] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};
    char* unsorted_abc[] = {"M","J","U","F","D","W","Z","G","T","S","R","A","B","V","C","P","Q","X","Y","E","N","O","H","I","L","K",NULL};

    bool ok = true;
    for(size_t i = 0; unsorted_abc[i] != NULL; i++){
        ok = abb_guardar(abb, unsorted_abc[i], &vector_abc[i]);
        if(!ok) break;
    }

    print_test("Se guardo el abecedario desordenado: ", ok);
    int contador = 0;
    int *contador_p = &contador;
    abb_in_order(abb, verificar_orden_corte, contador_p);
    print_test("Se verifico el orden de las primeras 10 claves", contador == 10);
    abb_destruir(abb);

}

static void pruebas_abb_volumen(size_t largo){
    printf("\nPRUEBA ABB VOLUMEN\n");
    abb_t *abb = abb_crear(strcmp, NULL);

    const size_t largo_clave = 10;
    char **claves = malloc(sizeof(char*) * largo);

    unsigned* valores[largo];

    bool ok = true;
    for(unsigned i = 0; i < largo; i++){
        valores[i] = malloc(sizeof(int));
        claves[i] = malloc(sizeof(char)*largo_clave);
        sprintf(claves[i], "%08d", i);
        *valores[i] = i;
    }
    for(unsigned i = 0; i < largo; i++){

        long unsigned random = rand() % largo;
        char *aux = claves[i];
        claves[i] = claves[random];
        claves[random] = aux;       
    }
    for(size_t i = 0; i < largo; i++){
        ok = abb_guardar(abb, claves[i], valores[i]);
        if(!ok) break;
    }

    print_test("Prueba abb guardar muchos elementos", ok);
    print_test("Prueba abb cantidad es la correcta", abb_cantidad(abb) == largo);

    for(size_t i = 0; i < largo; i++){
        ok = abb_pertenece(abb, claves[i]);
        ok = abb_obtener(abb, claves[i]) == valores[i];
        if(!ok) break;
    }
    print_test("Prueba abb pertenece y obtener es el resultado esperado", ok);

    for(size_t i = 0; i < largo; i++){
        ok = abb_borrar(abb, claves[i]) == valores[i];
        
    }
    print_test("Prueba abb borrar devuelve el valor correcto", ok);
    print_test("Prueba abb cantidad es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
    abb = abb_crear(strcmp, free);

    for(size_t i = 0; i < largo; i++){
        abb_guardar(abb, claves[i], valores[i]);
    }
    abb_destruir(abb);
    for(size_t i = 0; i < largo; i++){
        free(claves[i]);
    }
    free(claves);
}

static void pruebas_iterar_volumen(size_t largo){
    printf("\nPRUEBA ABB ITERAR EN VOLUMEN\n");
    abb_t *abb = abb_crear(strcmp, NULL);

    const size_t largo_clave = 10;
    char **claves = malloc(sizeof(char*) * largo);

    unsigned* valores[largo];

    bool ok = true;
    for(unsigned i = 0; i < largo; i++){
        valores[i] = malloc(sizeof(int));
        claves[i] = malloc(sizeof(char)*largo_clave);
        sprintf(claves[i], "%08d", i);
        *valores[i] = i;
    }
    for(unsigned i = 0; i < largo; i++){

        long unsigned random = rand() % largo;
        char *aux = claves[i];
        claves[i] = claves[random];
        claves[random] = aux;       
    }
    for(size_t i = 0; i < largo; i++){
        ok = abb_guardar(abb, claves[i], valores[i]);
        if(!ok) break;
    }

    abb_iter_t *iter = abb_iter_in_crear(abb);
    print_test("Prueba abb iter esta al final es false", !abb_iter_in_al_final(iter));

    ok = true;
    for(size_t i = 0; i < largo; i++){
        if(!abb_iter_in_avanzar(iter)){
            ok = false;
            break;
        }
        
    }
    print_test("Prueba abb iter recorrio todo el largo", ok);
    print_test("Prueba abb iter esta al final", abb_iter_in_al_final(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);

    abb = abb_crear(strcmp, free);

    for(size_t i = 0; i < largo; i++){
        abb_guardar(abb, claves[i], valores[i]);
    }
    abb_destruir(abb);
    for(size_t i = 0; i < largo; i++){
        free(claves[i]);
    }
    free(claves);
}


bool contar_nodos(const char* clave, void* dato, void* extra){
    int* extra_d = extra;
    *extra_d += 1;
    return true;
}

static void pruebas_contador_nodos(){
    printf("\nPREBAS ITERADOR INTERNO:CONTAR NODOS\n");
    abb_t *abb = abb_crear(strcmp, NULL);
    size_t tam = 26;
    int vector_abc[26] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};
    char* unsorted_abc[] = {"M","J","U","F","D","W","Z","G","T","S","R","A","B","V","C","P","Q","X","Y","E","N","O","H","I","L","K",NULL};

    bool ok = true;
    for(size_t i = 0; unsorted_abc[i] != NULL; i++){
        ok = abb_guardar(abb, unsorted_abc[i], &vector_abc[i]);
        if(!ok) break;
    }

    print_test("Se insertaron elementos en el arbol correctamente: ", ok);
    int contador = 0;
    int* p_contador = &contador;
    abb_in_order(abb, contar_nodos, p_contador);
    print_test("Los nodos fueron chequeados correctamente: ", contador == tam);
    abb_destruir(abb);
}

void pruebas_abb_estudiante(){
    prueba_abb_vacio();
    pruebas_iter_abb_vacio();
    pruebas_abb_guardar();
    pruebas_abb_reemplazar();
    pruebas_abb_reemplazar_con_destruir();
    pruebas_abb_borrar();
    pruebas_abb_clave_vacia();
    pruebas_abb_valor_null();
    pruebas_abb_volumen(5000);
    pruebas_iter_algunos_elementos();
    pruebas_iterar_volumen(5000);
    pruebas_iter_interno();
    pruebas_iter_interno_con_corte();
    pruebas_contador_nodos();
}
#ifndef CORRECTOR

int main(void){
    pruebas_abb_estudiante();
    return failure_count() > 0;
}

#endif