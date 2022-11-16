#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "geracodigo.h"

#define ARR_SIZE 1024  /* tamanho máximo do vetor */
#define INIT_LENGTH 24 /* tamanho inicial do array sempre iniciado com codigo de maquina de: iniciar RA, abrir espaço no RA e alocar variáveis locais */

/* typedef */
typedef int (*funcp)();

/* functions prototypes */
// return
void return_var(unsigned char arr[], int var_id, int line, int *curr_length);
void return_parameter(unsigned char arr[], int idx0, int line, int *curr_length);

// var manipulation
void var_attribute_operation(unsigned char arr[], char var0, int idx0, char var1, int idx1, char op, int line, int *array_length);
void var_add_operation(unsigned char arr[], char var0, int idx0, char var1, int idx1, char op, int line, int *array_length);
void var_sub_operation(unsigned char arr[], char var0, int idx0, char var1, int idx1, char op, int line, int *array_length);
void var_mult_operation(unsigned char arr[], char var0, int idx0, char var1, int idx1, char op, int line, int *array_length);

// parameter manipulation
void par_attribute_operation(unsigned char arr[], char var0, int idx0, char var1, int idx1, char op, int line, int *array_length);
void par_add_operation(unsigned char arr[], int *arr_size, char var0, int idx0, char var1, int idx1, char op, int line);

// aux
static void error(const char *msg, int line);
int string2num(char *s, int base);

