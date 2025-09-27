// Heleper functions
#include "includes.h"

// - Constantes -
int FALSE = 0;
int TRUE = 1;

// Retorna id da string na lista (negativo se não encontrado)
int strIn(char* str, char** arr) {
    for (int i = 0; strcmp(arr[i], "0") != 0; i++) {
        if (strcmp(str, arr[i]) == 0) {
            return i;
        }
    }
    return -1;
}

// Verifica se a string é numérica (inteira)
// Meio verbose...
int eNumerico(char* str) {
    int pos = 0;
    do {
        // Primeiro, verifica se o primeiro dígito é '+' ou '-' ou numérico
        if (str[pos] >= 48 && str[pos] <= 57) {
            continue;
        }
        else {
            if (pos == 0) {
                if ((str[pos] == '+') || (str[pos] == '-')) {
                    continue;
                }
                else {
                    return FALSE;
                }
            }
            return FALSE;
        }
    } while (str[++pos]);
    return TRUE;
}


// Potenciação de inteiros
int pot(int base, int exp) {
    int val = 1;
    for (int i = 0; i < exp; i++) {
        val *= base;
    }
    return val;
}