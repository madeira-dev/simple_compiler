.text
.globl ret_test4
ret_test4:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp /* abrindo espaço no RA pra variável local */
    leaq -32(%rbp), %rdx
    leaq -24(%rbp), %rcx
    leaq -16(%rbp), %r8
    leaq -8(%rbp), %r9

    movl $200, %ecx
    movl %ecx, %eax

    leave
    ret
