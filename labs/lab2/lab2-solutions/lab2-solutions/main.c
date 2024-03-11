#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
 Note my use of assertions. This is an easy way of debugging during development.
 Proper error handling should be implemented for any 'final' product. I just
 wanted to show something different for the sake of an example.
 */

void checkpoint2(char* filename)
{
    // open file
    FILE* file = fopen(filename, "r");
    assert(file != NULL);
    
    // get size of file
    fseek(file, 0L, SEEK_END);
    int file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);
    assert(file_size > 0);
    
    // allocate appropriate amount of space for parsing the file as longs
    int num_data_points = file_size / sizeof(long);
    long* values = calloc(num_data_points, sizeof(long));
    assert(values != NULL);
    
    // read in file
    int values_read = fread(values, sizeof(long), num_data_points, file);
    assert(values_read == num_data_points);
    
    // output parsed contents
    for (int i = 0 ; i < num_data_points; ++i) {
        printf("Data point #%2d: %ld\n", i, values[i]);
    }
    
    // close file
    fclose(file);
}

void checkpoint3(char* filename)
{
    // open file
    FILE* file = fopen(filename, "r");
    assert(file != NULL);
    
    // get size of file
    fseek(file, 0L, SEEK_END);
    int file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);
    assert(file_size > 0);
    
    // allocate appropriate amount of space for parsing the file as unsigned ints
    int num_data_points = file_size / sizeof(unsigned int);
    unsigned int* values = calloc(num_data_points, sizeof(unsigned int));
    assert(values != NULL);
    
    // read in file
    int values_read = fread(values, sizeof(unsigned int), num_data_points, file);
    assert(values_read == num_data_points);
    
    // output parsed contents
    for (int i = 0 ; i < num_data_points; ++i) {
        printf("Data point #%2d: %u\n", i, values[i]);
    }
    
    // close file
    fclose(file);
}

int main()
{
    // /Users/adityabehal/Documents/RPI/Sophomore/Fall/ComputerOrganization/labs/lab2/lab2-solutions/lab2-solutions/lab02-data.little.dat
    char filename[256];
    scanf("%s", filename);
    
    checkpoint2(filename);
    checkpoint3(filename);
    
    return 0;
}
