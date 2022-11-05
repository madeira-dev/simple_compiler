.text
.globl ret_test3
ret_test3:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp /* abrindo espaço no RA pra variável local */
    leaq -16(%rbp), %rdx

    movl $200, %edx
    movl %edx, %eax

    leave
    ret
