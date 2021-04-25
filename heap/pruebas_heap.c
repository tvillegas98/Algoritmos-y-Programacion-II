#include "heap.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>

int comparador(const void *a, const void *b)
{
    int *int_a = (int *)a;
    int *int_b = (int *)b;
    if (!a)
        return 1;
    if (!b)
        return -1;
    return *int_a > *int_b ? 1 : (*int_a < *int_b ? -1 : 0);
}

int comparador_dos(const void *a, const void *b)
{
    int *int_a = (int *)a;
    int *int_b = (int *)b;
    if (!a)
        return -1;
    if (!b)
        return 1;
    return *int_a < *int_b ? 1 : (*int_a > *int_b ? -1 : 0);
}

static void pruebas_heap_vacio()
{
    printf("PRUEBAS HEAP VACIO\n");
    heap_t *heap = heap_crear(comparador);
    print_test("Prueba heap crear heap vacio", heap);
    print_test("Prueba heap esta vacio", heap_esta_vacio(heap));
    print_test("Prueba heap cantidad es 0", heap_cantidad(heap) == 0);
    print_test("Prueba heap ver maximo es NULL", !heap_ver_max(heap));
    print_test("Prueba heap desencolar devuelve NULL", !heap_desencolar(heap));
    heap_destruir(heap, NULL);
    print_test("Se destruyo el heap", true);
}

static void pruebas_heap_encolar()
{
    printf("\nPRUEBAS HEAP ENCOLAR\n");
    int a = 1;
    int b = 5;
    int c = 3;

    heap_t *heap = heap_crear(comparador);
    print_test("Prueba heap encolar a = 1", heap_encolar(heap, &a));
    print_test("Prueba heap cantidad es 1", heap_cantidad(heap) == 1);
    print_test("Prueba heap ver maximo es a", heap_ver_max(heap) == &a);
    print_test("Prueba heap encolar b = 5", heap_encolar(heap, &b));
    print_test("Prueba heap cantidad es 2", heap_cantidad(heap) == 2);
    print_test("Prueba heap ver maximo es b", heap_ver_max(heap) == &b);
    print_test("Prueba heap encolar c = 3", heap_encolar(heap, &c));
    print_test("Prueba heap cantidad es 3", heap_cantidad(heap) == 3);
    print_test("Prueba heap ver maximo es b", heap_ver_max(heap) == &b);
    heap_destruir(heap, NULL);
}

static void pruebas_heap_desencolar()
{
    printf("\nPRUEBAS HEAP DESENCOLAR\n");
    int a = 1;
    int b = 5;
    int c = 3;

    heap_t *heap = heap_crear(comparador);
    bool ok = true;
    ok = heap_encolar(heap, &a);
    ok = heap_encolar(heap, &b);
    ok = heap_encolar(heap, &c);

    print_test("Se encolaron 3 elementos", ok);
    print_test("Prueba heap cantidad es 3", heap_cantidad(heap) == 3);
    print_test("Prueba heap ver maximo es el correcto", heap_ver_max(heap) == &b);
    print_test("Prueba heap desencolar devuelve el maximo", heap_desencolar(heap) == &b);
    print_test("Prueba heap no esta vacio", !heap_esta_vacio(heap));
    print_test("Prueba heap cantidad es 2", heap_cantidad(heap) == 2);
    print_test("Prueba heap ver maximo es el correcto", heap_ver_max(heap) == &c);
    print_test("Prueba heap desencolar devuelve el maximo", heap_desencolar(heap) == &c);
    print_test("Prueba heap no esta vacio", !heap_esta_vacio(heap));
    print_test("Prueba heap cantidad es 1", heap_cantidad(heap) == 1);
    print_test("Prueba heap ver maximo es el correcto", heap_ver_max(heap) == &a);
    print_test("Prueba heap desencolar devuelve el maximo", heap_desencolar(heap) == &a);
    print_test("Prueba heap cantidad es 0", heap_cantidad(heap) == 0);
    print_test("Prueba heap esta vacio", heap_esta_vacio(heap));
    print_test("Prueba heap desencolar devuelve NULL", !heap_desencolar(heap));
    heap_destruir(heap, NULL);
}

