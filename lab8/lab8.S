// Lab 8
.section .rodata
arguments: .asciz "Left: %d, Op: %d, Right: %d\n"
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
    bne     a0, t0, bad_args

    // Load in all command-line arguments
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
    lw      a1, 9(sp)       // load left int from sp + 9
    lb      a2, 8(sp)       // load op from sp + 8
    lw      a3, 13(sp)      // load right int from sp + 13

    // Check what operator was input
    li      t0, 43          // 43 is +
    beq     a2, t0, add

    li      t0, 45          // 45 is -
    beq     a2, t0, subtract

    li      t0, 120         // 120 is x
    beq     a2, t0, multiply

    li      t0, 47          // 47 is /
    beq     a2, t0, divide

    li      t0, 37          // 37 is %
    beq     a2, t0, mod

    // If the program hasn't jumped, we don't know the operator
    j       unknown_op

add:
    add     a4, a1, a3
    j       print_result

subtract:
    sub     a4, a1, a3
    j       print_result

multiply:
    mul     a4, a1, a3
    j       print_result

divide:
    // Check if dividing by 0
    beq     a3, zero, divide_by_0

    div     a4, a1, a3
    j       print_result

divide_by_0:
    // Set result to -1
    li      a4, -1
    j       print_result

mod:
    // Check if mod by 0
    beq     a3, zero, mod_by_0

    rem     a4, a1, a3
    j       print_result

mod_by_0:
    // Set result to 0
    li      a4, 0
    j       print_result

unknown_op:
    la      a0, unknown
    mv      a1, a2          // a1 = operator
    call    printf@plt
    j       return

bad_args:
    mv      a1, t1
    la      a0, usage
    call    printf@plt
    j       return

print_result:
    lw      a1, 9(sp)       // load left int from sp + 9
    lb      a2, 8(sp)       // load op from sp + 8
    lw      a3, 13(sp)      // load right int from sp + 13

    la      a0, result
    call    printf@plt

return:
    // Restore ra to what we found it as
    ld      ra, 0(sp)

    // Return the bytes to the stack
    addi    sp, sp, 21

    ret
