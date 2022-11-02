#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "geracodigo.h"

#define true 1
#define false 0

// typedef
typedef int (*funcp)();

// prototypes
funcp geraCodigo(FILE *f, unsigned char codigo[]);
static void error(const char *msg, int line);
int is_empty(unsigned char arr[]);

// code
funcp geraCodigo(FILE *f, unsigned char codigo[])
{
    funcp func;
    int line = 1;
    int c;
    unsigned char *tmp_arr; /* array local temporaria para adicionar os opcodes e depois passar para array do parametro */
    tmp_arr = (unsigned char *)malloc(4 * sizeof(unsigned char));
    tmp_arr[0] = 0x55;
    tmp_arr[1] = 0x48;
    tmp_arr[2] = 0x89;
    tmp_arr[3] = 0xe5;
    /* por estar usando malloc n é possível pegar o length do array com a linha de baixo?? */
    // int length = sizeof(tmp_arr) / sizeof(tmp_arr[0]);

    int length = 4; /* se necessário alterar do hard-coded */

    codigo = (unsigned char *)malloc(4 * sizeof(unsigned char));
    codigo = tmp_arr;

    while ((c = fgetc(f)) != EOF)
    {
        switch (c)
        {

        case 'r': /* retorno */
        {
            char var0; /* representa qual variavel: local ou parametro */
            int idx0;  /* numero da variavel retornada 1 ou 2 */
            unsigned char *aux_arr;

            if (fscanf(f, "et %c%d", &var0, &idx0) != 2)
                error("comando invalido", line);

            /* debug printf */
            printf("\ntmp:\n");
            for (int i = 0; i < length; i++)
                printf("[%x] ", tmp_arr[i]);
            printf("\n");

            /* caso retornar parametro */
            if (var0 == 'p')
            {
                aux_arr = (unsigned char *)malloc((length + 4) * sizeof(unsigned char)); /* array auxiliar pra aumentar o tamanho do array inicial */
                /* somando 4 no tamanho original pois: (mov parametro, eax) ocupa 2 bytes; leave ocupa 1 byte; ret ocupa 1 byte */

                /* passando elementos pra array auxiliar */
                for (int i = 0; i < length; i++)
                    aux_arr[i] = tmp_arr[i];

                /* debug printf */
                printf("\naux:\n");
                for (int i = 0; i < length + 4; i++)
                    printf("[%x] ", aux_arr[i]);
                printf("\n");

                /* caso seja primeiro parametro (familia rdi) */
                if (idx0 == 1)
                {
                    /* (mov edi, eax) na posicao len-3 e len-2; leave na posicao len-1; ret na posicao len */
                    aux_arr[(length + 4) - 4] = 0x89; /* primeiro byte do mov */
                    aux_arr[(length + 4) - 3] = 0xf8; /* segundo byte do mov */
                    aux_arr[(length + 4) - 2] = 0xc9; /* leave */
                    aux_arr[(length + 4) - 1] = 0xc3; /* ret */

                    /* debug printf */
                    printf("\naux preenchido:\n");
                    for (int i = 0; i < length + 4; i++)
                        printf("[%x] ", aux_arr[i]);
                    printf("\n");

                    codigo = (unsigned char *)realloc(codigo, (length + 4) * sizeof(unsigned char));

                    for (int i = 0; i < length + 4; i++)
                        codigo[i] = aux_arr[i];

                    /* debug printf */
                    printf("\ncodigo updated:\n");
                    for (int i = 0; i < length + 4; i++)
                        printf("[%x] ", codigo[i]);
                    printf("\n");
                }

                /* caso seja segundo parametro (familia rsi) */
                else
                {
                    /* (mov esi, eax) na posicao len-3 e len-2; leave na posicao len-1; ret na posicao len */
                }
            }

            /* caso retornar variavel */
            else
            {
                /* caso seja a primeira variavel local */
                if (idx0 == 1)
                {
                }

                /* caso seja a segunda variavel local */
                else if (idx0 == 2)
                {
                }

                /* caso seja a terceira variavel local */
                else if (idx0 == 3)
                {
                }

                /* caso seja a quarta variavel local */
                else
                {
                }
            }

            break;
        }

        /* função pra abrir espaço na pilha de acordo com o numero da variavel */
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

            // case 'i': /* desvio condicional (if) */
            // {
            //     char var0; /* variavel/parametro que esta sendo comparado */
            //     int idx0;  /* id do(a) parametro/variavel comparada */
            //     int n1;    /* numero da linha to go if true */
            //     int n2;    /* numero da linha to go if false */

            //     if (fscanf(f, "f %c%d %d %d", &var0, &idx0, &n1, &n2) != 4)
            //         error("comando invalido", line);

            //     printf("%d if %c%d %d %d\n", line, var0, idx0, n1, n2); /* opcode de cmp em assembly */
            //     break;
            // }

            // case 'g': /* desvio incondicional (go) */
            // {
            //     int n1; /* numero da linha to go */

            //     if (fscanf(f, "o %d", &n1) != 1)
            //         error("comando invalido", line);

            //     printf("%d go %d\n", line, n1); /* opcode de jmp em assembly */
            //     break;
            // }

        default:
            error("comando desconhecido", line);
        }

        line++;
        fscanf(f, " ");
    }

    // (*func)();
    return func;
}

static void error(const char *msg, int line)
{
    fprintf(stderr, "erro %s na linha %d\n", msg, line);
    exit(EXIT_FAILURE);
}

int is_empty(unsigned char arr[])
{
    return arr[0] == 0x0 ? true : false;
}
