// Header para o parser (leitor) da interface interativa
#include "lista.h"
#include "functionals.h"

// -- Constantes --
char* OPS[] = {
    "+", // Costura - 0
    ":", // Sequência - 1
    // "=", // Atribuição - 2 
    "0"
};

int MAX_ENTS = 10;
int MAX_TEXT = 255;
// -- Variávies --
// TEM QUE SER APAGADO

Lista pilha[50];  // Pilha de listas

// -- Funções --
// Faz o parser de uma dada linha
void parser(char* linha) {
    // Nome das entidades na linha (10 entidades de nomes de MAX_TEXT caracteres)
    char texto_ents[MAX_ENTS][MAX_TEXT];

    char* token = strtok(linha," ");
    int cur_id = 0;
    char* curr_op = NULL;
    while (token != NULL) {
        // Verifica se o token é operação
        int _is_op = str_is_in(token, OPS);
        if (_is_op > 0) {
            curr_op = token;
            token = strtok(NULL, " ");
            continue;
        }
        // Se houver uma operação na fla
        strcpy(texto_ents[cur_id++], token);
        // Se houver uma operação na fila ela é incluida
        if (curr_op != NULL) {
            strcpy(texto_ents[cur_id++], curr_op);
            curr_op = NULL;
        }
        token = strtok(NULL, " ");
    }

    // Leitura dos elementos
    int aux_op = -1;
    Lista* ent_final = (Lista*)malloc(sizeof(Lista));
    strcpy(ent_final->nome, "Out");
    ent_final->raiz = NULL;
    while (cur_id-- > 0) {
        char curr_texto[MAX_TEXT];
        strcpy(curr_texto, texto_ents[cur_id]);
        if (aux_op < 0) {
            aux_op = str_is_in(curr_texto, OPS);
            if (aux_op >= 0) {
                continue;
            }
        }

        Lista* temp_l = listificar(curr_texto, "0");

        switch (aux_op)
        {
        case 0: // +
            if (ent_final->raiz == NULL) {
                ent_final = temp_l;
            }
            else {
                lista_costurar(ent_final, *temp_l);
            }
            aux_op = -1;
            break;
        case 1: // :
            aux_op = -1;
            break;
        default:
            if (ent_final->raiz == NULL) {
                ent_final = temp_l;
            }
            break;
        }
    }
    lista_print(*ent_final);
}

void interface() {
    char linha[200];
    while (strcmp(linha, "quit")) {
        printf("> ");
        // Usa esse regex do scanset (%) 
        // para ler linha até um newline ser encontrado:
        scanf("%[^\n]", linha);
        
        // Split em 'C' (feito por espaço por agora)
        // Talvez vá ter que ser feito na mão...
        parser(linha);

        // printf("%s\n", linha);
        printf("\n");

        // Consome o newline que sobrou
        char c;
        scanf("%c", &c);
    }
}