/* code */
funcp geraCodigo(FILE *f, unsigned char codigo[])
{
    funcp func;
    int line = 1;
    int c, curr_length = 24;                 /* curr_length: variavel para contar espaços já preenchidos com código de máquina do array */
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
                    return_parameter(tmp_arr, idx0, line, &curr_length);
                    for (int i = 0; i < curr_length; i++)
                        codigo[i] = tmp_arr[i];
                }
                /* caso seja segundo parametro (familia rsi) */
                else
                {
                    return_parameter(tmp_arr, idx0, line, &curr_length);
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
                    return_var(tmp_arr, idx0, line, &curr_length);
                    for (int i = 0; i < ARR_SIZE; i++)
                        codigo[i] = tmp_arr[i];
                }
                /* caso retorne a segunda variavel local */
                else if (idx0 == 2)
                {
                    return_var(tmp_arr, idx0, line, &curr_length);
                    for (int i = 0; i < ARR_SIZE; i++)
                        codigo[i] = tmp_arr[i];
                }
                /* caso retorne a terceira variavel local */
                else if (idx0 == 3)
                {
                    return_var(tmp_arr, idx0, line, &curr_length);
                    for (int i = 0; i < ARR_SIZE; i++)
                        codigo[i] = tmp_arr[i];
                }
                /* caso retorne a quarta variavel local */
                else
                {
                    return_var(tmp_arr, idx0, line, &curr_length);

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

            switch (op)
            {
            case ':':
                var_attribute_operation(tmp_arr, var0, idx0, var1, idx1, op, line, &curr_length);
                for (int i = 0; i < ARR_SIZE; i++)
                    codigo[i] = tmp_arr[i];
                break;
            case '+':
                // var_add_operation(tmp_arr, var0, idx0, var1, idx1, op, line, &curr_length);
                break;
            case '-':
                break;
            case '*':
                break;
            default:
                error("operação inválida", line);
                break;
            }
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

            switch (op)
            {
            case ':':
                break;
            case '+':
                break;
            case '-':
                break;
            case '*':
                break;
            default:
                error("operação inválida", line);
                break;
            }
            // printf("%d %c%d %c= %c%d\n", line, var0, idx0, op, var1, idx1); /* machine code de manipulacao de parametros em assembly */
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

void return_var(unsigned char arr[], int idx0, int line, int *curr_length)
{

    switch (idx0)
    {
    case 1 /* rdx */:
        /* codigos de maquina de mover para eax e leave/ret */
        arr[*curr_length] = 0x89;
        arr[*curr_length + 1] = 0xd0;
        arr[*curr_length + 2] = 0xc9;
        arr[*curr_length + 3] = 0xc3;
        *curr_length += 4;
        break;

    case 2 /* rdx e rcx */:
        /* codigos de maquina de mover para eax e leave/ret */
        arr[*curr_length] = 0x89;
        arr[*curr_length + 1] = 0xc8;
        arr[*curr_length + 2] = 0xc9;
        arr[*curr_length + 3] = 0xc3;
        *curr_length += 4;
        break;

    case 3 /* rdx, rcx e r8 */:
        /* codigos de maquina de mover para eax e leave/ret */
        arr[*curr_length] = 0x44;
        arr[*curr_length + 1] = 0x89;
        arr[*curr_length + 2] = 0xc0;
        arr[*curr_length + 3] = 0xc9;
        arr[*curr_length + 4] = 0xc3;
        *curr_length += 4;
        break;

    case 4 /* rdx, rcx, r8 e r9 */:
        /* codigos de maquina de mover para eax e leave/ret */
        arr[*curr_length] = 0x44;
        arr[*curr_length + 1] = 0x89;
        arr[*curr_length + 2] = 0xc8;
        arr[*curr_length + 3] = 0xc9;
        arr[*curr_length + 4] = 0xc3;
        *curr_length += 4;
        break;

    default:
        error("número de variável local inválido", line);
    }
}

void return_parameter(unsigned char arr[], int idx0, int line, int *curr_length)
{
    /* caso seja primeiro parametro (familia rdi) */
    if (idx0 == 1)
    {
        /* (mov edi, eax) na posicao len-3 e len-2; leave na posicao len-1; ret na posicao len */
        arr[*curr_length] = 0x89;     /* primeiro byte do mov  edi, eax */
        arr[*curr_length + 1] = 0xf8; /* segundo byte do mov edi, eax */
        arr[*curr_length + 2] = 0xc9; /* leave */
        arr[*curr_length + 3] = 0xc3; /* ret */
        *curr_length += 4;
    }

    /* caso seja segundo parametro (familia rsi) */
    else if (idx0 == 2)
    {
        /* (mov esi, eax) na posicao len-3 e len-2; leave na posicao len-1; ret na posicao len */
        arr[*curr_length] = 0x89;     /* primeiro byte do mov esi, eax */
        arr[*curr_length + 1] = 0xf0; /* segundo byte do mov esi, eax */
        arr[*curr_length + 2] = 0xc9; /* leave */
        arr[*curr_length + 3] = 0xc3; /* ret */
        *curr_length += 4;
    }
    else
        error("número de parâmetro inválido", line);
}

void var_attribute_operation(unsigned char arr[], char var0, int idx0, char var1, int idx1, char op, int line, int *curr_length)
{
    // verificando segunda variavel/constante/parametro
    if (var1 == '$') /* constante */
    {
        char tmp_arr[20];
        sprintf(tmp_arr, "%x", idx1);
        int tmp_int = string2num(tmp_arr, 16);

        switch (idx0) /* verificando o valor da primeira variavel */
        {
        case 1:
            arr[*curr_length] = 0xba;
            break;
        case 2:
            arr[*curr_length] = 0xb9;
            break;
        case 3:
            arr[*curr_length] = 0x41;
            arr[*curr_length + 1] = 0xb8;
            break;
        case 4:
            arr[*curr_length] = 0x41;
            arr[*curr_length + 1] = 0xb9;
            break;
        default:
            error("número de variável inválido", line);
            break;
        }

        if (idx0 == 1 || idx0 == 2)
        {
            arr[*curr_length + 1] = (tmp_int & 0x000000ff);
            arr[*curr_length + 2] = (tmp_int & 0x0000ff00) >> 8;
            arr[*curr_length + 3] = (tmp_int & 0x00ff0000) >> 16;
            arr[*curr_length + 4] = (tmp_int & 0xff000000) >> 24;
            *curr_length += 5;
        }
        else if (idx0 == 3 || idx0 == 4)
        {
            arr[*curr_length + 2] = (tmp_int & 0x000000ff);
            arr[*curr_length + 3] = (tmp_int & 0x0000ff00) >> 8;
            arr[*curr_length + 4] = (tmp_int & 0x00ff0000) >> 16;
            arr[*curr_length + 5] = (tmp_int & 0xff000000) >> 24;
            *curr_length += 6;
        }
    }

    else if (var1 == 'v') /* variavel */
    {
        switch (idx1) /* verificando id da segunda variavel */
        {
        case 1:
            /* verificar id da primeira variavel e colocar aqui o array correspondente a ela somada com primeira variavel */
            switch (idx0) /* verificando o id da primeira variavel*/
            {
            case 1:
                unsigned char var1_manip_attr_var1[] = {};
                break;
            case 2:
                unsigned char var2_manip_attr_var1[] = {};
                break;
            case 3:
                unsigned char var3_manip_attr_var1[] = {};
                break;
            case 4:
                unsigned char var4_manip_attr_var1[] = {};
                break;
            default:
                error("número de variável inválido", line);
                break;
            }
            break;
        case 2:
            /* verificar id da primeira variavel e colocar aqui o array correspondente a ela somada com segunda variavel */
            switch (idx0) /* verificando o id da primeira variavel*/
            {
            case 1:
                unsigned char var1_manip_attr_var2[] = {};
                break;
            case 2:
                unsigned char var2_manip_attr_var2[] = {};
                break;
            case 3:
                unsigned char var3_manip_attr_var2[] = {};
                break;
            case 4:
                unsigned char var4_manip_attr_var2[] = {};
                break;
            default:
                error("número de variável inválido", line);
                break;
            }

            break;
        case 3:
            /* verificar id da primeira variavel e colocar aqui o array correspondente a ela somada com terceira variavel */
            switch (idx0) /* verificando o id da primeira variavel*/
            {
            case 1:
                unsigned char var1_manip_attr_var3[] = {};
                break;
            case 2:
                unsigned char var2_manip_attr_var3[] = {};
                break;
            case 3:
                unsigned char var3_manip_attr_var3[] = {};
                break;
            case 4:
                unsigned char var4_manip_attr_var3[] = {};
                break;
            default:
                error("número de variável inválido", line);
                break;
            }

            break;
        case 4:
            /* verificar id da primeira variavel e colocar aqui o array correspondente a ela somada com quarta variavel */
            switch (idx0) /* verificando o id da primeira variavel*/
            {
            case 1:
                unsigned char var1_manip_attr_var4[] = {};
                break;
            case 2:
                unsigned char var2_manip_attr_var4[50];
                break;
            case 3:
                unsigned char var3_manip_attr_var4[50];
                break;
            case 4:
                unsigned char var4_manip_attr_var4[50];
                break;
            default:
                error("número de variável inválido", line);
                break;
            }
            break;

        default:
            error("número de variável inválido", line);
            break;
        }
    }

    else if (var1 == 'p') /* parametro */
    {
        if (idx1 == 1) /* verificando id do parametro */
        {
            switch (idx0) /* verificando o id da primeira variavel*/
            {
            case 1:
                arr[*curr_length] = 0x89;
                arr[*curr_length + 1] = 0xfa;
                *curr_length += 2;
                break;
            case 2:
                unsigned char var2_manip_attr_param1[] = {};
                break;
            case 3:
                unsigned char var3_manip_attr_param1[] = {};
                break;
            case 4:
                unsigned char var4_manip_attr_param1[] = {};
                break;
            default:
                error("número de variável inválido", line);
                break;
            }
        }

        else if (idx1 == 2)
        {
            switch (idx0) /* verificando o id da primeira variavel*/
            {
            case 1:
                unsigned char var1_manip_attr_param2[] = {};
                break;
            case 2:
                unsigned char var2_manip_attr_param2[50];
                break;
            case 3:
                unsigned char var3_manip_attr_param2[50];
                break;
            case 4:
                unsigned char var4_manip_attr_param2[50];
                break;
            default:
                error("número de variável inválido", line);
                break;
            }
        }

        else
            error("número de parâmetro inválido", line);
    }

    else
        error("variável inválida", line);
}

void var_add_operation(unsigned char arr[], char var0, int idx0, char var1, int idx1, char op, int line, int *curr_length)
{
    /* verificando o que esta sendo somado à variável */
    if (var1 == '$') /* constante */
    {
        /* verificar id da primeira variavel e colocar aqui o array correspondente a ela somada com a constante */
        switch (idx0) /* verificando o valor da constante */
        {
        case 1:
            unsigned char var1_manip_add_const[50];
            break;
        case 2:
            unsigned char var2_manip_add_const[50];
            break;
        case 3:
            unsigned char var3_manip_add_const[50];
            break;
        case 4:
            unsigned char var4_manip_add_const[50];
            break;
        default:
            error("número de variável inválido", line);
            break;
        }
    }

    else if (var1 == 'v') /* variavel */
    {
        switch (idx1) /* verificando id da segunda variavel */
        {
        case 1:
            /* verificar id da primeira variavel e colocar aqui o array correspondente a ela somada com primeira variavel */
            switch (idx0) /* verificando o id da primeira variavel*/
            {
            case 1:
                unsigned char var1_manip_add_var1[] = {0x01, 0xd2};
                break;
            case 2:
                unsigned char var2_manip_add_var1[] = {};
                break;
            case 3:
                unsigned char var3_manip_add_var1[] = {};
                break;
            case 4:
                unsigned char var4_manip_add_var1[] = {};
                break;
            default:
                error("número de variável inválido", line);
                break;
            }
            break;
        case 2:
            /* verificar id da primeira variavel e colocar aqui o array correspondente a ela somada com segunda variavel */
            switch (idx0) /* verificando o id da primeira variavel*/
            {
            case 1:
                unsigned char var1_manip_add_var2[] = {0x01, 0xca};
                break;
            case 2:
                unsigned char var2_manip_add_var2[50];
                break;
            case 3:
                unsigned char var3_manip_add_var2[50];
                break;
            case 4:
                unsigned char var4_manip_add_var2[50];
                break;
            default:
                error("número de variável inválido", line);
                break;
            }

            break;
        case 3:
            /* verificar id da primeira variavel e colocar aqui o array correspondente a ela somada com terceira variavel */
            switch (idx0) /* verificando o id da primeira variavel*/
            {
            case 1:
                unsigned char var1_manip_add_var3[] = {0x44, 0x01, 0xc2};
                break;
            case 2:
                unsigned char var2_manip_add_var3[50];
                break;
            case 3:
                unsigned char var3_manip_add_var3[50];
                break;
            case 4:
                unsigned char var4_manip_add_var3[50];
                break;
            default:
                error("número de variável inválido", line);
                break;
            }

            break;
        case 4:
            /* verificar id da primeira variavel e colocar aqui o array correspondente a ela somada com quarta variavel */
            switch (idx0) /* verificando o id da primeira variavel*/
            {
            case 1:
                unsigned char var1_manip_add_var4[] = {0x44, 0x01, 0xca};
                break;
            case 2:
                unsigned char var2_manip_add_var4[50];
                break;
            case 3:
                unsigned char var3_manip_add_var4[50];
                break;
            case 4:
                unsigned char var4_manip_add_var4[50];
                break;
            default:
                error("número de variável inválido", line);
                break;
            }
            break;

        default:
            error("número de variável inválido", line);
            break;
        }
    }

    else if (var1 == 'p') /* parametro */
    {
        if (idx1 == 1) /* verificando id do parametro */
        {
            switch (idx0) /* verificando o id da primeira variavel*/
            {
            case 1:
                unsigned char var1_manip_add_param1[] = {0x01, 0xfa};
                break;
            case 2:
                unsigned char var2_manip_add_param1[] = {};
                break;
            case 3:
                unsigned char var3_manip_add_param1[] = {};
                break;
            case 4:
                unsigned char var4_manip_add_param1[] = {};
                break;
            default:
                error("número de variável inválido", line);
                break;
            }
        }

        else if (idx1 == 2)
        {
            switch (idx0) /* verificando o id da primeira variavel*/
            {
            case 1:
                unsigned char var1_manip_add_param2[] = {0x01, 0xf2};
                break;
            case 2:
                unsigned char var2_manip_add_param2[50];
                break;
            case 3:
                unsigned char var3_manip_add_param2[50];
                break;
            case 4:
                unsigned char var4_manip_add_param2[50];
                break;
            default:
                error("número de variável inválido", line);
                break;
            }
        }

        else
            error("número de parâmetro inválido", line);
    }

    else
        error("variável inválida", line);
}

void par_add_operation(unsigned char arr[], int *arr_size, char var0, int idx0, char var1, int idx1, char op, int line)
{
    switch (idx0)
    {
    case 1:
    {
        if (var1 == 'p')
        {
            if (idx1 == 1) // p1 += p1
            {
                arr[*arr_size] = 0x01;
                arr[(*arr_size) + 1] = 0xff;
                *arr_size += 2;
            }
            else if (idx1 == 2) // p1 += p2
            {
                arr[*arr_size] = 0x01;
                arr[(*arr_size) + 1] = 0xf7;
                (*arr_size)++;
            }
            else
                error("numero de parametro invalido\n", line);
        }

        else if (var1 == 'v')
        {
            switch (idx1)
            {
            case 1: // p1 += v1
            {
                arr[*arr_size] = 0x01;
                arr[(*arr_size) + 1] = 0xd7;
                break;
            }
            case 2: // p1 += v2
            {
                arr[*arr_size] = 0x01;
                arr[(*arr_size) + 1] = 0xcf;
                break;
            }
            case 3: // p1 += v3
            {
                arr[*arr_size] = 0x44;
                arr[(*arr_size) + 1] = 0x01;
                arr[(*arr_size) + 2] = 0xc7;
                break;
            }
            case 4: // p1 += v4
            {
                arr[*arr_size] = 0x44;
                arr[(*arr_size) + 1] = 0x01;
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
            if (idx1 == 1) // p2 += p1
            {
                arr[*arr_size] = 0x01;
                arr[(*arr_size) + 1] = 0xfe;
            }
            else if (idx1 == 2) // p2 += p2
            {
                arr[*arr_size] = 0x01;
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
                arr[*arr_size] = 0x01;
                arr[(*arr_size) + 1] = 0xd6;
                break;
            }
            case 2: // p2 += v2
            {
                arr[*arr_size] = 0x01;
                arr[(*arr_size) + 1] = 0xce;

                break;
            }
            case 3: // p2 += v3
            {
                arr[*arr_size] = 0x44;
                arr[(*arr_size) + 1] = 0x01;
                arr[(*arr_size) + 2] = 0xc6;
                break;
            }
            case 4: // p2 += v4
            {
                arr[*arr_size] = 0x44;
                arr[(*arr_size) + 1] = 0x01;
                arr[(*arr_size) + 2] = 0xc7;
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
            error("numero de parametro invalido\n", line);
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

int string2num(char *s, int base)
{
    int a = 0;
    for (; *s; s++)
    {
        if isdigit (*s)
            a = a * base + (*s - '0');
        else if isupper (*s)
            a = a * base + (*s - 'A' + 10);
        else
            a = a * base + (*s - 'a' + 10);
    }
    return a;
}
