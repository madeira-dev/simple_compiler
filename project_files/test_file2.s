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

    movl $1 ,%edx

INICIO:
    cmpl $0, %edi
    jl FIM
    je FIM

    imull %edi, %edx
    subl $1, %edi
    jmp INICIO 

FIM:
    movl %edx, %eax
    leave
    ret
