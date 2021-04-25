#define _POSIX_C_SOURCE 200809L
#include "clinica.h"
#include <assert.h>
#define NOMBRE 0
#define ESPECIALIDAD 1
#define ANIO 1
#define URGENCIA 2
#define DOCTOR 0
#define ARG_TOTALES_TURNO 3
#define ARG_TOTALES_ATENDER 1
#define BASE 10
#define DOC_INI 0
#define DOC_FIN 1

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

struct clinica{
    abb_t* doctores;
    hash_t* pacientes;
    hash_t* especialidades;
};

/******************
 * Funciones      *
 * Auxiliares     *
 * ****************/
//Funcion de comparacion para un heap de minimos
int cmp_antiguedad(const void* a, const void* b){
    const paciente_t* p_a = a;
    const paciente_t* p_b = b;
    if(p_a->anio < p_b->anio) return 1;
    else if (p_a->anio > p_b->anio) return -1;
    else return 0;
}

//Verifica el largo del arreglo que se pasa por parametro
//Args:
//      parametros(char**):Arreglo a verificar
//Return:
//      size_t: Cantidad de campos en el arreglo
size_t cant_parametros(char** parametros){
    size_t indice = 0;
    while(parametros[indice]){
        indice++;
    }
    return indice;
}

//Funcion de callback para imprimir los doctores contenidos en un arbol o dentro de un rango
bool imprimir_doctor(const char* clave, void* dato, void* extra){
	doctor_t* doctor = dato;
	size_t* contador = extra;
	printf(INFORME_DOCTOR, *contador, doctor->nombre, doctor->especialidad, doctor->atendidos);
	*contador += 1;
	return true;
}
//Funcion de callback para contar los doctores contenidos en un arbol o dentro de un rango
bool contar_doctores(const char* clave, void* dato, void* extra){
	size_t* contador = extra;
	*contador += 1;
	return true;
}

/********************
 * Funciones       *
 * del Paciente     *
 * ******************/

//Destruye la informacion del paciente(Liberando tambien su memoria)
//Args:
//      dato(void*):Puntero generico para luego ser casteado a paciente_t*
void destruir_paciente(void* dato){
    paciente_t* paciente = dato;
    free(paciente->nombre);
    free(paciente);
}

//Funcion creadora de datos utilizada en la libreria csv proporcionada por la catedra
//Crea la estructura Paciente, en caso de algun error se retorna NULL
//Args:
//      argumentos(char**):Contiene la informacion del paciente
//      extra(void*):Puntero auxiliar
//Returns:
//      void*, contiene el puntero TDA Paciente recien creado
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

//Funcion utilizada para pasar los datos de la lista a un hash
//Retorna un hash creado con memoria dinamica
//Args:
//      lista(lista_t*):Lista que contiene la informacion de los pacientes
//Returns:
//      hash_t*: Hash conteniendo la informacion de los pacientes
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

/*******************
 * Funciones       *
 * Auxiliares de la*
 * estructura      *
 * especialidad    *
 * *****************/

//Crea una nueva especialidad para la clinica
//Se podran manejar colas regulares y de urgencia,
//Y poder chequear la cantidad de pacientes en espera
//Returns: 
//      especialidad_t*:puntero a la estructura especialidad 
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

//Agrega un paciente a la cola de pacientes urgentes.
//Args:
//      especialidad(especialidad_t*):Especialidad
//      paciente(char*): Paciente a encolar
void encolar_urgente(especialidad_t* especialidad, hash_t* pacientes, char* nombre_paciente){
    paciente_t* paciente = hash_obtener(pacientes, nombre_paciente);
    char* p_paciente = paciente->nombre;
    assert(cola_encolar(especialidad->pacientes_urgentes, p_paciente));
    especialidad->pacientes++;
}

