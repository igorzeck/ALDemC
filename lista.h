/*
Implementação de lista de valores atipados
Lista encadeada simples
*/
// TODO: Lista vazia ainda tá muito estranho (Depende ordem 'a + []' é != '[] + a') e [2] + [] dá erro!
// TODO: Lista não precisa sempre ser ponteiro!
#include "functionals.h"

// - Definições -

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

int strIn(char*, char**);
int eNumerico(char*);

Lista* listaCriar();
void listaOperar(Lista*, Lista, int);
void listaDeletar(Lista* lista);
void listaPrintar(Lista lista);
void listaInserir(Lista* lista, int val, int id);
void listaCopiar(Lista*, Lista) ;
void listaCosturar(Lista* l1, Lista l2, char join); 
Lista* listificar(char*, char*);

void listaDeletarNos(Lista* lista);
void arrAdicionar(Lista* tba);
void arrRemover(char* nome);
int arrContem(char* nome);

// - Constantes -
const int MAX_ARR_ENTS = 50;

// - Variáveis -
Lista* lista_arr[50] = {NULL};  // Array de listas

// Implementação de função para percorrer lista
// Por padrão só vai até o final da lista
// TODO: Consertar bug de quando '[] + algo' dá erro de segmentação
No* listaPer(Lista lista, int id) {
    No* aux_el = lista.raiz;

    for (int aux_i = 1; aux_el->prox != NULL; aux_i++) {
        // O operador ternário é só pra caso o id seja negativo ele ir até o fim
        if ((id > 0) ? (aux_i > id) : 0) {
            break;
        }
        aux_el = aux_el->prox;
    }

    return aux_el;
}

// -- Funções --
// TODO: usar função que verifica se nome está na lista abaixo (em remover e adicionar)
// Se estiver dentro retorna a posição, senão retorna menor que zero
int arrContem(char* nome) {
    for (int i = 0; i < MAX_ARR_ENTS; i++) {
        if (lista_arr[i]) {
            if (!strcmp(lista_arr[i]->nome, nome)) {
                return i;
            }
        }
    }
    return -1;
} 

// Adiciona lista To Be Added à pilha de listas
void arrAdicionar(Lista* tba) {
    // 1. Primeio percorre nomes
    int free_id = -1;
    for (int i = 0; i < MAX_ARR_ENTS; i++) {
        // Pula elementos com conteúdos
        if (lista_arr[i] != NULL) {
            if (!strcmp(tba->nome, lista_arr[i]->nome)) {
                free(lista_arr[i]);
                lista_arr[i] = tba;
                return;
            }
        }
        else {
            // Primeiro elemento livre encontrado
            if (free_id < 0) {
                free_id = i;
            }
        }
    }
    if (free_id >= 0) {
        lista_arr[free_id] = tba;
    }
    ;
}

// Remove nome especificado da pilha de listas
// Não tá adicionando o '\0'
void arrRemover(char* nome) {
    for (int i = 0; i < MAX_ARR_ENTS; i++) {
        if (lista_arr[i]) {
            if (!strcmp(lista_arr[i]->nome, nome)) {
                listaDeletar(lista_arr[i]);
                lista_arr[i] = NULL;
            }
        }
    }
}

// - Create -

