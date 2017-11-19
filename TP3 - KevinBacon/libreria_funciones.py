#LIBRERIA DE FUNCIONES DE GRAFO PARA IMPLEMENTAR PARA TP3

from collections import deque
import grafo
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
	while not cola.__len() == 0:
		nuevo_vertice = cola.pop()
		for v_ady in grafo.vertices[nuevo_vertice].keys():
			if vertices_visitados[v_ady] == ESTADO_NO_VISITADO:
				vertices_visitados[v_ady] = ESTADO_VISITADO
				padre_vertice[v_ady] = nuevo_vertice
				orden_vertice[v_ady] = orden_vertice[nuevo_vertice] + 1
				cola.append(v_ady)


def DepthFirstSearch(grafo):

	vertices_visitados = {}
	padre_vertice = {}
	orden_vertice = {}
	for vertice in grafo.vertices:
		if vertices_visitados[vertice] == ESTADO_NO_VISITADO:
			padre_vertice[vertice] = None
			orden_vertice[vertice] = 0
			dfs_visitar(grafo, vertice, vertices_visitados, padre_vertice, orden_vertice)


def dfs_visitar(grafo,origen, dicc_vertices_visitados, dicc_padres, dicc_orden):

	dicc_vertices_visitados[origen] = ESTADO_VISITADO
	for v_ady in grafo.vertices[origen].keys():
		if dicc_vertices_visitados[v_ady] == ESTADO_NO_VISITADO:
			dicc_padres[v_ady] = origen
			dicc_orden[v_ady] = dicc_orden[origen] + 1
			dfs_visitar(grafo,v_ady,dicc_vertices_visitados,dicc_padres,dicc_orden)