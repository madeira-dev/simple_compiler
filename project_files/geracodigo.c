#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "geracodigo.h"

#define ARR_SIZE 1024  /* tamanho máximo do vetor */
#define INIT_LENGTH 24 /* tamanho inicial do array sempre iniciado com codigo de maquina de: iniciar RA, abrir espaço no RA e alocar variáveis locais */
#define LIN 20

/* typedef */
typedef int (*funcp)();
// struct pra guardar posicoes do if e do go
typedef struct End_if_go
{
    unsigned char cod_maq_if_go;  // guardo o codigo de maquina de onde comeca o if / go para depois fazer a conta
    unsigned char pos_if_go;      // indice do tmp array onde if / go comeca
    unsigned char jmp_less_line;  // se for um if guardo a linha do jmp less / se for um go guardo a linha do jmp
    unsigned char jmp_equal_line; // se for um if guardo a linha do jmp equal / se for um go aqui fica negativo
} End_if_go;

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
void par_sub_operation(unsigned char arr[], int *arr_size, char var0, int idx0, char var1, int idx1, char op);
void par_mult_operation(unsigned char arr[], int *arr_size, char var0, int idx0, char var1, int idx1, char op);

// aux
static void error(const char *msg, int line);
int string2num(char *s, int base);
void cmp(unsigned char arr[], int *arr_size, char var0, int idx0);
void go(unsigned char arr[], int *arr_size);
void preenche_vazios(End_if_go vetor_ends[], int tam_vetor_ends, unsigned char end_arr[], int lines, unsigned char arr[]);

