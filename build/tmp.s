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
    sub rax, 8
    push rax
    pop rax
    mov rax, [rax]
    push rax
    push 2
    pop rdi
    pop rax
    cmp rax, rdi
    sete al
    movzb rax, al
    push rax
    pop rax
    cmp rax, 0
    je  .Lelse0
    mov rax, rbp
    sub rax, 16
    push rax
    push 1
    pop rdi
    pop rax
    mov [rax], rdi
    push rdi
    mov rax, rbp
    sub rax, 16
    push rax
    pop rax
    mov rax, [rax]
    push rax
    push 1
    pop rdi
    pop rax
    cmp rax, rdi
    sete al
    movzb rax, al
    push rax
    pop rax
    cmp rax, 0
    je  .Lelse1
    mov rax, rbp
    sub rax, 24
    push rax
    mov rax, rbp
    sub rax, 16
    push rax
    pop rax
    mov rax, [rax]
    push rax
    pop rdi
    pop rax
    mov [rax], rdi
    push rdi
    jmp .Lendif1
  .Lelse1:
    mov rax, rbp
    sub rax, 32
    push rax
    push 2
    pop rdi
    pop rax
    mov [rax], rdi
    push rdi
    mov rax, rbp
    sub rax, 24
    push rax
    mov rax, rbp
    sub rax, 16
    push rax
    pop rax
    mov rax, [rax]
    push rax
    mov rax, rbp
    sub rax, 32
    push rax
    pop rax
    mov rax, [rax]
    push rax
    pop rdi
    pop rax
    imul rax, rdi
    push rax
    pop rdi
    pop rax
    mov [rax], rdi
    push rdi
  .Lendif1:
    jmp .Lendif0
  .Lelse0:
    mov rax, rbp
    sub rax, 8
    push rax
    pop rax
    mov rax, [rax]
    push rax
    push 2
    pop rdi
    pop rax
    cmp rdi, rax
    setl al
    movzb rax, al
    push rax
    pop rax
    cmp rax, 0
    je  .Lelse2
    mov rax, rbp
    sub rax, 24
    push rax
    push 4
    pop rdi
    pop rax
    mov [rax], rdi
    push rdi
    jmp .Lendif2
  .Lelse2:
    mov rax, rbp
    sub rax, 16
    push rax
    push 1
    pop rdi
    pop rax
    mov [rax], rdi
    push rdi
    mov rax, rbp
    sub rax, 32
    push rax
    push 2
    pop rdi
    pop rax
    mov [rax], rdi
    push rdi
    mov rax, rbp
    sub rax, 24
    push rax
    mov rax, rbp
    sub rax, 16
    push rax
    pop rax
    mov rax, [rax]
    push rax
    mov rax, rbp
    sub rax, 32
    push rax
    pop rax
    mov rax, [rax]
    push rax
    pop rdi
    pop rax
    imul rax, rdi
    push rax
    push 1
    pop rdi
    pop rax
    add rax, rdi
    push rax
    pop rdi
    pop rax
    mov [rax], rdi
    push rdi
  .Lendif2:
  .Lendif0:
    pop rax
    mov rax, rbp
    sub rax, 24
    push rax
    pop rax
    mov rax, [rax]
    push rax
    pop rax
    mov rsp, rbp
    pop rbp
    ret
    pop rax
    mov rsp, rbp
    pop rbp
    ret
