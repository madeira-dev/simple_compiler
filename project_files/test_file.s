.text
.globl test
test:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp /* abrindo espaço no RA pra variável local */
    leaq -32(%rbp), %rdx /* primeira variavel local */
    leaq -24(%rbp), %rcx /* segunda variavel local */
    leaq -16(%rbp), %r8 /* terceira variavel local */
    leaq -8(%rbp), %r9 /* quarta variavel local */

<<<<<<< HEAD
INICIO:
    cmpl $0, %esi
    jl FIM
    subl $1, %edi
    jmp INICIO
=======
    movl $912631, %eax

>>>>>>> 78f65176671b4dec860d060fedb257491826e20d
FIM:
    leave
    ret
