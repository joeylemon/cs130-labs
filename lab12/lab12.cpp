// Lab 12
// COSC130
// This program disassembles RISCV instructions
// Joey Lemon
// 11/16/18
// TA: Jacob

#include <cstdio>
#include <cstring>
#include <string>

// Array to hold names of ABI registers
// x0 == ABI_REGS[0], etc
const std::string ABI_REGS[] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

// A mask to find bits 25-20 of an instruction
const int BITS_25_20 = 0x1F00000;

// A mask to find bits 20-15 of an instruction
const int BITS_20_15 = 0xF8000;

// A mask to find bits 12-7 of an instruction
const int BITS_12_7 = 0xF80;

// Global variable to determine if program is using ABI names
bool USE_ABI = false;

// A class to hold an instruction
class Instruction {
    private:
    
    int binary;
    std::string name = "inv";
    std::string format = "invalid";
    std::string arg1, arg2, arg3;
    
    public:
    
    // Default constructor
    Instruction(){}
    
    // Constructor to disassemble an instruction
    Instruction(int a1){
        binary = a1;
        
        // Find op code by masking last 7 bits
        int op = binary & 0x7F;
        
        // Find extended op code
        int extop = (binary & 0x7000) >> 12;
        
        // Find opening 7 bits
        int open = (binary & 0xFE000000) >> 25;
        
        // Disassemble the instruction based on op codes
        if(op == 0b0110111){            // LUI
            // arg1 is bits 12-7
            int a1 = (binary & BITS_12_7) >> 7;
            arg1 = USE_ABI ? ABI_REGS[a1] : "x" + std::to_string(a1);
            
            // arg2 is bits 31-12
            int a2 = (binary & 0xFFFFF000);
            arg2 = std::to_string(a2);
            
            format = "%s, %s";
            
            name = "lui";
        }else if(op == 0b1101111){      // JAL
            // arg1 is bits 12-7
            int a1 = (binary & BITS_12_7) >> 7;
            arg1 = USE_ABI ? ABI_REGS[a1] : "x" + std::to_string(a1);
            
            // arg2 is bits 1-20, but must be put in correct order
            int b19_12 = binary & 0xFF000;      // bits 19-12
            int b11 = binary & 0x100000;        // bit 11
            int b10_1 = binary & 0x7FE00000;    // bits 10-1
            int b20 = binary & 0x80000000;      // bit 20
            
            // Assemble the assorted bits into the correct order
            int a2 = ((b20 | (b19_12 << 11) | (b11 << 2)) | (b10_1 >> 9)) >> 11;
            arg2 = std::to_string(a2);
            
            format = "%s, %s";
            
            name = "jal";
        }else if(op == 0b1100111){      // JALR
            // arg1 is bits 12-7
            int a1 = (binary & BITS_12_7) >> 7;
            arg1 = USE_ABI ? ABI_REGS[a1] : "x" + std::to_string(a1);
            
            // arg2 is bits 20-15
            int a2 = (binary & BITS_20_15) >> 15;
            arg2 = USE_ABI ? ABI_REGS[a2] : "x" + std::to_string(a2);
            
            // arg3 is first 12 bits
            int a3 = binary >> 20;
            arg3 = std::to_string(a3);
            
            format = "%s, %s, %s";
            
            name = "jalr";
        }else if(op == 0b0000011){      // LOAD
            // arg1 is bits 12-7
            int a1 = (binary & BITS_12_7) >> 7;
            arg1 = USE_ABI ? ABI_REGS[a1] : "x" + std::to_string(a1);
            
            // arg3 is bits 20-15
            int a3 = (binary & BITS_20_15) >> 15;
            arg3 = USE_ABI ? ABI_REGS[a3] : "x" + std::to_string(a3);
            
            // arg2 is first 12 bits
            int a2 = binary >> 20;
            arg2 = std::to_string(a2);
            
            format = "%s, %s(%s)";
            
            if(extop == 0b000){
                name = "lb";
            }else if(extop == 0b001){
                name = "lh";
            }else if(extop == 0b010){
                name = "lw";
            }else if(extop == 0b011){
                name = "ld";
            }else if(extop == 0b110){
                name = "lwu";
            }else if(extop == 0b100){
                name = "lbu";
            }else if(extop == 0b101){
                name = "lhu";
            }
        }else if(op == 0b0100011){      // STORE
            // arg1 is bits 25-20
            int a1 = (binary & BITS_25_20) >> 20;
            arg1 = USE_ABI ? ABI_REGS[a1] : "x" + std::to_string(a1);
            
            /// arg3 is bits 20-15
            int a3 = (binary & BITS_20_15) >> 15;
            arg3 = USE_ABI ? ABI_REGS[a3] : "x" + std::to_string(a3);
            
            // arg2 is bits 32-20 and 12-7, but must be put in order
            int a2 = ((binary >> 20) & 0x1F000) | ((binary & BITS_12_7) >> 7);
            arg2 = std::to_string(a2);
            
            format = "%s, %s(%s)";
            
            if(extop == 0b000){
                name = "sb";
            }else if(extop == 0b001){
                name = "sh";
            }else if(extop == 0b010){
                name = "sw";
            }else if(extop == 0b011){
                name = "sd";
            }
        }else if(op == 0b0010011){      // IMMEDIATE
            // arg1 is bits 12-7
            int a1 = (binary & BITS_12_7) >> 7;
            arg1 = USE_ABI ? ABI_REGS[a1] : "x" + std::to_string(a1);
            
            // arg2 is bits 20-15
            int a2 = (binary & BITS_20_15) >> 15;
            arg2 = USE_ABI ? ABI_REGS[a2] : "x" + std::to_string(a2);
            
            // arg3 is first 12 bits
            int a3 = binary >> 20;
            arg3 = std::to_string(a3);
            
            format = "%s, %s, %s";
            
            if(extop == 0b000){
                name = "addi";
            }else if(extop == 0b010){
                name = "slti";
            }else if(extop == 0b011){
                name = "sltiu";
            }else if(extop == 0b100){
                name = "xori";
            }else if(extop == 0b110){
                name = "ori";
            }else if(extop == 0b111){
                name = "andi";
            }else if(extop == 0b001){
                name = "slli";
            }else if(extop == 0b101){
                if(open == 0b0100000){
                    name = "srai";
                    a3 = (binary & BITS_25_20) >> 20;
                    arg3 = std::to_string(a3);
                }else{
                    name = "srli";
                }
            }
        }else if(op == 0b0110011){      // EXECUTE
            // arg1 is bits 12-7
            int a1 = (binary & BITS_12_7) >> 7;
            arg1 = USE_ABI ? ABI_REGS[a1] : "x" + std::to_string(a1);
            
            // arg2 is bits 20-15
            int a2 = (binary & BITS_20_15) >> 15;
            arg2 = USE_ABI ? ABI_REGS[a2] : "x" + std::to_string(a2);
            
            // arg3 is bits 25-20
            int a3 = (binary & BITS_25_20) >> 20;
            arg3 = USE_ABI ? ABI_REGS[a3] : "x" + std::to_string(a3);
            
            format = "%s, %s, %s";
            
            if(extop == 0b000){
                if(open == 0b0100000){
                    name = "sub";
                }else{
                    name = "add";
                }
            }else if(extop == 0b001){
                name = "sll";
            }else if(extop == 0b010){
                name = "slt";
            }else if(extop == 0b011){
                name = "sltu";
            }else if(extop == 0b100){
                name = "xor";
            }else if(extop == 0b101){
                if(open == 0b0100000){
                    name = "sra";
                }else{
                    name = "srl";
                }
            }else if(extop == 0b110){
                name = "or";
            }else if(extop == 0b111){
                name = "and";
            }
        }
    }
    