/* code */
funcp geraCodigo(FILE *f, unsigned char codigo[])
{
    funcp func;
    int line = 1;
    int lineAux = line - 1;
    unsigned char end_arr[LIN];
    int c, curr_length = 24, aux_curr_length = 0, count_if_n_go = 0; /* curr_length: variavel para contar espaços já preenchidos com código de máquina do array */
    unsigned char tmp_arr[ARR_SIZE] = {0x55,                         /* array inicializado com codigos de maquina de: iniciar ra, abrir espaco no ra e alocar variaveis locais */
                                       0x48, 0x89, 0xe5,
                                       0x48, 0x83, 0xec, 0x20,
                                       0x48, 0x8d, 0x55, 0xe0,
                                       0x48, 0x8d, 0x4d, 0xe8,
                                       0x4c, 0x8d, 0x45, 0xf0,
                                       0x4c, 0x8d, 0x4d, 0xf8};
    End_if_go vetor_ends[LIN];
    while ((c = fgetc(f)) != EOF)
    {
        switch (c)
        {
        case 'r': /* retorno */
        {
            char var0; /* representa qual variavel: local ou parametro */
            int idx0;  /* numero da variavel retornada 1 ou 2 */
            aux_curr_length = curr_length;
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
            end_arr[lineAux] = tmp_arr[aux_curr_length + 1];
            lineAux += 1;
            break;
        }
        case 'v': /* atribuicao e op. aritmetica de variaveis locais */
        {
            char var0 = c;  /* primeira variável, à esquerda do = */
            char var1;      /* segundo(a) variável/parametro, à direita do = */
            char op;        /* sinal de operação sendo feita: soma subtração... */
            int idx0, idx1; /* id de cada variável/parametro, 1 ou 2 */
            aux_curr_length = curr_length;

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
            end_arr[lineAux] = tmp_arr[aux_curr_length + 1];
            lineAux += 1;
            break;
        }
        case 'p': /* atribuicao e op. aritmetica de parametros */
        {
            char var0 = c;  /* primeiro parametro, à esquerda do = */
            char var1;      /* segundo(a) variável/parametro, à direita do = */
            char op;        /* sinal de operação sendo feita: soma, subtração... */
            int idx0, idx1; /* id de cada variável/parametro, 1 ou 2 */
            aux_curr_length = curr_length;

            if (fscanf(f, "%d %c= %c%d", &idx0, &op, &var1, &idx1) != 4)
                error("comando invalido", line);

            switch (op)
            {
            case ':':
                par_attribute_operation(tmp_arr, var0, idx0, var1, idx1, op, &curr_length);
                break;
            case '+':
                par_add_operation(tmp_arr, &curr_length, var0, idx0, var1, idx1, op);
                break;
            case '-':
                par_sub_operation(tmp_arr, &curr_length, var0, idx0, var1, idx1, op);
                break;
            case '*':
                par_mult_operation(tmp_arr, &curr_length, var0, idx0, var1, idx1, op);
                break;
            default:
                error("operação inválida", line);
                break;
            }
            end_arr[lineAux] = tmp_arr[aux_curr_length + 1];
            lineAux += 1;
            break;
        }
        case 'i': /* desvio condicional (if) */
        {
            char var0; /* variavel/parametro que esta sendo comparado */
            int idx0;  /* id do(a) parametro/variavel comparada */
            int n1;    /* numero da linha to go if true */
            int n2;    /* numero da linha to go if false */
            aux_curr_length = curr_length;

            if (fscanf(f, "f %c%d %d %d", &var0, &idx0, &n1, &n2) != 4)
                error("comando invalido", line);

            cmp(tmp_arr, &curr_length, var0, idx0);
            printf("%d if %c%d %d %d\n", line, var0, idx0, n1, n2); /* machine code de cmp em assembly */
            end_arr[lineAux] = tmp_arr[aux_curr_length + 1];
            vetor_ends[count_if_n_go].cod_maq_if_go = tmp_arr[aux_curr_length + 1]; // codigo de maquina de onde comeca a linha do if
            vetor_ends[count_if_n_go].pos_if_go = aux_curr_length + 1;              // indice do if no vetor tmp arr
            vetor_ends[count_if_n_go].jmp_less_line = n1;                           // guardo a linha que tenho que ir se for less
            vetor_ends[count_if_n_go].jmp_equal_line = n2;                          // guardo a linha que eu tenho que ir se for equal

            lineAux += 1;
            count_if_n_go += 1;
            break;
        }

        case 'g': /* desvio incondicional (go) */
        {
            /* colocar contador para ter linha e indice do vetor principal */
            /* quando chegar no go "chamar de novo tal linha " e reescreve o codigo de maquina */

            int n1; /* numero da linha to go */
            int i;
            int length;
            aux_curr_length = curr_length;

            if (fscanf(f, "o %d", &n1) != 1)
                error("comando invalido", line);

            go(tmp_arr, &curr_length);
            printf("%d go %d\n", line, n1); /* machine code de jmp em assembly */

            end_arr[lineAux] = tmp_arr[aux_curr_length + 1];
            vetor_ends[count_if_n_go].cod_maq_if_go = tmp_arr[aux_curr_length + 1]; // codigo de maquina de onde comeca a linha do go
            vetor_ends[count_if_n_go].pos_if_go = aux_curr_length + 1;              // indice do if no vetor tmp arr
            vetor_ends[count_if_n_go].jmp_less_line = n1;                           // guardo a linha que tenho que pular
            vetor_ends[count_if_n_go].jmp_equal_line = -1;                          // -1 porque nao tem jump equal

            lineAux += 1;
            count_if_n_go += 1;
            break;
        }

        default:
            error("comando desconhecido", line);
        }

        line++;
        fscanf(f, " ");
    }
    preenche_vazios(vetor_ends, count_if_n_go, end_arr, lineAux + 1, tmp_arr);
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
        char aux_arr[20];
        sprintf(aux_arr, "%x", idx1);
        int tmp_int = string2num(aux_arr, 16);

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

// tem q completar aqui
void var_add_operation(unsigned char arr[], char var0, int idx0, char var1, int idx1, char op, int *curr_length)
{
    if (var1 == '$') /* constante */
    {
        switch (idx0)
        {
        case 1:
            arr[*curr_length] = 0x83;
            arr[*curr_length + 1] = 0xc2;
            break;
        case 2:
            arr[*curr_length] = 0x83;
            arr[*curr_length + 1] = 0xc1;
            break;
        case 3:
            arr[*curr_length] = 0x41;
            arr[*curr_length + 1] = 0x83;
            arr[*curr_length + 2] = 0xc0;
            break;
        case 4:
            arr[*curr_length] = 0x41;
            arr[*curr_length + 1] = 0x83;
            arr[*curr_length + 2] = 0xc1;
            break;
        default:
            break;
        }
    }
    else if (var1 == 'v') /* variavel */
    {
        switch (idx1) /* verificando id da segunda variavel */
        {
        case 1: /* segunda variavel == primeira variavel local */
            if (idx0 == 1 || idx0 == 2)
            {
                arr[*curr_length] = 0x01;
                if (idx0 == 1) /* primeira variavel == primeira variavel local */
                    arr[*curr_length + 1] = 0xd2;
                else /* primeira variavel == segunda variavel local */
                    arr[*curr_length + 1] = 0xd1;
                *curr_length += 2;
            }
            else
            {
                arr[*curr_length] = 0x41;
                arr[*curr_length + 1] = 0x01;
                if (idx0 == 3) /* primeira variavel == terceira variavel local */
                    arr[*curr_length + 2] = 0xd0;
                else /* primeira variavel == quarta variavel local */
                    arr[*curr_length + 2] = 0xd1;
                *curr_length += 3;
            }
            break;
        case 2: /* segunda variavel == segunda variavel local */
            if (idx0 == 1 || idx0 == 2)
            {
                arr[*curr_length] = 0x01;
                if (idx0 == 1) /* primeira variavel == primeira variavel local */
                    arr[*curr_length + 1] = 0xca;
                else /* primeira variavel == segunda variavel local */
                    arr[*curr_length + 1] = 0xc9;
                *curr_length += 2;
            }
            else
            {
                arr[*curr_length] = 0x41;
                arr[*curr_length + 1] = 0x01;
                if (idx0 == 3) /* primeira variavel == terceira variavel local */
                    arr[*curr_length + 2] = 0xc8;
                else /* primeira variavel == quarta variavel local */
                    arr[*curr_length + 2] = 0xc9;
                *curr_length += 3;
            }
            break;
        case 3: /* segunda variavel == terceira variavel local */
            if (idx0 == 1 || idx0 == 2)
            {
                arr[*curr_length] = 0x44;
                arr[*curr_length + 1] = 0x01;
                if (idx0 == 1) /* primeira variavel == primeira variavel local */
                    arr[*curr_length + 2] = 0xc2;
                else /* primeira variavel == segunda variavel local */
                    arr[*curr_length + 2] = 0xc1;
                *curr_length += 3;
            }
            else
            {
                arr[*curr_length] = 0x45;
                arr[*curr_length + 1] = 0x01;
                if (idx0 == 3) /* primeira variavel == terceira variavel local */
                    arr[*curr_length + 2] = 0xc0;
                else /* primeira variavel == quarta variavel local */
                    arr[*curr_length + 2] = 0xc1;
                *curr_length += 3;
            }
            break;
        case 4: /* segunda variavel == quarta variavel local */
            if (idx0 == 1 || idx0 == 2)
            {
                arr[*curr_length] = 0x44;
                arr[*curr_length + 1] = 0x01;
                if (idx0 == 1) /* primeira variavel == primeira variavel local */
                    arr[*curr_length + 2] = 0xca;
                else /* primeira variavel == segunda variavel local */
                    arr[*curr_length + 2] = 0xc9;
                *curr_length += 3;
            }
            else
            {
                arr[*curr_length] = 0x45;
                arr[*curr_length + 1] = 0x01;
                if (idx0 == 3) /* primeira variavel == terceira variavel local */
                    arr[*curr_length + 2] = 0xc8;
                else /* segunda variavel == quarta variavel local */
                    arr[*curr_length + 2] = 0xc9;
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
                arr[*curr_length] = 0x01;
                if (idx0 == 1) /* primeira variavel == primeira variavel local */
                    arr[*curr_length + 1] = 0xfa;
                else /* primeira variavel == segunda variavel local */
                    arr[*curr_length + 1] = 0xf9;
                *curr_length += 2;
            }
            else
            {
                arr[*curr_length] = 0x41;
                arr[*curr_length + 1] = 0x01;
                if (idx0 == 3) /* primeira variavel == terceira variavel local */
                    arr[*curr_length + 2] = 0xf8;
                else /* primeira variavel == quarta variavel local */
                    arr[*curr_length + 2] = 0xf9;
                *curr_length += 3;
            }
        }

        else if (idx1 == 2)
        {
            if (idx0 == 1 || idx0 == 2)
            {
                arr[*curr_length] = 0x01;
                if (idx0 == 1) /* primeira variavel == primeira variavel local */
                    arr[*curr_length + 1] = 0xf2;
                else /* primeira variavel == segunda variavel local */
                    arr[*curr_length + 1] = 0xf1;
                *curr_length += 2;
            }
            else
            {
                arr[*curr_length] = 0x41;
                arr[*curr_length + 1] = 0x01;
                if (idx0 == 3) /* primeira variavel == terceira variavel local */
                    arr[*curr_length + 2] = 0xf0;
                else /* primeira variavel == quarta variavel local */
                    arr[*curr_length + 2] = 0xf1;
                *curr_length += 3;
            }
        }
    }
}

void var_sub_operation(unsigned char arr[], char var0, int idx0, char var1, int idx1, char op, int *array_length)
{
    if (var1 == '$') /* constante */
    {
        switch (idx0) /* verificando o valor da constante */
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
            break;
        }
    }
    else if (var1 == 'v') /* variavel */
    {
        switch (idx1) /* verificando id da segunda variavel */
        {
        case 1:
            if (idx0 == 1 || idx0 == 2)
            {
                arr[*array_length] = 0x29;
                if (idx0 == 1)
                    arr[*array_length + 1] = 0xd2;
                else
                    arr[*array_length + 1] = 0xd1;
                *array_length += 2;
            }
            else
            {
                arr[*array_length] = 0x41;
                arr[*array_length + 1] = 0x29;
                if (idx0 == 3)
                    arr[*array_length + 2] = 0xd0;
                else
                    arr[*array_length + 2] = 0xd1;
                *array_length += 3;
            }
            break;
        case 2:
            if (idx0 == 1 || idx0 == 2)
            {
                arr[*array_length] = 0x29;
                if (idx0 == 1)
                    arr[*array_length + 1] = 0xca;
                else
                    arr[*array_length + 1] = 0xc9;
                *array_length += 2;
            }
            else
            {
                arr[*array_length] = 0x41;
                arr[*array_length + 1] = 0x29;
                if (idx0 == 3)
                    arr[*array_length + 2] = 0xc8;
                else
                    arr[*array_length + 2] = 0xc9;
                *array_length += 3;
            }
            break;
        case 3:
            if (idx0 == 1 || idx0 == 2)
            {
                arr[*array_length] = 0x44;
                arr[*array_length + 1] = 0x29;
                if (idx0 == 1)
                    arr[*array_length + 2] = 0xc2;
                else
                    arr[*array_length + 2] = 0xc1;
                *array_length += 3;
            }
            else
            {
                arr[*array_length] = 0x45;
                arr[*array_length + 1] = 0x29;
                if (idx0 == 3)
                    arr[*array_length + 2] = 0xc0;
                else
                    arr[*array_length + 2] = 0xc1;
                *array_length += 3;
            }
            break;
        case 4:
            if (idx0 == 1 || idx0 == 2)
            {
                arr[*array_length] = 0x44;
                arr[*array_length + 1] = 0x29;
                if (idx0 == 1)
                    arr[*array_length + 2] = 0xca;
                else
                    arr[*array_length + 2] = 0xc9;
                *array_length += 3;
            }
            else
            {
                arr[*array_length] = 0x45;
                arr[*array_length + 1] = 0x29;
                if (idx0 == 3)
                    arr[*array_length + 2] = 0xc8;
                else
                    arr[*array_length + 2] = 0xc9;
                *array_length += 3;
            }
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
                if (idx0 == 1)
                {
                }
                else
                {
                }
            }
            else
            {
                if (idx0 == 3)
                {
                }
                else
                {
                }
            }
        }

        else if (idx1 == 2)
        {
            if (idx0 == 1 || idx0 == 2)
            {
                if (idx0 == 1)
                {
                }
                else
                {
                }
            }
            else
            {
                if (idx0 == 3)
                {
                }
                else
                {
                }
            }
        }
    }
}

