// Heleper functions
#include "includes.h"

// Retorna id da string na lista (negativo se não encontrado)
int str_is_in(char* str, char** arr) {
    for (int i = 0; strcmp(arr[i], "0") != 0; i++) {
        if (strcmp(str, arr[i]) == 0) {
            return i;
        }
    }
    return -1;
}