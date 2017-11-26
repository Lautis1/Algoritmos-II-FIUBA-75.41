from collections import defaultdict

# ###################################################################################


class Grafo:
    """Inicializacion de la clase Grafo"""

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
        self.vertices[nombre_vertice] = defaultdict(set)

    def eliminar_vertice(self, vertice_a_borrar):
        """Borra el vertice 'vertice_a_borrar' del grafo.
        Ademas, hay que eliminar las aristas que tuviesen al vertice borrado
        como origen o destino"""
        if vertice_a_borrar not in self.vertices:
            return
        for vertice_ady in self.vertices[vertice_a_borrar]:
            del self.vertices[vertice_ady][vertice_a_borrar]
        del self.vertices[vertice_a_borrar]

    def agregar_arista(self, vertice_origen, vertice_destino, nombre):
        """Une los vertices recibidos mediante una arista a crear, cuyo nombre es
        el nombre recibido"""

        self.vertices[vertice_origen][vertice_destino].add(nombre)
        self.vertices[vertice_destino][vertice_origen].add(nombre)

    def eliminar_arista(self, vertice_origen, vertice_destino):
        """Borra la arista que une los dos vertices recibidos por parametro"""

        if vertice_origen not in self.vertices or vertice_destino not in self.vertices:
            return
        del self.vertices[vertice_origen][vertice_destino]
        del self.vertices[vertice_destino][vertice_origen]

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
        return vertice1 in self.vertices[vertice2]

    def obtener_vertices(self):
        """Devuelve una lista de todos los vertices presentes en el grafo"""
        return self.vertices.keys()

    def obtener_aristas(self, vertice1, vertice2):
        """Devuelve la informacion que contiene la arista que une dos vertices"""
        return self.vertices[vertice1][vertice2]

    def __str__(self):
        return str(self.vertices)
