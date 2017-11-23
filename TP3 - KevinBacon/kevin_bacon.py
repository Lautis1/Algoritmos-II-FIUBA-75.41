###################################################################################
#                            IMPLEMENTACION DE COMANDOS                           #
###################################################################################
from grafo import *
from seis_grados import *
import sys

#Constantes de comandos para no hardcodear
CAMINO_A_KB = "camino_hasta_KB"
KBN = "bacon_number"
KBN_MAYOR_6 = "bacon_number_mayor_a_6"
KBN_INFINITO = "bacon_number_infinito"
KBN_PROMEDIO = "KBN_promedio"
KB_SIMILARES = "similares_a_KB"
KB_POPULARIDAD = "popularidad_contra_KB"
CANT_PELICULAS = "cantidad_peliculas"
CANT_ACTORES = "cantidad_actores"


def camino_hasta_kb(grafo, actor_origen):
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
	if len(actores_kbn_mayor_a_6) == 0:
		print("No existen actores a mas de 6 pasos de Kevin Bacon")
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
print(camino_hasta_KB(grafo, "Bacon Kevin"))
#print(bacon_number(grafo, ""))
#print(bacon_number_mayor_a_6(grafo))

###################################################################################
#                            INTERFAZ PARA EL USUARIO                             #
###################################################################################

def recibir_comandos(grafo, linea = input()):
	"""Recibe el grafo principal y una lista de parametros ingresada por el usuario.
	Efectua las operaciones correspondientes sobre dichos parametros."""
	
	lista_parametros = linea.split()
	if lista_parametros[0] == CAMINO_A_KB:
		camino_hasta_kb(grafo, lista_parametros[1])
	else if lista_parametros[0] == KBN:
		bacon_number(grafo, lista_parametros[1])
	else if lista_parametros[0] == KBN_unMAYOR_6:
		bacon_number_mayor_a_6(grafo)



def main():
	"""Recibe por entrada estandar el nombre del programa y el archivo a abrir.
	Crea el grafo con ese archivo."""
	grafo = grafo_crear(sys.argv[1])


if __name__ == '__main__':
	main()