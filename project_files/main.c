#include <stdio.h>
#include <stdlib.h>
#include "geracodigo.h"

int main(void)
{
    funcp func;
    int check;
    unsigned char opcode_array[512];
    FILE *myfp;

    if ((myfp = fopen("sb_code1.txt", "rt")) == NULL)
    {
        perror("nao conseguiu abrir arquivo!");
        exit(1);
    }

    func = (funcp)geraCodigo(myfp, opcode_array);
    fclose(myfp);

    check = (*func)();
    printf("%d\n", check);

    return 0;
}
