#define  _POSIX_C_SOURCE 200809L
#include "strutil.h"
#include "calc_helper.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define OPERANDO_DER 0 //Indice operando derecho
#define OPERANDO_IZQ 1 //Indice operando izq
#define OPERANDO_TER 2 //Indice operando ternario
#define BASE_MIN 2 // Base minima del logaritmo
#define ARG 1 //Indice del argumento del logaritmo
#define BASE 0 //Indice base del logaritmo
#define NULO 0 //Para errores por casos menores o iguales a 0

bool aplicar_operacion(calc_num vector[], struct calc_token token, calc_num* resultado){
    if(token.oper.op == OP_ADD){
        *resultado = vector[OPERANDO_IZQ] + vector[OPERANDO_DER];
    }else if(token.oper.op == OP_SUB) {
        *resultado = vector[OPERANDO_IZQ] - vector[OPERANDO_DER];
    }else if(token.oper.op == OP_DIV){
        if(vector[OPERANDO_DER] == NULO){
            fprintf(stdout, "ERROR\n"); 
            return false;
        }
        *resultado = vector[OPERANDO_IZQ] / vector[OPERANDO_DER];
    }else if(token.oper.op == OP_MUL) {
        *resultado =  vector[OPERANDO_IZQ] * vector[OPERANDO_DER];
    }else if(token.oper.op == OP_POW){
        if(vector[OPERANDO_DER] < NULO || (vector[OPERANDO_IZQ] == NULO &&  vector[OPERANDO_DER] == NULO)){
            fprintf(stdout, "ERROR\n"); 
            return false;
        }
        *resultado = pow(vector[OPERANDO_IZQ], vector[OPERANDO_DER]);
    }else if(token.oper.op == OP_TERN){
        *resultado = vector[OPERANDO_TER] ? vector[OPERANDO_IZQ] : vector[OPERANDO_DER];
    }else if(token.oper.op == OP_RAIZ){
        if(vector[OPERANDO_DER] < NULO){
            fprintf(stdout, "ERROR\n");
            return false;
        }
        *resultado = sqrtl(vector[OPERANDO_DER]);
    }
    if(token.oper.op == OP_LOG){
        if(vector[ARG] <= NULO || vector[BASE] < BASE_MIN){
            fprintf(stdout, "ERROR\n");
            return false;
        }
        *resultado = logl(vector[ARG])/logl(vector[BASE]); //Cambio de base
    }
    return true;
}

bool sacar_operandos(pilanum_t* pila, struct calc_token token){
    calc_num vector[token.oper.arity];
    calc_num resultado;
    for(size_t i = 0; i < token.oper.arity; i++){
        if(!desapilar_num(pila, &vector[i])){
            fprintf(stdout, "ERROR\n");
            return false;
        }
    }
    if(aplicar_operacion(vector, token, &resultado)){
        apilar_num(pila, resultado);
        return true;
    }
    return false;
}

void mostrar_resultado(pilanum_t* pila){
    calc_num resul_final;
    desapilar_num(pila, &resul_final);
    if(desapilar_num(pila, &resul_final)){
        fprintf(stdout, "ERROR\n");
    }else{
        fprintf(stdout, "%ld\n", resul_final);
    }
    pilanum_destruir(pila);
}

void leer_entrada(char** linea){
    pilanum_t* pila = pilanum_crear();
    struct calc_token token;
    for(size_t i = 0; linea[i] != NULL; i++){
        if(!calc_parse(linea[i], &token)){
            fprintf(stdout, "ERROR\n");
            pilanum_destruir(pila);
            return;
        }else if(token.type == TOK_OPER){
            if(!sacar_operandos(pila, token)){
                pilanum_destruir(pila);
                return;
            }
        }else if(token.type == TOK_NUM){
            apilar_num(pila, token.value);
        }
    }
    mostrar_resultado(pila);
}

int main(void){
	char** linea_nueva;
    char* linea = NULL;
    size_t tam = 0;

    while(getline(&linea, &tam, stdin) != EOF){
        linea_nueva = dc_split(linea);
        leer_entrada(linea_nueva);
        free_strv(linea_nueva);
    }

	free(linea);
}