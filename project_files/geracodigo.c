#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "geracodigo.h"

#define true 1
#define false 0
#define ARR_SIZE 512

// typedef
typedef int (*funcp)();

// prototypes
funcp geraCodigo(FILE *f, unsigned char codigo[]);
static void error(const char *msg, int line);
unsigned char *update_arr_with_var(unsigned char arr[], int var_id, int line);

// code
funcp geraCodigo(FILE *f, unsigned char codigo[])
{
    funcp func;
    int line = 1;
    int c, init_length = 4, length; /* se necessário alterar o hard code de init_length */
    unsigned char tmp_arr[ARR_SIZE] = {0x55,
                                       0x48, 0x89, 0xe5}; /* array local temporaria para adicionar os opcodes e depois passar para array do parametro */
    length = sizeof(tmp_arr) / sizeof(tmp_arr[0]);
    printf("SIZE: %d\n", length);

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

            /* caso retornar parametro */
            if (var0 == 'p')
            {
                aux_arr = (unsigned char *)malloc((init_length + 4) * sizeof(unsigned char)); /* array auxiliar pra aumentar o tamanho do array inicial */
                /* somando 4 no tamanho original pois: (mov parametro, eax) ocupa 2 bytes; leave ocupa 1 byte; ret ocupa 1 byte */

                /* passando elementos pra array auxiliar */
                for (int i = 0; i < init_length; i++)
                    aux_arr[i] = tmp_arr[i];

                /* caso seja primeiro parametro (familia rdi) */
                if (idx0 == 1)
                {
                    /* (mov edi, eax) na posicao len-3 e len-2; leave na posicao len-1; ret na posicao len */
                    aux_arr[init_length] = 0x89;     /* primeiro byte do mov */
                    aux_arr[init_length + 1] = 0xf8; /* segundo byte do mov */
                    aux_arr[init_length + 2] = 0xc9; /* leave */
                    aux_arr[init_length + 3] = 0xc3; /* ret */

                    for (int i = 0; i < init_length + 4; i++)
                        codigo[i] = aux_arr[i];
                }

                /* caso seja segundo parametro (familia rsi) */
                else
                {
                    /* (mov esi, eax) na posicao len-3 e len-2; leave na posicao len-1; ret na posicao len */
                    aux_arr[init_length] = 0x89;     /* primeiro byte do mov */
                    aux_arr[init_length + 1] = 0xf0; /* segundo byte do mov */
                    aux_arr[init_length + 2] = 0xc9; /* leave */
                    aux_arr[init_length + 3] = 0xc3; /* ret */

                    for (int i = 0; i < init_length + 4; i++)
                        codigo[i] = aux_arr[i];
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

    func = (funcp)codigo;
    return func;
}

static void error(const char *msg, int line)
{
    fprintf(stderr, "erro %s na linha %d\n", msg, line);
    exit(EXIT_FAILURE);
}

/* retona array com o "cabeçalho" atualizado de acordo com a quantidade de variáveis locais */
unsigned char *update_arr_with_var(unsigned char arr[], int var_id, int line)
{
    unsigned char tmp_arr[ARR_SIZE];
    switch (var_id)
    {
    case 1 /* rdx */:
        unsigned char arr_1var[ARR_SIZE] = {0x55,                   /* pushq %rbp */
                                            0x48, 0x89, 0xe5,       /* movq %rsp, %rbp*/
                                            0x48, 0x83, 0xec, 0x10, /* subq $16, %rsp */
                                            0x48, 0x8d, 0x55, 0xf0};

        int old_arr_size = 4;  /* numero de opcodes ocupados para cabecalho se não tiver variavel local */
        int new_arr_size = 12; /* numero de opcodes ocupados para cabecalho de 1 variavel local */

        /* for loop pra passar informacoes, exceto cabecalho, do array original pro novo array com cabecalho pra 1 variavel local */
        for (int i = new_arr_size, j = old_arr_size; i < ARR_SIZE; i++, j++)
        {
            arr_1var[i] = arr[j];
        }

        return arr_1var;
        break;

    case 2 /* rdx e rcx */:
        unsigned char arr_2var[ARR_SIZE] = {0x55,                   /* pushq %rbp */
                                            0x48, 0x89, 0xe5,       /* movq %rsp, %rbp*/
                                            0x48, 0x83, 0xec, 0x10, /* subq $16, %rsp */
                                            0x48, 0x8d, 0x55, 0xf0,
                                            0x48, 0x8d, 0x4d, 0xf8};

        int old_arr_size = 12; /* numero de opcodes ocupados para cabecalho de 1 variaveis locais */
        int new_arr_size = 16; /* numero de opcodes ocupados para cabecalho de 2 variaveis locais */

        for (int i = new_arr_size, j = old_arr_size; i < ARR_SIZE; i++, j++)
        {
            arr_2var[i] = arr[j];
        }

        /* se for a segunda variável local, iterar até opcode de alocacao da primeira variavel local (leaq xxx??) */
        return arr_2var;
        break;

    case 3 /* rdx, rcx e r8 */:
        unsigned char arr_3var[ARR_SIZE] = {0x55,                   /* pushq %rbp */
                                            0x48, 0x89, 0xe5,       /* movq %rsp, %rbp*/
                                            0x48, 0x83, 0xec, 0x20, /* subq $32, %rsp*/
                                            0x48, 0x8d, 0x55, 0xe0,
                                            0x48, 0x8d, 0x4d, 0xe8,
                                            0x4c, 0x8d, 0x45, 0xf0};

        int old_arr_size = 16; /* numero de opcodes ocupados para cabecalho de 2 variaveis locais */
        int new_arr_size = 20; /* numero de opcodes ocupados para cabecalho de 3 variaveis locais */

        for (int i = new_arr_size, j = old_arr_size; i < ARR_SIZE; i++, j++)
        {
            arr_3var[i] = arr[j];
        }

        /* se for a terceira variável local, iterar até opcode de alocacao da segunda variavel local (leaq xxx??) */
        return arr_3var;
        break;

    case 4 /* rdx, rcx, r8 e r9 */:
        unsigned char arr_4var[ARR_SIZE] = {0x55,                   /* pushq %rbp */
                                            0x48, 0x89, 0xe5,       /* movq %rsp, %rbp*/
                                            0x48, 0x83, 0xec, 0x20, /* subq $32, %rsp*/
                                            0x48, 0x8d, 0x55, 0xe0,
                                            0x48, 0x8d, 0x4d, 0xe8,
                                            0x4c, 0x8d, 0x45, 0xf0,
                                            0x4c, 0x8d, 0x4d, 0xf8};

        int old_arr_size = 20; /* numero de opcodes ocupados para cabecalho de 3 variaveis locais */
        int new_arr_size = 24; /* numero de opcodes ocupados para cabecalho de 4 variavel local */

        for (int i = new_arr_size, j = old_arr_size; i < ARR_SIZE; i++, j++)
        {
            arr_4var[i] = arr[j];
        }

        /* se for a quarta variável local, iterar até opcode de alocacao da terceira variavel local (leaq xxx??) */
        return arr_4var;
        break;

    default:
        error("número de variável local incorreto", line);
    }
}
