.text
.globl test_add_var1_var3
test_add_var1_var3:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp /* abrindo espaço no RA pra variável local */
    leaq -32(%rbp), %rdx
    leaq -24(%rbp), %rcx
    leaq -16(%rbp), %r8
    leaq -8(%rbp), %r9

    addl %r8d, %edx

    leave
    ret
