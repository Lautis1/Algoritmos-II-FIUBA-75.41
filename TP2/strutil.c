#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "strutil.h"


char** split(const char* str, char sep) {
    if (sep == '\0') return NULL;

    int i = 0;
    int cant_apariciones_sep = 0;
    size_t largo_de_cadena = strlen(str);

    while (str[i] != '\0') {
        if (str[i] == sep) {
            cant_apariciones_sep++;
        }
        i++;
    }

    char** arreglo_de_subcadenas = malloc(sizeof(char*) * (cant_apariciones_sep + 2));
    if (arreglo_de_subcadenas == NULL) return NULL;
    arreglo_de_subcadenas[cant_apariciones_sep + 1] = NULL;

    int pos_act_en_en_arreglo = 0;
    int contador_de_largo_subcadenas = 0;
    for (int j = 0; j <= largo_de_cadena; j++) {
        if (str[j] == sep || str[j] == '\0') {
            char* cad_aux = malloc(sizeof(char) * (contador_de_largo_subcadenas) + 1);
            strncpy(cad_aux, &str[j - contador_de_largo_subcadenas], contador_de_largo_subcadenas);
            cad_aux[contador_de_largo_subcadenas] = '\0';
            arreglo_de_subcadenas[pos_act_en_en_arreglo] = cad_aux;
            pos_act_en_en_arreglo++;
            contador_de_largo_subcadenas = 0;
        }
        else {
            contador_de_largo_subcadenas++;
        }
    }
    return arreglo_de_subcadenas;
}


void free_strv(char* strv[]) {
    int i = 0;
    while (strv[i] != NULL) {
        free(strv[i]);
        i++;
    }
    free(strv);
}