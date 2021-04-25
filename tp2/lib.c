#define _POSIX_C_SOURCE 200809L
#include "lib.h"
#include <assert.h>
#define NOMBRE 0
#define ESPECIALIDAD 1
#define ANIO 1
#define URGENCIA 2
#define DOCTOR 0
#define ARG_TOTALES_TURNO 3
#define ARG_TOTALES_ATENDER 1
#define BASE 10

/********************
 * Definicion       *
 * de               *
 * estructuras      *
 * *****************/
typedef struct doctor{
    char* nombre;
    char* especialidad;
    size_t atendidos;
}doctor_t;

typedef struct especialidad {
    cola_t* pacientes_urgentes;
    heap_t* pacientes_regulares;
    size_t pacientes;
}especialidad_t;

typedef struct paciente {
    char* nombre;
    long int anio;
}paciente_t;

/******************
 * Funciones      *
 * Auxiliares     *
 * ****************/


int cmp_antiguedad(const void* a, const void* b){
    const paciente_t* p_a = a;
    const paciente_t* p_b = b;
    if(p_a->anio < p_b->anio) return 1;
    else if (p_a->anio > p_b->anio) return -1;
    else return 0;
}

size_t cant_parametros(char** parametros){
    size_t indice = 0;
    while(parametros[indice]){
        indice++;
    }
    return indice;
}
/********************
 * Primitivas       *
 * TDA Paciente     *
 * ******************/

void destruir_paciente(void* dato){
    paciente_t* paciente = dato;
    free(paciente->nombre);
    free(paciente);
}

void* crear_paciente(char** argumentos, void* extra){
    paciente_t* paciente = malloc(sizeof(paciente_t));
    if(!paciente) return NULL;

    char* p_extra;
    long int anio = strtol(argumentos[ANIO], &p_extra, BASE);
    if(strcmp(argumentos[ANIO], p_extra) == 0 || strcmp(p_extra, "\0") != 0){ 
        printf(ENOENT_ANIO, argumentos[ANIO]);
        exit(1);
    }

    paciente->nombre = strdup(argumentos[NOMBRE]);
    if(!paciente->nombre) return NULL;
    paciente->anio = anio;
    return paciente;

}

char* obtener_nombre_paciente(paciente_t* paciente){
    return paciente->nombre;
}

hash_t* almacenar_pacientes(lista_t* lista){
    hash_t* pacientes = hash_crear(destruir_paciente);
    if(!pacientes) return NULL;

    lista_iter_t* iterador = lista_iter_crear(lista);
    if(!iterador) return NULL;

    while(!lista_iter_al_final(iterador)){
        paciente_t* paciente = lista_iter_ver_actual(iterador);
        if(!hash_guardar(pacientes, paciente->nombre, paciente)) return NULL;
        lista_iter_avanzar(iterador);
    }

    free(iterador);
    
    return pacientes;
}

/**************
 * Primitivas *
 * TDA Doctor *
 * ***********/
void* crear_doctor(char** argumentos, void* extra){
    doctor_t* doctor = malloc(sizeof(doctor_t));
    if(!doctor) return NULL;
    
    hash_t* especialidades = extra;
    if(!hash_pertenece(especialidades, argumentos[ESPECIALIDAD])){
        especialidad_t* especialidad = crear_especialidad();
        if(!hash_guardar(especialidades, argumentos[ESPECIALIDAD], especialidad)) return NULL;
    }
    
    doctor->nombre = strdup(argumentos[NOMBRE]);
    doctor->especialidad = strdup(argumentos[ESPECIALIDAD]);
    doctor->atendidos = 0;

    return doctor;
}

char* obtener_nombre_doctor(doctor_t* doctor){
    return doctor->nombre;
}

char* obtener_nombre_especialidad(doctor_t* doctor){
    return doctor->especialidad;
}

size_t obtener_atendidos(doctor_t* doctor){
    return doctor->atendidos;
}

void incrementar_atendidos(doctor_t* doctor){
    doctor->atendidos++;
}

void destruir_doctor(void* dato){
	doctor_t* doctor = dato;
	free(doctor->nombre);
	free(doctor->especialidad);
	free(doctor);
}

