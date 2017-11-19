#include "visitantes.h"
/********************************************************************************/

int comparar_ips(const char* ip_1, const char* ip_2){

    char** dir_ip1 = split(ip_1, '.');
    char** dir_ip2 = split(ip_2, '.');
    int retorno;
    for(int i=0; i<4; i++){
        if(atoi(dir_ip1[i]) < atoi(dir_ip2[i])){
            retorno = -1;
            break;
        }
        else if(atoi(dir_ip1[i]) > atoi(dir_ip2[i])){
            retorno = 1;
            break;
        }
        else retorno = 0;
    }
    free_strv(dir_ip1);
    free_strv(dir_ip2);

    return retorno;
}

bool imprimir_claves(const char* ip, void* dato1, void* dato2){
    
    printf("\t%s\n", ip);
    return true;
}

