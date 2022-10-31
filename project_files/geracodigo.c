#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "geracodigo.h"

// typedef
typedef int (*funcp)();

// prototypes
funcp geraCodigo(FILE *f, unsigned char codigo[]);
static void error(const char *msg, int line);

// code
static void error(const char *msg, int line)
{
    fprintf(stderr, "erro %s na linha %d\n", msg, line);
    exit(EXIT_FAILURE);
}

funcp geraCodigo(FILE *f, unsigned char codigo[])
{
    int line = 1;
    int c;

    while ((c = fgetc(f)) != EOF)
    {
        switch (c)
        {

        case 'r': /* retorno */
        {
            char var0; /* representa qual variavel: local ou parametro */
            int idx0;  /* numero da variavel retornada 1 ou 2 */

            if (fscanf(f, "et %c%d", &var0, &idx0) != 2)
                error("comando invalido", line);

            printf("%d ret %c%d\n", line, var0, idx0); /* trocar por append do opcode equivalente. opcode de ret ou leave/ret?? */
            /* acho q o opcode de leave/ret são sempre [c9 c3] */
            break;
        }

        case 'v': /* atribuicao e op. aritmetica de variaveis locais */
        {
            char var0 = c;  /* primeira variável, à esquerda do = */
            char var1;      /* segundo(a) variável/parametro, à direita do = */
            char op;        /* sinal de operação sendo feita: soma subtração... */
            int idx0, idx1; /* id de cada variável/parametro, 1 ou 2 */

            if (fscanf(f, "%d %c= %c%d", &idx0, &op, &var1, &idx1) != 4)
                error("comando invalido", line);

            printf("%d %c%d %c= %c%d\n", line, var0, idx0, op, var1, idx1); /* opcode de manipulacao de variavel local em assembly */
            break;
        }

        case 'p': /* atribuicao e op. aritmetica de parametros */
        {
            char var0 = c;  /* primeiro parametro, à esquerda do = */
            char var1;      /* segundo(a) variável/parametro, à direita do = */
            char op;        /* sinal de operação sendo feita: soma, subtração... */
            int idx0, idx1; /* id de cada variável/parametro, 1 ou 2 */

            if (fscanf(f, "%d %c= %c%d", &idx0, &op, &var1, &idx1) != 4)
                error("comando invalido", line);

            printf("%d %c%d %c= %c%d\n", line, var0, idx0, op, var1, idx1); /* opcode de manipulacao de parametros em assembly */
            break;
        }

        case 'i': /* desvio condicional (if) */
        {
            char var0; /* variavel/parametro que esta sendo comparado */
            int idx0;  /* id do(a) parametro/variavel comparada */
            int n1;    /* numero da linha to go if true */
            int n2;    /* numero da linha to go if false */

            if (fscanf(f, "f %c%d %d %d", &var0, &idx0, &n1, &n2) != 4)
                error("comando invalido", line);

            printf("%d if %c%d %d %d\n", line, var0, idx0, n1, n2); /* opcode de cmp em assembly */
            break;
        }

        case 'g': /* desvio incondicional (go) */
        {
            int n1; /* numero da linha to go */

            if (fscanf(f, "o %d", &n1) != 1)
                error("comando invalido", line);

            printf("%d go %d\n", line, n1); /* opcode de jmp em assembly */
            break;
        }

        default:
            error("comando desconhecido", line);
        }

        line++;
        fscanf(f, " ");
    }

    return 0;
}
