#define  _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include "mensajes.h"
#include "clinica.h"
#define CSV_DOC 1
#define CSV_PAC 2
#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_comando(const char* comando, char** parametros, clinica_t* clinica) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		if(!errores_turno(parametros, clinica)){
			clinica_pedir_turno(parametros, clinica);
		}
	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		if(!errores_atender(parametros, clinica)){
			clinica_atender_siguiente(parametros, clinica);
		}		
	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		size_t len_parametros = cant_parametros(parametros);
		if(len_parametros == 2){
			clinica_informe_doctores(parametros, clinica);
		}
		else printf(ENOENT_PARAMS, comando);
	} else {
		printf(ENOENT_CMD, comando);
	}
}

void procesar_entrada(clinica_t* clinica) {
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		if (campos[1] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;	
		}
		char** parametros = split(campos[1], ',');
		procesar_comando(campos[0], parametros, clinica);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}

int main(int argc, char** argv) {
	if(argc > 3){
		printf(ENOENT_CANT_PARAMS);
		exit(1);
	}
	char* ruta_doctores = argv[CSV_DOC];
	char* ruta_pacientes = argv[CSV_PAC];
	clinica_t* clinica = clinica_crear(ruta_doctores, ruta_pacientes);
	procesar_entrada(clinica);
	clinica_destruir(clinica);
}
