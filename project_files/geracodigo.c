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
void return_var(unsigned char arr[], int var_id, int *curr_length);
void return_parameter(unsigned char arr[], int idx0, int *curr_length);

// var manipulation
void var_attribute_operation(unsigned char arr[], char var0, int idx0, char var1, int idx1, char op, int *curr_length);
void var_add_operation(unsigned char arr[], char var0, int idx0, char var1, int idx1, char op, int *array_length);
void var_sub_operation(unsigned char arr[], char var0, int idx0, char var1, int idx1, char op, int *array_length);
void var_mult_operation(unsigned char arr[], char var0, int idx0, char var1, int idx1, char op, int *array_length);

// parameter manipulation
void par_attribute_operation(unsigned char arr[], char var0, int idx0, char var1, int idx1, char op, int *array_length);
void par_add_operation(unsigned char arr[], int *arr_size, char var0, int idx0, char var1, int idx1, char op);

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
                return_parameter(tmp_arr, idx0, &curr_length);
                for (int i = 0; i < curr_length; i++)
                    codigo[i] = tmp_arr[i];
            }
            /* caso retornar variavel */
            else
            {
                /* caso retorne variavel local */
                return_var(tmp_arr, idx0, &curr_length);
                for (int i = 0; i < ARR_SIZE; i++)
                    codigo[i] = tmp_arr[i];
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
                var_attribute_operation(tmp_arr, var0, idx0, var1, idx1, op, &curr_length);
                for (int i = 0; i < ARR_SIZE; i++)
                    codigo[i] = tmp_arr[i];
                break;
            case '+':
                var_add_operation(tmp_arr, var0, idx0, var1, idx1, op, &curr_length);
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
                // par_attribute_operation(tmp_arr, var0, idx0, var1, idx1, op, &curr_length);
                break;
            case '+':
                par_add_operation(tmp_arr, &curr_length, var0, idx0, var1, idx1, op);
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

// return functions
void return_var(unsigned char arr[], int idx0, int *curr_length)
{
    if (idx0 == 1 || idx0 == 2)
    {
        arr[*curr_length] = 0x89;
        if (idx0 == 1)
            arr[*curr_length + 1] = 0xd0;
        else
            arr[*curr_length + 1] = 0xc8;
        arr[*curr_length + 2] = 0xc9;
        arr[*curr_length + 3] = 0xc3;
        *curr_length += 4;
    }
    else
    {
        arr[*curr_length] = 0x44;
        arr[*curr_length + 1] = 0x89;
        if (idx0 == 3)
            arr[*curr_length + 2] = 0xc0;
        else
            arr[*curr_length + 2] = 0xc8;
        arr[*curr_length + 3] = 0xc9;
        arr[*curr_length + 4] = 0xc3;
        *curr_length += 5;
    }
}

void return_parameter(unsigned char arr[], int idx0, int *curr_length)
{
    arr[*curr_length] = 0x89;
    /* caso seja primeiro parametro (familia rdi) */
    if (idx0 == 1)
        arr[*curr_length + 1] = 0xf8; /* segundo byte do mov edi, eax */

    /* caso seja segundo parametro (familia rsi) */
    else
        arr[*curr_length + 1] = 0xf0; /* segundo byte do mov esi, eax */
    arr[*curr_length + 2] = 0xc9;     /* leave */
    arr[*curr_length + 3] = 0xc3;     /* ret */
    *curr_length += 4;
}

// variable manipulation functions
void var_attribute_operation(unsigned char arr[], char var0, int idx0, char var1, int idx1, char op, int *curr_length)
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
            if (idx0 == 3 || idx0 == 4)
            {
                arr[*curr_length] = 0x41;
                arr[*curr_length + 1] = 0x89;
                if (idx0 == 3)
                    arr[*curr_length + 2] = 0xd0;
                else
                    arr[*curr_length + 2] = 0xd1;
                *curr_length += 3;
            }
            else
            {
                arr[*curr_length] = 0x89;
                arr[*curr_length + 1] = 0xd1;
                *curr_length += 2;
            }
            break;
        case 2:
            if (idx0 == 3 || idx0 == 4)
            {
                arr[*curr_length] = 0x41;
                arr[*curr_length + 1] = 0x89;
                if (idx0 == 3)
                    arr[*curr_length + 2] = 0xc8;
                else
                    arr[*curr_length + 2] = 0xc9;
                *curr_length += 3;
            }
            else
            {
                arr[*curr_length] = 0x89;
                arr[*curr_length + 1] = 0xca;
                *curr_length += 2;
            }
            break;
        case 3:
            if (idx0 == 1 || idx0 == 2)
            {
                arr[*curr_length] = 0x44;
                arr[*curr_length + 1] = 0x89;
                if (idx0 == 1)
                    arr[*curr_length + 2] = 0xc2;
                else
                    arr[*curr_length + 2] = 0xc1;
                *curr_length += 3;
            }
            else
            {
                arr[*curr_length] = 0x45;
                arr[*curr_length + 1] = 0x89;
                arr[*curr_length + 2] = 0xc1;
                *curr_length += 3;
            }
            break;
        case 4:
            if (idx0 == 1 || idx0 == 2)
            {
                arr[*curr_length] = 0x44;
                arr[*curr_length + 1] = 0x89;
                if (idx0 == 1)
                    arr[*curr_length + 2] = 0xca;
                else
                    arr[*curr_length + 2] = 0xc9;
                *curr_length += 3;
            }
            else
            {
                arr[*curr_length] = 0x45;
                arr[*curr_length + 1] = 0x89;
                arr[*curr_length + 2] = 0xc8;
                *curr_length += 3;
            }
            break;
        default:
            break;
        }
    }

    else if (var1 == 'p') /* parametro */
    {
        if (idx1 == 1) /* verificando id do parametro */
        {
            if (idx0 == 1 || idx0 == 2)
            {
                arr[*curr_length] = 0x89;
                if (idx0 == 1)
                    arr[*curr_length + 1] = 0xfa;
                else
                    arr[*curr_length + 1] = 0xf9;
                *curr_length += 2;
            }

            else
            {
                arr[*curr_length] = 0x41;
                arr[*curr_length + 1] = 0x89;
                if (idx0 == 3)
                    arr[*curr_length + 2] = 0xf8;
                else
                    arr[*curr_length + 2] = 0xf9;
                *curr_length += 3;
            }
        }

        else if (idx1 == 2)
        {
            if (idx0 == 1 || idx0 == 2)
            {
                arr[*curr_length] = 0x89;
                if (idx0 == 1)
                    arr[*curr_length + 1] = 0xf2;
                else
                    arr[*curr_length + 1] = 0xf1;
                *curr_length += 2;
            }
            else
            {
                arr[*curr_length] = 0x41;
                arr[*curr_length + 1] = 0x89;
                if (idx0 == 3)
                    arr[*curr_length + 2] = 0xf0;
                else
                    arr[*curr_length + 2] = 0xf1;
                *curr_length += 3;
            }
        }
    }
}

