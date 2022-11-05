.text
.globl test1
ret_test2:
    pushq %rbp
    movq %rsp, %rbp

    movl %esi, %eax
    
    leave
    ret
