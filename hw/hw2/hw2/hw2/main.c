//
//  main.c
//  hw2
//
//  Created by Aditya Behal on 9/30/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, const char * argv[]) {
    // variable definitions
    char title[256];
    char colOneHeader[256];
    char colTwoHeader[256];
    char string[256];
    char tokenSecond[256];
    char** strings;
    int value;
    int* values;
    int currentIndex = 0;
    int currentSize = 1;
    // dynamic allocations
    strings = (char **)malloc(currentSize*sizeof(char *));
    values = (int *)malloc(currentSize*sizeof(int));
    // reading in user input
    printf("Enter a title for the data:\n");
    fgets(title, 256, stdin);
    title[strlen(title) - 1] = '\0';
    printf("You entered: %s\n", title);
    printf("\nEnter the column 1 header:\n");
    fgets(colOneHeader, 256, stdin);
    colOneHeader[strlen(colOneHeader) - 1] = '\0';
    printf("You entered: %s\n", colOneHeader);
    printf("\nEnter the column 2 header:\n");
    fgets(colTwoHeader, 256, stdin);
    colTwoHeader[strlen(colTwoHeader) - 1] = '\0';
    printf("You entered: %s\n", colTwoHeader);
    printf("\nEnter a data point (-1 to stop input):\n");
    // processing user input and checking for errors
    while (1) {
        fgets(string, 256, stdin);
        if (atoi(string) == -1) {
            break;
        }
        if (strchr(string,',') == NULL) {
            printf("Error: No comma in string.\n");
            printf("\nEnter a data point (-1 to stop input):\n");
        } else if (strchr(string,',') != strrchr(string,',')) {
            printf("Error: Too many commas in input.\n");
            printf("\nEnter a data point (-1 to stop input):\n");
        } else {
            char* token = strtok(string, ",");
            char* dataString = token;
            token = strtok(NULL, ",");
            for (unsigned int i = 1; i < strlen(token); i++) {
                tokenSecond[i - 1] = token[i];
            }
            tokenSecond[strlen(token)] = '\0';
            // checking for integer error
            if (isdigit(tokenSecond[0]) != 0) {
                if (currentIndex >= currentSize) {
                    currentSize *= 2;
                    strings = realloc(strings, currentSize*sizeof(char *));
                    values = realloc(values, currentSize*sizeof(int));
                }
                strings[currentIndex] = malloc((strlen(dataString)+1)*sizeof(char));
                strcpy(strings[currentIndex], dataString);
                strings[currentIndex][strlen(dataString)] = '\0';
                value = atoi(tokenSecond);
                values[currentIndex] = value;
                currentIndex++;
                printf("Data string: %s\n", string);
                printf("Data integer: %d\n", value);
                printf("\nEnter a data point (-1 to stop input):\n");
            } else {
                printf("Error: Comma not followed by an integer.\n");
                printf("\nEnter a data point (-1 to stop input):\n");
            }
        }
    }
    printf("\n%33s\n", title);
    printf("%-20s", colOneHeader);
    printf("|%23s\n", colTwoHeader);
    // formatting output table
    for (unsigned int i = 0; i < 44; i++) {
        printf("-");
    }
    printf("\n");
    for (unsigned int i = 0; i < currentIndex; i++) {
        printf("%-20s", strings[i]);
        printf("|%23d\n", values[i]);
    }
    printf("\n");
    for (unsigned int i = 0; i < currentIndex; i++) {
        printf("%20s ", strings[i]);
        for (unsigned int j = 0; j < values[i]; j++) {
            printf("*");
        }
        printf("\n");
    }
    for (unsigned int i = 0; i < currentIndex; i++) {
        free(strings[i]);
    }
    // freeing up dynamically allocated memory
    free(strings);
    free(values);
    return EXIT_SUCCESS;
}

