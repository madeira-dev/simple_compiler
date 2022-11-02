.text
.globl test1
test1:
    pushq %rbp
    movq %rsp, %rbp

    movl %edi, %eax
    
    leave
    ret
