section .data
    hello db 'Hell, orld!', 10  ; 'Hello, World!' string followed by newline (0xa)
    len equ $ - hello              ; Length of the string

section .text
    global _start

_start:
    ; Write the string to stdout
    mov rax, 1              ; syscall number for sys_write
    mov rdi, 1              ; file descriptor 1 (stdout)
    mov rsi, hello          ; pointer to string
    mov rdx, len         ; length of string
    syscall                 ; invoke kernel

    ; Exit the program
    mov rax, 60             ; syscall number for sys_exit
    xor rdi, 0            ; exit status 0
    syscall                 ; invoke kernel


; hello 
; this is an name assinged to the address in the memory that this data is located in 
; wethever we use "text" later in the code when the code is compiled the compiler weill determine the actual location in memeory of the data and replace all future instances of the "text" with that memory adress
; db define bytes 
; ut essentially means that we are goin gti defeine some raw bytes of data to insert into our code
; 'Hello, World!' 
; this is the bit of the daya we are define 
; each char in the string if the text is a single byte 
; the "10" or 0xa is newline character which is often denote as "/n" in cPP


in 64 index pointer (hold the adreaa of next instructopm pointer) 
it geneally move from top to bottom

jmp _start

in this case the memory address of the _start is loded in the rip

cmp  rax 23
cmp rax rbx
after this certaom flag is set
ZF mean zero flag

a=b ZF = 1
a != b ZF 0
