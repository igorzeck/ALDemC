// Heleper functions
#include "includes.h"

// - Constantes -
const int FALSE = 0;
const int TRUE = 1;
const int MAX_LINHA_ENTS = 32;
const int MAX_TEXT = 255;
const int MAX_TEXT_BUFFER = 255;

const int LISTA = 1;
const int NUMERO = 2;
const int TEXTO = 3; // TODO: Usar

// Retorna id da string na lista (negativo se não encontrado)
int strEmArr(char* str, char** arr) {
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

void matrizLimpar(char** matriz, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        free((matriz)[i]);
    }
    free(matriz);
}

void matrizPrintar(char** matriz, int tam) {
    for (int i = 0; i < tam && matriz[i]; i++) {
        printf("(%d: '%s') ", i, matriz[i]);
    }
}

void intArrPrintar(int* arr, int tam) {
    for (int i = 0; i < tam; i++) {
        printf("(%d: %d)", i, arr[i]);
    }
}