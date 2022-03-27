/* Cliente RPC simples */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "agenda.h"

static int respondeu = 0;

void *timeout(void *vargp) {
   int i = 0;
   for (i = 0; i < 6; i++) {
      usleep(200000);
      if(respondeu == 1) {
         return NULL;
      }
   }
   printf("O servidor demorou muito para responder.\n");
   exit(0);
}

int insere(CLIENT *clnt, char *nome, char *endereco, int telefone){
    contato c;
    int *result;
    pthread_t thread_id;

    c.nome = (char*)malloc(sizeof(char)*(strlen(nome)+1));
    strcpy(c.nome, nome);
    c.endereco = (char*)malloc(sizeof(char)*(strlen(endereco)+1));
    strcpy(c.endereco, endereco);
    c.telefone = telefone;
    c.next = NULL;
    c.erro = 0;

    pthread_create(&thread_id, NULL, timeout, NULL);
    result = insere_1(&c, clnt);

    if(c.nome != NULL)
        free(c.nome);
    if(c.endereco != NULL)
        free(c.endereco);

    if(result == NULL){
        printf("Erro ao chamar funcao remota\n");
        exit(1);
    }

    respondeu = 1;
    pthread_join(thread_id, NULL);
    respondeu = 0;

    return *result;
}

contato consulta(CLIENT *clnt, char *nome){
    contato *result;
    pthread_t thread_id;
    
    pthread_create(&thread_id, NULL, timeout, NULL);
    result = consulta_1(&nome, clnt);

    if(result == NULL){
        printf("Erro ao chamar funcao remota\n");
        exit(1);
    }
    respondeu = 1;
    pthread_join(thread_id, NULL);
    respondeu = 0;

    return *result;
}

int altera(CLIENT *clnt, contato c){
    int *result;
    pthread_t thread_id;

    c.erro = 0;
    c.next = NULL;

    pthread_create(&thread_id, NULL, timeout, NULL);
    result = altera_1(&c, clnt);

    if(result == NULL){
        printf("Erro ao chamar funcao remota\n");
        exit(1);
    }

    respondeu = 1;
    pthread_join(thread_id, NULL);
    respondeu = 0;

    return *result;
}

int remover(CLIENT *clnt, char *nome){
    int *result;
    pthread_t thread_id;

    pthread_create(&thread_id, NULL, timeout, NULL);
    result = remover_1(&nome, clnt);

    if(result == NULL){
        printf("Erro ao chamar funcao remota\n");
        exit(1);
    }

    respondeu = 1;
    pthread_join(thread_id, NULL);
    respondeu = 0;

    return *result;
}

int main( int argc, char *argv[])
{
    CLIENT *clnt;
    int opcao, telefone;
    char nome[20], endereco[30];
    int retorno;
    contato c;
    pthread_t thread_id;


    /* verifica se o cliente foi chamado corretamente */
    if (argc!=2)
    {
        fprintf (stderr,"Usage: %s hostname\n",argv[0]);
        exit (1);
    }

    /* cria uma struct CLIENT que referencia uma interface RPC */
    //    printf("%s\n", argv[1]);
    pthread_create(&thread_id, NULL, timeout, NULL);
    clnt = clnt_create (argv[1], AGENDA_PROG, AGENDA_VERSION, "udp");

    /* verifica se a refer�ncia foi criada */
    if (clnt == (CLIENT *) NULL)
    {
        clnt_pcreateerror (argv[1]);
        exit(1);
    }
    
    respondeu = 1;
    pthread_join(thread_id, NULL);
    respondeu = 0;

    retorno = insere(clnt, "joao", "endereço 1", 9999999);
    retorno = insere(clnt, "maria", "endereço 1", 8888888);
    retorno = insere(clnt, "jose", "endereço 1", 7777777);
    retorno = insere(clnt, "nome", "endereço 1", 6666666);

    while (1){
        printf("##################### AGENDA ####################\n");
        printf("INSERIR CONTATO:   1\n");
        printf("CONSULTAR CONTATO: 2\n");
        printf("ALTERAR CONTATO:   3\n");
        printf("EXCLUIR CONTATO:   4\n");
        printf("SAIR:              5\n");
        printf("Digite o numero da opção que deseja: ");
        scanf(" %d", &opcao);
        switch (opcao){
        case 1:
            printf("Digite o nome do seu contato: ");
            scanf(" %[^\n]", nome);
            printf("Digite o endereço do seu contato: ");
            scanf(" %[^\n]", endereco);
            printf("Digite o telefone do seu contato (somente números): ");
            scanf(" %d", &telefone);
            retorno = insere(clnt, nome, endereco, telefone);

            if(retorno)
                printf("Contato inserido\n");
            else
                printf("Erro ao inserir contato\n");

            break;
        
        case 2:
            printf("Digite o nome do seu contato: ");
            scanf(" %[^\n]", nome);
            c = consulta(clnt, nome);
            if(c.erro == 1){
                printf("Contato não encontrado\n");
            }else{
                printf("########## CONTATO ###########\n");
                printf("Nome: %s\n", c.nome);
                printf("Endereço: %s\n", c.endereco);
                printf("Telefone: %d\n", c.telefone);
            }

            break;
        
        case 3:
            printf("Digite o nome do contato a ser alterado: ");
            scanf(" %[^\n]", nome);
            printf("Digite o novo endereço do seu contato: ");
            scanf(" %[^\n]", endereco);
            printf("Digite o novo telefone do seu contato (somente números): ");
            scanf(" %d", &telefone);

            contato ctt;
            ctt.nome = malloc(sizeof(char)*(strlen(nome)+1));
            strcpy(ctt.nome, nome);
            ctt.endereco = malloc(sizeof(char)*(strlen(endereco)+1));
            strcpy(ctt.endereco, endereco);
            ctt.telefone = telefone;

            retorno = altera(clnt, ctt);

            if(retorno)
                printf("Contato alterado\n");
            else
                printf("Erro ao alterar contato\n");
                
            if(ctt.nome != NULL)
                free(ctt.nome);
            if(ctt.endereco != NULL)
                free(ctt.endereco);

            break;

        case 4:
            printf("Digite o nome do contato a ser alterado: ");
            scanf(" %[^\n]", nome);
            retorno = remover(clnt, nome);
            if(retorno)
                printf("Contato removido\n");
            else
                printf("Contato não encontrado\n");

            break;
        default:
            return(0);
            break;
        }
        printf("\n");
    }
    

   return (0);
} 
