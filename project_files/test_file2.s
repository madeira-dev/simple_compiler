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

    movl $3 ,%edx /* v1 = 0 */

INICIO:
    cmpl $0, %edi /* if p1 8 8 */
    jl FIM_V1
    je FIM_0

    movl %edi, %ecx /* v2 = p1 */
    imull %ecx, %ecx /* v2 *= v2 */
    addl %ecx, %edx /* v1 += v2 */
    subl $1, %edi /* p1 -= 1 */
    jmp INICIO

FIM_V1:
    movl %edx, %eax
    leave
    ret

FIM_0:
    movl %edi, %eax
    leave
    ret
