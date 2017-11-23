############ IMPLEMENTACION DE FUNCIONES PARA USO EN TP3 ################

from collections import deque
from collections import defaultdict
from grafo import *
import random
import csv

ESTADO_VISITADO = True
ESTADO_NO_VISITADO = False


###################################################################################
#                            RECORRIDOS EN GRAFO                                  #
###################################################################################

def breathfirstsearch(grafo, vertice_origen):
	"""Recorrido del grafo en anchura"""

	vertices_visitados = {}
	padre_vertice = {}
	orden_vertice = {}
	cola = deque()
	cola.append(vertice_origen)
	vertices_visitados[vertice_origen] = ESTADO_VISITADO
	padre_vertice[vertice_origen] = None
	orden_vertice[vertice_origen] = 0
	while not len(cola) == 0:
		nuevo_vertice = cola.pop()
		for v_ady in grafo.obtener_adyacentes(nuevo_vertice):
			if not v_ady in vertices_visitados:
				vertices_visitados[v_ady] = ESTADO_VISITADO
				padre_vertice[v_ady] = nuevo_vertice
				orden_vertice[v_ady] = orden_vertice[nuevo_vertice] + 1
				cola.append(v_ady)
	return orden_vertice #Devuelve esto por conveniencia para usar en actores_a_distancia


def depthfirstsearch(grafo):
	"""Recorrido del grafo en profundidad"""
	vertices_visitados = {}
	padre_vertice = {}
	orden_vertice = {}
	for vertice in grafo.obtener_vertices():
		if vertices_visitados[vertice] == ESTADO_NO_VISITADO:
			padre_vertice[vertice] = None
			orden_vertice[vertice] = 0
			dfs_visitar(grafo, vertice, vertices_visitados, padre_vertice, orden_vertice)


def dfs_visitar(grafo, origen, dicc_vertices_visitados, dicc_padres, dicc_orden):
	"""Funcion auxiliar de depthfirstsearch"""

	dicc_vertices_visitados[origen] = ESTADO_VISITADO
	for v_ady in grafo.obtener_adyacentes(origen):
		if dicc_vertices_visitados[v_ady] == ESTADO_NO_VISITADO:
			dicc_padres[v_ady] = origen
			dicc_orden[v_ady] = dicc_orden[origen] + 1
			dfs_visitar(grafo, v_ady, dicc_vertices_visitados, dicc_padres, dicc_orden)



def camino_minimo(grafo, origen, destino):
	"""Recorre el grafo con BFS para buscar el camino minimo entre dos vertices.
	Devuelve una lista con el camino, None en caso que no haya camino."""

	cola = deque()
	vertices_visitados = {}
	padre_vertices = {}
	orden_vertices = {}
	cola.append(origen)
	vertices_visitados[origen] = ESTADO_VISITADO
	padre_vertices[origen] = None
	orden_vertices[origen] = 0
	while len(cola) > 0:
		v_actual = cola.pop()
		if v_actual == destino:
			return padre_vertices, orden_vertices
		for v_ady in grafo.obtener_adyacentes(v_actual):
			if v_ady not in vertices_visitados:
				vertices_visitados[v_ady] = ESTADO_VISITADO
				padre_vertices[v_ady] = v_actual
				orden_vertices[v_ady] = orden_vertices[v_actual] + 1
				cola.append(v_ady)
	return (padre_vertices,orden_vertices)


###################################################################################
#                                    FUNCIONES                                    #
###################################################################################

def grafo_crear(nombre_archivo):
	"""
	Crea un grafo de conexiones de actores a partir de un archivo de datos.

	PRE: Recibe el nombre de un archivo separado por comas que contenga de lineas:
		actor,pelicula,pelicula,pelicula
		que equivalen a: vertice,arista,arista,arista
	POST: Devuelve un grafo creado a partir de estos datos.
	"""
	grafo = Grafo()
	dicc_pelis = defaultdict(list)  # Para ahorrarse el chequeo de ver si el dic tiene cierto elemento antes de agregar.
	with open(nombre_archivo) as actores:
		lector = csv.reader(actores)
		for linea in lector:
			actor = linea[0]
			peliculas = linea[1:]
			grafo.agregar_vertice(actor)
			for peli in peliculas:
				dicc_pelis[peli].append(actor)
	for pelicula in dicc_pelis:
		for i in range(len(dicc_pelis[pelicula])):
			for j in range(i + 1, len(dicc_pelis[pelicula])):
				grafo.agregar_arista(dicc_pelis[pelicula][i], dicc_pelis[pelicula][j], pelicula)
	return grafo

	# raise NotImplementedError