void var_mult_operation(unsigned char arr[], char var0, int idx0, char var1, int idx1, char op, int *array_length)
{
    if (var1 == '$') /* constante */
    {
        switch (idx0) /* verificando o valor da constante */
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
            break;
        }
    }

    else if (var1 == 'v') /* variavel */
    {
        switch (idx1) /* verificando id da segunda variavel */
        {
        case 1:
            if (idx0 == 1 || idx0 == 2)
            {
                arr[*array_length] = 0x29;
                if (idx0 == 1)
                    arr[*array_length + 1] = 0xd2;
                else
                    arr[*array_length + 1] = 0xd1;
                *array_length += 2;
            }
            else
            {
                arr[*array_length] = 0x41;
                arr[*array_length + 1] = 0x29;
                if (idx0 == 3)
                    arr[*array_length + 2] = 0xd0;
                else
                    arr[*array_length + 2] = 0xd1;
                *array_length += 3;
            }
            break;
        case 2:
            if (idx0 == 1 || idx0 == 2)
            {
                arr[*array_length] = 0x29;
                if (idx0 == 1)
                    arr[*array_length + 1] = 0xca;
                else
                    arr[*array_length + 1] = 0xc9;
                *array_length += 2;
            }
            else
            {
                arr[*array_length] = 0x41;
                arr[*array_length + 1] = 0x29;
                if (idx0 == 3)
                    arr[*array_length + 2] = 0xc8;
                else
                    arr[*array_length + 2] = 0xc9;
                *array_length += 3;
            }
            break;
        case 3:
            if (idx0 == 1 || idx0 == 2)
            {
                arr[*array_length] = 0x44;
                arr[*array_length + 1] = 0x29;
                if (idx0 == 1)
                    arr[*array_length + 2] = 0xc2;
                else
                    arr[*array_length + 2] = 0xc1;
                *array_length += 3;
            }
            else
            {
                arr[*array_length] = 0x45;
                arr[*array_length + 1] = 0x29;
                if (idx0 == 3)
                    arr[*array_length + 2] = 0xc0;
                else
                    arr[*array_length + 2] = 0xc1;
                *array_length += 3;
            }
            break;
        case 4:
            if (idx0 == 1 || idx0 == 2)
            {
                arr[*array_length] = 0x44;
                arr[*array_length + 1] = 0x29;
                if (idx0 == 1)
                    arr[*array_length + 2] = 0xca;
                else
                    arr[*array_length + 2] = 0xc9;
                *array_length += 3;
            }
            else
            {
                arr[*array_length] = 0x45;
                arr[*array_length + 1] = 0x29;
                if (idx0 == 3)
                    arr[*array_length + 2] = 0xc8;
                else
                    arr[*array_length + 2] = 0xc9;
                *array_length += 3;
            }
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
                if (idx0 == 1)
                {
                }
                else
                {
                }
            }
            else
            {
                if (idx0 == 3)
                {
                }
                else
                {
                }
            }
        }
        else if (idx1 == 2)
        {
            if (idx0 == 1 || idx0 == 2)
            {
                if (idx0 == 1)
                {
                }
                else
                {
                }
            }
            else
            {
                if (idx0 == 3)
                {
                }
                else
                {
                }
            }
        }
    }
}

