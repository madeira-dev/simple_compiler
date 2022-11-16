#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "geracodigo.h"

#define ARR_SIZE 1024  /* tamanho máximo do vetor */
#define INIT_LENGTH 24 /* tamanho inicial do array sempre iniciado com codigo de maquina de: iniciar RA, abrir espaço no RA e alocar variáveis locais */

/* typedef */
typedef int (*funcp)();

/* functions prototypes */
// return
void return_var(unsigned char arr[], int var_id, int line, int arr_size);     /* pensar em um nome melhor pra essa função */
void return_parameter(unsigned char arr[], int idx0, int line, int arr_size); /* pensar em um nome melhor pra essa função */

// var manipulation
void var_add_operation(char var0, int idx0, char var1, int idx1, char op, int line);
void var_sub_operation(char var0, int idx0, char var1, int idx1, char op, int line);
void var_mult_operation(char var0, int idx0, char var1, int idx1, char op, int line);

void par_add_operation(unsigned char arr[], int *arr_size, char var0, int idx0, char var1, int idx1, char op, int line);
void par_sub_operation(unsigned char arr[], int *arr_size, char var0, int idx0, char var1, int idx1, char op, int line);
void par_mult_operation(unsigned char arr[], int *arr_size, char var0, int idx0, char var1, int idx1, char op, int line);
void par_attr_operation(unsigned char arr[], int *arr_size, char var0, int idx0, char var1, int idx1, char op, int line);

    // error
    static void error(const char *msg, int line);

/* code */
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

                    return_parameter(tmp_arr, idx0, line, curr_length);

                    for (int i = 0; i < curr_length + 4; i++)
                        codigo[i] = tmp_arr[i];
                }
                /* caso seja segundo parametro (familia rsi) */
                else
                {
                    return_parameter(tmp_arr, idx0, line, curr_length);

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
                    return_var(tmp_arr, idx0, line, curr_length);

                    for (int i = 0; i < ARR_SIZE; i++)
                        codigo[i] = tmp_arr[i];
                }
                /* caso retorne a segunda variavel local */
                else if (idx0 == 2)
                {
                    return_var(tmp_arr, idx0, line, curr_length);

                    for (int i = 0; i < ARR_SIZE; i++)
                        codigo[i] = tmp_arr[i];
                }
                /* caso retorne a terceira variavel local */
                else if (idx0 == 3)
                {
                    return_var(tmp_arr, idx0, line, curr_length);

                    for (int i = 0; i < ARR_SIZE; i++)
                        codigo[i] = tmp_arr[i];
                }
                /* caso retorne a quarta variavel local */
                else
                {
                    return_var(tmp_arr, idx0, line, curr_length);

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

            /* verificar numero da variavel */
            /* verificar operação */
            /* -> uma funcao para cada operacao */
            /* verificar variavel/parametro depois do sinal de operacao */
            /* verificar numero da variavel/parametro depois do sinal de operacao */

            printf("%d %c%d %c= %c%d\n", line, var0, idx0, op, var1, idx1); /* machine code de manipulacao de variavel local em assembly */
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
                if(op == '+')
                {
                    par_add_operation(tmp_arr, &curr_length, var0, idx0, var1, idx1, op, line);
                }
                else if(op == '-')
                {
                    // chama funcao de subtracao de parametros
                    par_sub_operation(tmp_arr, &curr_length, var0, idx0, var1, idx1, op, line);
                }
                else if(op == ':')
                {
                    // chama funcao de atribuicao
                    par_attr_operation(tmp_arr, &curr_length, var0, idx0, var1, idx1, op, line);
                }
                else
                {
                    // chama funcao de multiplicacao
                    par_mult_operation(tmp_arr, &curr_length, var0, idx0, var1, idx1, op, line);
                } 
                printf("%d %c%d %c= %c%d\n", line, var0, idx0, op, var1, idx1); /* machine code de manipulacao de parametros em assembly */
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
void return_var(unsigned char arr[], int idx0, int line, int arr_size)
{
    /* talvez precise usar esse tmp_arr depois que implementar função para contar quantidade de machine codes que estão sendo usados */
    // unsigned char tmp_arr[ARR_SIZE];

    switch (idx0)
    {
    case 1 /* rdx */:
        /* codigos de maquina de mover para eax e leave/ret */
        arr[arr_size] = 0x89;
        arr[arr_size + 1] = 0xd0;
        arr[arr_size + 2] = 0xc9;
        arr[arr_size + 3] = 0xc3;

        break;

    case 2 /* rdx e rcx */:
        /* codigos de maquina de mover para eax e leave/ret */
        arr[arr_size] = 0x89;
        arr[arr_size + 1] = 0xc8;
        arr[arr_size + 2] = 0xc9;
        arr[arr_size + 3] = 0xc3;

        break;

    case 3 /* rdx, rcx e r8 */:
        /* codigos de maquina de mover para eax e leave/ret */
        arr[arr_size] = 0x44;
        arr[arr_size + 1] = 0x89;
        arr[arr_size + 2] = 0xc0;
        arr[arr_size + 3] = 0xc9;
        arr[arr_size + 4] = 0xc3;

        break;

    case 4 /* rdx, rcx, r8 e r9 */:
        /* codigos de maquina de mover para eax e leave/ret */
        arr[arr_size] = 0x44;
        arr[arr_size + 1] = 0x89;
        arr[arr_size + 2] = 0xc8;
        arr[arr_size + 3] = 0xc9;
        arr[arr_size + 4] = 0xc3;

        break;

    default:
        error("numero de variavel local invalido", line);
    }
}

void return_parameter(unsigned char arr[], int idx0, int line, int arr_size)
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
        error("numero de parâmetro invalido", line);
}

