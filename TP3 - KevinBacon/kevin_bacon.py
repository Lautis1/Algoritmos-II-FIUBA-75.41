###################################################################################
#                            IMPLEMENTACION DE COMANDOS                           #
###################################################################################
from grafo import *
from seis_grados import *


def camino_hasta_KB(grafo, actor_origen):
	"""Imprime el camino mas corto con el cual se llega desde cualquier actor
	hasta Kevin Bacon. De no haber camino posible, se debe imprimir un mensaje
	acorde (por ejemplo: "No hay conexion entre KB y el actor") y devolverse
	una lista vacia, y de no existir el actor ingresado, se debe imprimir un
	mensaje acorde"""

	camino_a_kb = camino(grafo, actor_origen, "Bacon Kevin")
	if camino_a_kb == None:
		print("No hay conexión entre Kevin Bacon y '{}'".format(actor_origen))
		return []
	for dato in camino_a_kb:
		print("'{}' actuó con '{}' en '{}'.".format(dato[0], dato[1], dato[2]))

def bacon_number(grafo, actor_origen):
	"""Imprime el Kevin Bacon Number del actor recibido. Para representar un
	KBN infinito (no hay conexion entre KB y el actor), el KBN es -1, y de
	no existir el actor ingresado, se debe imprimir un mensaje acorde.
	Tener en cuenta que el KBN de Kevin Bacon es 0"""

	padres, orden_actores = camino_minimo(grafo, actor_origen, "Bacon Kevin")
	if not actor_origen in grafo.obtener_vertices():
		raise ValueError("Actor no encontrado en el grafo")
	if not "Bacon Kevin" in orden_actores:
		print(-1)
	else:
		print("'{}' tiene un Kevin Bacon Number igual a {}".format(actor_origen, orden_actores["Bacon Kevin"]))

def bacon_number_mayor_a_6(grafo):
	"""Imprime la cantidad de actores a una distancia mayor a 6 pasos de Kevin Bacon.
	De no existir actores a mas pasos que 6, se imprime un mensaje acorde. En este numero no
	influyen la cantidad de actores con KBN infinito."""

	orden = breathfirstsearch(grafo, "Bacon Kevin")
	actores_kbn_mayor_a_6 = []
	for vertice in orden:
		if orden[vertice] > 6:
			actores_kbn_mayor_a_6.append(vertice)
	return sorted(actores_kbn_mayor_a_6)

def bacon_number_infinito(grafo):
	"""Imprime la cantidad de actores con un KBN infinito. De no haber, se debe imprimir
	un mensaje acorde."""

def bacon_number_promedio(grafo):
	"""Imprime el KBN promedio. En este numero no influyen la cantidad de actores con
	KBN infinito, pero si lo hace ek KBN de Kevin Bacon"""

def similares_a_kb(grafo, n):
	"""Imprime una lista de los n actores mas similares a Kevin Bacon, ordenados de 
	mayor similitud a menor."""

def popularidad_contra_kb(grafo, actor):
	"""Usando la popularidad de KB como base, imprime en porcentaje cuan popular es el
	actor en comparacion a KB. De no existir el actor ingresado, se debe imprimir 
	un mensaje acorde y devolver None. Tener en cuenta que Kevin Bacon es un
	100% de lo popular que es Kevin Bacon"""

##ESTADISTICAS

def cantidad_peliculas(grafo):
	"""Imprime la cantidad de peliculas en el DataSet"""

def cantidad_actores(grafo):
	"""Imprime la cantidad de actores en el DataSet"""


		


grafo = grafo_crear("test.csv")
#print(camino_hasta_KB(grafo, "Eastwood Clint"))
#print(bacon_number(grafo, ""))
print(bacon_number_mayor_a_6(grafo))
