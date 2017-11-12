
#include "visitantes.h"


//Funcion de comparacion de IPS
//Devuelve 0 si son iguales, 1 si el primer parametro es mayor al 2do,
//-1 si el segundo parametro es mayor al primero.

int comparar_ips(const char* ip_1, const char* ip_2){

    char** dir_ip1 = split(ip_1, '.');
    char** dir_ip2 = split(ip_2, '.');
    int retorno;
    for(int i=0; i<4; i++){
        if(atoi(dir_ip1[i]) > atoi(dir_ip2[i])){
            retorno = 1;
            break;
        }
        else if(atoi(dir_ip1[i]) < atoi(dir_ip2[i])){
            retorno = -1;
            break;
        }
        else retorno = 0;
    }
    free_strv(dir_ip1);
    free_strv(dir_ip2);

    return retorno;
}

//Wrapper para la funcion de comparacion de ip's
void comparacion(void* dato1, void* dato2){

    comparar_ips(dato1,dato2);
}

bool imprimir_claves(const char* ip, void* dato1, void* dato2){

    printf("\t%s\n", ip);
    return true;
}