// Cria lista a partir de uma string
// TODO: Tacar toda lista aqui criada 
// Em uma pilha de listas (facilita pra apagar depois)
// Listas com nome igual se sobrescrevem
// E no final são todas desalocadas!
Lista* listificar(char* str, char* nome) {
    Lista* lista = (Lista*)malloc(sizeof(Lista));
    lista->raiz = (No*)malloc(sizeof(No));
    strcpy(lista->nome, nome);

    // Adiciona lista à pilha de listas
    arrAdicionar(lista);

    // Divide string em partes
    // Primeiro verifica se o primeiro caracter é '['
    unsigned open_brack = FALSE;

    int num_stack[64];
    int n = 0;

    // char curr_num[64];
    char c;
    int i = 0;
    int aux_num = 0;

    // Divisão (retira por ',') -> strtok
    char* curr_num = strtok(str,",");
    while (curr_num != NULL) {
        if (eNumerico(curr_num)) {
            num_stack[n++] = atoi(curr_num);
        }
        curr_num = strtok(NULL, ",");
    }

    // do {
    //     c = str[i++];
    //     if (c == '[') {
    //         open_brack = TRUE;
    //         continue;
    //     }
    //     if (c == ']') {
    //         curr_num[aux_num] = '\0';
    //         aux_num = 0;

    //         if (!eNumerico(curr_num)) {
    //             continue;
    //         }

    //         num_stack[n++] = atoi(curr_num);
    //         open_brack = FALSE;
    //         break;
    //     }
    //     if (open_brack) {
    //         if (c != ',') {
    //             curr_num[aux_num++] = c;
    //         }
    //         else {
    //             curr_num[aux_num] = '\0';
    //             aux_num = 0;

    //             if (!eNumerico(curr_num)) {
    //                 continue;
    //             }
                
    //             num_stack[n++] = atoi(curr_num);
    //         }
    //     }
    // } while (c != EOF);

    // Deselegante, mas...
    No** no_atual = &(lista->raiz);
    for (int i = 0; i < n; i++) {
        (*no_atual)->valor = num_stack[i];
        lista->tamanho++;
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

// Cria lista padrão
// Por agora só cria lista vazia
Lista* listaCriar() {
    Lista* nova_lista = (Lista*)malloc(sizeof(Lista));
    strcpy(nova_lista->nome, "0");
    nova_lista->raiz = NULL;
    nova_lista->tamanho = 0;
    return nova_lista;
}

// Copia valores src para dest
void listaCopiar(Lista* dest, Lista src) {
    No* no_aux = src.raiz;
    No** no_dest = &(dest->raiz);
    
    // Deleta lista anteior
    listaDeletarNos(dest);

    // Passa o novo tamanho para a lista de destino
    dest->tamanho = src.tamanho;
    
    // Copia, nó a nó, do src ao dest
    while(no_aux) {
        (*no_dest) = (No*)malloc(sizeof(No));
        (*no_dest)->valor = no_aux->valor;
        no_dest = &((*no_dest)->prox);
        no_aux = no_aux->prox;
    }
    (*no_dest) = NULL;
}

// -- Funções de inserção --
// - Insert -
// Insere um elemento na lista
void listaInserir(Lista* lista, int val, int id) {
    // Percorre até id
    No* el = NULL;
    if (id != 0) {
        el = listaPer(*lista, id - 1);
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
    lista->tamanho++;
}

// - Remove -
// ...

// - Extend (por Left/Right join) -
// Costura as duas listas
void listaCosturar(Lista* l1, Lista l2, char join) {
    if ((join != 'R') && (join != 'L')) {
        // Inválido!
        return;
    }
    Lista* nl2 = (Lista*)malloc(sizeof(Lista));
    listaCopiar(nl2, l2);

    if (join == 'R') {
        No* cauda = listaPer(*l1, -1);
        cauda->prox = nl2->raiz;
    }
    if (join == 'L') {
        No* cauda = listaPer(*nl2, -1);
        cauda->prox = l1->raiz;
        l1->raiz = nl2->raiz;
    }
    
    l1->tamanho += nl2->tamanho;
    return;
}

// - Delete de nó -
void listaDeletarNos(Lista* lista) {
    No* next_el = NULL;
    No** aux_el = &(lista->raiz);
    while (*aux_el != NULL) {
        next_el = (*aux_el)->prox;
        free(*aux_el);
        *aux_el = NULL;

        aux_el = &(next_el);
    }
}


// - Delete (lista toda) -
void listaDeletar(Lista* lista){
    listaDeletarNos(lista);
    
    // Garantia para caso tente acessar deletado
    strcpy(lista->nome, "DELETADO");
    lista->tamanho = 0;
    
    free(lista);
}

// - Somar -
void listaOperar(Lista* dest, Lista l1, int op) {
    No* no_atual = dest->raiz;
    
    // Ignora soma se a lista l1 estiver vazia
    if (l1.tamanho == 0) {
        return;
    }
    No* no_aux = l1.raiz;

    while ((no_atual != NULL) && (no_aux != NULL)) {
        // Soma valores
        switch(op) {
            case 1:  // **
                no_atual->valor = pot(no_atual->valor, no_aux->valor);
                break;
            case 2: // *
                no_atual->valor *= no_aux->valor;
                break;
            case 3: // /
                no_atual->valor /= no_aux->valor;
                break;
            case 4: // -
                no_atual->valor -= no_aux->valor;
                break;
            case 5: // +
                no_atual->valor += no_aux->valor;
                break;
        }

        // Passa ao próximo
        no_atual = no_atual->prox;
        no_aux = (no_aux->prox != NULL) ? no_aux->prox : l1.raiz;
    }
}

// Função para printar lista
void listaPrintar(Lista lista) {
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