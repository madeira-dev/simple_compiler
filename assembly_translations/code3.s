/*
v1 := p1     // 1: i = x
v1 += p2     // 2: i = x + y
p1 -= p2     // 3: x = x - y
v1 *= p1     // 4: i = i * x
ret v1       // 5: retorna i
*/

.text
.globl code3
code3:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp /* abrindo espaco no RA para variavel local */
    leaq -16(%rbp), %rcx /* rcx é o registrador para a variável local */

INICIO:
    movl %edi, %ecx /* salvando o valor do primeiro parametro como variavel local na pilha */
    addl %esi, %ecx /* variavel local = variavel local + segundo parametro */

    subl %esi, %edi /* primeiro parametro = primeiro parametro - segundo parametro (?) */
    imull %edi, %ecx /* variavel local = variavel local * primeiro parametro */

    movl %ecx, %eax

FIM:
    leave
    ret
