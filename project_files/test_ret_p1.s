.text
.globl test1
ret_test1:
    pushq %rbp
    movq %rsp, %rbp

    movl %edi, %eax
    
    leave
    ret
