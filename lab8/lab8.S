// Lab 8
// COSC130
// This program takes a mathematical expression
// from the user and calculates the result
// Joey Lemon
// 10/19/18
.section .rodata
scan: .asciz "%d"
result: .asciz "%d %c %d = %d\n"
unknown: .asciz "Unknown operator \'%c\'\n"
usage: .asciz "Usage: %s <left> <op> <right>\n"

.section .text
.global main
main:
    // Allocate bytes on the stack
    addi    sp, sp, -21

    // Store the return address
    sd      ra, 0(sp)

    // Ensure there are 4 arguments
    ld      t1, 0(a1)       // t1 = a1[0]
    li      t0, 4
    bne     a0, t0, 7f      // a0 != t0

    // Load in all command-line arguments (pointers are 8 bytes each)
    ld      t1, 8(a1)       // left = a1[1]
    ld      t2, 16(a1)      // op = a1[2]
    ld      t3, 24(a1)      // right = a1[3]

    // Get first character of operator string
    lb      t2, 0(t2)       // op = op[0]

    // Save operator and right string since they might be overwritten
    sb      t2, 8(sp)       // save operator at sp + 8
    sd      t3, 13(sp)      // save right string at sp + 13

    // Scan the left operand
    mv      a0, t1          // a0 = left string
    la      a1, scan        // a1 = scan string
    addi    a2, sp, 9       // a2 = sp + 9
    call    sscanf@plt

    // Scan the right operand
    ld      a0, 13(sp)      // load a0 from sp + 13 (right string loc)
    la      a1, scan        // a1 = scan string
    addi    a2, sp, 13      // a2 = sp + 13
    call    sscanf@plt

    // Load the operands and operator
    lw      a0, 9(sp)       // load left int from sp + 9
    lb      a2, 8(sp)       // load op from sp + 8
    lw      a1, 13(sp)      // load right int from sp + 13

    // Check what operator was input
    li      t0, '+'
    beq     a2, t0, 1f

    li      t0, '-'
    beq     a2, t0, 2f

    li      t0, 'x'
    beq     a2, t0, 3f

    li      t0, '/'
    beq     a2, t0, 4f

    li      t0, '%'
    beq     a2, t0, 5f

    // If the program hasn't jumped, we don't know the operator
    j       6f

1: // Add
    call    AddNums
    mv      a4, a0

    j       8f

2: // Subtract
    call    SubNums
    mv      a4, a0
    j       8f

3: // Multiply
    call    MulNums
    mv      a4, a0
    j       8f

4: // Divide
    call    DivNums
    mv      a4, a0
    j       8f

5: // Remainder
    call    ModNums
    mv      a4, a0
    j       8f

6: // Unknown operator
    la      a0, unknown
    mv      a1, a2          // a1 = operator
    call    printf@plt
    j       9f

7: // Bad arguments
    mv      a1, t1
    la      a0, usage
    call    printf@plt
    j       9f

8: // Print the result
    lw      a1, 9(sp)       // load left int from sp + 9
    lb      a2, 8(sp)       // load op from sp + 8
    lw      a3, 13(sp)      // load right int from sp + 13

    la      a0, result
    call    printf@plt

9: // Clean up the stack
    // Restore ra to what we found it as
    ld      ra, 0(sp)

    // Return the bytes to the stack
    addi    sp, sp, 21

    ret

// int AddNums(int left, int right)
//     a0        a0         a1
AddNums:
    add     a0, a0, a1
    ret

// int SubNums(int left, int right)
//     a0        a0         a1
SubNums:
    sub     a0, a0, a1
    ret

// int MulNums(int left, int right)
//     a0        a0         a1
MulNums:
    mul     a0, a0, a1
    ret

// int DivNums(int left, int right)
//     a0        a0         a1
DivNums:
    beq     a1, zero, 1f
    div     a0, a0, a1
    ret
1: // Right num is 0
    li      a0, -1
    ret

// int ModNums(int left, int right)
//     a0        a0         a1
ModNums:
    beq     a1, zero, 1f
    rem     a0, a0, a1
    ret
1: // Right num is 0
    li      a0, 0
    ret
