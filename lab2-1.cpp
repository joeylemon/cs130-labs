// Lab 2
// COSC130
// This program uses a bitset class to store a large
//     number of boolean values
// Joey Lemon
// 9/7/18
// TA: Jacob
#include <vector>
#include <string>

// Converts an integer to a binary string with the given spacing
std::string ToBinary(int val, int spacing);

// Prints out all sets in binary to console
void PrintAllSets();

// The bitset class that holds boolean values
class BITSET {
    // Define the vector in the private namespace
    std::vector<int> sets;
    
    public:
    
    // 0-argument constructor to initialize the vector with 1 set of 0
    BITSET(){
        sets.resize(1, 0);
    }
    
    // Tests the given bit location, returning true if the bit is set
    bool Test(int bit) {
        // Since int rounds down, diving bit by 32 gives the set
        int set = bit / 32;
        
        // Use the correct bit within the set
        // Not necessary, but helps keep track of what is happening
        bit -= (32 * set);
        
        // Left shifting 0b...0001 by "bit" lets you test the bit 
		// at any position
        // If ANDing the set equals 0, then that bit is 0
        // If ANDing the set is anything other than 0, then that bit is 1
        if((sets[set] & (1 << bit)) != 0){
            return true;
        }else{
            return false;
        }
    }
    
    // Sets a bit to 1
    void Set(int bit) {
        int set = bit / 32;
        bit -= (32 * set);
        
        // If set is greater than # of sets, resize vector to new set
        if(set > GetNumSets() - 1){
            sets.resize(set + 1, 0);
        }
        
        // By ORing a left-shifted 1 with the set, the 0 at the bit becomes 
		// a 1 if it wasn't already
        // All other bits are unchanged
        sets[set] = sets[set] | (1 << bit);
    }
    
    // Clears a bit to 0
    void Clear(int bit) {
        int set = bit / 32;
        bit -= (32 * set);
        
        // Same concept as Test(), except we invert the left-shifted 1 so the 
		// given bit becomes a 0 instead of a 1
        // Therefore, ANDing with the newly created 0 will always make that 
		// bit a 0, and leave all the other 1's unchanged
        sets[set] = sets[set] & ~(1 << bit);
        
        // If the set has become a 0, shrink the vector
        if(sets[set] == 0 && set == GetNumSets() - 1){
            // Check if upper sets are 0 as well
            for(int i = set; i > 0; i--){
                if(sets[i] == 0){
                    sets.erase(sets.begin() + i);
                }else{
                    // Stop shrinking once a set is not 0
                    break;
                }
            }
        }
    }
    
	// Get total sets in the bitset
    int GetNumSets() {
        return sets.size();
    }
    
    // Get the set at the index
    int GetSet(int i) {
        return sets[i];
    }
} Bitset;

int main() {
    // Print the commands
    printf ("Commands:\n");
    printf ("   t <bit>     Prints 1 or 0 depending on bit\n");
    printf ("   s <bit>     Sets the given bit to 1\n");
    printf ("   c <bit>     Clears the given bit to 0\n");
    printf ("   g <set>     Prints the binary representation of the given set\n");
    printf ("   v <set>     Prints the decimal representation of the given set\n");
    printf ("   p           Prints all sets\n");
    printf ("   n           Prints the number of sets in the bitset\n");
    printf ("   q           Quits and returns to the console\n\n");
    
    // Listen for command input until user enters 'q'
    char command[100];
    while(command[0] != 'q'){
        printf("Enter a command: ");
        
        // Read in a string until the newline
        // End the program if the user enters Ctrl+D (EOF)
        if(scanf(" %[^\n]s", command) == EOF){
            break;
        }
        
        // Convert to a C++ string
        std::string cmd = command;
        
        // Get the command's action
        char action = cmd[0];
        
        if(action == 'q'){
            // End the program
            break;
        }else if(action == 'n'){
            printf("Number of sets: %d\n", Bitset.GetNumSets());
        }else if(action == 'p'){
            PrintAllSets();
        }else if(cmd.length() > 1){
            try{
                // Convert the given string to an integer
                int num = stoi(cmd.substr(2, cmd.length()));
                
                if(action == 't'){ // Test the given bit
                    // Print 1 or 0 if true or false
                    std::string result = Bitset.Test(num) ? "1" : "0";
                    printf("Bit #%d: %s\n", num, result.c_str());
                }else if(action == 's'){ // Set the given bit
                    Bitset.Set(num);
                    printf("Set bit #%d to 1\n", num);
                }else if(action == 'c'){ // Clear the given bit
                    Bitset.Clear(num);
                    printf("Cleared bit #%d to 0\n", num);
                }else if(action == 'g'){ // Print the set in binary
                    // Ensure the set does not exceed the amount of sets
                    if(num >= Bitset.GetNumSets()){
                        printf("Invalid set\n");
                        continue;
                    }
                    
                    printf("Set #%d [%d...%d] %s\n", num, ((num + 1) * 32) - 1, (32 * num), ToBinary(Bitset.GetSet(num), 4).c_str());
                }else if(action == 'v'){
                    // Ensure the set does not exceed the amount of sets
                    if(num >= Bitset.GetNumSets()){
                        printf("Invalid set\n");
                        continue;
                    }
                    
                    printf("Set #%d value: %d\n", num, Bitset.GetSet(num));
                }else{
                    printf("Invalid command\n");
                }
            }catch (...){
                // If stoi couldn't convert the string, print error
                printf("Invalid command\n");
            }
        }else{
            printf("Invalid command\n");
        }
    }
    
    // Ensure Ctrl+D does not mess with command line format
    printf("\n");
    
    return 0;
}

std::string ToBinary(int val, int spacing) {
    // Initialize an empty string to hold the binary
    std::string binary = "";
    
    int j = 0;
    
    // Loops each bit in the integer
    for(int i = 31; i >= 0; i--){
        // Right-shift the integer to the index
        int k = val >> i;
        
        // Test if the last bit is equal to 1
        // Since 1 is 0b...0001, only the last bit of k is tested
        if((k & 1) == 1){
            binary.append("1");
        }else{
            binary.append("0");
        }
        
        j++;
        // Add a space every x (spacing) bits
        if(j % spacing == 0 && i != 0){
            binary.append(" ");
        }
    }
    return binary;
}

void PrintAllSets() {
    // Loop each set
    for(int i = 0; i < Bitset.GetNumSets(); i++){
        // Print the binary representation of the set at the index
        printf("Set #%d [%d...%d] %s\n", i, ((i + 1) * 32) - 1, (32 * i), ToBinary(Bitset.GetSet(i), 4).c_str());
    }
}
