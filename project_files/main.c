#include <stdio.h>
#include <stdlib.h>
#include "geracodigo.h"

int main(void)
{
    funcp func;
    int check;
    unsigned char opcode_array[1024];
    FILE *myfp;

    if ((myfp = fopen("../tests_return/test_ret_v1.txt", "rt")) == NULL)
    {
        perror("nao conseguiu abrir arquivo!");
        exit(1);
    }

    func = (funcp)geraCodigo(myfp, opcode_array);
    fclose(myfp);

    check = (*func)(1234, 4321);
    printf("resultado: ");
    printf("%d\n", check);

    return 0;
}