abb_t* crear_estructura_doctores(lista_t* lista){
    abb_t* arbol = abb_crear(strcmp, destruir_doctor);
    if(!arbol) return NULL;

    lista_iter_t* iterador = lista_iter_crear(lista);
    if(!iterador) return NULL;

    while(!lista_iter_al_final(iterador)){
        doctor_t* doctor = lista_iter_ver_actual(iterador);
        if(!abb_guardar(arbol, doctor->nombre, doctor)) return NULL;
        lista_iter_avanzar(iterador);
    }
    
	free(iterador);

    return arbol;
}

/*******************
 * Primitivas      *
 * TDA Especialidad*
 * *****************/

void* crear_especialidad(){
    especialidad_t* especialidad = malloc(sizeof(especialidad_t));
    if(!especialidad) return NULL;
    especialidad->pacientes_urgentes = cola_crear();
    especialidad->pacientes_regulares = heap_crear(cmp_antiguedad);

    if(!especialidad->pacientes_regulares || !especialidad->pacientes_urgentes){
        if(especialidad->pacientes_regulares) free(especialidad->pacientes_regulares);
        if(especialidad->pacientes_urgentes) free(especialidad->pacientes_urgentes);
        free(especialidad);
        return NULL;
    }

    especialidad->pacientes = 0;
    return especialidad;
}

void encolar_urgente(especialidad_t* especialidad, hash_t* pacientes, char* nombre_paciente){
    paciente_t* paciente = hash_obtener(pacientes, nombre_paciente);
    char* p_paciente = obtener_nombre_paciente(paciente);
    assert(cola_encolar(especialidad->pacientes_urgentes, p_paciente));
    especialidad->pacientes++;
}

void encolar_regular(especialidad_t* especialidad, hash_t* pacientes, char* nombre_paciente){
    paciente_t* paciente = hash_obtener(pacientes, nombre_paciente);
    assert(heap_encolar(especialidad->pacientes_regulares, paciente));
    especialidad->pacientes++;
}

char* desencolar_paciente(hash_t *especialidades, char* nombre_especialidad, especialidad_t* especialidad){
    if(obtener_espera(especialidad) == 0) return NULL;
    char* nombre;
    if(!cola_esta_vacia(especialidad->pacientes_urgentes)){
        nombre = cola_desencolar(especialidad->pacientes_urgentes);
        especialidad->pacientes--;
    }
    else{
        paciente_t* regular = heap_desencolar(especialidad->pacientes_regulares);
        if(!regular) return NULL;
        nombre = regular->nombre;
        especialidad->pacientes--; 
    }

    hash_guardar(especialidades, nombre_especialidad, especialidad);
    return nombre;
}

size_t obtener_espera(const especialidad_t* especialidad){
    return especialidad->pacientes;
}

void destruir_especialidad(void* dato){
    especialidad_t* especialidad = dato;
    cola_destruir(especialidad->pacientes_urgentes, NULL);
    heap_destruir(especialidad->pacientes_regulares, NULL);
    free(especialidad);
}

/*************
 * Manejo de *
 * errores   *
 * ***********/
bool errores_turno(char** parametros, hash_t* pacientes, hash_t* especialidades){
    bool error = false;
    
    if(cant_parametros(parametros) != ARG_TOTALES_TURNO) error = true;
    
    if(!hash_pertenece(pacientes,parametros[NOMBRE])){
        printf(ENOENT_PACIENTE, parametros[NOMBRE]);
        error = true;
    }

    if(!hash_pertenece(especialidades, parametros[ESPECIALIDAD])){
        printf(ENOENT_ESPECIALIDAD, parametros[ESPECIALIDAD]);
        error = true;
    }

    if(strcmp("URGENTE", parametros[URGENCIA]) != 0 && strcmp("REGULAR", parametros[URGENCIA]) != 0){
        printf(ENOENT_URGENCIA, parametros[URGENCIA]);
        error = true;
    }
    
    return error;
}

bool errores_atender(char** parametros, abb_t* doctores){
    bool error = false;
    if(cant_parametros(parametros) != ARG_TOTALES_ATENDER){
        printf(ENOENT_PARAMS, "ATENDER_SIGUIENTE");
        error = true;
    }
    if(!abb_pertenece(doctores, parametros[DOCTOR])){
        printf(ENOENT_DOCTOR, parametros[DOCTOR]);
        error = true;
    }
    return error;
}
