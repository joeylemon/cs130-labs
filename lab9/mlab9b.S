// test
.section .text
.global CalculateASM
CalculateASM:
    // void Calculate(MyStruct &s, char op)
    //                     a0         a1

    // Load the left and right pointers from the struct
    ld      t0, 8(a0)       // t0 = left
    ld      t1, 16(a0)      // t1 = right

    // Dereference the left and right pointers into ints
    lw      t0, 0(t0)       // t0 = *t0
    lw      t1, 0(t1)       // t1 = *t1

    // Check if operator is +
    li      t3, '+'
    beq     a1, t3, 1f

    // Check if operator is -
    li      t3, '-'
    beq     a1, t3, 2f

    // Operator is unknown, return -1
    li      t2, -1
    sw      t2, 0(a0)
    ret

1:
    add     t2, t0, t1

    // Save to result (starts at offset 0)
    sw      t2, 0(a0)

    ret

2:
    sub     t2, t0, t1

    // Save to result (starts at offset 0)
    sw      t2, 0(a0)

    ret
