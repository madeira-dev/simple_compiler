.text
.globl test_attr_param_var
test_attr_param_var:
    pushq %rbp
    movq %rsp, %rbp 
    subq $32, %rsp /* abrindo espaço no RA pra variável local */
    leaq -32(%rbp), %rdx
    leaq -24(%rbp), %rcx
    leaq -16(%rbp), %r8
    leaq -8(%rbp), %r9

    movl %r9d, %esi

    leave
    ret