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
	#lista_a_devolver = []

	#Recorro cada vertice del grafo y lo marco como NO visitado, y sin padre
	for vertice in grafo.obtener_vertices():
		vertices_visitados[vertice] = ESTADO_NO_VISITADO
		padre_vertices[vertice] = None
	cola.append(origen)
	vertices_visitados[origen] = ESTADO_VISITADO
	padre_vertices[origen] = None
	while not len(cola) == 0:
		v_actual = cola.pop()
		vertices_visitados[v_actual] = ESTADO_VISITADO
		for v_ady in grafo.obtener_adyacentes(v_actual):
			if not v_ady in vertices_visitados:
				vertices_visitados[v_ady] = ESTADO_VISITADO
				padre_vertices[v_ady] = v_actual
				cola.append(v_ady)
	return padre_vertices


#############################################
def crear_grafo(archivo):
	"""Crea un grafo a partir de las lineas de un archivo csv"""
	grafo = Grafo()
	with open(archivo,'r') as actores:
		lector = csv.reader(actores)
		for linea in lector:
			actor = linea[0]
			grafo.agregar_vertice(actor)
	return grafo
	

grafo = crear_grafo("actores.csv")
print(grafo)








