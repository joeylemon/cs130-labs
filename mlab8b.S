// Declare global constants
.section .rodata
prompt: .asciz "Enter left and right operands: "
scan: .asciz "%d %d"
output: .asciz "%d / %d = %d\n"

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

    // a1 and a2 are memory addresses, so we need to convert them to 4-byte integers
    lw      a1, 8(sp)       // Put a1 bytes at 8-12 on the stack
    lw      a2, 12(sp)      // Put a2 bytes at 12-16 on the stack

    // Set t0 = 0
    li      t0, 0

    // Check if a2 == t0 (0)
    // If true, jump to label 1
    beq     a2, t0, 1f

2: // If the beq didn't move the program to label 1, the following is executed
    // Divide the inputted numbers
    div     a3, a1, a2

    // Jump to label 0
    j       0f

1: // beq jumps here if a2 == 0
    // Go ahead and set a3 = 0
    li      a3, 0

    // Jump to label 0
    j       0f

0:
    // Load the output line into a0 for printf
    la      a0, output

    // Print the output string
    call    printf@plt

    // Restore ra to what we found it as
    ld      ra, 0(sp)

    // Return the 16 bytes to the stack
    addi    sp, sp, 16

    ret
