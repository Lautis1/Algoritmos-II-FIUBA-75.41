##IMPLEMENTACION DE LA CLASE GRAFO.
#EMPIEZO COMO DICCIONARIO DE DICCIONARIOS, SI PREFERIS OTRA COSA MODIFICAMOS

class Grafo:

	"""Inicializacion de la clase Grafo"""
	def __init__(self):
		"""Constructor de un grafo vacio. Crea un diccionario de vertices"""
		self.vertices = {}

	def agregar_vertice(self, nombre_vertice):
		"""Recibe el grafo y un vertice. Si el vertice no se encuentra en el grafo, 
		lo agrega."""
		if nombre_vertice in self.vertices:
			return
		self.vertices[nombre_vertice] = {}

	def eliminar_vertice(self, vertice_a_borrar):
		"""Borra el vertice 'vertice_a_borrar' del grafo.
		Ademas, hay que eliminar las aristas que tuviesen al vertice borrado
		como origen o destino"""
		if not vertice_a_borrar in self.vertices:
			return
		for vertice_ady in self.vertices[vertice_a_borrar].keys():
			del(self.vertices[vertice_ady])
		del(self.vertices[vertice_a_borrar])

	def agregar_arista(self, vertice_origen, vertice_destino, nombre):
		"""Une los vertices recibidos mediante una arista a crear, cuyo nombre es
		el nombre recibido"""

		self.vertices[vertice_origen] = Arista(vertice_origen, vertice_destino, nombre)


class Arista:

	def __init__(self, v_origen, v_destino, nombre_arista):
		"""Constructor de una Arista que une a los vertices recibidos.
		Dicha arista tiene nombre."""
		self.v_origen = v_origen
		self.v_destino = v_destino
		self.nombre_arista = nombre_arista

		#Uno los vertices
		self.vertices[v_origen][v_destino] = nombre_arista
		self.vertices[v_destino][v_origen] = nombre_arista

	def nombre_arista(self, arista):

		return self.arista.nombre_arista




