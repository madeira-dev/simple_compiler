#include <stdio.h>
#include <stdlib.h>
#include "geracodigo.h"

int main(void)
{
    int tmp;
    unsigned char opcode_array[tmp];
    FILE *myfp;

    if ((myfp = fopen("sb_code5.txt", "rt")) == NULL)
    {
        perror("nao conseguiu abrir arquivo!");
        exit(1);
    }

    geraCodigo(myfp, opcode_array);
    fclose(myfp);

    return 0;
}
