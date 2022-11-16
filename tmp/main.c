#include <stdio.h>
#include <stdlib.h>
#include "geracodigo.h"

int main(void)
{
    funcp func;
    int check;
    unsigned char opcode_array[1024];
    FILE *myfp;

    if ((myfp = fopen("../assembly_translations/test_add_param_param.txt", "rt")) == NULL)
    {
        perror("nao conseguiu abrir arquivo!");
        exit(1);
    }

    func = (funcp)geraCodigo(myfp, opcode_array);
    fclose(myfp);

    check = (*func)(2,2);
    printf("%d\n", check);

    return 0;
}