#define  _POSIX_C_SOURCE 200809L
#include "strutil.h"
#include "calc_helper.h"
#include "cola.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void destruir_datos(struct calc_token* token, pila_t* pila, cola_t* cola, void(*destruir)(void*)){
    pila_destruir(pila);
    cola_destruir(cola, destruir);
    if(token) free(token);
}

void escribir_en_salida(struct calc_token* token){
    if(token->type == TOK_NUM){
        printf("%ld ", token->value);
        return;
    }

    if(token->oper.op == OP_ADD){
        printf("+ ");
    }else if(token->oper.op == OP_SUB){
        printf("- ");
    }else if(token->oper.op == OP_MUL){
        printf("* ");
    }else if(token->oper.op == OP_DIV){
        printf("/ ");
    }else if(token->oper.op == OP_POW){
        printf("^ ");
    }

}

void vaciar_cola(cola_t* cola){
    
    while(!cola_esta_vacia(cola)){
        struct calc_token* token_actual = cola_desencolar(cola);
        escribir_en_salida(token_actual);
        free(token_actual);
    }
    printf("\n");
}

void vaciar_pila(pila_t* pila, cola_t* cola){

    while(!pila_esta_vacia(pila)){
        cola_encolar(cola, pila_desapilar(pila));
    }
}

bool trabajar_pila_parentesis(pila_t* pila, cola_t* cola){
    struct calc_token* token_tope = pila_ver_tope(pila);
    while(token_tope->type != TOK_LPAREN){
        if(!cola_encolar(cola, token_tope)) return false;
        pila_desapilar(pila);
        token_tope = pila_ver_tope(pila);
    }
    free(pila_desapilar(pila)); //Saco el paretensis izq y lo libero
    return true;
}

bool trabajar_pila_operadores(pila_t* pila, cola_t* cola, struct calc_token* token){
    if(pila_esta_vacia(pila)){
        if(!pila_apilar(pila, token)) return false;
        return true;
    }

    struct calc_token* token_tope = pila_ver_tope(pila);
    while(token_tope && token_tope->type != TOK_LPAREN && token_tope->oper.precedencia >= token->oper.precedencia && token->oper.asociatividad == ASSOC_IZQ){
        if(!cola_encolar(cola, token_tope)) return false;
        pila_desapilar(pila);
        token_tope = pila_ver_tope(pila);
    }

    if(!pila_apilar(pila, token)) return false;
    return true;
}

void leer_entrada(char** linea){
    pila_t* pila = pila_crear();
    cola_t* cola = cola_crear();
    for(size_t i = 0; linea[i] != NULL; i++){
        struct calc_token* token = malloc(sizeof(struct calc_token));
        if(!calc_parse(linea[i], token)){
            printf("ERROR\n");
            destruir_datos(token, pila, cola, free);
            return;
        }else if(token->type == TOK_NUM){
            if(!cola_encolar(cola, token)) destruir_datos(token, pila, cola, free);
        }else if(token->type == TOK_OPER){
            if(!trabajar_pila_operadores(pila, cola, token)) destruir_datos(token, pila, cola, free);
        }else if(token->type == TOK_LPAREN){
            if(!pila_apilar(pila, token)) destruir_datos(token, pila, cola, free);
        }else if(token->type == TOK_RPAREN){
            if(!trabajar_pila_parentesis(pila, cola)) destruir_datos(token, pila, cola, free);
            free(token); //Libero el paretensis derecho
        }
    }
    if(!pila_esta_vacia(pila)){
        vaciar_pila(pila, cola);
    }
    vaciar_cola(cola);
    destruir_datos(NULL, pila, cola, NULL);
}

int main(void){
    char** vector_cadenas = NULL;
    char* linea = NULL;
    size_t tam = 0;

    while(getline(&linea, &tam, stdin) != EOF){
        vector_cadenas = infix_split(linea);
        leer_entrada(vector_cadenas);
        free_strv(vector_cadenas);
    }

    free(linea);
    return 0;
}