/*
Implementação de lista de valores atipados
Lista encadeada simples
*/
// TODO: Função para alocamento de elementos ou criar cabeça de lista
// TODO: Lidar com deleção de elementos da lista
#include "includes.h"

// - Constats -
unsigned TRUE = 1;
unsigned FALSE = 0;

// Criação de struct representativa de uma lista e suas operações
typedef struct No{
    // void* valor_ptr; // Ponteiro para uma variável com o valor
    int valor;  // Por agora só aceita valores inteiros 
    struct No* prox; // Ponteiro ao próximo elemento
}No;

typedef struct Lista {
    char nome[200];
    No* raiz;
    int tamanho; // TBA
} Lista;

// Implementação de função para percorrer lista
// Por padrão só vai até o final da lista
No* lista_per(Lista lista, int id) {
    No* aux_el = lista.raiz;

    for (int aux_i = 1; aux_el->prox != NULL; aux_i++) {
        // O operador ternário é só pra caso o id seja negativo ele ir pro até o fim
        if ((id > 0) ? (aux_i > id) : 0) {
            break;
        }
        aux_el = aux_el->prox;
    }

    return aux_el;
}

// - Create -

// Cria lista a partir de uma string
Lista* listificar(char* str, char* nome) {
    Lista* lista = (Lista*)malloc(sizeof(Lista));
    lista->raiz = (No*)malloc(sizeof(No));
    strcpy(lista->nome, nome);
    // Divide string em partes
    // Primeiro verifica se o primeiro caracter é '['
    unsigned open_brack = FALSE;

    int num_stack[64];
    int n = 0;

    char curr_num[64];
    char c;
    int i = 0;
    int aux_num = 0;
    do {
        c = str[i++];
        if (c == '[') {
            open_brack = TRUE;
            continue;
        }
        if (c == ']') {
            curr_num[aux_num] = '\0';
            num_stack[n++] = atoi(curr_num);
            aux_num = 0;
            open_brack = FALSE;
            break;
        }
        if (open_brack) {
            if (c != ',') {
                curr_num[aux_num++] = c;
            }
            else {
                curr_num[aux_num] = '\0';
                num_stack[n++] = atoi(curr_num);
                aux_num = 0;
            }
        }
    } while (c != EOF);

    // Deselegante, mas...
    No** no_atual = &(lista->raiz);
    for (int i = 0; i < n; i++) {
        (*no_atual)->valor = num_stack[i];
        if ((i + 1) < n) {
            (*no_atual)->prox = (No*)malloc(sizeof(No));
            no_atual = &((*no_atual)->prox);
        }
        else{
            (*no_atual)->prox = NULL;
        }
    }
    if (n == 0) {
        *no_atual = NULL;
    }
    
    return lista; 
}

// -- Funções de inserção --
// - Insert -
// Insere um elemento na lista
void lista_inserir(Lista* lista, int val, int id) {
    // Percorre até id
    No* el = NULL;
    if (id != 0) {
        el = lista_per(*lista, id - 1);
    }
    else {
        el = (lista->raiz);
    }
    // Cria novo elemento
    No* novo_el = (No*)malloc(sizeof(No));
    novo_el->valor = val;
    
    // Costura elemento(s) na lista
    No* prox_aux = el->prox;
    el->prox = novo_el;
    novo_el->prox = prox_aux;
}

// - Remove -
// ...

// - Extend (por Left join) -
// Costura as duas listas por Left join
void lista_costurar(Lista* l1, Lista l2) {
    No* cauda = lista_per(*l1, -1);
    cauda->prox = l2.raiz;
}
// TODO: Função par extender a lista (costurar)


// - Delete (lista toda) -
// TODO: Mudar para receber lista
void lista_deletar(No** cabeca){
    No* next_el = NULL;
    No* aux_el = *cabeca;
    while (aux_el != NULL) {
        next_el = aux_el->prox;
        printf("Deletando %d\n", aux_el->valor);
        free(aux_el);

        aux_el = next_el;
    }
    *cabeca = NULL;
}


// Função para printar lista
void lista_print(Lista lista) {
    No* aux_el = lista.raiz;
    printf("%s: [", lista.nome);
    while (aux_el != NULL) {
        printf("%d", aux_el->valor);
        aux_el = aux_el->prox;
        if (aux_el) {
            printf(",");
        }
    }
    printf("]");
}