#include <stdio.h>
#include <string.h>
#include "agenda.h"

// DEFININDO A AGENDA, QUE SERÁ UMA LISTA DESTE TIPO (contato)
typedef struct{
    contato *first;
    contato *last;
    int qtd;
}agenda;

// LISTA VAZIA
agenda *list = NULL;

void printAgenda(){
    if(list == NULL){
        printf("################# Agenda Atual #######################\n");
        printf("# Agenda vazia\n");
        printf("######################################################\n");
        return;
    }
    contato *atual = (contato*)list->first;
    printf("################# Agenda Atual #######################\n");
    while (atual != NULL){
        printf("# nome: %s\n", atual->nome);
        atual = atual->next;
    }
    printf("######################################################\n");
}

int *insere_1_svc(contato *c, struct svc_req *rqstp){
    static int result = 0;
    char *nome = NULL, *endereco = NULL;
    if(c == NULL)
        return &result;
    // alocando contato
    contato *new = (contato*)malloc(sizeof(contato));
    nome = (char*)malloc(sizeof(char)*(strlen(c->nome)+1));
    endereco = (char*)malloc(sizeof(char)*(strlen(c->endereco)+1));
    // pasando o conteudo que veio para memoria
    strcpy(nome, c->nome);
    strcpy(endereco, c->endereco);
    new->nome = nome;
    new->endereco = endereco;
    new->telefone = c->telefone;
    new->erro = 0;
    new->next = NULL;

    if(list == NULL){
        list = (agenda*)malloc(sizeof(agenda));
        list->first = new;
        list->last = new;
        list->qtd = 0;
    } else {
        list->last->next = new;
        list->last = new;
    }
    list->qtd++;
    result = 1;
    printf("Contato inserido.\n");
    printAgenda();
    return &result;
}
// FUNCAO PARA ENCONTRAR OS CONTATOS NA AGENDA
contato *busca_contato(char *nome){
    contato *atual = NULL;
    if(list == NULL){
        printf("Não há contatos na agenda\n");
        return atual;
    }else{
        atual = (contato*)list->first;
        while(atual != NULL){
            if(strcmp(nome, atual->nome) == 0){
                return atual;
            }
            atual = atual->next;
        }
    }
    printf("Contato não encontrado\n");
    return atual;
}

contato *consulta_1_svc(char **nome, struct svc_req *rqstp){
    contato *result;
    static contato c;
    result = busca_contato(*nome);
    if(result == NULL){
        c.erro = 1; // indica que não foi encontrado o contato
        c.nome = "teste";
        c.endereco = "teste";
        c.telefone = 0;
        c.next = NULL;
        printAgenda();
        return &c;
    }else{
        result->erro = 0;
    }
    printAgenda();
    return result;
}
// FUNCAO QUE DADO UM CONTATO ENVIADO PELO CLIENTE, A QUAL ESTE CONTATO 
// DEVE TER O NOME DO CONTATO, ELE ALTERA AS INFORMACOES
int *altera_1_svc(contato *c, struct svc_req *rqstp){
    static int result = 0;
    char *endereco = NULL;

    contato *c_achado = busca_contato(c->nome);
    if(c_achado != NULL){
        printf("Contato encontrado\n");
        if(c_achado->endereco != NULL)
            free(c_achado->endereco);
        c_achado->endereco = (char*)malloc(sizeof(char)*(strlen(c->endereco)+1));
        strcpy(c_achado->endereco, c->endereco);
        c_achado->telefone = c->telefone;
        printf("Contato alterado.\n");
    }else{
        result = 0; // contato não encontrado
        printAgenda();
        return &result;
    }

    result = 1;
    printAgenda();
    return &result;
}

int *remover_1_svc(char **nome, struct svc_req *rqstp){
    static int result = 0;
    int is_find = 0;
    contato *atual = NULL, *anterior = NULL;
    atual = (contato*)list->first;
    while (atual != NULL){
        if(strcmp(*nome, atual->nome) == 0){
            if(list->qtd==1){
                free(list);
                list = NULL;
            }else{
                if(atual->next == NULL){
                    anterior->next = NULL;
                    list->last = anterior;
                }else if(anterior == NULL){
                    list->first = atual->next;
                } else {
                    anterior->next = atual->next;
                }
            }
            if(atual->nome !=NULL)
                free(atual->nome);
            if(atual->endereco != NULL)
                free(atual->endereco);
            free(atual);
            result++;
            printf("Contato excluído\n");
            break;
        }
        anterior = atual;
        atual = atual->next;
    }
    if(list != NULL)
        list->qtd--;
    printAgenda();
    return &result;
}

int *remover_todos_1_svc(void *param, struct svc_req *rqstp){
    static int result = 0;
    contato *atual = NULL, *proximo = NULL;
    
    if(list != NULL) {
        atual = (contato*) list->first;
        while (atual != NULL) {
            if(atual->nome !=NULL)
                free(atual->nome);
            if(atual->endereco != NULL)
                free(atual->endereco);
            proximo = atual->next;
            free(atual);
            atual = proximo;
        }
        free(list);
        list = NULL;
    }

    result++;
    printAgenda();

    return &result;
    
}