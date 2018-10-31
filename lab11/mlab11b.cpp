// Mini lab 11
// Part B
// Joey Lemon

#include <cstdio>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if(argc != 2){
        printf("Usage: %s <value>\n", argv[0]);
        return 0;
    }
    
    unsigned long page;
    sscanf(argv[1], "%lx", &page);
    
    printf("Page Table Entry: 0x%016lx\n", page);
    
    printf("Physical Address: 0x%016lx\n", (page & 0x003ffffffffffc00) << 2);
    
    printf("Entry bits:\n");
    printf("Bit %c: %s\n", 'V', (page & 0x01) > 0 ? "SET" : "CLEAR");
    printf("Bit %c: %s\n", 'R', (page & 0x02) > 0 ? "SET" : "CLEAR");
    printf("Bit %c: %s\n", 'W', (page & 0x04) > 0 ? "SET" : "CLEAR");
    printf("Bit %c: %s\n", 'X', (page & 0x08) > 0 ? "SET" : "CLEAR");
    printf("Bit %c: %s\n", 'U', (page & 0x10) > 0 ? "SET" : "CLEAR");
    printf("Bit %c: %s\n", 'G', (page & 0x20) > 0 ? "SET" : "CLEAR");
    printf("Bit %c: %s\n", 'A', (page & 0x40) > 0 ? "SET" : "CLEAR");
    printf("Bit %c: %s\n", 'D', (page & 0x80) > 0 ? "SET" : "CLEAR");
    
    return 0;
}
