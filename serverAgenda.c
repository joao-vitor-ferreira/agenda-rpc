#include <stdio.h>
#include <string.h>
#include "agenda.h"

// DEFININDO A AGENDA, QUE SERÁ UMA LISTA DESTE TIPO (contato)
typedef struct{
    contato *first;
    contato *last;
}agenda;

// LISTA VAZIA
agenda *list = NULL;

int *insere_1_svc(contato *c, struct svc_req *rqstp){
    static int result = 0;
    char *nome = NULL, *endereco = NULL;
    if(c == NULL)
        return &result;
    // alocando contato
    contato *new = (contato*)malloc(sizeof(contato));
    nome = (char*)malloc(sizeof(char)*(strlen(c->nome)+1));
    endereco = (char*)malloc(sizeof(char)*(strlen(c->endereco)+1));
    // agenda *new_l = (agenda)malloc(sizeof(agenda));

    // pasando o conteudo que veio para memoria
    strcpy(nome, c->nome);
    strcpy(endereco, c->endereco);
    new->nome = nome;
    new->endereco = endereco;
    new->telefone = c->telefone;
    new->erro = 0;

    if(list == NULL){
        list = (agenda*)malloc(sizeof(agenda));
        list->first = new;
        list->last = new;
    } else {
        list->last->next = new;
    }
    new->next = NULL;
    result = 1;
    printf("Contato inserido.\n");
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
        }
    }
    printf("Contato não encontrado\n");
    return atual;
}

contato *consulta_1_svc(char **nome, struct svc_req *rqstp){
    contato *result;
    printf("%s\n", *nome);
    result = busca_contato(*nome);
    if(result == NULL){
        contato c;
        c.erro = 1;
        return &c;
    }
    return result;
}
// FUNCAO QUE DADO UM CONTATO ENVIADO PELO CLIENTE, A QUAL ESTE CONTATO 
// DEVE TER O NOME DO CONTATO, ELE ALTERA AS INFORMACOES
int *altera_1_svc(contato *c, struct svc_req *rqstp){
    static int result = 0;
    char *nome = NULL, *endereco = NULL;

    contato *c_achado = busca_contato(c->nome);
    if(c_achado != NULL){
        printf("Contato encontrado\n");
        endereco = (char*)malloc(sizeof(char)*(strlen(c->endereco)+1));
        if(c_achado->endereco != NULL)
            free(c_achado->endereco);
        c_achado->nome = nome;
        c_achado->telefone = c->telefone;
        printf("Contato alterado.\n");
    }else{
        result = 0; // contato não encontrado
        return &result;
    }

    result = 1;
    return &result;
}

int *remover_1_svc(char **nome, struct svc_req *rqstp){
    static int result = 0;
    contato *c = busca_contato(*nome);
    if(c != NULL){
        if(c->nome !=NULL)
            free(c->nome);
        if(c->endereco != NULL)
            free(c->endereco);
        free(c);
        result++;
    }
    return &result;
}