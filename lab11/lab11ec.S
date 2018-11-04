// Lab 11
// COSC130
// This program dissects a virtual address
// Joey Lemon
// 11/9/18

.section .rodata
usage: .asciz "Usage: %s <address>\n"
hex: .asciz "%lx"
entry: .asciz "Page Table Entry: 0x%016lx\n"
physical: .asciz "Physical Address: 0x%016lx\n"
header: .asciz "Entry bits:\n"
bit: .asciz "     Bit %c: %s\n"
set: .asciz "SET"
clear: .asciz "CLEAR"

.section .text
.global main
main:
    // Allocate bytes on the stack
    addi    sp, sp, -32

    // Store the return address
    sd      ra, 0(sp)

    // Store the saved registers
    sd      s0, 8(sp)
    sd      s1, 16(sp)

    // Ensure there are 2 arguments
    ld      t1, 0(a1)       // t1 = a1[0]
    li      t0, 2
    bne     a0, t0, 14f      // a0 != t0

    // Load in all command-line arguments (pointers are 8 bytes each)
    ld      a0, 8(a1)       // a0 = a1[1]
    la      a1, hex
    addi    a2, sp, 24
    call    sscanf@plt

    // Dereference the given hexadecimal number
    ld      s0, 24(sp)

    // Print the page table entry
    mv      a1, s0
    la      a0, entry
    call    printf@plt

    // Mask the entry to find the physical address
    li      t0, 0x003ffffffffffc00
    and     t1, s0, t0

    // Shift left twice to align physical address properly
    slli    a1, t1, 2

    // Print physical address
    la      a0, physical
    call    printf@plt

    // Print entry bits header
    la      a0, header
    call    printf@plt

    // Start loop to print entry bits
    li      s1, 0x01
1:
    // If s1 > 0x80, we've tested all the bits
    li      t0, 0x80
    bgt     s1, t0, 1f

    // Mask the entry with the counter to test that bit
    and     t1, s0, s1

    // If the number is > 0, the bit is 1
    bgt     t1, zero, 2f

    // Otherwise, the bit is 0
    j       3f
2:  // It's a set bit
    la      a2, set
    j       4f
3:  // It's a clear bit
    la      a2, clear
    j       4f
4:  // Print the bit
    li      t0, 0x01
    beq     s1, t0, 5f

    li      t0, 0x02
    beq     s1, t0, 6f

    li      t0, 0x04
    beq     s1, t0, 7f

    li      t0, 0x08
    beq     s1, t0, 8f

    li      t0, 0x10
    beq     s1, t0, 9f

    li      t0, 0x20
    beq     s1, t0, 10f

    li      t0, 0x40
    beq     s1, t0, 11f

    li      t0, 0x80
    beq     s1, t0, 12f
5:
    li      a1, 'V'
    j       13f
6:
    li      a1, 'R'
    j       13f
7:
    li      a1, 'W'
    j       13f
8:
    li      a1, 'X'
    j       13f
9:
    li      a1, 'U'
    j       13f
10:
    li      a1, 'G'
    j       13f
11:
    li      a1, 'A'
    j       13f
12:
    li      a1, 'D'
    j       13f
13:
    la      a0, bit
    call    printf@plt

    // Multiply the counter by 2
    li      t0, 2
    mul     s1, s1, t0

    j       1b

14:  // Invalid arguments
    // Print out the program usage
    mv      a1, t1
    la      a0, usage
    call    printf@plt
    j       1f

1:  // End program
    // Restore ra to what we found it as
    ld      ra, 0(sp)

    // Restore the saved registers
    ld      s0, 8(sp)
    ld      s1, 16(sp)

    // Return the bytes to the stack
    addi    sp, sp, 32

    ret
