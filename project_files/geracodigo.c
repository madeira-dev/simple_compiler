// exemplo leitura e interpretacao do codigo SB
/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void error(const char *msg, int line)
{
    fprintf(stderr, "erro %s na linha %d\n", msg, line);
    exit(EXIT_FAILURE);
}

int main(void)
{
    int line = 1;
    int c;
    FILE *myfp;

    if ((myfp = fopen("programa", "r")) == NULL)
    {
        perror("nao conseguiu abrir arquivo!");
        exit(1);
    }

    while ((c = fgetc(myfp)) != EOF)
    {
        switch (c)
        {
        case 'r':
        { /* retorno
            char var0;
            int idx0;
            if (fscanf(myfp, "et %c%d", &var0, &idx0) != 2)
                error("comando invalido", line);
            printf("%d ret %c%d\n", line, var0, idx0);
            break;
        }
        case 'v':
        case 'p':
        { /* atribuiÃ§Ã£o e op. aritmetica
            char var0 = c, var1, op;
            int idx0, idx1;

            if (fscanf(myfp, "%d %c= %c%d", &idx0, &op, &var1, &idx1) != 4)
                error("comando invalido", line);
            printf("%d %c%d %c= %c%d\n", line, var0, idx0, op, var1, idx1);
            break;
        }
        case 'i':
        { /* desvio condicional
            char var0;
            int idx0, n1, n2;
            if (fscanf(myfp, "f %c%d %d %d", &var0, &idx0, &n1, &n2) != 4)
                error("comando invalido", line);
            printf("%d if %c%d %d %d\n", line, var0, idx0, n1, n2);
            break;
        }
        case 'g':
        { /* desvio incondicional
            int n1;
            if (fscanf(myfp, "o %d", &n1) != 1)
                error("comando invalido", line);
            printf("%d go %d\n", line, n1);
            break;
        }
        default:
            error("comando desconhecido", line);
        }
        line++;
        fscanf(myfp, " ");
    }
    return 0;
}
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*funcp)();

funcp geraCodigo(FILE *f, unsigned char codigo[]);

int main(void)
{
    int tmp;
    unsigned char opcode_array[tmp];

    FILE *input_file = fopen("test_sb1", "rt");

    geraCodigo(input_file, opcode_array);
    return 0;
}

funcp geraCodigo(FILE *f, unsigned char codigo[])
{
}
