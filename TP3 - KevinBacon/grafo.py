from collections import defaultdict

# ###################################################################################

class Grafo:
    """Inicializacion de la clase Grafo"""

    def __init__(self):
        """Constructor de un grafo vacio. Crea un diccionario de vertices"""
        self.vertices = {}
        self.cantidad_aristas = 0

    def agregar_vertice(self, nombre_vertice):
        """Recibe el grafo y un vertice. Si el vertice no se encuentra en el grafo,
        lo agrega."""
        if nombre_vertice in self.vertices:
            return
        self.vertices[nombre_vertice] = defaultdict(list)

    def eliminar_vertice(self, vertice_a_borrar):
        """Borra el vertice 'vertice_a_borrar' del grafo.
        Ademas, hay que eliminar las aristas que tuviesen al vertice borrado
        como origen o destino"""
        if vertice_a_borrar not in self.vertices:
            return False
        for vertice_ady in self.vertices:
            self.vertices[vertice_ady].pop(vertice_a_borrar)
            self.cantidad_aristas -= 1
        self.vertices.pop(vertice_a_borrar)

    def agregar_arista(self, vertice_origen, vertice_destino, nombre):
        """Une los vertices recibidos mediante una arista a crear, cuyo nombre es
        el nombre recibido"""

        self.vertices[vertice_origen][vertice_destino].append(nombre)
        self.vertices[vertice_destino][vertice_origen].append(nombre)
        self.cantidad_aristas += 1

    def eliminar_arista(self, vertice_origen, vertice_destino):
        """Borra la arista que une los dos vertices recibidos por parametro"""

        if not (vertice_origen or vertice_destino) in self.vertices:
            return
        self.vertices[vertice_origen].pop(vertice_destino)
        self.vertices[vertice_destino].pop(vertice_origen)
        self.cantidad_aristas -= 1

    def obtener_adyacentes(self, vertice_padre):
        """Devuelve una lista de todos los vertices adyacentes al vertice recibido por
        parametro."""
        if not vertice_padre in self.vertices:
        	raise ValueError("Vertice no encontrado")

        return [ady for ady in self.vertices[vertice_padre]]

    def chequear_adyacencia(self, vertice1, vertice2):
        """Devuelve un booleano segun si los vertices dados son adyacentes o no"""

        verticeA = self.vertices[vertice1]
        verticeB = self.vertices[vertice2]
        return verticeA in verticeB and verticeB in verticeA

    def obtener_vertices(self):
        """Devuelve una lista de todos los vertices presentes en el grafo"""
        return self.vertices.keys()

    def obtener_aristas(self, vertice1, vertice2):
        """Devuelve la informacion que contiene la arista que une dos vertices"""
        return self.vertices[vertice1][vertice2]  # sin usar clase arista

    def cantidad_vertices(self):
        """Devuelve la cantidad de vertices presentes en el grafo"""
        return self.cantidad_vertices

    def cantidad_aristas(self):
        """Devuelve la cantidad de aristas presentes en el grafo"""
        return self.cantidad_aristas

    def __str__(self):

        vertices = self.vertices
        return str(vertices)


# ################################################################################
class Arista:  # Hice esta clase xq fede dijo que era recomendable

    def __init__(self, v_origen, v_destino, nombre):
        """Constructor de una Arista que une a los vertices recibidos.
        Dicha arista tiene nombre."""
        self.v_origen = v_origen
        self.v_destino = v_destino
        self.nombre_arista = nombre
        vertices = grafo.vertices

        # Uno los vertices
        vertices[v_origen][v_destino] = nombre
        vertices[v_destino][v_origen] = nombre

    def nombre_arista(self, arista):
        return self.arista.nombre_arista
