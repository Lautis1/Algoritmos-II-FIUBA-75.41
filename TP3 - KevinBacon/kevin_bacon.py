###################################################################################
#                            IMPLEMENTACION DE COMANDOS                           #
###################################################################################
from grafo import *
from seis_grados import *
import sys
from collections import defaultdict

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


###############################################33
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
		return("Actor no encontrado en el grafo")
	if not "Bacon Kevin" in orden_actores:
		print(-1)
	else:
		print("'{}' tiene un Kevin Bacon Number igual a {}.".format(actor_origen, orden_actores["Bacon Kevin"]))

def bacon_number_mayor_a_6(grafo):
	"""Imprime la cantidad de actores a una distancia mayor a 6 pasos de Kevin Bacon.
	De no existir actores a mas pasos que 6, se imprime un mensaje acorde. En este numero no
	influyen la cantidad de actores con KBN infinito."""

	orden = breathfirstsearch(grafo, "Bacon Kevin")
	if len(orden) == 0 : return
	#Invierto el diccionario de orden asi me quedan los ordenes como clave, y como valores todos los actores con ese orden
	orden_inverso = defaultdict(list)
	print("Los actores con un KBN mayor a 6 son: ")
	for orden, actor in orden.items():
		orden_inverso[actor].append(orden)
	#Calculo el maximo orden que hay en el dicc asi recorro de 6 a max+1
	orden_max = max(list(orden_inverso.keys()))
	for i in range(6, orden_max + 1):
		cant_actores = len(orden_inverso[i])
		print("Con KBN igual a {}: {} actores".format(i, cant_actores))
	

def bacon_number_infinito(grafo):
	"""Imprime la cantidad de actores con un KBN infinito. De no haber, se debe imprimir
	un mensaje acorde."""

	#Los actores con KBN infinito son aquellos que no estan conectados con KB, por lo que
	#no aparecerian en el diccionario de ordenes de vertices conectados a KB.
	#Entonces la cantidad total de actores con KBN infinto va a estar dada por la resta
	#entre la cantidad de vertices del Grafo y la cantidad de actores conectados a KB.

	dicc_orden = breathfirstsearch(grafo, "Bacon Kevin")
	print(len(grafo.obtener_vertices()) - len(dicc_orden))


def bacon_number_promedio(grafo):
	"""Imprime el KBN promedio. En este numero no influyen la cantidad de actores con
	KBN infinito, pero si lo hace ek KBN de Kevin Bacon"""

	dicc_orden = breathfirstsearch(grafo, "Bacon Kevin")
	suma = 0
	for orden in dicc_orden.values():
		suma += orden
	print("El Kevin Bacon Number promedio es {0:.2f}".format(suma / len(dicc_orden.values())))


def similares_a_kb(grafo, n):
	"""Imprime una lista de los n actores mas similares a Kevin Bacon, ordenados de 
	mayor similitud a menor."""

	v_origen = "Bacon Kevin"
	lista_similares = similares(grafo, v_origen, n)
	return lista_similares

def popularidad_contra_kb(grafo, actor):
	"""Usando la popularidad de KB como base, imprime en porcentaje cuan popular es el
	actor en comparacion a KB. De no existir el actor ingresado, se debe imprimir 
	un mensaje acorde y devolver None. Tener en cuenta que Kevin Bacon es un
	100% de lo popular que es Kevin Bacon"""

	popularidad_de_kb = popularidad(grafo, "Bacon Kevin")
	popularidad_actor = popularidad(grafo, actor)
	porcentaje = ((popularidad_actor * 100) / popularidad_de_kb)
	print("{} es un {:.2f} % de lo popular que es Kevin Bacon".format(actor,porcentaje))

##ESTADISTICAS

def cantidad_peliculas(grafo):
	"""Imprime la cantidad de peliculas en el DataSet"""

	#Tendriamos que tener una estructura donde las claves sean los actores
	#y sus valores asociados, una lista de peliculas
	#Asi, contamos cantidad de actores y cantidad de peliculas facilmente. ESTO ESTA BIEN PENSADO?

	


def cantidad_actores(grafo):
	"""Imprime la cantidad de actores en el DataSet"""



###################################################################################
#                            INTERFAZ PARA EL USUARIO                             #
###################################################################################

def recibir_comandos(grafo, linea_entrada):
	"""Recibe el grafo principal y una lista de parametros ingresada por el usuario.
	Efectua las operaciones correspondientes sobre dichos parametros."""
	
	if len(linea_entrada) == 0:
		return -1
	elif linea_entrada[0] == CAMINO_A_KB:
		print(camino_hasta_kb(grafo, linea_entrada[1]))
	elif linea_entrada[0] == KBN:
		print(bacon_number(grafo, linea_entrada[1]))
	elif linea_entrada[0] == KBN_MAYOR_6:
		print(bacon_number_mayor_a_6(grafo))
	elif linea_entrada[0] == KB_SIMILARES:
		print(similares_a_kb(grafo, linea_entrada[1]))
	
	
def main():
	"""Recibe por entrada estandar el nombre del programa y el archivo a abrir.
	Crea el grafo con ese archivo."""

	grafo = grafo_crear(sys.argv[1])
	for linea in sys.stdin:
		linea = linea.rstrip().split("'")
		recibir_comandos(grafo, linea)


if __name__ == '__main__':
	main()