#ifndef ALGOS_GITHUB_VISITANTES_H
#define ALGOS_GITHUB_VISITANTES_H

#include "tp2.h"
/************************************************************************************/
//Funcion de comparacion de IPS
//Devuelve 0 si son iguales, 1 si el segundo parametro es mayor al primero,
//-1 si el primer parametro es mayor al segundo.(Los valores de retono son por
//un tema de convencion a la hora de imprmir las ip's en orden creciente).
int comparar_ips(const char* ip_1, const char* ip_2);

//Funcion que imprime una ip dada por parametro.
bool imprimir_claves(const char* ip, void* dato1, void* dato2);

#endif //ALGOS_GITHUB_VISITANTES_H