void var_add_operation(unsigned char arr[], char var0, int idx0, char var1, int idx1, char op, int *curr_length)
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
                break;
            }
            break;

        default:
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
                break;
            }
        }
    }
}

// parameter manipulation functions
void par_add_operation(unsigned char arr[], int *arr_size, char var0, int idx0, char var1, int idx1, char op)
{
    switch (idx0)
    {
    case 1:
    {
        if (var1 == 'p')
        {
            arr[*arr_size] = 0x01;
            if (idx1 == 1) // p1 += p1
                arr[*arr_size + 1] = 0xff;
            else // p1 += p2
                arr[*arr_size + 1] = 0xf7;
            *arr_size += 2;
        }

        else if (var1 == 'v')
        {
            if (idx1 == 1 || idx1 == 2)
            {
                arr[*arr_size] = 0x01;
                if (idx1 == 1)
                    arr[*arr_size + 1] = 0xd7;
                else
                    arr[*arr_size + 1] = 0xcf;
                *arr_size += 2;
            }
            else
            {
                arr[*arr_size] = 0x44;
                arr[*arr_size + 1] = 0x01;
                if (idx1 == 3)
                    arr[*arr_size + 2] = 0xc7;
                else
                    arr[*arr_size + 2] = 0xcf;
                *arr_size += 3;
            }
        }
        else if (var1 == '$')
        {
        }
        break;
    }

    case 2:
    {
        if (var1 == 'p')
        {
            arr[*arr_size] = 0x01;
            if (idx1 == 1) // p2 += p1
                arr[*arr_size + 1] = 0xfe;
            else // p2 += p2
                arr[*arr_size + 1] = 0xf6;
            *arr_size += 2;
        }
        else if (var1 == 'v')
        {
            if (idx1 == 1 || idx1 == 2)
            {
                arr[*arr_size] = 0x01;
                if (idx1 == 1)
                    arr[*arr_size + 1] = 0xd6;
                else
                    arr[*arr_size + 1] = 0xce;
                *arr_size += 2;
            }
            else
            {
                arr[*arr_size] = 0x44;
                arr[*arr_size + 1] = 0x01;
                if (idx1 == 3)
                    arr[*arr_size + 2] = 0xc6;
                else
                    arr[*arr_size + 2] = 0xc7;
                *arr_size += 3;
            }
        }
        else if (var1 == '$')
        {
        }
        break;
    }
    default:
        break;
    }
    return;
}

// aux functions
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