static void pruebas_heap_encolar_NULL()
{
    printf("\nPRUEBAS HEAP ENCOLAR NULL\n");

    int a = 1;

    heap_t *heap = heap_crear(comparador);

    print_test("Prueba heap encolar elemento no nulo", heap_encolar(heap, &a));
    print_test("Prueba heap cantidad es 1", heap_cantidad(heap) == 1);
    print_test("Prueba heap no esta vacio", !heap_esta_vacio(heap));
    print_test("Prueba heap encolar NULL", heap_encolar(heap, NULL));
    print_test("Prueba heap cantidad es 2", heap_cantidad(heap) == 2);
    print_test("Prueba heap ver maximo es NULL", heap_ver_max(heap) == NULL);
    print_test("Prueba heap encolar NULL", heap_encolar(heap, NULL));
    print_test("Prueba heap cantidad es 3", heap_cantidad(heap) == 3);
    print_test("Prueba heap ver maximo es NULL", heap_ver_max(heap) == NULL);
    print_test("Prueba desencolar es NULL", heap_desencolar(heap) == NULL);
    print_test("Prueba desencolar es NULL", heap_desencolar(heap) == NULL);
    print_test("Prueba ver maximo es el elemento no nulo", heap_ver_max(heap) == &a);
    heap_destruir(heap, NULL);
    print_test("Se destruyo el heap", true);
}

static void pruebas_heap_destruir_con_free()
{
    printf("\nPRUEBAS HEAP DESTRUIR CON FUNCION DE DESTRUCCION\n");
    size_t tam = 10;
    void **arreglo = malloc(sizeof(void *) * tam);

    int *enteros[tam];
    for (int i = 0; i < tam; i++)
    {
        enteros[i] = malloc(sizeof(int));
        *enteros[i] = i;
        arreglo[i] = enteros[i];
    }

    heap_t *heap = heap_crear_arr(arreglo, tam, comparador);
    free(arreglo);
    print_test("Prueba heap crear arr se creo a partir de un arreglo correctamente", heap);
    print_test("Prueba heap destruir con free", true);
    bool ok = *(int *)heap_ver_max(heap) == *enteros[tam - 1];
    print_test("Se destruyo el arreglo original y heap todavia funciona", ok);
    heap_destruir(heap, free);
}

static void pruebas_heap_crear_arr()
{
    printf("\nPRUEBAS HEAP CREAR ARR\n");
    size_t tam_arreglo = 6;
    void *arr[tam_arreglo];
    int enteros[tam_arreglo];

    for (size_t i = 0; i < tam_arreglo; i++)
    {
        enteros[i] = (int)i;
    }
    for (size_t i = 0; i < tam_arreglo; i++)
    {
        arr[i] = &enteros[i];
    }

    heap_t *heap = heap_crear_arr(arr, tam_arreglo, comparador);

    print_test("Prueba heap crear arr se creo correctamente", heap);
    print_test("Prueba heap cantidad es igual al tamaño del arreglo", heap_cantidad(heap) == tam_arreglo);
    print_test("Prueba heap ver maximo es el correcto", *(int *)heap_ver_max(heap) == 5);

    bool ok = true;
    int *maximo = heap_ver_max(heap);
    for (size_t i = 0; i < tam_arreglo; i++)
    {
        ok = heap_desencolar(heap) == maximo;
        maximo = heap_ver_max(heap);
    }

    print_test("Prueba heap desencolar los elementos", ok);
    print_test("Prueba heap cantidad es 0", heap_cantidad(heap) == 0);
    print_test("Prueba heap esta vacio es true", heap_esta_vacio(heap));
    print_test("Prueba heap desencolar devuelve NULL", !heap_desencolar(heap));

    heap_destruir(heap, NULL);
}

