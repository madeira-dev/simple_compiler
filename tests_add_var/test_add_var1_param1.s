.text
.globl test_add_var_param1
test_add_var_param1:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp /* abrindo espaço no RA pra variável local */
    leaq -32(%rbp), %rdx /* primeira variável local */
    leaq -24(%rbp), %rcx /* segunda variável local */
    leaq -16(%rbp), %r8 /* terceira variável local */
    leaq -8(%rbp), %r9 /* quarta variável local */

    addl %edi, %edx

    leave
    ret