void var_add_operation(char var0, int idx0, char var1, int idx1, char op, int line)
{
    if (var1 == '$') /* constante */
    {
    }

    else if (var1 == 'v') /* variavel */
    {
        switch (idx1)
        {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            error("numero de variavel invalido", line);
            break;
        }
    }

    else if (var1 == 'p') /* parametro */
    {
        if (idx1 == 1)
        {
        }

        else if (idx1 == 2)
        {
        }
        else
            error("numero de parametro invalido", line);
    }

    else
        error("variavel invalida", line);
}
void var_sub_operation(char var0, int idx0, char var1, int idx1, char op, int line);
void var_mult_operation(char var0, int idx0, char var1, int idx1, char op, int line);

// var0 "%d %c= %c%d", &idx0, &op, &var1, &idx1
void par_add_operation(unsigned char arr[], int *arr_size, char var0, int idx0, char var1, int idx1, char op, int line)
{
    switch (idx0)
    {
    case 1:
    {
        if(var1 == 'p')
        {
            if (idx1 == 1) // p1 += p1
            {
                arr[*arr_size] = 0x01;
                arr[(*arr_size) + 1] = 0xff;
                (*arr_size) += 2;

            }
            else if (idx1 == 2) // p1 += p2
            {
                arr[*arr_size] = 0x01;
                arr[(*arr_size) + 1] = 0xf7;
                (*arr_size) += 2;
            }
            else 
            {
                error("numero de parametro invalido\n", line);
            }

        }
        
        else if(var1 == 'v')
        {
            switch (idx1)
            {
            case 1: // p1 += v1
            {
                arr[*arr_size] = 0x01;
                arr[(*arr_size) + 1] = 0xd7;
                *arr_size = (*arr_size) + 2;
                break;
            }
            case 2: // p1 += v2
            {
                arr[*arr_size] = 0x01;
                arr[(*arr_size) + 1] = 0xcf;
                *arr_size = (*arr_size) + 2;
                break;
            }
            case 3: // p1 += v3
            {
                arr[*arr_size] = 0x44;
                arr[(*arr_size) + 1] = 0x01;
                arr[(*arr_size) + 2] = 0xc7;
                *arr_size = (*arr_size) + 3;
                break;
            }
            case 4: // p1 += v4
            {
                arr[*arr_size] = 0x44;
                arr[(*arr_size) + 1] = 0x01;
                arr[(*arr_size) + 2] = 0xcf;
                *arr_size = (*arr_size) + 3;

                break;
            }
            default:
            {
                error("numero de variavel invalido\n", line);
                break;
            }
            
            }
        }
        else if (var1 == '$')
        {

        }
        else
            error("numero de variavel invalido\n", line);
        break;
    }

    case 2: 
    {
        if (var1 == 'p')
        {
            if (idx1 == 1) // p2 += p1
            {
                arr[*arr_size] = 0x01;
                arr[(*arr_size) + 1] = 0xfe;
                *arr_size = (*arr_size) + 2;
            }
            else if (idx1 == 2) // p2 += p2
            {
                arr[*arr_size] = 0x01;
                arr[(*arr_size) + 1] = 0xf6;
                *arr_size = (*arr_size) + 2;
            }
            else
            {
                error("numero de parametro invalido\n", line);
            }
        }
        else if (var1 == 'v')
        {
            switch (idx1)
            {
            case 1: // p2+= v1
            {
                arr[*arr_size] = 0x01;
                arr[(*arr_size) + 1] = 0xd6;
                *arr_size = (*arr_size) + 2;
                break;
            }
            case 2: // p2 += v2
            {
                arr[*arr_size] = 0x01;
                arr[(*arr_size) + 1] = 0xce;
                *arr_size = (*arr_size) + 2;

                break;
            }
            case 3: // p2 += v3
            {
                arr[*arr_size] = 0x44;
                arr[(*arr_size) + 1] = 0x01;
                arr[(*arr_size) + 2] = 0xc6;
                *arr_size = (*arr_size) + 3;
                break;
            }
            case 4: // p2 += v4
            {
                arr[*arr_size] = 0x44;
                arr[(*arr_size) + 1] = 0x01;
                arr[(*arr_size) + 2] = 0xce;
                *arr_size = (*arr_size) + 3;
                break;
            }
            default:
            {
                error("numero de variavel invalido\n", line);
                break;
            }
            break;
            }
        }
        else if (var1 == '$')
        {

        }
        else
        {
            error("numero de parametro invalido\n", line);
        }
            break;
        }
    default:
        error("numero de parametro invalido\n", line);
        break;
    }
    return;
}



