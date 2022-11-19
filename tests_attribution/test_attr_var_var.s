.text
.globl test_attr_var_const
test_attr_var_const:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp /* abrindo espaço no RA pra variável local */
    leaq -32(%rbp), %rdx /* primeira variavel local */
    leaq -24(%rbp), %rcx /* segunda variavel local */
    leaq -16(%rbp), %r8 /* terceira variavel local */
    leaq -8(%rbp), %r9 /* quarta variavel local */

    movl %r9d, %r8d

    leave
    ret
