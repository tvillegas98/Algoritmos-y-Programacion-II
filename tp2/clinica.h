#ifndef _CLINICA_H
#define _CLINICA_H_
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
#include "csv.h"

typedef struct clinica clinica_t;

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
//      clinica(clinica_t*): Contiene los datos de la clinica
//Return:
//      bool:Retorna true si hubo algun error en la verificacion, false en caso contrario
bool errores_turno(char** parametros, clinica_t* clinica);

//Verifica si hubo algun error en los parametros recibidos del comando ATENDER_SIGUIENTE.
//Por cada error encontrado se imprime el respectivo mensaje.
//Args:
//      parametros(const char**):Parametros a verificar
//      clinica(clinica_t*):Contiene los datos de la clincia
//Return:
//      bool:Retorna true si hubo algun error en la verificacion, false en caso contrario
bool errores_atender(char** parametros, clinica_t* clinica);

//Recibe como parametros las rutas hacia los archivos csv que 
//contienen la informacion sobre la clinica
//Pre: Ninguna
//Post: Se guardaron los datos de la clinica en la memoria y se le devuelve
//al usuario el TDA Clinica
//Args;
//      ruta_doctores(char*): Ruta hacia el csv de doctores
//      ruta_pacientes(char*): Ruta hacia el csv de pacientes
clinica_t* clinica_crear(char* ruta_doctores, char* ruta_pacientes);

//Se recibe el nombre de un paciente, la especialidad requerida y el grado de
//urgencia, y se lo añade a la lista de espera correspondiente.
//Pre: Se chequeo que no exista ningun error en los datos de ingreso
//Post: Se encolo al paciente en la lista de espera requerida.
//Args:
//      parametros(const char**):Parametros a procesar
//      clinica(clinica_t*):Contiene los datos de la clinica
void clinica_pedir_turno(char** parametros, clinica_t* clinica);

//Se recibe el nombre del doctor solicitado y los datos de la clinica
//Pre: Se chequeo que no exista ningun error en los datos de ingreso
//Post: Se desencolo al paciente con mayor prioridad en el momento y se muestra en pantalla
//cuantos faltan por atender.
//Args:
//      parametros(const char**):Parametros a procesar
//      clinica(clinica_t* ): Datos de la clinica
void clinica_atender_siguiente(char** parametros, clinica_t* clinica);

//Utilizado para generar un informe en orden alfabetico sobre los datos
//de los doctores en la clinica, como parametros se recibe el nombre de
//dos doctores, en caso de necesitar un informe dentro de un determinado, sino,
//pueden dejar vacio uno de los espacios o ambos, para generar informes
//con rango inicial y sin rango final, sin rango inicial y con rango final o
//sin rango inicial ni final, es decir, informe completo de los doctores.
//Pre:Se chequeo que no exista ningun error en los datos de ingreso
//Post: Se emitio correctamente el informe solicitado
//Args:
//      parametros()
//
void clinica_informe_doctores(char** parametros, clinica_t* clinica);


//Destruye los datos de la clinica y
//la memoria asociada a ella
//Pre: Los datos de la clinica fueron creados
//Args: 
//      clinica(clinica_t*):Datos de la clinica
void clinica_destruir(clinica_t* clinica);
#endif