#!/usr/bin/python3
import recomendify_lib
import sys
import csv
import os
from grafo import Grafo
from collections import deque

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
SEPARADOR_CANCIONES = " - "
SEPARADOR_COMANDOS = " >>>> "
SEPARADOR_ESPACIOS = " "
SEPARADOR_PAGERANK = "; "
CMD_CAMINO = "camino"
CMD_RECOMENDACION = "recomendacion"
CMD_IMPORTANTES = "mas_importantes"
CMD_CICLO = "ciclo"
CMD_RANGO = "rango"
CMD_CLUSTERING = "clustering"
RUTA = 1
ARGS_MAX = 2
ARGS_VACIO = 1

def procesar_comando (comando, parametros, grafo_bipartito, grafo_canciones, playlist, pagerank_score, grafo_c_generado):
    
    if comando == CMD_CAMINO:
        parametros = SEPARADOR_ESPACIOS.join(parametros)
        parametros = parametros.split(SEPARADOR_COMANDOS)
        if not parametros[0] in grafo_canciones or not parametros[1] in grafo_canciones:
            print("Tanto el origen como el destino deben ser canciones")
        else:
            print(recomendify_lib.camino_mas_corto(grafo_bipartito, parametros[0], parametros[1]))
    
    elif comando == CMD_IMPORTANTES:
        cantidad = int(parametros[0])
        if not pagerank_score:
            pagerank_score = recomendify_lib.mas_importantes(grafo_bipartito, grafo_canciones)
        listado_canciones = recomendify_lib.k_mayores(pagerank_score, cantidad)
        print(SEPARADOR_PAGERANK.join(listado_canciones))

    elif comando == CMD_RECOMENDACION:
        tipo_recomendacion = parametros[0]
        cantidad = int(parametros[1])
        listado = SEPARADOR_ESPACIOS.join(parametros[2:]).split(SEPARADOR_COMANDOS)
        pagerank_personalizado = recomendify_lib.recomendacion(grafo_bipartito, grafo_canciones, listado, tipo_recomendacion)
        recomendaciones = recomendify_lib.k_mayores(pagerank_personalizado, cantidad)
        print(SEPARADOR_PAGERANK.join(recomendaciones))

    elif comando == CMD_CICLO:
        largo = int(parametros[0])
        parametros = SEPARADOR_ESPACIOS.join(parametros[1:])
        if not grafo_c_generado:
            grafo_canciones = recomendify_lib.generar_grafo_canciones(grafo_canciones, playlist)
            grafo_c_generado = True
        recomendify_lib.n_ciclo(grafo_canciones, parametros, largo)

    elif comando == CMD_RANGO:
        if not grafo_c_generado:
            grafo_canciones = recomendify_lib.generar_grafo_canciones(grafo_canciones, playlist)
            grafo_c_generado = True
        saltos = int(parametros[0])
        parametros = SEPARADOR_ESPACIOS.join(parametros[1:])
        print(recomendify_lib.canciones_n_rango(grafo_canciones, parametros, saltos))

    elif comando == CMD_CLUSTERING:
        if not grafo_c_generado:
            grafo_canciones = recomendify_lib.generar_grafo_canciones(grafo_canciones, playlist)
            grafo_c_generado = True
        parametros = SEPARADOR_ESPACIOS.join(parametros)
        recomendify_lib.clustering(grafo_canciones, parametros)
    
    return pagerank_score, grafo_c_generado 
    #Retorno el resultado del pagerank para que se vaya pasando por cada ejecucion y que no lo vuelva a calcular

def procesar_entrada(grafo_bipartito, grafo_canciones, playlist):
    pagerank = None
    grafo_c_generado = False
    with open(sys.stdin.fileno()) as entradas:
        instruccion = entradas.readline()
        while instruccion:
            instruccion = instruccion.rstrip().split(SEPARADOR_ESPACIOS)
            comando = instruccion[0]
            parametros = instruccion[1:]
            pagerank, grafo_c_generado = procesar_comando(comando, parametros, grafo_bipartito, grafo_canciones, playlist, pagerank, grafo_c_generado)
            instruccion = entradas.readline()

def preprocesamiento(ruta):
    """
    Lee la ruta del archivo pasada por parametro y parsea el archivo TSV.
    Se crea un grafo bipartito entre Usuarios <-Playlist-> Canciones, otro
    grafo que contenga solo las canciones relacionandolas por playlist y un diccionario
    que contiene como claves el nombre de las playlist y como valor una lista que contenga
    todas las canciones que se relacionen con la playlist
    Args:
        ruta(str):Ruta del archivo a procesar
    Returns
        grafo_bipartito(Grafo):Grafo que relaciona Usuarios <-Playlist-> Canciones
        grafo_canciones(Grafo):Grafo que inicialmente solo contiene vertices
        playlist(dict):Diccionario que relaciona playlist con canciones 
    """
    grafo_bipartito = Grafo()
    grafo_canciones = Grafo()
    playlist = {}
    with open(ruta, newline="") as file:
        reader = csv.DictReader(file, delimiter='\t', quoting=csv.QUOTE_NONE)
        for row in reader:
            if not row['USER_ID'] in grafo_bipartito:
                grafo_bipartito.agregar_vertice(row['USER_ID'])
            if not row["PLAYLIST_NAME"] in playlist:
                playlist[row["PLAYLIST_NAME"]] = list()
            if not row["TRACK_NAME"]+SEPARADOR_CANCIONES+row["ARTIST"] in playlist[row["PLAYLIST_NAME"]]:
                playlist[row["PLAYLIST_NAME"]].append(row["TRACK_NAME"]+SEPARADOR_CANCIONES+row["ARTIST"])
            if not row['TRACK_NAME']+SEPARADOR_CANCIONES+row['ARTIST'] in grafo_bipartito:
                grafo_bipartito.agregar_vertice(row['TRACK_NAME']+SEPARADOR_CANCIONES+row['ARTIST'])
            if not row['TRACK_NAME']+SEPARADOR_CANCIONES+row['ARTIST'] in grafo_canciones:
                grafo_canciones.agregar_vertice(row['TRACK_NAME']+SEPARADOR_CANCIONES+row['ARTIST'])
            if not grafo_bipartito.estan_unidos(row['TRACK_NAME']+SEPARADOR_CANCIONES+row['ARTIST'], row['USER_ID']):
                grafo_bipartito.agregar_arista(row['TRACK_NAME']+SEPARADOR_CANCIONES+row['ARTIST'], row['USER_ID'], row['PLAYLIST_NAME'])

    return grafo_bipartito, grafo_canciones, playlist
    
def main():
    ruta = sys.argv[RUTA]
    if not os.path.exists(os.path.join(BASE_DIR, ruta)):
        raise ValueError("No existe ese archivo en el directorio")
    grafo_bipartito, grafo_canciones, playlist = preprocesamiento(ruta)
    procesar_entrada(grafo_bipartito, grafo_canciones, playlist)

if __name__ == "__main__":
    if(len(sys.argv) > ARGS_MAX or len(sys.argv) == ARGS_VACIO):
        raise ValueError("Ruta invalida")
    main()
