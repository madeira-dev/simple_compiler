.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp /* abrindo espaço no RA pra variável local */
    leaq -32(%rbp), %rdx /* primeira variavel local */
    leaq -24(%rbp), %rcx /* segunda variavel local */
    leaq -16(%rbp), %r8 /* terceira variavel local */
    leaq -8(%rbp), %r9 /* quarta variavel local */

    addl %r9d, %r9d

leave
ret
