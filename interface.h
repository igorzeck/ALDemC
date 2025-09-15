// Header para o parser (leitor) da interface interativa
#include "lista.h"
#include "functionals.h"

// -- Constantes --

char* OPS[] = {
    "+", // Costura - 0
    ":", // Sequência - 1
    "=", // Atribuição - 2 
    "0"
};

const int MAX_LINHA_ENTS = 10;
const int MAX_TEXT = 255;

// -- Funções --
// Faz o parser de uma dada linha
// Fazer com que retorne texto talvez?
void parser(char* linha) {
    // Entidades de texto na linha (10 entidades de nomes de MAX_TEXT caracteres)
    char texto_ents[MAX_LINHA_ENTS][MAX_TEXT];

    char* token = strtok(linha," ");
    int cur_ent = 0;
    while (token != NULL) {
        strcpy(texto_ents[cur_ent++], token);
        token = strtok(NULL, " ");
    }

    // Leitura dos elementos
    int aux_op = -1;
    Lista* ent_final = (Lista*)malloc(sizeof(Lista));
    char ent_texto_final[MAX_TEXT];
    strcpy(ent_final->nome, "Out");
    ent_final->raiz = NULL;
    
    // Quantidade de listas inseridas na leitura dessa linha (sequencialmente)
    int cont_lista = 0;

    while (cur_ent-- > 0) {
        char curr_texto[MAX_TEXT];
        Lista* temp_l;
        strcpy(curr_texto, texto_ents[cur_ent]);

        if (aux_op < 0) {
            aux_op = str_is_in(curr_texto, OPS);
            if (aux_op >= 0) {
                continue;
            }
        }

        // Verifica se é uma entidade
        // TODO: Fazer retornar o texto das listas!
        int arr_id = arr_contem(curr_texto);
        if (arr_id > 0) {
            lista_copiar(temp_l, *lista_arr[arr_id]);
        }
        else {
            char curr_nome[50];
            sprintf(curr_nome, "%d",  cont_lista++);
            temp_l = listificar(curr_texto, curr_nome);
        }

        switch (aux_op)
        {
        case 0: // +
            if (ent_final->raiz == NULL) {
                ent_final = temp_l;
            }
            else {
                lista_costurar(ent_final, temp_l, 'L');
            }
            aux_op = -1;
            break;
        case 1: // :
            aux_op = -1;
            break;
        case 2: // =
            // Meio silly, mas fazer o quê...
            Lista* nova_lista = (Lista*)malloc(sizeof(Lista));
            strcpy(nova_lista->nome, curr_texto);
            lista_copiar(nova_lista, *ent_final);
            arr_adicionar(nova_lista);

            aux_op = -1;
            break;
        default:
            // Só por garantia to mantendo o default
            if (ent_final->raiz == NULL) {
                ent_final = temp_l;
            }
            break;
        }
    }
    lista_print(*ent_final);
    // Apaga as listas temporárias
    for (int i = 0; i < cont_lista; i++) {
        char curr_nome[50];
        sprintf(curr_nome, "%d", i);
        arr_remover(curr_nome);
    }
}

void interface() {
    char linha[200];
    // strcmp retorna 0 'False' quando comparação está correta
    while (strcmp(linha, "quit")) {
        printf("> ");
        // Usa esse regex do scanset (%) 
        // para ler linha até um newline ser encontrado:
        scanf("%[^\n]", linha);
        
        // Split em 'C' (feito por espaço por agora)
        // Talvez vá ter que ser feito na mão...
        
        // Print da pilha de listas
        if (!strcmp(linha, "all")) {
            printf("Listas:\n");
            for (int i = 0; i < MAX_ARR_ENTS; i++) {
                if (lista_arr[i]) {
                    lista_print(*lista_arr[i]);
                }
                else {
                    printf("[] ");
                }
                printf("| %p \n", lista_arr[i]);
            }
        }

        parser(linha);

        // printf("%s\n", linha);

        printf("\n");

        // Consome o newline que sobrou
        char c;
        scanf("%c", &c);
    }
}
