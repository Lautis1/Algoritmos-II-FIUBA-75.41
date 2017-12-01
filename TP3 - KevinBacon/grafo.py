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
		"""Devuelve la info asociada a la arista que une dos vertices"""
		return self.adyacentes[vertice]

class Grafo:
	"""Definicion de la clase Grafo, el cual es del tipo:
	-NO DIRIGIDO
	-NO PESADO.
	"""

	def __init__(self):
		"""Constructor de un grafo vacio. Crea un diccionario de vertices"""
		self.vertices = {}

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
			del self.vertices[vertice_ady][vertice_a_borrar]
		del self.vertices[vertice_a_borrar]
		return True

	def agregar_arista(self, vertice_origen, vertice_destino, nombre):
		"""Une los vertices recibidos mediante una arista a crear, cuyo nombre es
		el nombre recibido"""
		if (vertice_origen and vertice_destino) not in self.vertices:
			return False
		self.vertices[vertice_origen].conectar(vertice_destino, nombre)
		self.vertices[vertice_destino].conectar(vertice_origen, nombre)
		return True

	def eliminar_arista(self, vertice_origen, vertice_destino):
		"""Borra la arista que une los dos vertices recibidos por parametro"""

		if (vertice_origen or vertice_destino) not in self.vertices:
			return False
		# TODO
		del self.vertices[vertice_origen][vertice_destino]
		del self.vertices[vertice_destino][vertice_origen]

	def obtener_adyacentes(self, vertice_padre):
		"""Devuelve una lista de todos los vertices adyacentes al vertice recibido por
		parametro."""
		if vertice_padre not in self.vertices:
			return None
		vertice = self.vertices[vertice_padre]
		return (list(vertice.v_adyacentes()))


	def chequear_adyacencia(self, vertice1, vertice2):
		"""Devuelve un booleano segun si los vertices dados son adyacentes o no"""

		#el grafo no es dirigido, la relacion es bilateral si vertice1 esta conectado
		# con vertice2 entonces vertice 2 esta conectado con vertice 1 seguramente.
		return vertice1 in self.vertices[vertice2]

	def obtener_vertices(self):
		"""Devuelve una lista de todos los vertices presentes en el grafo"""
		return self.vertices.keys()

	def obtener_info_vertice(self, vertice):
		"""Devuelve la informacion de un vertice"""
		return self.vertices[vertice].obtener_info()

	def cantidad_aristas(self):
		"""Cuenta la cantidad de aristas presentes en el grafo"""
		info_vertices = set()
		for vertice in self.vertices:
			for info in self.obtener_info_vertice(vertice):
				info_vertices.add(info)
		return len(info_vertices)

	def info_arista(self, vertice_origen, vertice_destino):
		"""Devuelve la informacion que contiene la arista que une dos vertices"""
		
		vertice1 = self.vertices[vertice_origen]
		vertice2 = self.vertices[vertice_destino]
		return vertice1.obtener_info_conexion(vertice_destino)


	def __str__(self):
		return str(self.vertices)

	def __iter__(self):
		return iter(self.vertices)

	def __len__(self):
		return len(self.vertices)

	def __contains__(self, item):
		return item in self.vertices
