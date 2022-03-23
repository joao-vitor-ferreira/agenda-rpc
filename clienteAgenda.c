/* Cliente RPC simples */

#include <stdio.h>
#include "agenda.h"

int insere(CLIENT *clnt, char *nome, char *endereco, int telefone){
    contato c;
    int *result;
    c.nome = nome;
    c.endereco = endereco;
    c.telefone = telefone;

    result = insere_1(&c, clnt);
    if(result == NULL){
        printf("Erro ao chamar funcao remota\n");
        exit(1);
    }
    return *result;
}

contato consulta(CLIENT *clnt, char *nome){
    contato *result;
    result = consulta_1(&nome, clnt);
    if(result == NULL){
        printf("Erro ao chamar funcao remota\n");
        exit(1);
    }
    return *result;
}

int altera(CLIENT *clnt, contato c){
    int *result;
    result = altera_1(&c, clnt);
    if(result == NULL){
        printf("Erro ao chamar funcao remota\n");
        exit(1);
    }
    return *result;
}

int remover(CLIENT *clnt, char *nome){
    int *result;
    result = remover_1(&nome, clnt);
    if(result == NULL){
        printf("Erro ao chamar funcao remota\n");
        exit(1);
    }
    return *result;
}

int main( int argc, char *argv[])
{
   CLIENT *clnt;
   int opcao, telefone;
   char *nome = NULL, *endereco = NULL;
   int retorno;
   contato *c;


   /* verifica se o cliente foi chamado corretamente */
   if (argc!=2)
   {
      fprintf (stderr,"Usage: %s hostname num1 num2\n",argv[0]);
      exit (1);
   }

   /* cria uma struct CLIENT que referencia uma interface RPC */
   clnt = clnt_create (argv[1], AGENDA_PROG, AGENDA_VERSION, "udp");

   /* verifica se a refer�ncia foi criada */
   if (clnt == (CLIENT *) NULL)
   {
      clnt_pcreateerror (argv[1]);
      exit(1);
   }

    while (1){
        printf("##################### AGENDA ####################\n");
        printf("INSERIR CONTATO:   1\n");
        printf("CONSULTAR CONTATO: 2\n");
        printf("ALTERAR CONTATO:   3\n");
        printf("EXCLUIR CONTATO:   4\n");
        printf("SAIR:              5\n");
        printf("Digite o numero da opção que deseja: ");
        scanf(" %d", &opcao);
        if(opcao > 0 && opcao < 5){
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
                *c = consulta(clnt, nome);
                if(c == NULL){
                    printf("Contato não encontrado\n");
                }else{
                    printf("########## CONTATO ###########\n");
                    printf("Nome: %s\n", c->nome);
                    printf("Endereço: %s\n", c->endereco);
                    printf("Telefone: %d\n", c->telefone);
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
                ctt.nome = nome;
                ctt.endereco = endereco;
                ctt.telefone = telefone;

                retorno = altera(clnt, ctt);

                if(retorno)
                    printf("Contato alterado\n");
                else
                    printf("Erro ao alterar contato\n");
                    
                break;

            case 4:
                printf("Digite o nome do contato a ser alterado: ");
                scanf(" %[^\n]", nome);
                retorno = remover(clnt, nome);
                if(retorno)
                    printf("Contato removido\n");
                else
                    printf("Erro ao remover contato\n");

                break;
            default:
                return 0;
                break;
            }
        }
    }
    

   return (0);
} 