    // Get the bytes of the instruction
    int getBinary() {
        return binary;
    }
    
    // Get the name of the instruction
    std::string getName() {
        return name;
    }
    
    // Get the format of the instruction
    std::string getFormat() {
        return format;
    }
    
    // Get an argument of the instruction
    std::string getArgument(int i) {
        if(i == 1) {
            return arg1;
        }else if(i == 2){
            return arg2;
        }else{
            return arg3;
        }
    }
    
    // Write the instruction to a file
    void writeToFile(FILE *fout) {
        char instr[25];
        
        // Use sprintf to put the instruction and parameters into a single string
        // Allows us to justify the column width
        sprintf(instr, format.c_str(), arg1.c_str(), arg2.c_str(), arg3.c_str());
        
        // Print the name in a 6-wide field
        // fprintf lets us put formatted text into a file
        fprintf(fout, "     %-6s", name.c_str());
        
        // Print the instruction and parameters in a 25-wide field
        fprintf(fout, "%-25s", instr);
        
        // Print the hexadecimal with zero-padding up to 8 characters
        fprintf(fout, "// 0x%08x\n", binary);
    }
    
    // Print the instruction to console
    void print() {
        char instr[25];
        
        // Use sprintf to put the instruction and parameters into a single string
        // Allows us to justify the column width
        sprintf(instr, format.c_str(), arg1.c_str(), arg2.c_str(), arg3.c_str());
        
        // Print the name in a 6-wide field
        printf("     %-6s", name.c_str());
        
        // Print the instruction and parameters in a 25-wide field
        printf("%-25s", instr);
        
        // Print the hexadecimal with zero-padding up to 8 characters
        printf("// 0x%08x\n", binary);
    }
};

