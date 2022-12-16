.section .text
    .global _start
_start:
loop_initial:
    

    mov $1, %rax 
    mov $2, %rdi
    mov $messages,%rsi
    mov $6, %rdx
    syscall

    mov $0, %rax 
    mov $1, %rdi
    mov count,%rsi
    mov $1, %rdx
    syscall

    
  #  mov $1, %rax      
  #  mov %rdx, %rsi
   # mov $2, %rdi 
  #  mov count,%rsi 
   # mov $1, %rdx
    #syscall

    mov count,%eax

loop_for:

    push %rax

    mov $1, %rax   
    mov $message,%rsi
    mov $10, %rdx
    syscall

    pop %rax

    dec %eax
    test %eax,%eax
    jne loop_for
 
exit:
    mov $60,%rax
    xor %rdi, %rdi
    syscall

message:
    .asciz "YZU nice!\n"
messages:
    .asciz "input:"    

    .data
count: .quad 5
