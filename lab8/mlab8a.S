// Declare global constants
.section .rodata
prompt: .asciz "Enter left and right operands: "
scan: .asciz "%d %d"
output: .asciz "%d + %d = %d\n"

// Write CPU instructions
.section .text
.global main
main:
    // Allocate 16 bytes on the stack
    addi    sp, sp, -16

    // Store the return address
    sd      ra, 0(sp)

    // Prompt the user for their numbers
    la      a0, prompt
    call    printf@plt

    // Load the scan line into a0 for scanf
    la      a0, scan

    // Allocate room for the integers on the stack
    addi    a1, sp, 8
    addi    a2, sp, 12

    // Read the user's input into a1 and a2
    call    scanf@plt

    // a1 and a2 are memory addresses, so we need to dereference them
    lw      a1, 8(sp)
    lw      a2, 12(sp)

    // Add the inputted numbers
    add     a3, a2, a1

    // Load the output line into a0 for printf
    la      a0, output

    // Print the output string
    call    printf@plt

    // Restore ra to what we found it as
    ld      ra, 0(sp)

    // Return the 16 bytes to the stack
    addi    sp, sp, 16

    ret
