/* Cliente RPC simples */

#include <stdio.h>
#include "addsub.h"

/* fun��o que chama a RPC add_1 */
int add (CLIENT *clnt, int x, int y)
{
   operands ops;
   int *result;

   /* junta os par�metros em um struct */
   ops.x = x;
   ops.y = y;

   /* chama a fun��o remota */
   result = add_1 (&ops,clnt);
   if (result == NULL)
   {
     printf ("Problemas ao chamar a fun��o remota\n");
     exit (1);
   }

   return (*result);
}

/* fun��o que chama a RPC sub_1 */
int sub (CLIENT *clnt, int x, int y)
{
   operands ops;
   int *result;

   /* junta os par�metros em um struct */
   ops.x = x;
   ops.y = y;

   /* chama a fun��o remota */
   result = sub_1 (&ops,clnt);
   if (result == NULL)
   {
      printf ("Problemas ao chamar a fun��o remota\n");
      exit (1);
   }
   return (*result);
}

aa soma (CLIENT *clnt, int x, int y)
{
   operands ops;
   aa *o;

   /* junta os par�metros em um struct */
   ops.x = x;
   ops.y = y;

   /* chama a fun��o remota */
   o = soma_1 (&ops, clnt);
   if (o == NULL){
      printf ("Problemas ao chamar a fun��o remota\n");
      exit (1);
   } else if(o->error == 1) {
      printf("Nao encontrado\n");
   }
   return (*o);
} 

int main( int argc, char *argv[])
{
   CLIENT *clnt;
   int x,y;
   aa o;

   /* verifica se o cliente foi chamado corretamente */
   if (argc!=4)
   {
      fprintf (stderr,"Usage: %s hostname num1 num2\n",argv[0]);
      exit (1);
   }

   /* cria uma struct CLIENT que referencia uma interface RPC */
   clnt = clnt_create (argv[1], ADDSUB_PROG, ADDSUB_VERSION, "udp");

   /* verifica se a refer�ncia foi criada */
   if (clnt == (CLIENT *) NULL)
   {
      clnt_pcreateerror (argv[1]);
      exit(1);
   }

   /* obt�m os dois inteiros que ser�o passados via RPC */
   x = atoi (argv[2]);
   y = atoi (argv[3]);

   /* executa os procedimentos remotos */
   printf ("%d + %d = %d\n", x, y, add (clnt,x,y));
   printf ("%d - %d = %d\n", x, y, sub (clnt,x,y));
   o = soma(clnt, x, y);
   printf ("soma: %d sub: %d\n", o.soma, o.sub);


   return (0);
} 