//Agrega un paciente a la cola de pacientes regulares.
//Args:
//      especialidad(especialidad_t*):Especialidad
//      pacientes(hash_t*):Hash
//      paciente(char*):Paciente a encolar
void encolar_regular(especialidad_t* especialidad, hash_t* pacientes, char* nombre_paciente){
    paciente_t* paciente = hash_obtener(pacientes, nombre_paciente);
    assert(heap_encolar(especialidad->pacientes_regulares, paciente));
    especialidad->pacientes++;
}

//Desencola el proximo paciente a ser atendido y lo devuelve, en caso de no
//haber mas pacientes retorna NULL.
//Args:
//      especialidades(hash_t*):Hash
//      clave(char*):Nombre de la especialidad
//      especialidad(especialidad_t*):Especialidad
//Return:
//      char*:Retorna el nombre del paciente desencolado, si no habia pacientes retorna NULL
char* desencolar_paciente(hash_t *especialidades, char* nombre_especialidad, especialidad_t* especialidad){
    if(especialidad->pacientes == 0) return NULL;
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

//Destruye la estructura especialidad(Incluyendo su memoria asociada)
//Args:
//      dato(void*):Luego sera casteado a especialidad_t*
void destruir_especialidad(void* dato){
    especialidad_t* especialidad = dato;
    cola_destruir(especialidad->pacientes_urgentes, NULL);
    heap_destruir(especialidad->pacientes_regulares, NULL);
    free(especialidad);
}


//Funcion auxiliar que destruye las estructuras especialidad 
//contenidas en un hash
void destruir_especialidades(hash_t* especialidades){
	hash_iter_t* iter = hash_iter_crear(especialidades);
	while(!hash_iter_al_final(iter)){
		const char* clave = hash_iter_ver_actual(iter);
		especialidad_t* especialidad = hash_obtener(especialidades, clave);
		destruir_especialidad(especialidad);
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);
	hash_destruir(especialidades);
}

/**************
 * Funciones  *
 * del Doctor *
 * ***********/
//Funcion creadora de datos utilizada en la libreria csv proporcionada por la catedra
//Crea la estructura doctor, en caso de algun error se retorna NULL
//Args:
//      argumentos(char**):Contiene la informacion del doctor
//      extra(void*):Utilizado como puntero auxiliar para guardar datos
//Returns:
//      void*, contiene el puntero TDA Doctor recien creado
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

//Destruye la informacion del doctor(Libera la memoria asociada)
//Args:
//      dato(void*):Puntero generico para ser casteado a doctor_t*
void destruir_doctor(void* dato){
	doctor_t* doctor = dato;
	free(doctor->nombre);
	free(doctor->especialidad);
	free(doctor);
}

//Construye un arbol conteniendo la informacion de los doctores de la clinica
//a partir de la lista pasado por parametro
//Args:
//      lista(lista_t*):Lista con la informacion de los doctores
//Returns:
//      abb_t*: Con la informacion de los doctores, en caso de algun problema,
//se retorna NULL
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

/*************
 * Manejo de *
 * errores   *
 * ***********/
bool errores_turno(char** parametros, clinica_t* clinica){
    bool error = false;
    
    if(cant_parametros(parametros) != ARG_TOTALES_TURNO) error = true;
    
    if(!hash_pertenece(clinica->pacientes,parametros[NOMBRE])){
        printf(ENOENT_PACIENTE, parametros[NOMBRE]);
        error = true;
    }

    if(!hash_pertenece(clinica->especialidades, parametros[ESPECIALIDAD])){
        printf(ENOENT_ESPECIALIDAD, parametros[ESPECIALIDAD]);
        error = true;
    }

    if(strcmp("URGENTE", parametros[URGENCIA]) != 0 && strcmp("REGULAR", parametros[URGENCIA]) != 0){
        printf(ENOENT_URGENCIA, parametros[URGENCIA]);
        error = true;
    }
    
    return error;
}

bool errores_atender(char** parametros, clinica_t* clinica){
    bool error = false;
    if(cant_parametros(parametros) != ARG_TOTALES_ATENDER){
        printf(ENOENT_PARAMS, "ATENDER_SIGUIENTE");
        error = true;
    }
    if(!abb_pertenece(clinica->doctores, parametros[DOCTOR])){
        printf(ENOENT_DOCTOR, parametros[DOCTOR]);
        error = true;
    }
    return error;
}

/*****************************
 * Primitivas del            *
 * TDA CLINICA               *
 * **************************/
clinica_t* clinica_crear(char* ruta_doctores, char* ruta_pacientes){
    clinica_t* clinica = malloc(sizeof(clinica_t));
    if(!clinica) return NULL;

    hash_t* especialidades = hash_crear(NULL);
	if(!especialidades) exit(1);
	lista_t* lista_doctores = csv_crear_estructura(ruta_doctores, crear_doctor, especialidades);
	if(!lista_doctores) exit(1);
	lista_t* lista_pacientes = csv_crear_estructura(ruta_pacientes, crear_paciente, NULL);
	if(!lista_pacientes) exit(1);
	abb_t* doctores = crear_estructura_doctores(lista_doctores);
	if(!doctores) exit(1);
	hash_t* pacientes = almacenar_pacientes(lista_pacientes);
	if(!pacientes) exit(1);

	lista_destruir(lista_pacientes, NULL);
	lista_destruir(lista_doctores, NULL);

    clinica->doctores = doctores;
    clinica->pacientes = pacientes;
    clinica->especialidades = especialidades;
    return clinica;
}

void _clinica_pedir_turno(char** parametros, hash_t* especialidades, hash_t* pacientes){
    especialidad_t* especialidad = hash_obtener(especialidades, parametros[ESPECIALIDAD]);
    
    if (strcmp(parametros[URGENCIA], "URGENTE") == 0) encolar_urgente(especialidad, pacientes, parametros[NOMBRE]);
    else encolar_regular(especialidad, pacientes, parametros[NOMBRE]);
    
    printf(PACIENTE_ENCOLADO, parametros[NOMBRE]);
    printf(CANT_PACIENTES_ENCOLADOS, especialidad->pacientes, parametros[ESPECIALIDAD]);
}

void clinica_pedir_turno(char** parametros, clinica_t* clinica){
    _clinica_pedir_turno(parametros, clinica->especialidades, clinica->pacientes);
}

void _clinica_atender_siguiente(char** parametros, abb_t* doctores, hash_t* especialidades){
    doctor_t* doctor = abb_obtener(doctores, parametros[DOCTOR]);
    especialidad_t* especialidad = hash_obtener(especialidades, doctor->especialidad);
    char* paciente = desencolar_paciente(especialidades, doctor->especialidad, especialidad);
    if(paciente){
        doctor->atendidos++;
        printf(PACIENTE_ATENDIDO, paciente);
        printf(CANT_PACIENTES_ENCOLADOS, especialidad->pacientes, doctor->especialidad);
	    hash_guardar(especialidades, doctor->especialidad, especialidad);
    }
    else{
        printf(SIN_PACIENTES);
    }
}

void clinica_atender_siguiente(char** parametros, clinica_t* clinica){
    _clinica_atender_siguiente(parametros, clinica->doctores, clinica->especialidades);
}

void _clinica_informe_doctores(char** parametros, abb_t* doctores){
	char* doctor_inicio = parametros[DOC_INI];
	char* doctor_final = parametros[DOC_FIN];
	size_t contador_doctores = 0;
	abb_iterar_en_rango(doctores, contar_doctores, &contador_doctores , doctor_inicio, doctor_final);
	size_t contador = 1;
	printf(DOCTORES_SISTEMA, contador_doctores);
	abb_iterar_en_rango(doctores, imprimir_doctor, &contador, doctor_inicio, doctor_final);
}

void clinica_informe_doctores(char** parametros, clinica_t* clinica){
    _clinica_informe_doctores(parametros, clinica->doctores);
}

void clinica_destruir(clinica_t* clinica){
    destruir_especialidades(clinica->especialidades);
    hash_destruir(clinica->pacientes);
    abb_destruir(clinica->doctores);
    free(clinica);
}