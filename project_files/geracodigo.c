#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "geracodigo.h"

#define ARR_SIZE 1024  /* tamanho máximo do vetor */
#define INIT_LENGTH 24 /* tamanho inicial do array sempre iniciado com codigo de maquina de: iniciar RA, abrir espaço no RA e alocar variáveis locais */

// typedef
typedef int (*funcp)();

// prototypes
void add_var_ret_machineCode(unsigned char arr[], int var_id, int line, int arr_size); /* pensar em um nome melhor pra essa função */
void add_param_ret_machineCode(unsigned char arr[], int idx0, int line, int arr_size); /* pensar em um nome melhor pra essa função */
static void error(const char *msg, int line);

// code
funcp geraCodigo(FILE *f, unsigned char codigo[])
{
    funcp func;
    int line = 1;
    int c, curr_length = INIT_LENGTH;
    unsigned char tmp_arr[ARR_SIZE] = {0x55, /* array inicializado com codigos de maquina de: iniciar ra, abrir espaco no ra e alocar variaveis locais */
                                       0x48, 0x89, 0xe5,
                                       0x48, 0x83, 0xec, 0x20,
                                       0x48, 0x8d, 0x55, 0xe0,
                                       0x48, 0x8d, 0x4d, 0xe8,
                                       0x4c, 0x8d, 0x45, 0xf0,
                                       0x4c, 0x8d, 0x4d, 0xf8};

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

            /* caso retornar parametro */
            if (var0 == 'p')
            {
                /* caso seja primeiro parametro (familia rdi) */
                if (idx0 == 1)
                {

                    add_param_ret_machineCode(tmp_arr, idx0, line, curr_length);

                    for (int i = 0; i < curr_length + 4; i++)
                        codigo[i] = tmp_arr[i];
                }
                /* caso seja segundo parametro (familia rsi) */
                else
                {
                    add_param_ret_machineCode(tmp_arr, idx0, line, curr_length);

                    for (int i = 0; i < curr_length + 4; i++)
                        codigo[i] = tmp_arr[i];
                }
            }
            /* caso retornar variavel */
            else
            {
                /* caso retorne a primeira variavel local */
                if (idx0 == 1)
                {
                    add_var_ret_machineCode(tmp_arr, idx0, line, curr_length);

                    for (int i = 0; i < ARR_SIZE; i++)
                        codigo[i] = tmp_arr[i];
                }
                /* caso retorne a segunda variavel local */
                else if (idx0 == 2)
                {
                    add_var_ret_machineCode(tmp_arr, idx0, line, curr_length);

                    for (int i = 0; i < ARR_SIZE; i++)
                        codigo[i] = tmp_arr[i];
                }
                /* caso retorne a terceira variavel local */
                else if (idx0 == 3)
                {
                    add_var_ret_machineCode(tmp_arr, idx0, line, curr_length);

                    for (int i = 0; i < ARR_SIZE; i++)
                        codigo[i] = tmp_arr[i];
                }
                /* caso retorne a quarta variavel local */
                else
                {
                    add_var_ret_machineCode(tmp_arr, idx0, line, curr_length);

                    for (int i = 0; i < ARR_SIZE; i++)
                        codigo[i] = tmp_arr[i];
                }
            }

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

            printf("%d %c%d %c= %c%d\n", line, var0, idx0, op, var1, idx1); /* machine code de manipulacao de variavel local em assembly */
            break;
        }

            // case 'p': /* atribuicao e op. aritmetica de parametros */
            // {
            //     char var0 = c;  /* primeiro parametro, à esquerda do = */
            //     char var1;      /* segundo(a) variável/parametro, à direita do = */
            //     char op;        /* sinal de operação sendo feita: soma, subtração... */
            //     int idx0, idx1; /* id de cada variável/parametro, 1 ou 2 */

            //     if (fscanf(f, "%d %c= %c%d", &idx0, &op, &var1, &idx1) != 4)
            //         error("comando invalido", line);

            //     printf("%d %c%d %c= %c%d\n", line, var0, idx0, op, var1, idx1); /* machine code de manipulacao de parametros em assembly */
            //     break;
            // }

            // case 'i': /* desvio condicional (if) */
            // {
            //     char var0; /* variavel/parametro que esta sendo comparado */
            //     int idx0;  /* id do(a) parametro/variavel comparada */
            //     int n1;    /* numero da linha to go if true */
            //     int n2;    /* numero da linha to go if false */

            //     if (fscanf(f, "f %c%d %d %d", &var0, &idx0, &n1, &n2) != 4)
            //         error("comando invalido", line);

            //     printf("%d if %c%d %d %d\n", line, var0, idx0, n1, n2); /* machine code de cmp em assembly */
            //     break;
            // }

            // case 'g': /* desvio incondicional (go) */
            // {
            //     int n1; /* numero da linha to go */

            //     if (fscanf(f, "o %d", &n1) != 1)
            //         error("comando invalido", line);

            //     printf("%d go %d\n", line, n1); /* machine code de jmp em assembly */
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

