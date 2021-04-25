from grafo import Grafo
from collections import deque
from random import shuffle
D = 0.85
MAX_ITERACIONES = 20

def grados_vertices(grafo):
    """
    Calcula los grados de cada vertice de un grafo(dirigido) pasado
    por parametro
    """
    grados = dict()
    for v in grafo:
        grados[v] = len(grafo.adyacentes(v))
    return grados

def bfs(grafo, origen, destino):
    """
    Algoritmo que consiste en recorrer el grafo de forma radial
    partiendo desde un origen y terminando en un destino

    Args:
        grafo(Grafo):Conjunto de vertices y aristas
        origen(str):Punto de partida del algoritmo BFS
        destino(str):Punto final del algoritmo BFS
    Returns:
        padres(dict):Diccionario de la forma hijo:padre
        orden(dict):Diccionario de la forma vertice:distancia del origen

    """
    visitados = set()
    padres = dict()
    orden = dict()
    for v in grafo:
        padres[v] = None
    orden[origen] = 0
    q = deque()
    q.append(origen)
    while len(q) != 0:
        v = q.popleft()
        if(v == destino):
            break
        for w in grafo.adyacentes(v):
            if w not in visitados:
                padres[w] = v
                orden[w] = orden[v] + 1
                visitados.add(w)
                q.append(w)
    return padres, orden 

def n_rango(grafo, v, n):
    '''
    Recorrido BFS que cuenta la cantidad de vertices que estan a N distancia
    del origen
    Args:
        grafo(Grafo):Conjunto de vertices y aristas
        v(str):Vertice del origen
        n(int):Distancia desde el origen
    Returns:
        can_t(int):Cantidad de vertices a distancia N del origen
    '''
    orden = {}
    cola = deque()
    cola.append(v)
    orden[v] = 0
    cant_n = 0

    while len(cola) != 0:
        w = cola.popleft()
        for x in grafo.adyacentes(w):
            if x in orden:
                continue
            orden[x] = orden[w] + 1
            if orden[x] == n:
                cant_n += 1
            cola.append(x)
            if orden[x] > n:
                return cant_n

    return cant_n

def _n_ciclo(grafo, v, origen, n, visitados, camino):
    """
    Utilizando un algoritmo de backtracking, buscamos los ciclos posibles
    de rango n en el grafo pasado por parametro
    Args:
        grafo(Grafo):Conjunto de vertices y aristas
        v(str):Vertice actual
        origen(str):Vertice de origen
        n(int):Largo del ciclo
        visitados(set):Conjunto de vertices visitados
        camino(list):Recorrido del grafo
    Returns:
        bool:True si existe ciclo de largo n, False si no existe
    """
    visitados.add(v)
    camino.append(v)

    if origen in grafo.adyacentes(v) and len(camino) == n:
        camino.append(origen) #Agregamos el inicio al camino parar cerrar el ciclo
        return True

    for w in grafo.adyacentes(v):
        if w not in visitados:
            if len(camino) > n:
                visitados.remove(v)
                camino.remove(v)
                return False
            if(_n_ciclo(grafo, w, origen, n, visitados, camino)):
                return True
    
    visitados.remove(v)
    camino.remove(v)
    return False


def pagerank(grafo):
    """
    Consiste en calcular el pagerank de los vertices del grafo pasado por parametro
    Args:
        grafo(Grafo):Conjunto de vertices y aristas
    Returns:
        pagerank(dict):Diccionario que contiene el puntaje de todos los vertices del grafo
    """
    vertices = grafo.obtener_vertices()
    shuffle(vertices)
    valor_inicial = (1 - D)/len(vertices)
    pr_vertices = dict()

    for v in vertices:
        pr_vertices[v] = valor_inicial

    grados = grados_vertices(grafo)

    for i in range(MAX_ITERACIONES):
        pr_actual = dict()
        for v in vertices:
            pr_actual[v] = valor_inicial
            for w in grafo.adyacentes(v):
                pr_actual[v] += D * (pr_vertices[w] / grados[w])
        pr_vertices = pr_actual.copy()

    return pr_vertices

def _clustering(grafo, v):
    """
    Calcula el coeficiente de clustering de un vertice en particular,
    si el grado del vertice en cuestion es menor o igual a 1, se considera un 
    coeficiente de 0

    """
    adyacentes_v = grafo.adyacentes(v)
    grado = len(adyacentes_v)
    uniones = 0

    if grado <= 1:
        return 0
    
    cancion = 1
    for w in adyacentes_v:
        for x in adyacentes_v[cancion:]:
            if grafo.estan_unidos(w, x):
                uniones += 1
        cancion += 1
    
    return 2*uniones/(grado*(grado-1))


def clustering_global(grafo):
    """
    Calcula el coeficiente de clustering promedio de todo el grafo
    Args:
        grafo(Grafo):Conjunto de vertices y aristas
    Returns:
        float:Coeficiente de clustering promedio
    """
    total = 0
    for v in grafo:
        total += _clustering(grafo, v)
    return total/len(grafo)

def random_walk(grafo, largo, origen):
    """
    Genera un camino aleatorio en un grafo desde un origen determinado y un largo
    pasado por parametro
    Args:
        grafo(Grafo):Conjunto de vertices y aristas
        largo(int):Largo del camino deseado
        origen(str):Nombre del vertice de origen
    Returns:
        camino(list):Una lista con un camino completamente aleatorio
    """
    v = origen
    
    camino = list()
    camino.append(v)
    for i in range(largo):
        adyacentes = grafo.adyacentes(v)
        shuffle(adyacentes)
        v = adyacentes[0]
        camino.append(v)

    return camino