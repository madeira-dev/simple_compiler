/*
p1 += $1     // 1: x = x + 1
ret p1       // 2: retorna x
*/

.text
.globl code1
code1:
    pushq %rbp
    movq %rsp, %rbp

    addl $1, %edi /* edi += 1 */
    movl %edi, %eax /* eax = edi */

    leave
    ret
