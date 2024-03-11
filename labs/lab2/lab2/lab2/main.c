//
//  main.c
//  lab2
//
//  Created by Aditya Behal on 9/15/21.
//

#include <stdio.h>
#include <stdlib.h>


void checkpoint2(char* filename) {
    /*
     Assume that the data file contains an array of long variables. Write a C program to read this data file entirely into memory, then display the data using a loop. You’ll need to open the file, determine its size in bytes, allocate an array, and then read the binary data into the array. You can easily accomplish this using the following functions:
     calloc(), fopen(), fseek(), ftell(), fread(), and fclose().
     Note: fread() reads in binary data directly to some memory address. fseek() and ftell()
     can get you the size of the file.
     Finally, display the data as follows (hint: “%2d” might be useful):
     */
    
    // open file
    FILE* my_file = fopen(filename, "rb");
    
    // move pointer to end of file using fseek
    fseek(my_file, 0, SEEK_END);
    // get size of file using ftell
    long int fileSize = ftell(my_file);
    // move pointer back to beginning of file using fseek
    fseek(my_file, 0, SEEK_SET);
    
    // allocate appropriate amount of space for parsing the file as longs
    unsigned int numElements = fileSize/sizeof(long int);
    long int* buffer = (long int*)calloc(numElements, sizeof(long int));
    
    // read in file
    // reading in values from file to array buffer (reading sizeof(long) bytes per time for numElements iterations)
    fread(buffer, sizeof(long int), numElements, my_file);
    
    // output parsed contents
    for (int i = 0; i < numElements; i++) {
        printf("Data point #%2d: %ld\n", i, buffer[i]);
    }
    
    
    // close file
    fclose(my_file);
    free(buffer);
}

void checkpoint3(char* filename) {
    // open file
    FILE* my_file = fopen(filename, "rb");`
    
    // move pointer to end of file using fseek
    fseek(my_file, 0, SEEK_END);
    // get size of file using ftell
    long int fileSize = ftell(my_file);
    // move pointer back to beginning of file using fseek
    fseek(my_file, 0, SEEK_SET);
    
    // allocate appropriate amount of space for parsing the file as longs
    unsigned int numElements = fileSize/sizeof(unsigned int);
    unsigned int* buffer = (unsigned int*)calloc(numElements, sizeof(unsigned int));
    
    // read in file
    // reading in values from file to array buffer (reading sizeof(long) bytes per time for numElements iterations)
    fread(buffer, sizeof(unsigned int), numElements, my_file);
    
    // output parsed contents
    // sequence of prime numbers printed out when reading in as unsigned integers and useful for encryption
    for (int i = 0; i < numElements; i++) {
        printf("Data point #%2d: %u\n", i, buffer[i]);
    }
    
    
    // close file
    fclose(my_file);
    free(buffer);
}


int main(int argc, const char * argv[]) {
    /* Insert your eight bytes of ASCII for your secret message */
    /* The 0x prefix below indicates a hexadecimal number */
    // decodes to CompOrg!
    // long int z = 0x2167724F706D6F43;
    // decodes to adityab! but outputted as !baytida so my MacOS is little endian
    long int z = 0x6164697479616221;
    char* c;
    
    int i;
    // purpose of commas is to initialize and increment two variables in for loop
    // output does not change at all if c++ is replaced with ++c or ++i is replaced with i++
    // sizeof z tells how many bytes z occupies (parantheses are required only when you are getting sizeof type but are not required for expressions)
    // e.g. sizeof(long int) - parentheses required here
    // e.g. sizeof z vs. sizeof(z) - parantheses optional here (both work)
    // we need use to sizeof to determine how long the for loop needs to run to step through each of the hexadecimal value pairs to decode the entire hexadecimal address to characters
    for (c = (char*)&z, i = 0; i < sizeof z; c++, ++i) {
        printf("%c", *c);
    }
    printf("\n");
    
    char filename[256];
    // /Users/adityabehal/Documents/RPI/Sophomore/Fall/ComputerOrganization/labs/lab2/lab2/lab2/lab02-data.little.dat
    scanf("%s", filename);
    
    checkpoint2(filename);
    checkpoint3(filename);
    
    return EXIT_SUCCESS;
}
