// Mini lab 11
// Part A
// Joey Lemon

#include <cstdio>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if(argc != 2){
        printf("Usage: %s <value>\n", argv[0]);
        return 0;
    }
    
    unsigned long hex;
    sscanf(argv[1], "%lx", &hex);
    
    printf("Address: 0x%lx\n", hex);
    
    hex &= 0x7FFFFFFFFF;
    
    printf("VPN[2] = %lu\n", hex>>30);
    printf("VPN[1] = %lu\n", (hex>>21) & 0x1FF);
    printf("VPN[0] = %lu\n", (hex>>12) & 0x1FF);
    
    unsigned long poff = hex & 0xFFF;
    printf("POFF = %lx (%lu)\n", poff, poff);
    
    return 0;
}
