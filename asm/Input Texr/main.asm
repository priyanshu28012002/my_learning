; ; Professional Name Input Program
; ; Assembler: NASM
; ; OS: Ubuntu 22.04 64-bit
; ; Syntax: Intel

; section .data
;     ; Constants with proper naming and alignment
;     prompt      db  "What is your name? ", 0
;     greeting    db  "Hello, ", 0
;     newline     db  0xa
;     ; Calculate lengths at assembly time
;     prompt_len  equ $ - prompt
;     greeting_len equ $ - greeting
;     nl_len      equ $ - newline

; section .bss
;     ; Reserve 32 bytes for name (more reasonable size)
;     name        resb 32
;     ; Store actual length of input
;     name_len    resq 1

; section .text
;     global _start

; _start:
;     ; Print prompt
;     call print_prompt
    
;     ; Get user input
;     call get_name
    
;     ; Print greeting
;     call print_greeting
    
;     ; Print user's name
;     call print_name
    
;     ; Print newline
;     call print_newline
    
;     ; Exit cleanly
;     mov rax, 60         ; sys_exit
;     xor rdi, rdi        ; exit code 0
;     syscall

; ;-----------------------------------------------------
; ; Prints the name prompt
; print_prompt:
;     mov rax, 1          ; sys_write
;     mov rdi, 1          ; stdout
;     mov rsi, prompt
;     mov rdx, prompt_len
;     syscall
;     ret

; ;-----------------------------------------------------
; ; Gets user input and stores length
; get_name:
;     mov rax, 0          ; sys_read
;     mov rdi, 0          ; stdin
;     mov rsi, name
;     mov rdx, 32         ; max length
;     syscall
    
;     ; Store actual length (excluding newline)
;     dec rax             ; remove newline from count
;     mov [name_len], rax
;     ret

; ;-----------------------------------------------------
; ; Prints the greeting
; print_greeting:
;     mov rax, 1          ; sys_write
;     mov rdi, 1          ; stdout
;     mov rsi, greeting
;     mov rdx, greeting_len
;     syscall
;     ret

; ;-----------------------------------------------------
; ; Prints the user's name
; print_name:
;     mov rax, 1          ; sys_write
;     mov rdi, 1          ; stdout
;     mov rsi, name
;     mov rdx, [name_len] ; use actual length
;     syscall
;     ret

; ;-----------------------------------------------------
; ; Prints a newline
; print_newline:
;     mov rax, 1          ; sys_write
;     mov rdi, 1          ; stdout
;     mov rsi, newline
;     mov rdx, nl_len
;     syscall
;     ret

; Name Input Program with Proper Formatting
; Assembler: NASM
; OS: Ubuntu 22.04 64-bit

section .data
    prompt      db  "What is your name? ", 10
    
    newline     db  0xa
    greeting    db  "s, ", 0
    prompt_len  equ $ - prompt - 1  ; -1 to exclude null terminator
    greeting_len equ $ - greeting - 1
    nl_len      equ $ - newline

section .bss
    name        resb 32
    name_len    resq 1

section .text
    global _start

_start:
    ; Print prompt (without newline)
    call print_prompt
    
    ; Get user input (will include newline)
    call get_name
    
    ; Print greeting
    ; call print_greeting
    
    ; Print user's name (without newline)
    call print_name
    
    ; Print final newline
    call print_newline
    
    ; Exit
    mov rax, 60
    xor rdi, rdi
    syscall

print_prompt:
    mov rax, 1
    mov rdi, 1
    mov rsi, prompt
    mov rdx, prompt_len
    syscall
    ret

get_name:
    mov rax, 0
    mov rdi, 0
    mov rsi, name
    mov rdx, 32
    syscall
    
    ; Store actual length (including newline)
    mov [name_len], rax
    
    ; Remove the newline from the name
    mov rcx, [name_len]
    dec rcx             ; point to last character
    mov byte [name + rcx], 0  ; replace newline with null
    ret

print_greeting:
    mov rax, 1
    mov rdi, 1
    mov rsi, greeting
    mov rdx, greeting_len
    syscall
    ret

print_name:
    mov rax, 1
    mov rdi, 1
    mov rsi, name
    mov rdx, [name_len]
    dec rdx             ; print without newline
    syscall
    ret

print_newline:
    mov rax, 1
    mov rdi, 1
    mov rsi, newline
    mov rdx, nl_len
    syscall
    ret