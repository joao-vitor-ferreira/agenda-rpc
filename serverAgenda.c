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
    if(c == null)
        return &result;
    // alocando contato
    contato *new = (contato)malloc(sizeof(contato));
    nome = (char*)malloc(sizeof(char)*(strlen(c->nome)+1));
    endereco = (char*)malloc(sizeof(char)*(strlen(c->endereco)+1));
    // agenda *new_l = (agenda)malloc(sizeof(agenda));

    // pasando o conteudo que veio para memoria
    strcpy(nome, c->nome);
    strcpy(endereco, c->endereco);
    new->nome = nome;
    new->endereco = endereco;
    new->telefone = c->telefone;

    if(list == null){
        list = (agenda)malloc(sizeof(agenda));
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

contato busca_contato(char *nome){
    if(list == NULL){
        printf("Não há contatos na agenda\n");
        return NULL;
    }else{
        contato *atual = (contato)list->first;
        while(atual != NULL){
            if(strcmp(nome, atual->nome) == 0){
                return atual;
            }
        }
    }
    printf("Contato não encontrado\n");
    return NULL;
}

contato *consulta_1_svc(char *nome, struct svc_req *rqstp){
    static contato result = busca_contato(nome);
    return &result;
}

int *altera_1_svc(contato *c, struct svc_req *rqstp){
    static int result = 0;
    char *nome = NULL, *endereco = NULL;

    contato c_achado = busca_contato(c->nome);
    if(c_achado != NULL){
        endereco = (char*)malloc(sizeof(char)*(strlen(c->endereco)+1));
        if(c_achado->endereco != NULL)
            free(c_achado->endereco);
        c_achado->nome = nome;
        c_achado->telefone = c->telefone;
    }else{
        result = 2; // contato não encontrado
        return &result;
    }

    result = 1;
    return &result;
}


/* implementa��o da fun��o add */
int * add_1_svc (contato *argp, struct svc_req *rqstp)
{
   static int result;

   printf ("Recebi chamado: add %d %d\n", argp->x, argp->y);
   result = argp->x + argp->y;
   return (&result);
}

/* implementa��o da fun��o sub */
int * sub_1_svc (contato *argp, struct svc_req *rqstp)
{
   static int result;

   printf ("Recebi chamado: sub %d %d\n", argp->x, argp->y);
   result = argp->x - argp->y;
   return (&result);
} 