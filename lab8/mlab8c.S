// Declare global constants
.section .rodata
prompt: .asciz "Enter a min and max: "
scan: .asciz "%d %d"
output: .asciz "Random: %d\n"

// Write CPU instructions
.section .text
.global main
main:
    // Allocate 12 bytes on the stack
    addi    sp, sp, -16

    // Store the return address
    sd      ra, 0(sp)

    la      a0, prompt
    call    printf@plt

    la      a0, scan
    addi    a1, sp, 8   // min
    addi    a2, sp, 12  // max
    call    scanf@plt

    li      a0, 0
    call    time@plt
    call    srand@plt

    lw      a0, 8(sp)   // arg0 for getrandom = min
    lw      a1, 12(sp)  // arg1 for getrandom = max
    call GetRandom

    mv      a1, a0      // a1 = a0

    la      a0, output
    call    printf@plt

    // Restore ra to what we found it as
    ld      ra, 0(sp)

    // Return the 12 bytes to the stack
    addi    sp, sp, 16

    ret

GetRandom:
    addi    sp, sp, -16
    sd      ra, 0(sp)
    sw      a0, 8(sp)   // save a0
    sw      a1, 12(sp)  // save a1

    // int GetRandom(int min, int max);
    // a0 GetRandom(a0 min, a1 max);
    // return min + rand() % (max-min+1)
    call    rand@plt
    // a0 = random number
    lw      a1, 8(sp)   // a1 = min
    lw      a2, 12(sp)  // a2 = max

    sub     a2, a2, a1  // a2 = max - min
    addi    a2, a2, 1   // a2 = max - min + 1
    rem     a0, a0, a2  // a0 = a0 % a2 (rand % max-min+1)
    add     a0, a0, a1  // a0 = min + rand() % max-min+1

    ld      ra, 0(sp)
    addi    sp, sp, 16
    ret
