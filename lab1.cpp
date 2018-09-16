// Lab 1A
// COSC130
// This program performs math functions on two variables
//    given by the user.
// Joey Lemon
// 8/24/18
// TA: Jacob
#include <iostream>
#include <string>
using namespace std;

// Converts a char to an int
int  CharToInt(char v);

// Converts an int to a char
char IntToChar(int v);

// Converts a string to an int
int StringToInt(string val);

// Converts an int to a string
string IntToString(int val);

int main(int argc, char *argv[])
{
        string sresult;
        int left;
        int right;
        char op;

        if (4 != argc) {
                printf("Usage: %s <left> <op> <right>\n", argv[0]);
                return -1;
        }
        //Notice that this calls your StringToInt. So, make sure you debug
        //StringToInt() to make sure that left and right get a proper
        //value.
        left = StringToInt(argv[1]);
        right = StringToInt(argv[3]);
        op = argv[2][0];
        //Calculate based on the op. Notice that this calls IntToString,
        //so debug your IntToString() function to make sure that sresult
        //is given the proper result. This assumes your StringToInt already
        //works.
        switch (op)
        {
                case 'x':
                        sresult = IntToString(left * right);
                        break;
                case '/':
                        if(right == 0){
                            sresult = "error";
                            break;
                        }
                        sresult = IntToString(left / right);
                        break;
                case '+':
                        sresult = IntToString(left + right);
                        break;
                case '-':
                        sresult = IntToString(left - right);
                        break;
                case '%':
                        if(right == 0){
                            sresult = "error";
                            break;
                        }
                        sresult = IntToString(left % right);
                        break;
                default:
                        sresult = IntToString(left);
                        break;
        }

        //Remember, printf is the only output function you may use for this lab!
        //The format string is %d %c %d = %s. This means that the first argument
        //is %d (decimal / integer), %c (character), %d (decimal /integer),
        //%s (string). Notice that because printf() is a C-style function, you
        //must pass strings as character arrays. We can convert a C++ string
        //to a character array (C-style string) by using the c_str() member function.
        printf("%d %c %d = %s\n", left, op, right, sresult.c_str());
        return 0;
}

int CharToInt(char v) {
    int char_code = (int) v;
    // 48-57 are the ASCII values for 0-9
    if(char_code >= 48 && char_code <= 57){
        // Subtract 48 from the code to get the actual integer value
        return char_code - 48;
    }else{
        // Return 0 if char isn't a digit
        return 0;
    }
}

char IntToChar(int v) {
    // Ensure int is between 0 and 9
    if(v >= 0 && v <= 9){
        // Add 48 to the char code to get char value
        char x = v + 48;
        return x;
    }else{
        // Return 0 if int isn't between 0 and 9
        return '0';
    }
}

int StringToInt(string val) {
    try{
        int total = 0;
        // Loop through the string
        // i must be unsigned to be compared to string length
        for(int i = 0; (unsigned)i < val.length(); i++){
            int v = CharToInt(val.at(i));
            // Add each integer, moving total to the next ten's place each loop
            total = (total * 10) + v;
        }
        return total;
    }catch (...){
        // Return 0 if the string couldn't be converted
        return 0;
    }
}

string IntToString(int val) {
    try{
        bool negative = false;
        
        if(val == 0){
            // Go ahead and return 0 if value is 0 to avoid errors
            return "0";
        }else if(val < 0){
            // Keep track of negative numbers but do math with its absolute value
            val *= -1;
            negative = true;
        }
        
        // Initialize an empty string
        string initial = "";
        while(val != 0) {
            // Add the last digit of the value to the empty string
            initial += IntToChar(val % 10);
            
            // "Remove" the last digit of the value by dividing by 10
            val = val/10;
        }
        
        // Reverse the string
        string complete = "";
        for(int i = initial.length() - 1; i >= 0; i--){
            complete += initial.at(i);
        }
        
        // Set the end value to negative if it started negative
        if(negative){
            complete = "-" + complete;
        }
        return complete;
    }catch (...){
        // Return "error" if the int couldn't be converted
        return "error";
    }
}
