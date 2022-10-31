/*
if p1 3 2    // 1: if (p1 < 0) desvia para linha 3
ret $0       // 2: retorna 0 (false)
ret $1       // 3: retorna 1 (true)
*/

.text
.globl code2
code2:
    pushq %rbp
    movq %rsp, %rbp

    cmpl $0, %edi
    jg MAIOR

    movl $1, %eax
    jmp FIM

MAIOR:
    movl $0, %eax
    jmp FIM

FIM:
    leave
    ret
