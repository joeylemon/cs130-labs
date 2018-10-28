// Lab 9
// COSC130
// This program searches an array for a person or finds the median
// of an integer array
// Joey Lemon
// 11/3/18

#include <cstdio>
#include <cstring>
#include <time.h>
#include <stdlib.h>

struct PERSON {
    char name[51];
    int age;
    char gender;
};

int MedianCPP(const int array[], int array_size, int *buffer);

extern "C" {
    PERSON *Search(PERSON people[], int num_people, const char *name, int age, char gender);
    int Median(const int array[], int array_size, int *buffer);
}

int main() {
    int size = 10;
    int *numbers = new int[size];
    int *buffer = new int[size];
    
    srand(time(NULL));
    for(int i = 0; i < size; i++){
        numbers[i] = rand() % 100;
    }
    
    printf("CPP Median: %d\n", MedianCPP(numbers, size, buffer));
    printf("ASM Median: %d\n", Median(numbers, size, buffer));
    
    delete[] numbers;
    delete[] buffer;
    
    
    int total_people = 5;
    PERSON people[] = {
        {"Joey", 18, 'M'},
        {"Nick", 18, 'M'},
        {"Kaylee", 18, 'F'},
        {"Casey", 20, 'M'},
        {"Rhonda", 54, 'F'}
    };
    
    while(true){
        char name[51];
        int age;
        char gender;
        printf("Enter name, age, and gender: ");
        scanf("%s %d %c", name, &age, &gender);
        
        PERSON *person = Search(people, total_people, name, age, gender);
        if(person != nullptr){
            printf("Found: %s, Age %d, Gender %c\n", person->name, person->age, person->gender);
        }else{
            printf("Cannot find person\n");
        }
    }
}

int MedianCPP(const int array[], int array_size, int *buffer) {
    // Fill buffer with elements
    for(int i = 0; i < array_size; i++){
        buffer[i] = array[i];
    }
    
    // Sort array
    int index = 0;
    while (index < array_size) {
        if (index == 0){
            index++;
        }
        
        if (buffer[index] >= buffer[index - 1]){
            index++;
        }else{
            int temp = buffer[index];
            buffer[index] = buffer[index - 1];
            buffer[index - 1] = temp;
            index--;
        }
    }
    
    // Print unsorted array
    printf("Unsorted: ");
    for(int k = 0; k < array_size; k++) {
        printf("%d ", array[k]);
    }
    printf("\n");
    
    // Print sorted array
    printf("Sorted: ");
    for(int k = 0; k < array_size; k++) {
        printf("%d ", buffer[k]);
    }
    printf("\n");
    
    // If array is odd, median is middle element
    if (array_size % 2 != 0){
        return buffer[array_size / 2];
    }
    
    // If array is even, median is middle elements divided by 2
    return (buffer[(array_size - 1) / 2] + buffer[array_size / 2]) / 2;
    
    return 0;
}
