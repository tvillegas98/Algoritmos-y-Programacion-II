#ifndef _LIB_H_
#define _LIB_H_
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "cola.h"
#include "heap.h"
#include "hash.h"
#include "mensajes.h"
#include "abb.h"
#include "lista.h"

struct doctor;
struct especialidad;
struct paciente;

typedef struct doctor doctor_t;
typedef struct especialidad especialidad_t;
typedef struct paciente paciente_t;

//Funcion creadora de datos utilizada en la libreria csv proporcionada por la catedra
//Crea el TDA Paciente, en caso de algun error se retorna NULL
//Args:
//      argumentos(char**):Contiene la informacion del paciente
//      extra(void*):Puntero auxiliar
//Returns:
//      void*, contiene el puntero TDA Paciente recien creado
void* crear_paciente(char** argumentos, void* extra);

//Destruye la informacion del paciente(Liberando tambien su memoria)
//Args:
//      dato(void*):Puntero generico para luego ser casteado a paciente_t*
void destruir_paciente(void* dato);

//Retorna el nombre del paciente
//Args:
//      paciente(paciente_t*):Contiene informacion de los pacientes
//Return:
//      char*:Nombre del paciente
char* obtener_nombre_paciente(paciente_t* paciente);

//Funcion utilizada para pasar los datos de la lista a un hash
//Retorna un hash creado con memoria dinamica
//Args:
//      lista(lista_t*):Lista que contiene la informacion de los pacientes
//Returns:
//      hash_t*: Hash conteniendo la informacion de los pacientes
hash_t* almacenar_pacientes(lista_t* lista);

//Funcion creadora de datos utilizada en la libreria csv proporcionada por la catedra
//Crea el TDA doctor, en caso de algun error se retorna NULL
//Args:
//      argumentos(char**):Contiene la informacion del doctor
//      extra(void*):Utilizado como puntero auxiliar para guardar datos
//Returns:
//      void*, contiene el puntero TDA Doctor recien creado
void* crear_doctor(char** argumentos, void* extra);

//Destruye la informacion del doctor(Libera la memoria asociada)
//Args:
//      dato(void*):Puntero generico para ser casteado a doctor_t*
void destruir_doctor(void* dato);

//Retorna el nombre del doctor
//Args:
//      doctor(doctor_t*):Puntero hacia la informacion de un doctor
char* obtener_nombre_doctor(doctor_t* doctor);

//Retorna el nombre de la especialidad que maneja el doctor pasado por parametro
//Args:
//      doctor(doctor_t*):Puntero hacia la informacion de un doctor
char* obtener_nombre_especialidad(doctor_t* doctor);

//Retorna la cantidad de pacientes atendidos por el doctor pasado por parametro
//Args;
//      doctor(doctor_t*):Puntero hacia la informacion de un doctor

//Return:
//      size_t:Cantidad de pacientes atendidos
size_t obtener_atendidos(doctor_t* doctor);

//Agrega un atendido al doctor
//Args:
//      doctor(doctor_t*):Puntero hacia la informacion de un doctor
void incrementar_atendidos(doctor_t* doctor);


//Construye un arbol conteniendo la informacion de los doctores de la clinica
//a partir de la lista pasado por parametro
//Args:
//      lista(lista_t*):Lista con la informacion de los doctores
//Returns:
//      abb_t*: Con la informacion de los doctores, en caso de algun problema,
//      se retorna NULL
abb_t*  crear_estructura_doctores(lista_t* lista);

//Crea una nueva especialidad para la clinica
//Se podran manejar colas regulares y de urgencia,
//Y poder chequear la cantidad de pacientes en espera
//Returns: 
//      void*:Contiene el puntero TDA especialidad recien creado
void* crear_especialidad();

//Destruye el TDA especialidad(Incluyendo su memoria asociada)
//Args:
//      dato(void*):Luego sera casteado a especialidad_t*
void destruir_especialidad(void* dato);

//Retorna la cantidad de pacientes que se encuentran en la lista de espera
//Args:
//      especialidad(especialidad_t*):Especialidad
//Return;
//      size_t: Cantidad de pacientes en espera
size_t obtener_espera(const especialidad_t* especialidad);

//Verifica el largo del arreglo que se pasa por parametro
//Args:
//      parametros(char**):Arreglo a verificar
//Return:
//      size_t: Cantidad de campos en el arreglo
size_t cant_parametros(char** parametros);

//Verifica si hubo algun error en los parametros recibidos del comando PEDIR_TURNO.
//Por cada error encontrado se imprime el respectivo mensaje.
//Args:
//      parametros(char**): Parametros a verificar
//      pacientes(hash_t*):Hash
//      especialidades(hash_t*):Hash
//Return:
//      bool:Retorna true si hubo algun error en la verificacion, false en caso contrario
bool errores_turno(char** parametros, hash_t* pacientes, hash_t* especialidades);

//Agrega un paciente a la cola de pacientes urgentes.
//Args:
//      especialidad(especialidad_t*):Especialidad
//      paciente(char*): Paciente a encolar
void encolar_urgente(especialidad_t* especialidad, hash_t* pacientes, char* paciente);

//Agrega un paciente a la cola de pacientes regulares.
//Args:
//      especialidad(especialidad_t*):Especialidad
//      pacientes(hash_t*):Hash
//      paciente(char*):Paciente a encolar
void encolar_regular(especialidad_t* especialidad, hash_t* pacientes, char* paciente);

//Desencola el proximo paciente a ser atendido y lo devuelve, en caso de no
//haber mas pacientes retorna NULL.
//Args:
//      especialidades(hash_t*):Hash
//      clave(char*):Nombre de la especialidad
//      especialidad(especialidad_t*):Especialidad
//Return:
//      char*:Retorna el nombre del paciente desencolado, si no habia pacientes retorna NULL
char* desencolar_paciente(hash_t *especialidades, char* clave, especialidad_t* especialidad);

//Se recibe el nombre de un paciente, la especialidad requerida y el grado de
//urgencia, y se lo añade a la lista de espera correspondiente
//Args:
//      parametros(const char**):Parametros a procesar
//      especialidades(hash_t*):Hash
//      pacientes(hash_t*):Hash
void pedir_turno(char** parametros, hash_t *especialidades, hash_t* pacientes);

//Verifica si hubo algun error en los parametros recibidos del comando ATENDER_SIGUIENTE.
//Por cada error encontrado se imprime el respectivo mensaje.
//Args:
//      parametros(const char**):Parametros a verificar
//      doctores(abb_t*):ABB
//Return:
//      bool:Retorna true si hubo algun error en la verificacion, false en caso contrario
bool errores_atender(char** parametros, abb_t* doctores);

//Se recibe el nombre del doctor que quedo libre, y este atiende al siguiente
//paciente en la lista de espera.
//Args:
//      parametros(const char**):Parametros a procesar
//      doctores(abb_t*):ABB
//      especialidades(hash_t*):Hash
void atender_siguiente(char** parametros, abb_t* doctores, hash_t* especialidades);

#endif