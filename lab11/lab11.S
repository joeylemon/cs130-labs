// Lab 11
// COSC130
// This program dissects a virtual address
// Joey Lemon
// 11/9/18

.section .rodata
usage: .asciz "Usage: %s <address>\n"
hex: .asciz "%lx"
header: .asciz "  Level   Index   Byte Offset\n"
vpn: .asciz "VPN[%03d] = %-3lu     (%4lu)\n"
poff: .asciz "POFF =     %lx\n"

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
    bne     a0, t0, 2f      // a0 != t0

    // Load in all command-line arguments (pointers are 8 bytes each)
    ld      a0, 8(a1)       // a0 = a1[1]
    la      a1, hex
    addi    a2, sp, 24
    call    sscanf@plt

    // Dereference the given hexadecimal number
    ld      s0, 24(sp)

    // Print the output header
    la      a0, header
    call    printf@plt

    // Load 2 into the iterator
    li      s1, 2
1:  // Begin loop
    // If s1 < 0, break loop
    blt     s1, zero, 1f

    // Find VPN[s1] by multipling s1 by 9 and adding 12
    li      t0, 9
    mul     t1, s1, t0
    addi    t1, t1, 12

    // Shift the address logically right
    srl     a2, s0, t1

    // Mask the shifted address with 0x1FF
    li      t0, 0x1FF
    and     a2, a2, t0

    // Multiply the index by 8 to find byte offset
    li      t0, 8
    mul     a3, a2, t0

    // Print the VPN index
    mv      a1, s1
    la      a0, vpn
    call    printf@plt

    // Decrement the iterator
    addi    s1, s1, -1

    j       1b
1:  // Loop has finished
    // Mask the address with 0xFFF to find page offset
    li      t0, 0xFFF
    and     a1, s0, t0

    // Print the page offset
    la      a0, poff
    call    printf@plt

    j       1f

2:  // Invalid arguments
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
