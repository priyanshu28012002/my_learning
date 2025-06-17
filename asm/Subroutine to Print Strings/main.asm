section .data
    text db "Hello, World!",10,0
    text2 db "Ka hi",0

section .text
    global _start

_start:
    mov rsi, text      ; address of string to print
    call _print
    mov rsi, text2      ; address of string to print
    call _print

    mov rax, 60        ; sys_exit
    mov rdi, 0         ; exit code 0
    syscall

_print:
    ; Calculate length first
    mov rdi, rsi       ; copy string address
    mov rcx, 0         ; initialize counter
    
_count_loop:
    cmp byte [rdi], 0  ; check for null terminator
    je _do_print
    inc rdi
    inc rcx
    jmp _count_loop

_do_print:
    ; RCX now contains string length
    mov rax, 1         ; sys_write
    mov rdi, 1         ; stdout
    mov rdx, rcx       ; length
    syscall
    ret