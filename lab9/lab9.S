// Lab 9
// COSC130
// This program searches an array for a person or finds the median
// of an integer array
// Joey Lemon
// 11/3/18

.section .rodata
name: .asciz "Name: %c\n"

.section .text
.global Search
Search:
    // PERSON *Search(PERSON people[], int num_people, const char *name, int age, char gender)
    //      a0              a0              a1               a2            a3         a4

    // Name         Offset          Size
    // name         0               51
    // age          52              4
    // gender       56              1
    // total                        60

    // Allocate bytes on the stack
    addi    sp, sp, -8

    // Save the people pointer
    sd      a0, 0(sp)

    // Begin a for-loop to parse each person in array
    li      t0, 0           // int i = 0
1:
    bge     t0, a1, 3f      // i < num_people

2:  // Compare string characters
    // Load the characters from the current person name and given name
    lb      t4, 0(a0)
    lb      t3, 0(a2)

    // If the characters don't match, move on to next person
    bne     t3, t4, 2f

    // If we've reached the end of the string, then they're the same
    // Move on to compare the age and gender
    beqz    t3, 4f
    beqz    t4, 4f

    // Move to the next character
    addi    a0, a0, 1
    addi    a2, a2, 1

    // Repeat
    j       2b

4:  // Strings are the same
    ld      a0, 0(sp)
    li      t5, 60
    mul     t6, t0, t5      // Each person is 60 bytes
    add     a0, a0, t6

    lw      t3, 52(a0)      // t3 = people[i].age
    lb      t4, 56(a0)      // t4 = people[i].gender

    bne     t3, a3, 2f
    bne     t4, a4, 2f

    j       1f
2:  // Not the right person
    addi    t0, t0, 1       // i++

    // Increment the pointer to the next person in array
    ld      a0, 0(sp)
    li      t5, 60
    mul     t6, t0, t5
    add     a0, a0, t6

    j       1b
3:  // Out of people to parse
    li      a0, 0
    j       1f
1:
    // Return the bytes to the stack
    addi    sp, sp, 8

    ret

.global Median
Median:
    // int Median(const int array[], int array_size, int *buffer)
    //     a0            a0                a1            a2

    // Allocate bytes on the stack
    addi    sp, sp, -8

    // Save the buffer pointer
    sd      a2, 0(sp)

    // Begin a for-loop to move array to buffer
    li      t0, 0
1:
    bge     t0, a1, 1f

    // Set buffer[i] to array[i]
    lw      t1, 0(a0)
    sw      t1, 0(a2)

    addi    t0, t0, 1   // i++

    // Increment array pointers by sizeof(int)
    addi    a0, a0, 4
    addi    a2, a2, 4

    j       1b

1:
    // Begin a for-loop to sort the array
    // Implementing a Gnome Sort
    li      t0, 0
1:
    bge     t0, a1, 1f  // i < array_size

    // If index is 0, add 1
    bne     t0, zero, 2f
    addi    t0, t0, 1
2:
    // Move the buffer pointer to the correct position
    // Account for sizeof(int)
    ld      a2, 0(sp)
    li      t5, 4
    mul     t6, t0, t5
    add     a2, a2, t6

    lw      t1, 0(a2)   // buffer[index]
    addi    a2, a2, -4
    lw      t2, 0(a2)   // buffer[index - 1]

    // If buffer[index] >= buffer[index - 1], add 1 to index
    blt     t1, t2, 3f
    addi    t0, t0, 1

    // Keep sorting
    j       1b
3:
    // If buffer[index] < buffer[index - 1], swap values
    sw      t2, 4(a2)
    sw      t1, 0(a2)

    addi    t0, t0, -1

    // Keep sorting
    j       1b

1:  // Buffer has been sorted
    // Move buffer to first element
    ld      a2, 0(sp)

    // Check if array size is odd
    li      t0, 2
    rem     t0, a1, t0
    bne     t0, zero, 1f

    // Array size is even if we're here
    // Divide array size by 2
    li      t0, 2
    div     t3, a1, t0

    // Multiply by sizeof(int)
    li      t0, 4
    mul     t3, t3, t0

    // Move pointer
    add     a2, a2, t3

    // t1 is now buffer[array_size / 2]
    lw      t1, 0(a2)

    ld      a2, 0(sp)
    // Subtract array size by 1
    addi    a1, a1, -1

    // Divide array size by 2
    li      t0, 2
    div     t3, a1, t0

    // Multiply by sizeof(int)
    li      t0, 4
    mul     t0, t0, t0

    // Move pointer
    add     a2, a2, t0

    // t2 is now buffer[(array_size - 1) / 2]
    lw      t2, 0(a2)

    // Add the two values together
    add     t0, t1, t2

    // Divide the values in half to get the median
    li      t1, 2
    div     a0, t0, t1

    j       2f
1:  // Array size is odd
    // Divide array size by 2
    li      t1, 2
    div     t0, a1, t1

    // Multiply by sizeof(int)
    li      t1, 4
    mul     t0, t0, t1

    // Move pointer to new position (median value)
    add     a2, a2, t0

    lw      a0, 0(a2)

2:
    // Return the bytes to the stack
    addi    sp, sp, 8
    ret
