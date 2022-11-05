.text
.globl ret_test4
ret_test4:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp /* abrindo espaço no RA pra variável local */
    leaq -16(%rbp), %rdx
    leaq -8(%rbp), %rcx

    movl $200, %ecx
    movl %ecx, %eax

    leave
    ret
