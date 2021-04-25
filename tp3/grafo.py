class Grafo:
    '''
    Crea un determinado grafo dado los siguientes parametros:
        vertices(list):Elemetos a utilizar como vertices(opcional)
        dirigido(bool): Indica si el grafo es dirigido o no
    '''
    def __init__(self, vertices = list(), dirigido = False):
        self.vertices = {v:dict() for v in vertices}
        self.dirigido = dirigido
    
    def __contains__(self, v):
        return v in self.vertices

    def __len__(self):
        return len(self.vertices)

    def __iter__(self):
        return iter(self.vertices)

    def validar_vertice(self, v):
        '''
        Verifica si el vertice pasado por parametro existe dentro del grafo
        '''
        if not v in self.vertices:
            raise ValueError(f"No existe el vertice {v} en el grafo")

    def agregar_vertice(self, v):
        '''
        Agrega un vertice al grafo, en caso de que ya exista en el grafo se levanta una excepcion
        '''
        if v in self.vertices:
            raise ValueError(f"Ya existe el vertice{v} en el grafo")
        self.vertices[v] = dict()
    
    def borrar_vertice(self, v):
        '''
        Borra un vertice del grafo y todas las aristas que se relacionen con el vertice en cuestion.
        En caso de que no exista el vertice en el grafo, se levanta una excepcion
        '''
        self.validar_vertice(v)
        for w in self.vertices:
            if v in self.vertices[w]:
                del self.vertices[w][v]
        del self.vertices[v]
    
    def estan_unidos(self, v, w):
        '''
        Verifica si existe una arista entre v y w.
        En caso de que alguno de los vertices no exista en el grafo, se levanta una excepcion
        '''
        self.validar_vertice(v)
        self.validar_vertice(w)
        return w in self.vertices[v]

    def agregar_arista(self, v, w, peso = 0):
        '''
        Agregar un arista entre v y w, tambien se recibe el peso por parametro(opcional).
        En caso de que los vertices ya esten unidos, se levanta una excepcion
        '''
        if self.estan_unidos(v, w):
            raise ValueError(f"Los vertices {v} y {w} ya poseen una aristas en comun")
        self.vertices[v][w] = peso
        if not self.dirigido:
            self.vertices[w][v] = peso
    
    def borrar_arista(self, v, w):
        if not self.estan_unidos(v,w):
            raise ValueError(f"No se puede borrar la arista entre {v} y {w} porque no existe")
        del self.vertices[v][w]
        if not self.dirigido:
            del self.vertices[w][v]
    
    
    def peso_arista(self, v, w):
        '''
        Devuelve el peso de la arista que une a v y w.
        En caso de que no exista arista entre ellos, se levanta una excepcion
        '''
        if not self.estan_unidos(v,w):
            raise ValueError(f"No se puede obtener el peso entre {v} y {w} porque no existe arista entre ellos")
        return self.vertices[v][w]

    def obtener_vertices(self):
        '''
        Devuelve todos los vertices dentro del grafo
        '''
        return list(self.vertices.keys())
    
    def obtener_vertice_aleatorio(self):
        '''
        Devuelve un vertice aleatorio del grafo 
        '''
        return self.obtener_vertices()[0]
    
    def adyacentes(self, v):
        '''
        Devuelve los adyacentes del vertice v.
        En caso que no exista el vertice en el grafo se levanta una excepcion
        '''
        self.validar_vertice(v)
        return list(self.vertices[v].keys())
    
    def __str__(self):
        string = ""
        for v in self.vertices:
            string += f"{v}" 
            for w in self.adyacentes(v):
                string += f" >>> {w}"
            string += "\n"
        return string
        