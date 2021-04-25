def upheap(vector, hijo, cmp):
    if(hijo == 0): return
    padre = (hijo - 1) // 2
    if(cmp(vector[hijo], vector[padre]) > 0):
        vector[hijo], vector[padre] = vector[padre], vector[hijo]
        upheap(vector, padre, cmp)

def hallar_reemplazo(datos, tam, padre, izq, der, cmp):
    if(izq < tam and cmp(datos[izq], datos[padre]) > 0):
        padre = izq
    if(der < tam and cmp(datos[der], datos[padre]) > 0):
        padre = der
    return padre

def downheap(vector, padre, tam, cmp):
    if(padre == tam): return
    hijo_izq = (2 * padre) + 1
    hijo_der = (2 * padre) + 2

    reemplazo = hallar_reemplazo(vector, tam, padre, hijo_izq, hijo_der, cmp)
    
    if(reemplazo != padre):
        vector[padre], vector[reemplazo] = vector[reemplazo], vector[padre]
        downheap(vector, reemplazo, tam,cmp)

def heapify(vector, cmp):
    '''
    Aplica el algoritmo heapify a un arreglo pasado por parametro
    Complejidad: O(n)
    '''
    n = len(vector)
    m = n//2 - 1
    for i in range(m, -1, -1):
        downheap(vector, i, n,cmp)

def heapsort(vector, cmp):
    '''
    Aplica el algoritmo de ordenamiento comparativo al vector pasado por parametro
    Complejidad: O(n log n)
    '''
    heapify(vector, cmp)
    for i in range(len(vector)-1, 0, -1):
        vector[i], vector[0] = vector[0], vector[i]
        downheap(vector, 0, i,cmp)
    
class Heap:
    '''
    Crea un heap con los siguientes datos:
    cmp(function): Funcion de comparacion
    lista(list): Puede ser una lista con o sin elementos, si tiene elementos le aplicamos heapify
    '''
    def __init__(self, cmp, lista = list()):
        self.datos = lista
        self.cmp = cmp
        if(len(lista) != 0):
            heapify(self.datos, self.cmp)

    def __len__(self):
        return len(self.datos)

    def heap_esta_vacio(self):
        '''
        Verifica si el heap esta vacio
        '''
        return len(self) == 0
    
    def heap_ver_max(self):
        '''
        Devuelve el tope del heap
        '''
        if(self.heap_esta_vacio()): return None
        return self.datos[0]

    def heap_encolar(self, dato):
        '''
        Encola un elemento en el heap
        '''
        self.datos.append(dato)
        upheap(self.datos, len(self) - 1, self.cmp)

    def heap_desencolar(self):
        '''
        Desencola un elemento del heap
        '''
        if(self.heap_esta_vacio()): return None
        dato = self.heap_ver_max()
        self.datos[0], self.datos[len(self)-1] =  self.datos[len(self)-1], self.datos[0]
        self.datos.pop()
        downheap(self.datos, 0, len(self), self.cmp)
        return dato

    def __str__(self):
        string = f"{self.datos}"
        return string