// parameter manipulation functions
void par_attr_operation(unsigned char arr[], int *arr_size, char var0, int idx0, char var1, int idx1, char op)
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
                break;
            }
            }
        }
        else if (var1 == '$')
        {
            char aux_arr[20];
            sprintf(aux_arr, "%x", idx1);
            int tmp_int = string2num(aux_arr, 16);
        }
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
                break;
            }
            break;
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

void par_sub_operation(unsigned char arr[], int *arr_size, char var0, int idx0, char var1, int idx1, char op)
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
                *arr_size = (*arr_size) + 2;
            }
            else if (idx1 == 2) // p1 -= p2
            {
                arr[*arr_size] = 0x29;
                arr[(*arr_size) + 1] = 0xf7;
                *arr_size = (*arr_size) + 2;
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
                break;
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
                break;
            }
            break;
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

void par_mult_operation(unsigned char arr[], int *arr_size, char var0, int idx0, char var1, int idx1, char op)
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
                break;
            }
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
                break;
            }
            break;
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
        if (isdigit(*s))
            a = a * base + (*s - '0');
        else if (isupper(*s))
            a = a * base + (*s - 'A' + 10);
        else
            a = a * base + (*s - 'a' + 10);
    }
    return a;
}

void cmp(unsigned char arr[], int *arr_size, char var0, int idx0) /* codigo de maquina de if */
{
    switch (var0)
    {
    case 'p':
        arr[*arr_size] = 0x83;
        if (idx0 == 1)
            arr[*arr_size + 1] = 0xff;
        else
            arr[*arr_size + 1] = 0xfe;
        arr[*arr_size + 2] = 0x00;
        arr[*arr_size + 3] = 0x7c;
        arr[*arr_size + 5] = 0x74;
        *arr_size += 6;
        break;
    case 'v':
        if (idx0 == 1 | idx0 == 2)
        {
            arr[*arr_size] = 0x83;
            if (idx0 == 1)
                arr[*arr_size + 1] = 0xfa;
            else
                arr[*arr_size + 1] = 0xf9;
            arr[*arr_size + 2] = 0x00;
            arr[*arr_size + 3] = 0x7c;
            arr[*arr_size + 5] = 0x74;
            *arr_size += 6;
        }
        else
        {
            arr[*arr_size] = 0x41;
            arr[*arr_size + 1] = 0x83;
            if (idx0 == 3)
            {
                arr[*arr_size + 2] = 0xf8;
            }
            else
            {
                arr[*arr_size + 2] = 0xf9;
            }
            arr[*arr_size + 3] = 0x00;
            arr[*arr_size + 4] = 0x7c;
            arr[*arr_size + 6] = 0x74;
            *arr_size += 7;
        }
    }
}

