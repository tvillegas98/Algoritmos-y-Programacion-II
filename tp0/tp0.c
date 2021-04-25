#include "tp0.h"

/* *****************************************************************
 *                     FUNCIONES A COMPLETAR                       *
 *         (ver en tp0.h la documentación de cada función)         *
 * *****************************************************************/

void swap(int *x, int *y) {
    int aux = *x;
    *x = *y;
    *y = aux;
}

int maximo(int vector[], int n) {
    if(n == 0) return -1;
    int pos_max = vector[0];
    int i_max = 0;
    for(int i = 0; i < n; i++){
        if(vector[i] > pos_max){
            pos_max = vector[i];
            i_max = i;
        }
    }
    return i_max;
}


int comparar(int vector1[], int n1, int vector2[], int n2){
    
    for(int i = 0; i < n1 && i < n2; i++){
        if(vector1[i] > vector2[i]) return 1;
        if(vector1[i] < vector2[i]) return -1;
    }
    return n1 > n2 ? 1 : (n2 > n1 ? -1:0);
}

void seleccion(int vector[], int n){
    int max_pos;
    for(int i = 0; i < n; i++){
        max_pos = maximo(vector, n-i);
        swap(&vector[n-1-i], &vector[max_pos]);
    }
}


