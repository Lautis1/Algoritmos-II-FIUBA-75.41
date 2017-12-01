from collections import defaultdict


# ###################################################################################
class Vertice:
    """Definicion de la clase Vertice"""

    def __init__(self, nombre_vertice, info_extra):
        """Inicializacion de un vertice"""
        self.nombre = nombre_vertice
        self.adyacentes = defaultdict(list)
        self.info = info_extra

    def v_adyacentes(self):
        """Devuelve los vertices adyacentes al vertice recibido."""
        return list(self.adyacentes.keys())

    def conectar(self, n_vertice, info):
        """Agrega la informacion 'info' al vertice adyacente al n_vertice"""
        self.adyacentes[n_vertice].append(info)

    def obtener_info(self):
        """Devuelve la informacion asociada al vertice"""
        return self.info

    def obtener_info_conexion(self, vertice):
        """Devuelve la info asociada a la arista que une el vertice
         con el vertice pasado por parametro"""
        return self.adyacentes[vertice]

    def eliminar_adyacente(self, vertice):
        if vertice not in self.adyacentes:
            return False
        del self.adyacentes[vertice]


class Grafo:
    """Definicion de la clase Grafo, el cual es del tipo:
    -NO DIRIGIDO
    -NO PESADO.
    """

    def __init__(self):
        """Constructor de un grafo vacio. Crea un diccionario de vertices"""
        self.vertices = {}

    def __str__(self):
        return str(self.vertices)

    def __iter__(self):
        return iter(self.vertices)

    def __len__(self):
        return len(self.vertices)

    def __contains__(self, item):
        return item in self.vertices

    def agregar_vertice(self, nombre_vertice, info):
        """Recibe el grafo y un vertice. Si el vertice no se encuentra en el grafo,
        lo agrega."""
        if nombre_vertice in self.vertices:
            return False
        self.vertices[nombre_vertice] = Vertice(nombre_vertice, info)
        return True

    def eliminar_vertice(self, vertice_a_borrar):
        """Borra el vertice 'vertice_a_borrar' del grafo.
        Ademas, hay que eliminar las aristas que tuviesen al vertice borrado
        como origen o destino"""
        if vertice_a_borrar not in self.vertices:
            return False
        for vertice_ady in self.vertices[vertice_a_borrar].v_adyacentes():
            self.vertices[vertice_ady].eliminar_adyacente(vertice_a_borrar)
        del self.vertices[vertice_a_borrar]
        return True

    def agregar_arista(self, vertice_origen, vertice_destino, informacion):
        """Une los vertices recibidos mediante una arista con la informacion pasada por parametro,
        en caso de que los dos vertices ya esten conectados, solo se le agregara la informacion
        a la arista ya existente"""
        if (vertice_origen or vertice_destino) not in self.vertices:
            return False
        self.vertices[vertice_origen].conectar(vertice_destino, informacion)
        self.vertices[vertice_destino].conectar(vertice_origen, informacion)
        return True

    def eliminar_arista(self, vertice_origen, vertice_destino):
        """Borra la arista que une los dos vertices recibidos por parametro y
        toda la informacion asociada a ella"""

        if (vertice_origen or vertice_destino) not in self.vertices or not self.chequear_adyacencia(vertice_origen, vertice_destino):
            return False

        del self.vertices[vertice_origen][vertice_destino]
        del self.vertices[vertice_destino][vertice_origen]

    def obtener_adyacentes(self, vertice_padre):
        """Devuelve una lista de todos los vertices adyacentes al vertice recibido por
        parametro."""
        if vertice_padre not in self.vertices:
            return False
        vertice = self.vertices[vertice_padre]
        return list(vertice.v_adyacentes())

    def chequear_adyacencia(self, vertice1, vertice2):
        """Devuelve un booleano segun si los vertices dados son adyacentes o no"""
        if (vertice_origen or vertice_destino) not in self.vertices:
            raise NameError

        # el grafo no es dirigido, la relacion es bilateral si vertice1 esta conectado
        # con vertice2 entonces vertice 2 esta conectado con vertice 1 seguramente.
        return vertice1 in (self.vertices[vertice2].v_adyacentes())

    def obtener_vertices(self):
        """Devuelve una lista de todos los vertices presentes en el grafo"""
        return self.vertices.keys()

    def obtener_info_vertice(self, vertice):
        """Devuelve la informacion de un vertice"""
        if vertice not in self.vertices:
            return False
        return self.vertices[vertice].obtener_info()

    def info_arista(self, vertice_origen, vertice_destino):
        """Devuelve la informacion que contiene la arista que une dos vertices"""
        if (vertice_origen or vertice_destino) not in self.vertices:
            return False
        vertice1 = self.vertices[vertice_origen]
        return vertice1.obtener_info_conexion(vertice_destino)
