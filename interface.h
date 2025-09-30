// Header para o parser (leitor) da interface interativa
#include "lista.h"

// -- Definições --
void calcPilha(char**, int*, int);
int* opsPosOrdenar(char**, int);
char** dispor(char**, int);

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
    "^", // Acessor de índices - 1
    // Matemáticos
    "**", // Potênciação - 2
    "*", // Multiplicação - 3
    "/", // Divisão por inteiro - 4
    "-", // Subtração - 5
    "+", // Soma - 6
    // Técnicos
    "v", // Costura (União) - 7
    "^", // Recorte (Intersecção de índices) - 8
    ":", // Sequência - 9
    "obj", // Objetos (Restantes)
    "\0" // Fim
};

// -- Funções --
// - Ordenadores -
int* opsPosOrdenar(char** equacao, int aux_ent) {
    // Retorna pilha com id das operações na lista
    int* pilha = (int*)calloc(aux_ent + 1, sizeof(int));
    int cur_op = 0;
    for (int i = 0; OPS[i] != "\0"; i++) {
        int cur_ent = 0;
        do {
            char* curr_texto = equacao[cur_ent];
            if (!strcmp(OPS[i], curr_texto)) {
                pilha[cur_op] = cur_ent;
                cur_op++;
            }
        } while (++cur_ent < aux_ent);
    }
    // Marcador de final da pilha:
    pilha[cur_op++] = -1;
    return pilha;
}


// Dispõe substrings na tabuleiro de strings
char** dispor(char** tabuleiro, int tabuleiro_tam) {
    char** matriz = (char**)calloc(tabuleiro_tam, sizeof(char*));

    // Contadores
    int num_cont = 0;
    int ops_cont = 1;
    for (int l = 0; l < tabuleiro_tam; l++) {
        char cur_temp_text[MAX_TEXT_BUFFER];
        matrizPrintar(matriz, tabuleiro_tam);
        strncpy(cur_temp_text, tabuleiro[l], MAX_TEXT_BUFFER);
        // 1. Ignora caso se trate de um '\0' na posição 0
        if (cur_temp_text[0] == '\0') {
            continue;
        }
        // 2. Dispõe valor 2 a 2 (0, 2, 4, ...)
        if (tipoElemento(cur_temp_text)) {
            matriz[num_cont] = (char*)malloc(sizeof(char) * strlen(tabuleiro[l]) + 1);
            strcpy(matriz[num_cont], cur_temp_text);
            num_cont += 2;
            continue;
        }

        // 3. Dispõe operação 2 a 2 (1, 3, 5, ...)
        // Por agora supõe ser op que se não for valor
        matriz[ops_cont] = (char*)malloc(sizeof(char) * strlen(tabuleiro[l]) + 1);
        strcpy(matriz[ops_cont], cur_temp_text);
        ops_cont += 2;
    }
    
    matrizLimpar(tabuleiro, tabuleiro_tam);
    return matriz;
}

// Faz o parser de uma dada linha
// Fazer com que retorne texto talvez?
// TODO: Tomar cuidado com palavras restritas!
// TODO: Separar em várias funções!
void parser(char* linha) {
    // Acho melhor um array só pro texto!!!
    // TODO: Unidimensiolizar o array 2D!
    // Entidades de texto na linha (10 entidades de nomes de MAX_TEXT caracteres)
    char** texto_ents;
    char** texto_aux = (char**)calloc(MAX_LINHA_ENTS,sizeof(char*));

    // Tokeninzação
    char* token = strtok(linha," ");
    int aux_ent = 0;
    while (token != NULL) {
        texto_aux[aux_ent] = (char*)calloc(MAX_TEXT, sizeof(char)); // + 1 para garantir o null
        strcpy(texto_aux[aux_ent++], token);
        token = strtok(NULL, " ");
    }

    // Pilha de operações
    int* pilha_ops = opsPosOrdenar(texto_aux, aux_ent);

    calcPilha(texto_aux, pilha_ops, aux_ent);
    
    matrizLimpar(texto_aux, MAX_LINHA_ENTS);
    texto_aux = NULL;
}

