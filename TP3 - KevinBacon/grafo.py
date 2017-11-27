from collections import defaultdict

# ###################################################################################


class Grafo:
    """Inicializacion de la clase Grafo"""
    # Esta no es la inicializacion, eso es en el __init__. 
    # Agreguen la descripcion de que se trata de un grafo no pesado, no dirigido

    def __init__(self):
        """Constructor de un grafo vacio. Crea un diccionario de vertices"""
        self.vertices = {}
        # Ahora self.vertices es un diccionario de diccionarios de sets.
        # Elimine cantidad de aristas, no sirve, cantidad de aristas != cantidad de peliculas.

    def agregar_vertice(self, nombre_vertice):
        """Recibe el grafo y un vertice. Si el vertice no se encuentra en el grafo,
        lo agrega."""
        if nombre_vertice in self.vertices:
            return
            # Creo que podrían devolver true/false, o bien lanzar una excepcion, pero que no pase desapercibido
        self.vertices[nombre_vertice] = defaultdict(set)

    def eliminar_vertice(self, vertice_a_borrar):
        """Borra el vertice 'vertice_a_borrar' del grafo.
        Ademas, hay que eliminar las aristas que tuviesen al vertice borrado
        como origen o destino"""
        if vertice_a_borrar not in self.vertices:
            return
            # idem anterior
        for vertice_ady in self.vertices[vertice_a_borrar]:
            del self.vertices[vertice_ady][vertice_a_borrar]
        del self.vertices[vertice_a_borrar]

    def agregar_arista(self, vertice_origen, vertice_destino, nombre):
        """Une los vertices recibidos mediante una arista a crear, cuyo nombre es
        el nombre recibido"""

        self.vertices[vertice_origen][vertice_destino].add(nombre)
        self.vertices[vertice_destino][vertice_origen].add(nombre)
        # Aca que estan haciendo? permiten que haya mas de una arista a la vez entre dos pares de vertices?
        # O le estan dando informacion adicional a la arista? 

    def eliminar_arista(self, vertice_origen, vertice_destino):
        """Borra la arista que une los dos vertices recibidos por parametro"""

        if vertice_origen not in self.vertices or vertice_destino not in self.vertices:
            return
        del self.vertices[vertice_origen][vertice_destino]
        del self.vertices[vertice_destino][vertice_origen]
        # Haciendo esto, estan diciendo que el grafo no acepta mas de una arista entre pares de vertices
        # (porque se eliminan todas). Entonces el grafo esta sabiendo "que es" esa informacion adicional...

    def obtener_adyacentes(self, vertice_padre):
        """Devuelve una lista de todos los vertices adyacentes al vertice recibido por
        parametro."""
        if vertice_padre not in self.vertices:
            return

        return self.vertices[vertice_padre].keys()

    def chequear_adyacencia(self, vertice1, vertice2):
        """Devuelve un booleano segun si los vertices dados son adyacentes o no"""

        # Hace falta el chequeo doble?, el grafo no es dirigido, la relacion es bilateral si vertice1 esta conectado
        # con vertice2 entonces vertice 2 esta conectado con vertice 1 seguramente.
        # MB: No debería ser necesario el chequeo, justamente por ser no dirigido. 
        return vertice1 in self.vertices[vertice2]

    def obtener_vertices(self):
        """Devuelve una lista de todos los vertices presentes en el grafo"""
        return self.vertices.keys()

    def obtener_aristas(self, vertice1, vertice2):
        """Devuelve la informacion que contiene la arista que une dos vertices"""
        return self.vertices[vertice1][vertice2]

    def __str__(self):
        return str(self.vertices)

    # Podrian agregar (que no cuesta nada):
    # - Un metodo para ver si un vertice esta en el grafo. Pueden hacerlo escribiendo el metodo __contains__, entonces pueden hacer:
    #       if "Kevin Bacon" not in grafo:
    #           raise KevinBaconNoPuedeFaltarException()
    # - Un iterador para el grafo. Escribiendo el metodo __iter__ pueden devolver un iterador para el grafo, y hacer:
    #       for vertice in grafo: ...
    #   Además sería muy sencillo porque pueden simplemente devolver un iterador para el diccionario de vertices. 
    # - Tambien podrian agregarle al grafo poder obtener la cantidad de vertices (incluso, escribiendo __len__)
    
    