// mudar os codigos de maquina
void par_sub_operation(unsigned char arr[], int *arr_size, char var0, int idx0, char var1, int idx1, char op, int line)
{
    switch (idx0)
    {
    case 1:
    {
        if (var1 == 'p')
        {
            if (idx1 == 1) // p1 -= p1
            {
                arr[*arr_size] = 0x29;
                arr[(*arr_size) + 1] = 0xff;
                *arr_size = (*arr_size) +2;
                
            }
            else if (idx1 == 2) // p1 -= p2
            {
                arr[*arr_size] = 0x29;
                arr[(*arr_size) + 1] = 0xf7;
                *arr_size = (*arr_size) + 2;
            }
            else
            {
                error("numero de parametro invalido\n", line);
            }
        }

        else if (var1 == 'v')
        {
            switch (idx1)
            {
            case 1: // p1 -= v1
            {
                arr[*arr_size] = 0x29;
                arr[(*arr_size) + 1] = 0xd7;
                *arr_size = (*arr_size) + 2;
                break; 
            }
            case 2: // p1 -= v2
            {
                arr[*arr_size] = 0x29;
                arr[(*arr_size) + 1] = 0xcf;
                *arr_size = (*arr_size) + 2;
                break; 
            }
            case 3: // p1 -= v3
            {
                arr[*arr_size] = 0x44;
                arr[(*arr_size) + 1] = 0x29;
                arr[(*arr_size) + 2] = 0xc7;
                *arr_size = (*arr_size) + 3;
                break; 
            }
            case 4: // p1 -= v4
            {
                arr[*arr_size] = 0x44;
                arr[(*arr_size) + 1] = 0x29;
                arr[(*arr_size) + 2] = 0xcf;
                *arr_size = (*arr_size) + 3;
                break;
            }
            default:
            {
                error("numero de variavel invalido\n", line);
                break;
            }
            }
        }
        else if (var1 == '$')
        {
        }
        else
            error("numero de variavel invalido\n", line);
        break;
    }

    case 2:
    {
        if (var1 == 'p')
        {
            if (idx1 == 1) // p2 -= p1
            {
                arr[*arr_size] = 0x29;
                arr[(*arr_size) + 1] = 0xfe;
                *arr_size = (*arr_size) + 2;
            }
            else if (idx1 == 2) // p2 -= p2
            {
                arr[*arr_size] = 0x29;
                arr[(*arr_size) + 1] = 0xf6;
                *arr_size = (*arr_size) + 2;
            }
            else
            {
                error("numero de parametro invalido\n", line);
            }
        }
        else if (var1 == 'v')
        {
            switch (idx1)
            {
            case 1: // p2-= v1
            {
                arr[*arr_size] = 0x29;
                arr[(*arr_size) + 1] = 0xd6;
                *arr_size = (*arr_size) + 2;
                break;
            }
            case 2: // p2 -= v2
            {
                arr[*arr_size] = 0x29;
                arr[(*arr_size) + 1] = 0xce;
                *arr_size = (*arr_size) + 2;
                break;
            }
            case 3: // p2 -= v3
            {
                arr[*arr_size] = 0x44;
                arr[(*arr_size) + 1] = 0x29;
                arr[(*arr_size) + 2] = 0xc6;
                *arr_size = (*arr_size) + 3;
                break;
            }
            case 4: // p2 -= v4
            {
                arr[*arr_size] = 0x44;
                arr[(*arr_size) + 1] = 0x29;
                arr[(*arr_size) + 2] = 0xce;
                *arr_size = (*arr_size) + 3;
                break;
            }
            default:
            {
                error("numero de variavel invalido\n", line);
                break;
            }
            break;
            }
        }
        else if (var1 == '$')
        {
        }
        else
        {
            error("numero de parametro invalido\n", line);
        }
        break;
    }
    default:
        error("numero de parametro invalido\n", line);
        break;
    }
    return;
}