def camino(grafo, origen, llegada):
	"""
	Devuelve el camino entre un actor de origen y uno de llegada.

	PRE: Recibe el grafo, un actor de origen y un actor de llegada.
	POST: Devuelve una lista ordenada de cadenas (peliculas) para llegar desde
	el origen hasta el final.
	"""
	padresV,ordenV = camino_minimo(grafo, origen, llegada)
	recorrid_min = []
	v_actual = padresV[llegada]
	while v_actual is not None:
		recorrid_min.append(v_actual)
		v_actual = padresV[v_actual]
	#Revierto la lista de recorrido
	recorrido = list(reversed(recorrid_min))
	recorrido.append(llegada)
	if len(recorrido) == 0:
		return
	lista_de_tuplas = []
	for i in range(len(recorrido)-1):
		arista_pelicula = grafo.obtener_aristas(recorrido[i], recorrido[i+1])[0] #Como puede que haya mas de una pelicula, agrego la primera
		lista_de_tuplas.append((recorrido[i],recorrido[i+1],arista_pelicula))
	return lista_de_tuplas
	

	#raise NotImplementedError


def actores_a_distancia(grafo, origen, n):
	"""
	Devuelve los actores a distancia n del recibido.

	PRE: Recibe el grafo, el actor de origen y el numero deseado.
	POST: Devuelve la lista de cadenas (actores) a n pasos del recibido.
	"""

	dicc_orden = breathfirstsearch(grafo, origen)
	act_n_distantes = []
	for vertice in dicc_orden:
		if dicc_orden[vertice] == n:
			act_n_distantes.append(vertice)
	return sorted(act_n_distantes) #Tiene que imprimirlos en orden alfabetico
	#raise NotImplementedError


def popularidad(grafo, actor):
	"""
	Calcula la popularidad del actor recibido.

	PRE: Recibe el grafo y un actor de origen
	POST: Devuelve un entero que simboliza la popularidad: todos los adyacentes
		de los adyacentes del actor, multiplicado por su cantidad de peliculas
	"""
	if actor not in grafo.vertices:
		return 0
	peliculas_actor = set()
	actores_grado_dos = set()
	for actor_adyacente in grafo.vertices[actor]:
		for actor_grado_dos in grafo.vertices[actor_adyacente]:
			actores_grado_dos.add(actor_grado_dos)
		for pelis in grafo.vertices[actor][actor_adyacente]:
			peliculas_actor.add(pelis)
	return len(actores_grado_dos) * len(peliculas_actor)


def similares(grafo, origen, n):
	"""
	Calcula los n actores mas similares al actor de origen y los devuelve en una
	lista ordenada de mayor similitud a menor.

	PRE: Recibe el grafo, el actor de origen, y el n deseado
	POST: Devuelve una lista de los n actores no adyacentes mas similares al
		pedido. La lista no debe contener al actor de origen.
	"""
	cantidad_de_caminatas = 1000
	largo_caminata = 35  # Las cantidades estan son arbitrarias, despues vemos que cantidad va mejor.
	actores_similares = defaultdict(int)

	for i in range(cantidad_de_caminatas):
		actor_actual = origen
		for j in range(largo_caminata):
			actor_actual = random.choice(list(grafo.vertices[actor_actual].keys()))
			actores_similares[actor_actual] += 1

	# Saco el actor de origen de el dict de actores_similares
	if origen in actores_similares:
		del actores_similares[origen]
	# Ordeno en base a la cantidad de veces que la random_walk paso por cada actor.
	lista_similares = sorted(actores_similares, key=actores_similares.get, reverse=True)

	return lista_similares[0:n]

