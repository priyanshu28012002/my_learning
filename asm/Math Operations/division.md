# Division in x86-64 Assembly: Key Notes

## Basic Division Operation (DIV)

1. **Register Usage**:
   - Dividend is always a combination of **RDX:RAX** (128-bit value)
   - Divisor is specified as the operand
   - Quotient goes to RAX
   - Remainder goes to RDX

2. **Division Cases**:

   ```assembly
   ; 128-bit/64-bit division (RDX:RAX / operand)
   div rcx  ; Divide RDX:RAX by RCX
   ```

   - If RDX ≠ 0: Uses full 128-bit dividend (RDX as high 64 bits + RAX as low 64 bits)
   - If RDX = 0: Performs 64-bit division (just RAX / operand)

3. **Result Storage**:
   - Quotient → RAX
   - Remainder → RDX

## Important Considerations

1. **Division Width**:
   - `div r/m64` → 128-bit/64-bit division (RDX:RAX / operand)
   - `div r/m32` → 64-bit/32-bit division (EDX:EAX / operand)
   - `div r/m16` → 32-bit/16-bit division (DX:AX / operand)
   - `div r/m8` → 16-bit/8-bit division (AX / operand)

2. **Common Preparation**:
   ```assembly
   mov rax, dividend_low
   mov rdx, dividend_high  ; Or xor rdx,rdx for 64-bit division
   div divisor
   ```

3. **Special Cases**:
   - **Unsigned division**: Use `DIV`
   - **Signed division**: Use `IDIV`
   - **Division by zero**: Causes #DE (Divide Error) exception

## Example: 128-bit Division

```assembly
section .data
    dividend dq 0x0000000000000005, 0x0000000000000001  ; High, Low (0x10000000000000005)
    divisor  dq 0x2
    quotient dq 0
    remainder dq 0

section .text
    global _start

_start:
    ; Load dividend (128-bit)
    mov rax, [dividend]      ; Low 64 bits
    mov rdx, [dividend + 8]  ; High 64 bits
    
    ; Divide by divisor
    div qword [divisor]      ; RDX:RAX / [divisor]
    
    ; Store results
    mov [quotient], rax
    mov [remainder], rdx
    
    ; Exit
    mov rax, 60
    xor rdi, rdi
    syscall
```

## Key Points to Remember

1. **RDX must be properly set** - either:
   - Zero for 64-bit division
   - High part of dividend for 128-bit division

2. **Results are always**:
   - Quotient in RAX (or EAX/AX/AL depending on operand size)
   - Remainder in RDX (or EDX/DX/AH depending on operand size)

3. **Division is slow** - one of the slowest arithmetic operations

4. **Error handling** - should check for:
   - Division by zero
   - Overflow (if quotient is too large for destination)

Would you like me to provide any specific division examples or explain any part in more detail?