int main(int argc, char *argv[]) {
    // Ensure user supplies correct amount of arguments
    if(argc < 3 || argc > 4){
        printf("Usage: %s <input file> <output file> <register type>\n", argv[0]);
        return 0;
    }
    
    // Get the input file
    char *infile = argv[1];
    
    // Get the output file
    char *outfile = argv[2];
    
    // Check if user specified ABI arguments or x arguments
    if(argv[3] == nullptr || argv[3][0] == 'a'){
        USE_ABI = true;
    }else if(argv[3][0] == 'x'){
        USE_ABI = false;
    }else{
        printf("Invalid register type \'%c\'.\n", argv[3][0]);
        return 0;
    }
    
    // Open the given file
    FILE *fin = fopen(infile, "rb");
    
    // Ensure the file is valid
    if(fin == nullptr){
        printf("Unable to open file \'%s\'.\n", infile);
        return 0;
    }
    
    // Determine the size of the file by seeking to the end and using ftell()
    fseek(fin, 0, SEEK_END);
    int size = ftell(fin);
    // Seek back to the beginning of the file
    fseek(fin, 0, SEEK_SET);
    
    // Get number of instructions by dividing size by 4
    int numInstructions = size / 4;
    
    // Create an dynamic array to hold the instructions
    Instruction *instructions = new Instruction[numInstructions];
    
    // Read all the instructions into the array
    for(int i = 0; ftell(fin) != size; i++){
        int binary;
        fread(&binary, 1, sizeof(int), fin);
        
        Instruction instr(binary);
        instructions[i] = instr;
    }
    
    // Close the file
    fclose(fin);
    
    // Open the output file for writing
    FILE *fout = fopen(outfile, "w");
    
    // Write all instructions to the file
    for(int i = 0; i < numInstructions; i++) {
        instructions[i].writeToFile(fout);
    }
    
    // Close the file
    fclose(fout);
    
    // Print the result to console
    printf("Disassembled %d instructions.\nOutput to file \'%s\' using %s registers.\n", numInstructions, outfile, USE_ABI ? "ABI" : "x");
    
    delete [] instructions;
    return 0;
}
