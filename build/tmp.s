.intel_syntax noprefix
.globl main
main:
    push rbp
    mov rbp, rsp
    sub rsp, 208
    mov rax, rbp
    sub rax, 8
    push rax
    push 1
    pop rdi
    pop rax
    mov [rax], rdi
    push rdi
    pop rax
    mov rax, rbp
    sub rax, 16
    push rax
    push 1
    push 2
    pop rdi
    pop rax
    add rax, rdi
    push rax
    push 1
    pop rdi
    pop rax
    imul rax, rdi
    push rax
    push 1
    pop rdi
    pop rax
    cqo
    idiv rdi
    push rax
    pop rdi
    pop rax
    mov [rax], rdi
    push rdi
    pop rax
    mov rsp, rbp
    pop rbp
    ret
