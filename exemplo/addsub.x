/* addsub.x : defini��o da interface */

/*#define PROGRAM_NUMBER 12345678
#define VERSION_NUMBER 1*/

#define PROGRAM_NUMBER 1111111
#define VERSION_NUMBER 1

/* tipo de dado que ser� passado aos procedimentos remotos */

struct operands
{
        int x;
        int y;
};

struct aa{
  int soma;
  int sub;
  int error;
};

/* Defini��o da interface que ser� oferecida aos clientes */

program ADDSUB_PROG
{
   version ADDSUB_VERSION
   {
     int ADD (operands) = 1;
     int SUB (operands) = 2;
     aa SOMA(operands) = 3;
   }
   = VERSION_NUMBER;
}
= PROGRAM_NUMBER;

 
