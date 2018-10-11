// Lab 5
// COSC130
// This program decrypts a file using given keys
// Joey Lemon
// 9/28/18
// TA: Jacob
#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>

int main(int argc, char *argv[]) {
    // Ensure user supplies correct amount of arguments
    if(argc != 4){
        printf("Usage: %s <file> <dkey> <nkey>\n", argv[0]);
        return 0;
    }
    
    // Ensure the dkey is only one character
    if(strlen(argv[2]) > 1){
        printf("dkey is too long.\n");
        return 0;
    }
    char dkey = argv[2][0];
    
    // Convert the nkey to an integer
    int nkey;
    sscanf(argv[3], "%d", &nkey);
    
    // Open the given file
    FILE *fin = fopen(argv[1], "rb");
    
    // Ensure the file is valid
    if(fin == nullptr){
        printf("Unable to open that file.\n");
        return 0;
    }
    
    // Determine the size of the file by seeking to the end and using ftell()
    fseek(fin, 0, SEEK_END);
    int size = ftell(fin);
    // Seek back to the beginning of the file
    fseek(fin, 0, SEEK_SET);
    
    // Get max amount of chunks by dividing size by 8
    int maxChunk = size / 8;
    
    // Create new char array with size of max chunks
    char* data = new char[maxChunk];
    
    // Read the file until we reach the last byte
    for(int i = 0; ftell(fin) != size; i++){
        char dataChar;
        char decryptChar;
        int dataIndex;
        int decryptIndex;
        
        // Read the data byte
        fread(&dataChar, 1, sizeof(dataChar), fin);
        
        // Move 3 bytes forward
        fseek(fin, 3, SEEK_CUR);
        
        // Read the data index
        fread(&dataIndex, 1, sizeof(dataIndex), fin);
        
        // Decrypt the data by XORing with the keys
        decryptChar = dataChar ^ dkey;
        decryptIndex = dataIndex ^ nkey;
        
        // Ensure the index isn't out of bounds
        if(decryptIndex < 0 || decryptIndex >= maxChunk){
            printf("Error decoding chunk #%d\nGot index %d, but max chunks is %d\n", i, decryptIndex, maxChunk);
            return 0;
        }
        
        // Add the char to the array at the index
        data[decryptIndex] = decryptChar;
    }
    
    // If the decrypted string already has a newline, don't include one while printing
    if(data[strlen(data) - 1] == '\n'){
        printf("%s", data);
    }else{
        printf("%s\n", data);
    }
    
    // Close the file
    fclose(fin);
    
    return 0;
}
