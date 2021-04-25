from grafo import Grafo
from heap import Heap
from collections import deque
from random import shuffle
import grafoutils

SEPARADOR_SALIDA = " --> "
MAX_LARGO = 150
CANCION = 0
SCORE = 1

def cmp_canciones_min(cancion_a, cancion_b):
    '''
    Funcion de comparacion utilizada en el heap de minimos
    '''
    if(cancion_a[SCORE] > cancion_b[SCORE]): return -1
    elif(cancion_a[SCORE] < cancion_b[SCORE]): return 1
    return 0

def k_mayores(pageranks, k):
    '''
    Utilizando un heap y el algoritmo top-k, encontramos los scores mas grandes del pagerank pasado por parametro.
    Dado que utilizamos el algoritmo top-k con un heap de minimos, la complejidad de la funcion
    sera:
    O(N log K)
    Donde N es la cantidad de elementos del array y K es la cantidad de elementos que hay en el heap
    '''
    heap = Heap(cmp_canciones_min)

    for i in range(k):
        heap.heap_encolar(pageranks[i])
    
    for i in range(k, len(pageranks)):
        pagerank_tope = heap.heap_ver_max()
        if pageranks[i][SCORE] > pagerank_tope[SCORE]:
            heap.heap_desencolar()
            heap.heap_encolar(pageranks[i])
    
    k_mayores = list()
    while not heap.heap_esta_vacio():
        pagerank_actual = heap.heap_desencolar()
        k_mayores.insert(0, pagerank_actual[CANCION])

    return k_mayores

def generar_grafo_canciones(grafo_canciones, playlist):
    """
    Funcion que genera las aristas del grafo de canciones
    Args:
        grafo_canciones(Grafo):Conjunto de vertices sin aristas
        playlist(dict):Diccionario que relaciona clave con el nombre de una playlist
         y valor con una lista que contiene las canciones de esa playlist
    Returns:
        grafo_canciones(Grafo):Conjunto de vertices con aristas
    
    Complejidad:
    El peor caso se puede dar cuando las playlist contienen todas las canciones
    existentes en memoria.Como en el primero for iteramos las claves del diccionario
    y luego le siguen dos for mas iterando las canciones que pertenecen a esa playlist, quedariamos
    con una complejidad
    O(L * C^2)
    """
    for nombre in playlist:
        for cancion in playlist[nombre]:
            for _cancion in playlist[nombre]:
                if not grafo_canciones.estan_unidos(cancion, _cancion) and cancion != _cancion:
                    grafo_canciones.agregar_arista(cancion, _cancion, nombre)
    return grafo_canciones

def camino_mas_corto(grafo_bipartito, origen, destino):
    """
    Comando del programa que permite obtener un camino desde una cancion V
    hasta una cancion W, se utiliza un recorrido BFS y luego se crea
    el camino desde el destino hacia el origen.
    Args:
        grafo_bipartito(Grafo):Conjunto de vertices y aristas
        origen(str):Vertice donde comenzar
        destino(str):Vertice donde terminar
    Returns:
        string(str):Cadena que contiene la informacion del recorrido desde origen a destino
    Complejidad:
    Dado que utilizamos un recorrido BFS y nuestro grafo esta compuesto por 
    Usuarios,Canciones(Vertices) y Playlists(Aristas), obtenemos una complejidad de
    O(U + C + L)
    """
    padres,_ = grafoutils.bfs(grafo_bipartito, origen, destino)
    if destino not in padres:
        return "No se encontro recorrido"
        
    lista = deque()
    actual = destino
    while actual != origen:
        if(actual is None): #Es decir que no hay camino hacia el origen
            return "No se encontro recorrido"
        else:
            lista.appendleft(actual)
            if actual in padres:
                actual = padres[actual]

    v = origen
    string = f"{v}"
    while len(lista) != 0:
        string += f"{SEPARADOR_SALIDA}aparece en playlist{SEPARADOR_SALIDA}"
        w = lista.popleft()
        string += f"{grafo_bipartito.peso_arista(padres[w], w)}{SEPARADOR_SALIDA}"
        v = lista.popleft()
        string += f"de{SEPARADOR_SALIDA}{w}{SEPARADOR_SALIDA}tiene una playlist{SEPARADOR_SALIDA}{grafo_bipartito.peso_arista(v,w)}{SEPARADOR_SALIDA}"
        string += f"donde aparece{SEPARADOR_SALIDA}"
        string += f"{v}"
    
    return string
    