void par_mult_operation(unsigned char arr[], int *arr_size, char var0, int idx0, char var1, int idx1, char op, int line)
{
    switch (idx0)
    {
    case 1:
    {
        if (var1 == 'p')
        {
            if (idx1 == 1) // p1 *= p1
            {
                arr[*arr_size] = 0x0f;
                arr[(*arr_size) + 1] = 0xaf;
                arr[(*arr_size) + 2] = 0xff;
                *arr_size = (*arr_size) + 3;
            }
            else if (idx1 == 2) // p1 *= p2
            {
                arr[*arr_size] = 0x0f;
                arr[(*arr_size) + 1] = 0xaf;
                arr[(*arr_size) + 2] = 0xfe;
                *arr_size = (*arr_size) + 3;
            }
            else
            {
                error("numero de parametro invalido\n", line);
            }
        }

        else if (var1 == 'v')
        {
            switch (idx1)
            {
            case 1: // p1 *= v1
            {
                arr[*arr_size] = 0x0f;
                arr[(*arr_size) + 1] = 0xaf;
                arr[(*arr_size) + 2] = 0xfa;
                *arr_size = (*arr_size) + 3;
                break;
            }
            case 2: // p1 *= v2
            {
                arr[*arr_size] = 0x0f;
                arr[(*arr_size) + 1] = 0xaf;
                arr[(*arr_size) + 2] = 0xf9;
                *arr_size = (*arr_size) + 3;
                break; 
            }
            case 3: // p1 *= v3
            {
                arr[*arr_size] = 0x41;
                arr[(*arr_size) + 1] = 0x0f;
                arr[(*arr_size) + 2] = 0xaf;
                arr[(*arr_size) + 3] = 0xf8;
                *arr_size = (*arr_size) + 4;
                break;
            }
            case 4: // p1 *= v4
            {
                arr[*arr_size] = 0x41;
                arr[(*arr_size) + 1] = 0x0f;
                arr[(*arr_size) + 2] = 0xaf;
                arr[(*arr_size) + 3] = 0xf9;
                *arr_size = (*arr_size) + 4;

                break;
            }
            default:
            {
                error("numero de variavel invalido\n", line);
                break;
            }
            }
        }
        else if (var1 == '$')
        {
        }
        else
            error("numero de variavel invalido\n", line);
        break;
    }

    case 2:
    {
        if (var1 == 'p')
        {
            if (idx1 == 1) // p2 *= p1
            {
                arr[*arr_size] = 0x0f;
                arr[(*arr_size) + 1] = 0xaf;
                arr[(*arr_size) + 2] = 0xf7;
                *arr_size = (*arr_size) + 3;
            }
            else if (idx1 == 2) // p2 *= p2
            {
                arr[*arr_size] = 0x0f;
                arr[(*arr_size) + 1] = 0xaf;
                arr[(*arr_size) + 2] = 0xf6;
                *arr_size = (*arr_size) + 3;
            }
            else
            {
                error("numero de parametro invalido\n", line);
            }
        }
        else if (var1 == 'v')
        {
            switch (idx1)
            {
            case 1: // p2*= v1
            {
                arr[*arr_size] = 0x0f;
                arr[(*arr_size) + 1] = 0xaf;
                arr[(*arr_size) + 2] = 0xf2;
                *arr_size = (*arr_size) + 3;
                break;
            }
            case 2: // p2 *= v2
            {
                arr[*arr_size] = 0x0f;
                arr[(*arr_size) + 1] = 0xaf;
                arr[(*arr_size) + 2] = 0xf1;
                *arr_size = (*arr_size) + 3;
                break;
            }
            case 3: // p2 *= v3
            {
                arr[*arr_size] = 0x41;
                arr[(*arr_size) + 1] = 0x0f;
                arr[(*arr_size) + 2] = 0xaf;
                arr[(*arr_size) + 3] = 0xf0;
                *arr_size = (*arr_size) + 4;
                break;
            }
            case 4: // p2 *= v4
            {
                arr[*arr_size] = 0x41;
                arr[(*arr_size) + 1] = 0x0f;
                arr[(*arr_size) + 2] = 0xaf;
                arr[(*arr_size) + 3] = 0xf1;
                *arr_size = (*arr_size) + 4;
                break;
            }
            default:
            {
                error("numero de variavel invalido\n", line);
                break;
            }
            break;
            }
        }
        else if (var1 == '$')
        {
        }
        else
        {
            error("numero de parametro invalido\n", line);
        }
        break;
    }
    default:
        error("numero de parametro invalido\n", line);
        break;
    }
    return;
}

