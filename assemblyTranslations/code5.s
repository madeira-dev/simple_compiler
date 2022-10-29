/*
v1 := $0       // 1: sum = 0
if p1 8 8      // 2: if (x ≤ 0) desvia para linha 8
v2 := p1       // 3: aux = x
v2 *= v2       // 4: aux = x * x
v1 += v2       // 5: sum = sum + (x * x)
p1 -= $1       // 6: x = x - 1
go 2           // 7: desvia para linha 2
ret v1         // 8: retorna sum
*/

.text
.globl code5
code5:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp /* abrindo espaco no RA para as variavies locais */
    leaq -16(%rbp), %rcx /* v1 = sum */
    leaq -8(%rbp), %rdx /* v2 = aux */

    movl $0, %ecx /* v1 = 0 */

INICIO:
    cmpl $0, %edi /* primeiro parametro <= 0 ?? */
    jle FIM

    movl %edi, %edx /* edx = edi // segunda variavel local = primeiro parametro */
    imull %edx, %edx /* segunda variavel local = segunda variavel local * segunda variavel local */
    addl %edx, %ecx /* primeira variavel local = primeira variavel local + segunda variavel local */
    subl $1, %edi /* primeiro parametro = primeiro parametro - 1 */
    jmp INICIO

FIM:
    movl %ecx, %eax
    leave
    ret