/* atualizar "cabecalho" do array de acordo com a quantidade de variáveis locais */
void add_var_ret_machineCode(unsigned char arr[], int idx0, int line, int arr_size)
{
    /* talvez precise usar esse tmp_arr depois que implementar função para contar quantidade de machine codes que estão sendo usados */
    // unsigned char tmp_arr[ARR_SIZE];

    switch (idx0)
    {
    case 1 /* rdx */:

        /* preenchendo array com mov 200 para variavel local e retornar esse 200 para TESTES */
        arr[arr_size] = 0xba;
        arr[arr_size + 1] = 0xc8; // daqui
        arr[arr_size + 2] = 0x00;
        arr[arr_size + 3] = 0x00;
        arr[arr_size + 4] = 0x00; // até aqui é codigo de maquina no mov 200, edx (remover depois)
        arr[arr_size + 5] = 0x89;
        arr[arr_size + 6] = 0xd0;
        arr[arr_size + 7] = 0xc9;
        arr[arr_size + 8] = 0xc3;

        break;

    case 2 /* rdx e rcx */:

        /* preenchendo array com mov 200 para variavel local e retornar esse 200 para TESTES */
        arr[arr_size] = 0xb9;
        arr[arr_size + 1] = 0xc8; // daqui
        arr[arr_size + 2] = 0x00;
        arr[arr_size + 3] = 0x00;
        arr[arr_size + 4] = 0x00; // até aqui (remover depois)
        arr[arr_size + 5] = 0x89;
        arr[arr_size + 6] = 0xc8;
        arr[arr_size + 7] = 0xc9;
        arr[arr_size + 8] = 0xc3;

        break;

    case 3 /* rdx, rcx e r8 */:

        /* preenchendo array com mov 200 para variavel local e retornar esse 200 para TESTES */
        arr[arr_size] = 0x41;
        arr[arr_size + 1] = 0xb8;
        arr[arr_size + 2] = 0xc8; // daqui
        arr[arr_size + 3] = 0x00;
        arr[arr_size + 4] = 0x00;
        arr[arr_size + 5] = 0x00; // ate aqui (remover depois)
        arr[arr_size + 6] = 0x44;
        arr[arr_size + 7] = 0x89;
        arr[arr_size + 8] = 0xc0;
        arr[arr_size + 9] = 0xc9;
        arr[arr_size + 10] = 0xc3;

        break;

    case 4 /* rdx, rcx, r8 e r9 */:

        /* preenchendo array com mov 200 para variavel local e retornar esse 200 para TESTES */
        arr[arr_size] = 0x41;
        arr[arr_size + 1] = 0xb9;
        arr[arr_size + 2] = 0xc8; // daqui
        arr[arr_size + 3] = 0x00;
        arr[arr_size + 4] = 0x00;
        arr[arr_size + 5] = 0x00; // ate aqui (remover depois)
        arr[arr_size + 6] = 0x44;
        arr[arr_size + 7] = 0x89;
        arr[arr_size + 8] = 0xc8;
        arr[arr_size + 9] = 0xc9;
        arr[arr_size + 10] = 0xc3;

        break;

    default:
        error("número de variável local incorreto", line);
    }
}

void add_param_ret_machineCode(unsigned char arr[], int idx0, int line, int arr_size)
{
    /* caso seja primeiro parametro (familia rdi) */
    if (idx0 == 1)
    {
        /* (mov edi, eax) na posicao len-3 e len-2; leave na posicao len-1; ret na posicao len */
        arr[arr_size] = 0x89;     /* primeiro byte do mov  edi, eax */
        arr[arr_size + 1] = 0xf8; /* segundo byte do mov edi, eax */
        arr[arr_size + 2] = 0xc9; /* leave */
        arr[arr_size + 3] = 0xc3; /* ret */
    }

    /* caso seja segundo parametro (familia rsi) */
    else if (idx0 == 2)
    {
        /* (mov esi, eax) na posicao len-3 e len-2; leave na posicao len-1; ret na posicao len */
        arr[arr_size] = 0x89;     /* primeiro byte do mov esi, eax */
        arr[arr_size + 1] = 0xf0; /* segundo byte do mov esi, eax */
        arr[arr_size + 2] = 0xc9; /* leave */
        arr[arr_size + 3] = 0xc3; /* ret */
    }
    else
        error("número de parâmetro incorreto", line);
}

static void error(const char *msg, int line)
{
    fprintf(stderr, "erro %s na linha %d\n", msg, line);
    exit(EXIT_FAILURE);
}
