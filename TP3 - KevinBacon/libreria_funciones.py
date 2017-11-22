#LIBRERIA DE FUNCIONES DE GRAFO PARA IMPLEMENTAR PARA TP3

from collections import deque
from grafo import *
import csv

ESTADO_VISITADO = True
ESTADO_NO_VISITADO = False

#Recorridos de grafos

def BreathFirstSearch(grafo, vertice_origen):

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
			if vertices_visitados[v_ady] == ESTADO_NO_VISITADO:
				vertices_visitados[v_ady] = ESTADO_VISITADO
				padre_vertice[v_ady] = nuevo_vertice
				orden_vertice[v_ady] = orden_vertice[nuevo_vertice] + 1
				cola.append(v_ady)



def DepthFirstSearch(grafo):

	vertices_visitados = {}
	padre_vertice = {}
	orden_vertice = {}
	for vertice in grafo.obtener_vertices():
		if vertices_visitados[vertice] == ESTADO_NO_VISITADO:
			padre_vertice[vertice] = None
			orden_vertice[vertice] = 0
			dfs_visitar(grafo, vertice, vertices_visitados, padre_vertice, orden_vertice)


def dfs_visitar(grafo,origen, dicc_vertices_visitados, dicc_padres, dicc_orden):

	dicc_vertices_visitados[origen] = ESTADO_VISITADO
	for v_ady in grafo.obtener_adyacentes(origen):
		if dicc_vertices_visitados[v_ady] == ESTADO_NO_VISITADO:
			dicc_padres[v_ady] = origen
			dicc_orden[v_ady] = dicc_orden[origen] + 1
			dfs_visitar(grafo,v_ady,dicc_vertices_visitados,dicc_padres,dicc_orden)

#CAMINO MINIMO = DIJKSTRA

def camino_minimo(grafo, origen, destino):

	"""Recorre el grafo con BFS para buscar el camino minimo entre dos vertices.
	Devuelve una lista con el camino, None en caso que no haya camino."""

	cola = deque()
	vertices_visitados = {}
	padre_vertices = {}
	orden_vertices = {}

	#Recorro cada vertice del grafo y lo marco como NO visitado, y sin padre
	for vertice in grafo.obtener_vertices():
		vertices_visitados[vertice] = ESTADO_NO_VISITADO
		padre_vertices[vertice] = None
	cola.append(origen)
	vertices_visitados[origen] = ESTADO_VISITADO
	padre_vertices[origen] = None
	orden_vertices[origen] = 0
	while len(cola) > 0:
		v_actual = cola.pop()
		if v_actual == destino: return padre_vertices,orden_vertices
		for v_ady in grafo.obtener_adyacentes(v_actual):
			if not v_ady in vertices_visitados:
				vertices_visitados[v_ady] = ESTADO_VISITADO
				padre_vertices[v_ady] = v_actual
				orden_vertices[v_ady] = orden_vertices[v_actual] + 1
				cola.append(v_ady)
	return padre_vertices,orden_vertices


#############################################

def grafo_crear(nombre_archivo):
    """
    Crea un grafo de conexiones de actores a partir de un archivo de datos.

    PRE: Recibe el nombre de un archivo separado por comas que contenga de lineas:
        actor,pelicula,pelicula,pelicula
        que equivalen a: vertice,arista,arista,arista
    POST: Devuelve un grafo creado a partir de estos datos.
    """
    grafo = Grafo()
    dicc_pelis = {}
    lista_actores = []
    with open(nombre_archivo) as actores:
    	lector = csv.reader(actores)
    	for linea in lector:
    		actor = linea[0]
    		peliculas = linea[1:]
    		grafo.agregar_vertice(actor)
    		for peli in peliculas:
    			if not peli in dicc_pelis:
    				lista_actores.append(actor)
    				dicc_pelis[peli] = lista_actores
    			else:
    				dicc_pelis[peli] += actor
    			for i in range(len(lista_actores)-1):
    				grafo.agregar_arista(lista_actores[i], lista_actores[i+1], peli)
    return grafo
    
    #raise NotImplementedError
dicc = grafo_crear("test.csv")
print(dicc)

#print(dicc_peliculas)

'''def camino(grafo, origen, llegada):
    """
    Devuelve el camino entre un actor de origen y uno de llegada.

    PRE: Recibe el grafo, un actor de origen y un actor de llegada.
    POST: Devuelve una lista ordenada de cadenas (películas) para llegar desde
        el origen hasta el final.
    """

    padres_v, orden_v = camino_minimo(grafo,origen,llegada)
    recorrido_minimo = []
    v_actual = padres_v.get(llegada) #Devuelve ady de "llegada" o None si no tiene

    raise NotImplementedError


def actores_a_distancia(grafo, origen, n):
    """
    Devuelve los actores a distancia n del recibido.

    PRE: Recibe el grafo, el actor de origen y el número deseado.
    POST: Devuelve la lista de cadenas (actores) a n pasos del recibido.
    """
    raise NotImplementedError


def popularidad(grafo, actor):
    """
    Calcula la popularidad del actor recibido.

    PRE: Recibe el grafo y un actor de origen
    POST: Devuelve un entero que simboliza la popularidad: todos los adyacentes
        de los adyacentes del actor, multiplicado por su cantidad de peliculas
    """
    raise NotImplementedError


def similares(grafo,origen, n):
    """
    Calcula los n actores más similares al actor de origen y los devuelve en una
    lista ordenada de mayor similitud a menor.

    PRE: Recibe el grafo, el actor de origen, y el n deseado
    POST: Devuelve una lista de los n actores no adyacentes más similares al
        pedido. La lista no debe contener al actor de origen.
    """
    raise NotImplementedError'''






