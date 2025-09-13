// Script simulando "lista"
#include "interface.h"
// #include "functionals.h"

int main(void) {
    interface();
    // Lista lista;
    // lista.raiz = (No*)malloc(sizeof(No));
    // lista.raiz->valor = 10;
    // lista_inserir(&lista, 20, 1);
    // lista_inserir(&lista, 30, 2);
    
    // char* lista[] = {"-", "+", ":", "1", "0"};
    // int result = str_is_in("1", lista);
    // printf("Resultado: %d\n", result);
    // lista_print(lista);
    return 0;
}

// int main(void) {
//     // -- Input e Output --
//     Lista* l1 = (Lista*)malloc(sizeof(Lista));
//     l1->valor = 10;
//     Lista* n1 = (Lista*)malloc(sizeof(Lista));
//     l1->prox = n1;
//     n1->valor = 1;
//     Lista* n2 = (Lista*)malloc(sizeof(Lista));
//     n1->prox = n2;
//     n2->valor = 2;
//     Lista* n3 = (Lista*)malloc(sizeof(Lista));
//     n2->prox = n3;
//     n3->valor = 5;
//     n3->prox = NULL;

//     Lista* l2 = (Lista*)malloc(sizeof(Lista));
//     l2->valor = 102;
//     Lista* n12 = (Lista*)malloc(sizeof(Lista));
//     l2->prox = n12;
//     n12->valor = 12;
//     Lista* n22 = (Lista*)malloc(sizeof(Lista));
//     n12->prox = n22;
//     n22->valor = 22;
//     Lista* n32 = (Lista*)malloc(sizeof(Lista));
//     n22->prox = n32;
//     n32->valor = 52;
//     n32->prox = NULL;
    
//     lista_print(l1);
//     printf("\n\n");
//     lista_print(l2);
//     printf("\n\n");
//     lista_costurar(&l1, l2);
//     lista_print(l1);
//     lista_inserir(&l1, 111011, 6);
//     printf("\n\n");
//     lista_print(l1);
//     printf("\n\n");
//     lista_print(l2);

//     return 0;
    
//     lista_inserir(&l1, 22, 1);
//     lista_inserir(&l1, 33, 5);

//     lista_print(l1);
    
//     printf("%d\n", l1->valor);

//     lista_deletar(&l1);

//     printf("%p\n", l1);

//     return 0;
// }