void go(unsigned char arr[], int *arr_size)
{
    arr[*arr_size] = 0xeb;
    *arr_size += 2;
    return;
}

void preenche_vazios(End_if_go vetor_ends[], int tam_vetor_ends, unsigned char end_arr[], int lines, unsigned char arr[])
{
    int i, j;
    unsigned char conta, end_linha;
    for (i = 0; i < tam_vetor_ends; i++) // enquanto ainda houver um if ou go para tratar
    {
        j = vetor_ends[i].pos_if_go; // j pega indice do if / go no vetor tmp arr
        // ando no vetor ate encontrar um dos codigos de maquina do jmp jl ou je
        while (arr[j] != 0x7c /*&& arr[j] != 0x74*/ && arr[j] != 0xeb)
            j++;
        // vejo qual dos casos eh
        if (arr[j] == 0xeb) // encontrei o jmp incondicional
        {
            // endereco do jmp - endereco da linha que ele quer pular
            // em vetor_end.jmp_less_line esta guardado um inteiro com a linha que se deseja ir
            // o endereco da primeira instrucao de tal linha esta no end_arr
            // linha 1 esta na posicao 0 do end arr por isso o -1
            end_linha = end_arr[vetor_ends[i].jmp_less_line - 1];
            conta = end_linha - arr[j];
            j++;
            arr[j] = conta;
        }
        else /* (rr[j] == 0x7c) // encontrei jl  */
        {
            end_linha = end_arr[vetor_ends[i].jmp_less_line - 1];
            conta = end_linha - arr[j];
            j++;
            arr[j] = conta;
            // acaba jump less
            j++;
            end_linha = end_arr[vetor_ends[i].jmp_equal_line - 1];
            conta = end_linha - arr[j];
            j++;
            arr[j] = conta;
        }
        /*
        else // je
        {
            end_linha = end_arr[vetor_ends[i].jmp_equal_line - 1];
            conta = arr[j] - end_linha;
            j++;
            arr[j] = conta;
        }
        */
    }
    return;
}
