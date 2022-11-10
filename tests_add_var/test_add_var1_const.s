.text
.globl test_add_var_const
test_add_var_const:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp /* abrindo espaço no RA pra variável local */
    leaq -32(%rbp), %rdx
    leaq -24(%rbp), %rcx
    leaq -16(%rbp), %r8
    leaq -8(%rbp), %r9

    addl $1, %edx

    leave
    ret
