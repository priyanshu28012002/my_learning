; ====== Uninitialized Data Section ======
section .bss
    digitSpace    resb 100    ; Reserve 100-byte buffer to store digits in ASCII form
    digitSpacePose resb 8     ; Reserve 8-byte pointer to track current position in digitSpace

; ====== Code Section ======
section .text
    global _start     ; Declare entry point for linker

; ====== Program Entry Point ======
_start:
    mov rax, 123      ; Load the number we want to print (123 in this case)
    call _printRAX    ; Call our number printing function

    ; System exit
    mov rax, 60       ; syscall number for exit (60)
    mov rdi, 0        ; exit status code (0 = success)
    syscall           ; Invoke kernel to exit program

; ====== Print Number Function (prints value in RAX) ======
_printRAX:
    ; Initialize digit buffer and position pointer
    mov rcx, digitSpace   ; Point RCX to start of digit buffer
    mov rbx, 10           ; Store 10 in RBX (for division and as delimiter)
    mov [rcx], rbx        ; Store 10 at start of buffer (acts as end marker)
    inc rcx               ; Move buffer position forward
    mov [digitSpacePose], rcx ; Save current position in digitSpacePose

; ====== Digit Extraction Loop ======
; Converts number to ASCII digits by repeatedly dividing by 10
_printRAXLoop:
    mov rdx, 0      ; Clear RDX (upper 64 bits of dividend for DIV)
    mov rbx, 10     ; Set divisor to 10 (for decimal conversion)
    div rbx         ; Divide RDX:RAX by 10
                    ; - RAX = quotient (remaining number)
                    ; - RDX = remainder (current digit 0-9)

    push rax        ; Save quotient on stack (we need RAX for syscall)
    add rdx, 48     ; Convert digit (0-9) to ASCII character ('0'-'9')

    ; Store digit in buffer
    mov rcx, [digitSpacePose] ; Get current buffer position
    mov [rcx], dl   ; Store ASCII character (lower 8 bits of RDX)
    inc rcx         ; Move to next buffer position
    mov [digitSpacePose], rcx ; Save updated position

    pop rax         ; Restore quotient (remaining number)
    cmp rax, 0      ; Check if we've processed all digits
    jne _printRAXLoop ; If not zero, continue extracting digits

; ====== Digit Printing Loop ======
; Prints digits in reverse order (from most significant to least)
_printRAXLoop2:
    ; Prepare sys_write arguments
    mov rcx, [digitSpacePose] ; Get current position in digit buffer
    mov rax, 1      ; syscall number for write (1)
    mov rdi, 1      ; file descriptor (1 = stdout)
    mov rsi, rcx    ; pointer to character to print
    mov rdx, 1      ; number of bytes to print (1 character)

    syscall         ; Invoke kernel to print character

    ; Move backward through buffer
    mov rcx, [digitSpacePose]
    dec rcx         ; Move to previous character
    mov [digitSpacePose], rcx ; Save new position

    ; Check if we've reached the start of buffer
    cmp rcx, digitSpace
    jge _printRAXLoop2 ; Continue if not before start of buffer

    ret             ; Return from function