void par_attr_operation(unsigned char arr[], int *arr_size, char var0, int idx0, char var1, int idx1, char op, int line)
{
    switch (idx0)
    {
    case 1:
    {
        if (var1 == 'p')
        {
            if (idx1 == 1) // p1 := p1
            {
                arr[*arr_size] = 0x89;
                arr[(*arr_size) + 1] = 0xff;
                (*arr_size) += 2;
            }
            else if (idx1 == 2) // p1 := p2
            {
                arr[*arr_size] = 0x89;
                arr[(*arr_size) + 1] = 0xf7;
                (*arr_size) += 2;
            }
            else
            {
                error("numero de parametro invalido\n", line);
            }
        }

        else if (var1 == 'v')
        {
            switch (idx1)
            {
            case 1: // p1 := v1
            {
                arr[*arr_size] = 0x89;
                arr[(*arr_size) + 1] = 0xd7;
                break;
            }
            case 2: // p1 := v2
            {
                arr[*arr_size] = 0x89;
                arr[(*arr_size) + 1] = 0xcf;
                break;
            }
            case 3: // p1 := v3
            {
                arr[*arr_size] = 0x44;
                arr[(*arr_size) + 1] = 0x89;
                arr[(*arr_size) + 2] = 0xc7;
                break;
            }
            case 4: // p1 := v4
            {
                arr[*arr_size] = 0x44;
                arr[(*arr_size) + 1] = 0x89;
                arr[(*arr_size) + 2] = 0xcf;

                break;
            }
            default:
            {
                error("numero de variavel invalido\n", line);
                break;
            }
            }
        }
        else if (var1 == '$')
        {
        }
        else
            error("numero de variavel invalido\n", line);
        break;
    }

    case 2:
    {
        if (var1 == 'p')
        {
            if (idx1 == 1) // p2 := p1
            {
                arr[*arr_size] = 0x89;
                arr[(*arr_size) + 1] = 0xfe;
            }
            else if (idx1 == 2) // p2 := p2
            {
                arr[*arr_size] = 0x89;
                arr[(*arr_size) + 1] = 0xf6;
            }
            else
            {
                error("numero de parametro invalido\n", line);
            }
        }
        else if (var1 == 'v')
        {
            switch (idx1)
            {
            case 1: // p2+= v1
            {
                arr[*arr_size] = 0x89;
                arr[(*arr_size) + 1] = 0xd6;
                break;
            }
            case 2: // p2 += v2
            {
                arr[*arr_size] = 0x89;
                arr[(*arr_size) + 1] = 0xce;

                break;
            }
            case 3: // p2 += v3
            {
                arr[*arr_size] = 0x44;
                arr[(*arr_size) + 1] = 0x89;
                arr[(*arr_size) + 2] = 0xc6;
                break;
            }
            case 4: // p2 += v4
            {
                arr[*arr_size] = 0x44;
                arr[(*arr_size) + 1] = 0x89;
                arr[(*arr_size) + 2] = 0xce;
                break;
            }
            default:
            {
                error("numero de variavel invalido\n", line);
                break;
            }
            break;
            }
        }
        else if (var1 == '$')
        {
        }
        else
        {
            error("numero de parametro invalido\n", line);
        }
        break;
    }
    default:
        error("numero de parametro invalido\n", line);
        break;
    }
    return;
}

    static void error(const char *msg, int line)
{
    fprintf(stderr, "erro %s na linha %d\n", msg, line);
    exit(EXIT_FAILURE);
}