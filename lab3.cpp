// Lab 3
// COSC130
// This program performs arithmetic on 32-bit IEEE754 numbers
// Joey Lemon
// 9/14/18
// TA: Jacob

#include <cstdio>
#include <string>

struct Real
{
    // Sign bit
    int sign;
    
    // UNBIASED exponent
    long exponent;
    
    // Fraction including implied 1 at bit index 23
    unsigned long fraction;
};

// Adds two numbers using only bitwise operators
unsigned long BitAdd(unsigned long left, unsigned long right);

// Multiplies two numbers using only bitwise operators
unsigned long BitMultiply(unsigned long left, unsigned long right);

// Decodes a 32-bit number in IEEE-754 format into a Real struct
Real Decode(int float_value);

// Encodes a Real struct into a 32-bit number in IEEE-754 format
int Encode(Real real_value);

// Normalizes a Real's exponent and fraction
Real Normalize(Real value);

// Multiplies two Reals into a normalized Real
Real Multiply(Real left, Real right);

// Adds two Reals into a normalized Real
Real Add(Real left, Real right);

int main(int argc, char *argv[]) {
    int left, right;
    char op;
    int value;
    
    Real rLeft, rRight, result;
    if (argc < 4) {
        printf("Usage: %s <left> <op> <right>\n", argv[0]);
        return -1;
    }
    
    sscanf(argv[1], "%f", (float *)&left);
    sscanf(argv[2], "%c", &op);
    sscanf(argv[3], "%f", (float *)&right);
    
    rLeft = Decode(left);
    rRight = Decode(right);
    
    if (op == 'x') {
        result = Multiply(rLeft, rRight);
    }else if (op == '+') {
        result = Add(rLeft, rRight);
    }else {
        printf("Unknown operator '%c'\n", op);
        return -2;
    }
    
    value = Encode(result);
    printf("%.3f %c %.3f = %.3f (0x%08x)\n",
           *((float*)&left),
           op,
           *((float*)&right),
           *((float*)&value),
           value
           );
    return 0;
}

Real Decode(int float_value) {
    Real real;
    
    // Find the sign bit value by moving it to bit index 0 and ANDing 1
    real.sign = (float_value >> 31) & 1;
    
    // Isolate the 8 exponent bits and right-shift them to the end of the number
    // and then subtract the bias of 127
    real.exponent = ((float_value & 0x7F800000) >> 23) - 127;
    
    // Isolate the 23 fraction bits and add the implied 1 to bit index 24
    real.fraction = (float_value & 0x007FFFFF) | 1 << 23;
    
    return real;
}

int Encode(Real real_value) {
    int value = 0;
    
    // Put the sign bit at bit index 31
    value = real_value.sign << 31;
    
    // Add the exponent bits (with bias) by ORing the value with the 
	// exponent bits
    value |= (real_value.exponent + 127) << 23;
    
    // AND the fraction with 23 1's and then OR that value to add the 
	// fraction bits to the value
    value |= (real_value.fraction & 0x007FFFFF);
    
    return value;
}

Real Normalize(Real value) {
    Real normalized;
    normalized.sign = value.sign;
    normalized.fraction = value.fraction;
    normalized.exponent = value.exponent;
    
    // Loop from the left-most bit until we reach the last 1
    int bit = 63;
    while(bit){
        // Check if the value only has a single 1 at bit index 0
        if((value.fraction >> bit) & 1){
            break;
        }else{
            // Continue checking until we reach the last 1
            bit--;
        }
    }
    
    // Check if we need to move the fraction left or right
    if(bit < 23){
        int move = 23 - bit;
        
        // Move the fraction left
        normalized.fraction <<= move;
        
        // Decrease the exponent
        normalized.exponent -= move;
    }else{
        int move = bit - 23;
        
        // Move the fraction right
        normalized.fraction >>= move;
        
        // Increase the exponent
        normalized.exponent += move;
    }
    
    return normalized;
}

Real Multiply(Real left, Real right) {
    Real product;
    
    // Get the product sign by XORing both signs
    product.sign = left.sign ^ right.sign;
    
    // Add the exponents together
    product.exponent = BitAdd(left.exponent, right.exponent);
    
    // Multiply the fractions together
    product.fraction = BitMultiply(left.fraction, right.fraction);
    
    // Return the normalized product
    return Normalize(product);
}

Real Add(Real left, Real right) {
    Real sum;
    sum.sign = 0;
    
    // Loop until the exponents are the same
    while(left.exponent ^ right.exponent){
        // Check if the left exponent is larger
        if(left.exponent > right.exponent){
            // Move the left fraction left
            left.fraction <<= 1;
            
            // Decrement the left exponent
            left.exponent--;
        }else if(right.exponent > left.exponent){
            // Move the right fraction left
            right.fraction <<= 1;
            
            // Decrement the right exponent
            right.exponent--;
        }
    }
    
    // If the Real is negative, convert the fraction to two's complement
    if(left.sign){
        left.fraction = BitAdd(~left.fraction, 1);
    }
    if(right.sign){
        right.fraction = BitAdd(~right.fraction, 1);
    }
    
    // Add the fractions together
    sum.fraction = BitAdd(left.fraction, right.fraction);
    
    // Check if the sum's 64th bit is 1
    if((sum.fraction >> 63) & 1){
        // Convert the sum's fraction to two's complement
        sum.fraction = BitAdd(~sum.fraction, 1);
        
        // Set the sum's sign to negative
        sum.sign = 1;
    }
    
    // Set the sum's exponent to left exponent (could be right too)
    sum.exponent = left.exponent;
    
    // Return the normalized sum
    return Normalize(sum);
}

unsigned long BitAdd(unsigned long left, unsigned long right) {
    // Track the carry of the addition
    unsigned long carry;
    
    // Loop until the right number is 0
    while(right != 0){
        // If left and right == 1, carry the 1
        carry = left & right;
        
        // Set left to 1 if right is 0, or left to 0 if right is 1
        left = left ^ right;
        
        // Set right to the left-shifted carry amount
        right = carry << 1;
    }
    
    // left will become the sum of numbers after the loop
    return left;
}

unsigned long BitMultiply(unsigned long left, unsigned long right) {
    // Track the product
    unsigned long product = 0;
    
    // Loop until the right number is 0
    while(right != 0){
        // If right is odd, add left to the result
        if(right & 1){
            product = BitAdd(product, left);
        }
        
        // Shift left and right bits
        left <<= 1;
        right >>= 1;
    }
    
    // Move the product right 23 bits after performing the multiplication
    product >>= 23;
    
    return product;
}
