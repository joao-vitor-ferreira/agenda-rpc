#define PROGRAM_NUMBER 1111111
#define VERSION_NUMBER 1

/* tipo de dado que ser� passado aos procedimentos remotos */

struct contato
{
    string nome<>;
    string endereco<>;
    int telefone;
    int erro;
    contato *next;
};

program AGENDA_PROG
{
   version AGENDA_VERSION
   {
        int INSERE(contato) = 1;
        contato CONSULTA(string) = 2;
        int ALTERA(contato) = 3;
        int REMOVER(string) = 4;
        int REMOVER_TODOS(void) = 5;
   }
   = VERSION_NUMBER;
}
= PROGRAM_NUMBER;
