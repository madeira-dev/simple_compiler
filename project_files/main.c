#include <stdio.h>
#include <stdlib.h>
#include "geracodigo.h"

int main(void)
{
    funcp func;
    int check;
    unsigned char opcode_array[] = {0x55, 0x48, 0x89, 0xe5, 0x89, 0xf8, 0xc9, 0xc3};
    // opcode_array = (unsigned char *)malloc(5 * sizeof(unsigned char));
    FILE *myfp;

    func = (funcp)opcode_array;
    check = (*func)(2);

    // if ((myfp = fopen("test1.txt", "rt")) == NULL)
    // {
    //     perror("nao conseguiu abrir arquivo!");
    //     exit(1);
    // }

    // func = (funcp)geraCodigo(myfp, opcode_array);
    // fclose(myfp);

    /* debug printf */
    // printf("\nopcode_array main:\n");
    // for (int i = 0; i < 8; i++)
    //     printf("[%x] ", opcode_array[i]);
    // printf("\n");
    // check = (*func)();
    printf("%d\n", check);

    return 0;
}
