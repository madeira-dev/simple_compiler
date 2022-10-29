/*
v1 := $1     // 1: f = 1
if p1 6 6    // 2: if (n ≤ 0) desvia para linha 6
v1 *= p1     // 3: f = f * n
p1 -= $1     // 4: n = n - 1
go 2         // 5: desvia para linha 2
ret v1       // 6: retorna f
*/

.text
.globl code4
code4:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp /* abrindo espaço na pilha pra variavel local */
    leaq -16(%rbp), %rcx /* registrador pra variavel local */

    movl $1, %ecx /* guardando primeiro parametro na pilha como variavel local */

INICIO:
    cmpl $0, %edi /* primeiro parametro <= 0 ? */
    jle FIM

    imull %edi, %ecx /* variavel local = variavel local * primeiro parametro */
    subl $1, %edi /* primeiro parametro = primeiro parametro - 1 */
    jmp INICIO

FIM:
    movl %ecx, %eax
    leave
    ret