def mas_importantes(grafo_bipartito, grafo_canciones):
    """
    Comando para calcular las canciones mas importantes del grafo
    Args:
        grafo_bipartito(Grafo):Grafo bipartito que contiene canciones y usuarios
        grafo_canciones(Grafo): Grafo no dirigido que contiene canciones
    Returns:
        pagerank(list):Una lista que contiene elementos de la siguiente forma [cancion, score]
    
    Complejidad:
    Calcular el pagerank cuesta O(MAX_ITERACIONES*(U + C + L))
    Convertir el diccionario a lista cuesta O(U + C)
    Complejidad final:
    O(MAX_ITERACIONES(U + C + L))
    """
    pagerank = grafoutils.pagerank(grafo_bipartito)
    pagerank = [list(par) for par in pagerank.items() if par[0] in grafo_canciones]

    return pagerank

def n_ciclo(grafo, origen, n):
    """
    Comando que permite calcular un ciclo de largo n a partir de un origen en el grafo
    de canciones y luego imprime el ciclo de canciones por stdout
    Args:
        grafo(Grafo):Conjunto de vertices y aristas
        origen(str):Vertice de origen
        n(int):Indica el largo que debe cumplir el ciclo en el grafo
    Returns:
        None
    Complejidad:
    Dado que utilizamos un algoritmo de backtracking, este tendra una complejidad exponencial
    en este caso sabemos que cumplira el siguiente tope:
    O(C^n)
    """
    visitados = set()
    camino = list()
    if(grafoutils._n_ciclo(grafo, origen, origen, n, visitados, camino)):
        camino = SEPARADOR_SALIDA.join(camino)
        print(camino)
    else:
        print("No se encontro recorrido")


def canciones_n_rango(grafo, v, n):
    """
    Devuelve la cantidad de canciones que estan a rango n del vertice 
    v en el grafo
    Complejidad
    Dado que utilizamos un recorrido BFS, en el peor de los casos la complejidad es
    O(C + L), en el peor de los casos estamos hablando de que 'n' sea el diametro de la red
    Args:
        grafo(Grafo):Conjunto de vertices y aristas
        v(str):Vertice de origen
        n(int):Distancia desde el vertice de origen
    Returns:
        int:Cantidad de vertices a distancia n del origen
    """
    return grafoutils.n_rango(grafo, v, n)

def clustering(grafo, vertice):
    """
    Calcula el clustering de un vertice en particular o
    de todo el grafo pasado por parametro y luego lo imprime por stdout.
    Complejidad:
    Dado que en el peor de los casos no nos pasen por parametro un vertice de origen
    Calcular el clustering promedio del grafo costaria O((C+L)^2)
    Calcular el clustering de un vertice en particular(considerando que la red sea
    muy dispersa) cuesta O(1)
    Args:
        grafo(Grafo):Conjunto de vertices y aristas
        vertice(str):Vertice a calcular el clustering, en caso de estar vacio
        se calcula el clustering promedio
    Returns:
        None
    """
    if vertice == '':
        print("{:.3f}".format(grafoutils.clustering_global(grafo)))
    else:
        print("{:.3f}".format(grafoutils._clustering(grafo, vertice)))    

def recomendacion(grafo_bipartito, grafo_canciones, listado, tipo_recomendacion):
    """
    Dado un grafo bipartito y una lista de canciones, se genera
    un pagerank personalizado dependiendo de las canciones que existan en ese
    listado.
    Args:
        grafo_bipartito(Grafo):Grafo bipartito entre usuarios y canciones
        grafo_canciones(Grafo):Grafo no dirigido que contiene nombre de canciones como vertices
        listado(list):Una lista de canciones pasadas por el usuario que seran utiles para la recomendacion
        tipo_recomendacion(str): Determina si hay que recomendar canciones o usuarios
    Returns:
        pagerank(list):Retorna una lista ordenada de forma descendente cuyos elementos se encuentran de la forma
        [vertice, pagerank]
    """
    pagerank = dict()
    grados = grafoutils.grados_vertices(grafo_bipartito)
    shuffle(listado)
    
    for v in grafo_bipartito:
        pagerank[v] = 0

    pagerank[listado[0]] = 1

    for cancion in listado:  
        camino = grafoutils.random_walk(grafo_bipartito, MAX_LARGO, cancion)
        for i in range(1, len(camino)):
            pagerank[camino[i]] += pagerank[camino[i-1]] / grados[camino[i-1]]

    for v in listado:
        del pagerank[v]

    if tipo_recomendacion == "canciones":
        pagerank = [list(par) for par in pagerank.items() if par[0] in grafo_canciones]
    else:
        pagerank = [list(par) for par in pagerank.items() if par[0] not in grafo_canciones]

    return pagerank

