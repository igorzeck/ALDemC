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

// Mudar para ordem matemática ficar no topo!
char* OPS[] = {
    "=", // Atribuição - 0
    // Matemáticos
    "**", // Potênciação - 1
    "*", // Multiplicação - 2 
    "/", // Divisão por inteiro - 3
    "-", // Subtração - 4
    "+", // Soma - 5
    // Técnicos
    "v", // Costura (União) - 6
    ":", // Sequência - 7
    // TODO: "[..]" Acessor?
    // TODO: Usar id para ordenar operações
    
    // Lógicos
    "0"
};

const int MAX_LINHA_ENTS = 32;
const int MAX_TEXT = 255;

// -- Funções --
// Faz o parser de uma dada linha
// Fazer com que retorne texto talvez?
// TODO: Tomar cuidado com palavras restritas!
// TODO: Separar em várias funções!
void parser(char* linha) {
    // Entidades de texto na linha (10 entidades de nomes de MAX_TEXT caracteres)
    char texto_ents[MAX_LINHA_ENTS][MAX_TEXT];
    char texto_aux[MAX_LINHA_ENTS][MAX_TEXT];

    char* token = strtok(linha," ");
    int aux_ent = 0;
    while (token != NULL) {
        strcpy(texto_aux[aux_ent++], token);
        token = strtok(NULL, " ");
    }
    // Vai de operação em operação e deixa em pré-ordem as OPS
    int cur_ent = 0;
    for (int i = 0; OPS[i] != "0"; i++) {
        int aux_aux_ent = aux_ent;
        char* curr_op = OPS[i];
        while (aux_aux_ent-- >= 0) {
            // o texto_aux[aux_aux_ent][0] != '\0' teoricamente não precisa, mais por garantia...
            if (!strcmp(texto_aux[aux_aux_ent], curr_op)) {
                if (texto_aux[aux_aux_ent][0] != '\0') {
                    strcpy(texto_ents[cur_ent++], texto_aux[aux_aux_ent]); // Operação
                    texto_aux[aux_aux_ent][0] = '\0';
                }
                
                if (aux_aux_ent - 1 >= 0 && texto_aux[aux_aux_ent - 1][0] != '\0') {
                    strcpy(texto_ents[cur_ent++], texto_aux[aux_aux_ent - 1]); // Anterior
                    texto_aux[aux_aux_ent - 1][0] = '\0';
                }

                if (aux_aux_ent + 1 < aux_ent && texto_aux[aux_aux_ent + 1][0] != '\0') {
                    strcpy(texto_ents[cur_ent++], texto_aux[aux_aux_ent + 1]); // Posterior
                    texto_aux[aux_aux_ent + 1][0] = '\0';
                }  
            }
        }
    }

    // Leitura dos elementos
    int aux_op = -1;
    int aux_cur = 0;
    // Uma alternativa seria ent_final ser apenas uma Lista
    // E se passar o endereço...
    // Meio redundante ser ponteiro
    Lista* ent_final = listaCriar();
    char ent_texto_final[MAX_TEXT];
    strcpy(ent_final->nome, "Out");
    ent_final->raiz = NULL;

    while (aux_cur < cur_ent) {
        char curr_texto[MAX_TEXT];
        Lista* temp_l;
        temp_l = listaCriar();
        strcpy(curr_texto, texto_ents[aux_cur++]);

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
            // Se não for, é lista?
            int listavel = FALSE;
            if (!eNumerico(curr_texto)) {
                if ((curr_texto[0] == '[') 
                    &&
                    curr_texto[strlen(curr_texto) - 1] == ']') {
                        // Corta partes de lista
                        strcpy(curr_texto, curr_texto + 1);
                        curr_texto[strlen(curr_texto) - 1] = '\0';
                        listavel = TRUE;
                    }
            } else {
                listavel = TRUE;
            }
            // Verifica se é listificável
            if (listavel) {
                temp_l = listificar(curr_texto, "__temp__");
            }
        }

        int acao = (ent_final->tamanho > 0) ? aux_op : -1; 

        switch (acao)
        {
        case 0: // =
            // Meio silly, mas fazer o quê...
            Lista* nova_lista = (Lista*)malloc(sizeof(Lista));
            strcpy(nova_lista->nome, curr_texto);
            listaCopiar(nova_lista, *ent_final);
            arrAdicionar(nova_lista);

            aux_op = -1;
            break;
        case 1: // **
        case 2: // *
        case 3: // /
        case 4: // -
        case 5: // +
            listaOperar(ent_final, *temp_l, aux_op);
            // listaCopiar(ent_final, *temp_l);
            aux_op = -1;
            // Só por garantia
            // if (ent_final->raiz == NULL) {
            //     listaCopiar(ent_final, *temp_l);
            // }
        break;
        case 6: // v
            if (ent_final->raiz == NULL) {
                listaCopiar(ent_final, *temp_l);
            }
            else {
                listaCosturar(ent_final, *temp_l, 'L');
            }
            aux_op = -1;
            break;
        case 7: // :
            aux_op = -1;
            break;
        default:
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
