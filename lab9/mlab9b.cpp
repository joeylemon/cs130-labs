/*
 
*/

#include <cstdio>

struct MyStruct {
    int result;
    int *opr_left;
    int *opr_right;
};

void Calculate(MyStruct &s, char op);

extern "C" {
    void CalculateASM(MyStruct &s, char op);
}

int main() {
    int left;
    int right;
    char op;
    printf("Enter left <op> right operands: ");
    scanf("%d %c %d", &left, &op, &right);
    
    MyStruct mystruct;
    mystruct.opr_left = &left;
    mystruct.opr_right = &right;
    
    Calculate(mystruct, op);
    printf("C++: %d %c %d = %d\n", *mystruct.opr_left, op, *mystruct.opr_right, mystruct.result);
    
    CalculateASM(mystruct, op);
    printf("ASM: %d %c %d = %d\n", *mystruct.opr_left, op, *mystruct.opr_right, mystruct.result);
}

void Calculate(MyStruct &s, char op) {
    if(op == '+'){
        s.result = *s.opr_left + *s.opr_right;
    }else if(op == '-'){
        s.result = *s.opr_left - *s.opr_right;
    }else{
        s.result = -1;
    }
}