// Calcula elementos baseados na lista
void calcPilha(char** texto_ents, int* pilha_ops, int aux_ent) {
    // Leitura dos elementos
    int aux_op = -1;
    int aux_cur = 0;
    Lista* nova_lista;  // Elemento auxiliar
    Lista* l1 = listaCriar("__Out__");
    Lista* l2 = listaCriar("__temp__");
    // Temp
    Lista* temp_l;
    char* curr_texto = "Out";

    // Roda até não haver mais operações!
    for (int i = 0; pilha_ops[i] > 0; i++) {
        int pos_atual = pilha_ops[i];

        // Olha lado esquerdo e direito
        // Assume ser listificável
        // Puxar elementos tirar ordem dos índices
        char* substr;

        // Anterior (l1) - Ignorando elementos vazios
        int aux_el = pos_atual - 1;

        while (aux_el >= 0) {
            substr = texto_ents[aux_el];
            if (substr[0] != '\0') {
                int tipo = tipoElemento(substr);
                if (tipo == LISTA) {
                    strcpy(substr, substr + 1);
                    substr[strlen(substr) - 1] = '\0';
                } else {
                    // Assume possibilidade de ser nome se texto
                    int arr_id = arrContem(substr);
                    if (arr_id >= 0) {
                        listaCopiar(l1, *lista_arr[arr_id]);
                    } else {
                        // "Cria" nome
                        ;
                    }
                }
                
                l1 = listificar(substr, l1->nome);
                substr[0] = '\0';
                break;
            }
            aux_el--;
        }
        
        // Central (Op)
        int aux_op = strEmArr(texto_ents[pos_atual], OPS);
        
        texto_ents[pos_atual][0] = '\0';

        // Posterior (l2)
        aux_el = pos_atual + 1;

        while (aux_el < aux_ent) {
            substr = texto_ents[aux_el];
            if (substr[0] != '\0') {
                if (tipoElemento(substr) == LISTA) {
                    strcpy(substr, substr + 1);
                    substr[strlen(substr) - 1] = '\0';
                }
                l2 = listificar(substr, l2->nome);
                texto_ents[aux_el][0] = '\0';
                break;
            } else {
                // Assume possibilidade de ser nome se texto
                int arr_id = arrContem(substr);
                if (arr_id >= 0) {
                    listaCopiar(l2, *lista_arr[arr_id]);
                } else {
                    // "Cria" nome
                    ;
                }
            }
            aux_el++;
        }

        substr = NULL;

        // Seleciona '-1' caso o ent_final não estiver definido ainda
        // Senão, seleciona operação escolhida
        // switch ((l1->tamanho && l2->tamanho) ? aux_op : -1)
        switch (aux_op)
        {
        case 0: // =
            // Meio silly, mas fazer o quê...
            // Assim preserva a predominância do l1
            // Deve ahver jeitos melhores...
            // TODO: Consertar isso!
            listaCopiar(l1, *l2);
            nova_lista = listaCriar(curr_texto);
            listaCopiar(nova_lista, *l1);

            // TODO: Parar de sempre ficar colocando o aux_op como -1 em TODOS os cases!
            aux_op = -1;
            break;
        case 1: // ^
            // Seleciona apenas os ids especificados
            nova_lista = listaCriar(curr_texto);
            listaCopiar(nova_lista, *l1);
            listaRecortar(l1, *temp_l);

            aux_op = -1;
        break;
        case 2: // **
        case 3: // *
        case 4: // /
        case 5: // -
        case 6: // +
            // listaOperar(ent_final, *temp_l, aux_op);
            listaOperar(l1, *l2, aux_op);
            listaStr(*l1);
        break;
        case 7: // v
            if (l1->raiz == NULL) {
                listaCopiar(l1, *temp_l);
            }
            else {
                listaCosturar(l1, *temp_l, 'L');
            }
            aux_op = -1;
            break;
        case 8: // :
            aux_op = -1;
            break;
        default:
            // Só por garantia to mantendo o default
            if (l1->raiz == NULL) {
                listaCopiar(l1, *temp_l);
            }
            break;
        }
        strcpy(texto_ents[pos_atual + 1], listaStr(*l1));

        // if (l1->tamanho && l2->tamanho) {
        //     listaDeletarNos(l1);
        //     listaDeletarNos(l2);
        // }
    }
    listaPrintar(*l1);
    arrRemover("__temp__");
    arrRemover("__Out__");
    // listaDeletar(ent_final); Por que não dava double free?
    // Limpar listas
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

void interface(char* str_inicial) {
    char linha[MAX_TEXT];
    if (str_inicial[0] != '\0') {
        strcpy(linha, str_inicial);
    }
    int interrupt = FALSE;
    int comm = -1;
    // strcmp retorna 0 'False' quando comparação está correta
    while (TRUE) {
        printf("> ");
        // Usa esse regex do scanset (%) 
        // para ler linha até um newline ser encontrado:
        if (linha[0] == '\0') {
            scanf("%[^\n]", linha);
        }
        
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

        // "Apaga" linha
        linha[0] = '\0';
    }
}
