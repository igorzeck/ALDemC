// Header para o parser (leitor) da interface interativa
#include "lista.h"

// -- Constantes --

// Comandos (Nome, ID, Descrição)
char* COMM[4][3] = {
    {"quit", "0", "Sai do programa."},
    {"exit", "0", "Sai do programa."},
    {"all", "1", "Mostra todas as listas no ambiente."},
    {"END", "-1", "END"}
};

char* OPS[] = {
    // Técnicos
    "v", // Costura (União) - 0
    ":", // Sequência - 1
    "=", // Atribuição - 2
    // TODO: "[..]" Acessor?
    // TODO: Usar id para ordenar operações
    // Matemáticos
    "+", // Soma - 3 
    "-", // Subtração - 4 
    "*", // Multiplicação - 5
    "/", // Divisão por inteiro - 6
    // Lógicos
    "0"
};

const int MAX_LINHA_ENTS = 10;
const int MAX_TEXT = 255;

// -- Funções --
// Faz o parser de uma dada linha
// Fazer com que retorne texto talvez?
// TODO: Tomar cuidado com palavras restritas!
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

    while (cur_ent-- > 0) {
        char curr_texto[MAX_TEXT];
        Lista* temp_l;
        temp_l = listaCriar();
        strcpy(curr_texto, texto_ents[cur_ent]);

        if (aux_op < 0) {
            aux_op = strIn(curr_texto, OPS);
            if (aux_op >= 0) {
                continue;
            }
        }

        // Verifica se é uma entidade
        int arr_id = arrContem(curr_texto);
        if (arr_id >= 0 && aux_op != 2) {
            listaCopiar(temp_l, *lista_arr[arr_id]);
        }
        else {
            temp_l = listificar(curr_texto, "__temp__");
        }

        switch (aux_op)
        {
        case 0: // v
            if (ent_final->raiz == NULL) {
                listaCopiar(ent_final, *temp_l);
            }
            else {
                listaCosturar(ent_final, *temp_l, 'L');
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
            listaCopiar(nova_lista, *ent_final);
            arrAdicionar(nova_lista);

            aux_op = -1;
            break;
        default:
            // Por agora as operações sempre rodam da esquerda pra direita
            if (aux_op >= 3 && aux_op <= 6) {
                listaOperar(temp_l, *ent_final, aux_op);
                listaCopiar(ent_final, *temp_l);
                aux_op = -1;
            }
            // Só por garantia to mantendo o default
            if (ent_final->raiz == NULL) {
                listaCopiar(ent_final, *temp_l);
            }
            break;
        }
    }
    listaPrintar(*ent_final);
    arrRemover("__temp__");
    listaDeletar(ent_final);
}

void executarComm(int comm) {
    switch (comm)
    {
    case 1: // all
        // Print da pilha de listas
        printf("Listas:\n");
        for (int i = 0; i < MAX_ARR_ENTS; i++) {
            if (lista_arr[i]) {
                listaPrintar(*lista_arr[i]);
            }
            else {
                printf("[] ");
            }
            printf("| %p \n", lista_arr[i]);
        }
        break;
    default:
        break;
    }
}

void interface() {
    char linha[200];
    int interrupt = FALSE;
    int comm = -1;
    // strcmp retorna 0 'False' quando comparação está correta
    while (TRUE) {
        printf("> ");
        // Usa esse regex do scanset (%) 
        // para ler linha até um newline ser encontrado:
        scanf("%[^\n]", linha);
        
        // Split em 'C' (feito por espaço por agora)
        // 1. Verifica se é comando
        // ? para ver descrição do comando?
        for (int i = 0; COMM[i][0] != "END"; i++) {
            if (!strcmp(COMM[i][0], linha)) {
                comm = atoi(COMM[i][1]);
                break;
            }
        }

        // Para linha aqui se for comando
        if (comm >= 0) {
            executarComm(comm);
            if (!comm) {
                break;
            }
            comm = -1;
        }
        else {
            // Se não for comando
            // Lê linha como conjunto de listas
            parser(linha);
        }
        // Consome o newline que sobrou
        printf("\n");

        char c;
        scanf("%c", &c);
    }
}
