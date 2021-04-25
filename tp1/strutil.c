#include "strutil.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

char *substr(const char *str, size_t n){
    char* str_subs = calloc(n+1,sizeof(char)); 
    strncpy(str_subs, str, n);
    return str_subs;
}

char **split(const char *str, char sep){
    char sep_s[2];
    sep_s[0] = sep, sep_s[1] = '\0';
    
    size_t separaciones = 1; //Garantizo un espacio minimo
    size_t indice = 0, indice_aux = 0; // Utilizados para aritmetica de punteros


    for(size_t i = 0; i < strlen(str); i++){
        if(str[i] == sep) separaciones++;
    }


    char** vector = malloc(sizeof(char*)*(separaciones+1));
    if(!vector) return NULL;
    
    for(size_t i = 0; i < separaciones; i++){
        indice = strcspn(str + indice_aux, sep_s);
        vector[i] = substr(str + indice_aux, indice);
        indice_aux += indice;
        indice_aux++; //Evito los separadores 
    }

    vector[separaciones] = NULL;
    return vector;
}

char *join(char **strv, char sep){
    char sep_s[2];
    sep_s[0] = sep, sep_s[1] = '\0';
    size_t len = 0;
    
    for(size_t i = 0; strv[i] != NULL; i++){
        len += strlen(strv[i]);
        len++; //Siempre uno mas para el separador
    }

    len++; //Uno mas para el NULL

    char* nueva_palabra = calloc(len, sizeof(char));
    if(!nueva_palabra) return NULL;

    char* puntero_auxiliar = nueva_palabra; //Evito problemas de punteros
    
    while(*strv){
        strcpy(puntero_auxiliar, *strv);
        puntero_auxiliar += strlen(*strv);
        strv++;
        if(*strv){
            strcpy(puntero_auxiliar, sep_s);
            puntero_auxiliar += strlen(sep_s);
        }
    }

    return nueva_palabra;
}


void free_strv(char *strv[]){
    if(!strv) return;
    for(size_t i = 0; strv[i] != NULL; i++){
        free(strv[i]);
    }
    free(strv);
}