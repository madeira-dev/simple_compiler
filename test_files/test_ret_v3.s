.text
.globl ret_test5
ret_test5:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp /* abrindo espaço no RA pra variável local */
    leaq -32(%rbp), %rdx
    leaq -24(%rbp), %rcx
    leaq -16(%rbp), %r8
    leaq -8(%rbp), %r9

    movl $200, %r8d
    movl %r8d, %eax

    leave
    ret