static void pruebas_heap_prioridades_distintas()
{
    printf("\nPRUEBA DE HEAPS CON PRIORIDADES DISTINTAS\n");
    heap_t *heap_uno = heap_crear(comparador);
    heap_t *heap_dos = heap_crear(comparador_dos);
    size_t tam = 10;

    void **arr_void = malloc(sizeof(void *) * tam);
    int arr_int[tam];
    for (size_t i = 0; i < tam; i++)
    {
        arr_int[i] = (int)i;
        arr_void[i] = &arr_int[i];
    }

    bool ok = false;
    bool ok_i = false;
    bool ok_ii = false;

    for (size_t i = 0; i < tam; i++)
    {
        ok = heap_encolar(heap_uno, arr_void[i]);
        ok_i = heap_encolar(heap_dos, arr_void[i]);
    }
    print_test("Se encolaron correctamente elementos en ambos heap", ok && ok_i);

    ok = *(int *)heap_ver_max(heap_uno) == arr_int[tam - 1];
    ok_i = *(int *)heap_ver_max(heap_dos) == arr_int[0];
    ok_ii = *(int *)heap_ver_max(heap_uno) != *(int *)heap_ver_max(heap_dos);

    print_test("Los maximos de cada heap son correctos", ok);
    print_test("Los maximos de cada heap son distintos", ok_ii);

    for (size_t i = 0; i < tam; i++)
    {
        ok = *(int *)heap_desencolar(heap_uno) == arr_int[tam - i - 1];
        ok_i = *(int *)heap_desencolar(heap_dos) == arr_int[i];
    }

    print_test("Se desencolo en el orden correcto ambos heap", ok && ok_i);

    ok = heap_esta_vacio(heap_uno);
    ok_i = heap_esta_vacio(heap_dos);

    print_test("Ambos heaps estan vacios", ok && ok_i);

    heap_destruir(heap_uno, NULL);
    heap_destruir(heap_dos, NULL);
    free(arr_void);
    print_test("Se destruyeron correctamente ambos heaps", true);
}

static void pruebas_volumen()
{
    heap_t *heap = heap_crear(comparador);
    size_t tam = 10000;

    int *arr_int = malloc(sizeof(int) * tam);
    for (size_t i = 0; i < tam; i++)
    {
        arr_int[i] = (int)i;
    }

    bool ok = false;
    bool ok_i = false;
    for (size_t i = 0; i < tam; i++)
    {
        ok = heap_encolar(heap, &arr_int[i]);
        ok_i = *(int *)heap_ver_max(heap) == arr_int[i];
    }

    print_test("Se encolaron correctamente 10.000 elementos", ok && ok_i);

    for (size_t i = 0; i < tam; i++)
    {
        ok = *(int *)heap_desencolar(heap) == arr_int[tam - i - 1];
        if (tam - i - 1 != 0)
        {
            ok_i = *(int *)heap_ver_max(heap) == arr_int[tam - i - 2];
        }
    }

    print_test("Se desencolaron correctamente 10.000 elementos", ok && ok_i);

    print_test("El heap esta vacio", heap_esta_vacio(heap));

    heap_destruir(heap, NULL);
    free(arr_int);
    print_test("Se destruyo el heap correctamente", true);
}

static void pruebas_heap_sort()
{
    printf("\nPRUEBAS HEAPSORT\n");
    printf("SE ORDENARA EL AREGLO [5,4,2,1,5,7,3,5,9]\n");
    size_t tam = 9;
    int arr[] = {5, 4, 2, 1, 5, 7, 3, 5, 9};
    int sorted_arr_as[] = {1, 2, 3, 4, 5, 5, 5, 7, 9};
    void **arr_void = malloc(sizeof(void *) * tam);

    for (size_t i = 0; i < tam; i++)
    {
        arr_void[i] = &arr[i];
    }

    bool ok = false;

    heap_sort(arr_void, tam, comparador);

    for (size_t i = 0; i < tam; i++)
    {
        ok = *(int *)arr_void[i] == sorted_arr_as[i];
    }

    print_test("El arreglo fue ordenado de manera ascendente", ok);

    heap_sort(arr_void, tam, comparador_dos);

    for (size_t i = 0; i < tam; i++)
    {
        ok = *(int *)arr_void[i] == sorted_arr_as[tam - 1 - i];
    }
    print_test("El arreglo fue ordenado de forma descendente", ok);
    free(arr_void);
}

void pruebas_heap_estudiante()
{
    pruebas_heap_vacio();
    pruebas_heap_encolar();
    pruebas_heap_desencolar();
    pruebas_heap_encolar_NULL();
    pruebas_heap_destruir_con_free();
    pruebas_heap_crear_arr();
    pruebas_heap_prioridades_distintas();
    pruebas_volumen();
    pruebas_heap_sort();
}

#ifndef CORRECTOR

int main(void)
{
    pruebas_heap_estudiante();
    return failure_count() > 0;
}

#endif
