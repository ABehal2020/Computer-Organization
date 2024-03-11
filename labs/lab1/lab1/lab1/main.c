//
//  main.c
//  lab1
//
//  Created by Aditya Behal on 8/31/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void triangle_nested(int* n) {
    for (int i = *n - 1; i >= 0; i--) {
        int j = 2*i + 1;
        for (int k = j - 1; k >= 0; k--) {
            printf("*");
        }
        printf("\n");
    }
}

void triangle_single_loop(int* n) {
    char star_array[1000] = "*";
    // char star_add[] = "**";
    int size = 2 * (*n - 1) + 1;
    memset(star_array, '*', size);
    for (int i = 0; i < *n; i++) {
        printf("%s", star_array);
        printf("\n");
        if (i == *n - 1) {
            break;
        }
        char temp[1000];
        size -= 2;
        strncpy(temp, star_array, size);
        temp[size] = '\0';
        strcpy(star_array, temp);
    }
}

void hypotenuse(int* n) {
    int size = 2 * (*n - 1) + 1;
    float hypotenuse = sqrt(pow((*n), 2) + pow(size, 2));
    printf("Length of hypotenuse: %.2f\n", hypotenuse);
}

// due to the branching structure
long fibRecursive(int n) {
    if (n == 0) {
        return 0;
    } else if (n == 1) {
        return 1;
    } else {
        return fibRecursive(n - 1) + fibRecursive(n - 2);
    }
}

long fibIterative(int n) {
    int fibSeq[1000];
    fibSeq[0] = 0;
    fibSeq[1] = 1;
    for (int i = 2; i <= n; i++) {
        fibSeq[i] = fibSeq[i - 1] + fibSeq[i - 2];
    }
    return fibSeq[n];
}

int main(int argc, const char * argv[]) {
    int n;
    printf("What is n? ");
    scanf("%d", &n);
    triangle_nested(&n);
    triangle_single_loop(&n);
    hypotenuse(&n);
    printf("What is n? ");
    scanf("%d", &n);
    printf("Fibonacci Recursive: %ld\n", fibRecursive(n));
    printf("Fibonacci Iterative: %ld\n", fibIterative(n));
    return EXIT_SUCCESS;
}
