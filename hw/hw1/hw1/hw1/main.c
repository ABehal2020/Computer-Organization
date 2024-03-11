//
//  main.c
//  hw1
//
//  Created by Aditya Behal on 9/11/21.
//

/*
 Assignment 1: NxM Matrix Multiplication
 CSCI-2500 Fall 2021
 */

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

/******************************************************************************/
/* Function Calls */
/******************************************************************************/

// defining struct to contain matrix
typedef struct {
    double** data;
    int rows;
    int cols;
} matrix;

// function prototypes
int mm_alloc(matrix* mat);
int mm_free(matrix* mat);
int mm_print(matrix* mat);
int mm_read(char* filename, matrix* mat);
matrix* mm_matrix_mult(matrix* mat1, matrix* mat2);

// allocating matrices dynamically
int mm_alloc(matrix* mat) {
    /*
     Notes:
     Allocate an array of pointers to a double
     Then, allocate an row/array of doubles and assign each double pointer that row
     Make sure you're allocating using mat->data in the matrix struct
     You can use calloc for allocation and zeroing out all the bytes
     - otherwise, you'd want to manually set all arrays to 0 (good practice)
     If your malloc/calloc returns null, the allocation failed
     */
    // error checking for invalid dimensions
    if ((mat->rows < 0) || (mat->cols < 0)) {
        printf("mm_alloc: allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    // allocating memory to hold matrix and zeroing out the 2D array of doubles
    mat->data = (double **)malloc(mat->rows * sizeof(double *));
    for (int i = 0; i < mat->rows; i++) {
        mat->data[i] = (double *)calloc(mat->cols, sizeof(double));
    }
    // allocation fails (e.g. due to insufficient matrix values supplied)
    if (mat->data == NULL) {
        printf("mm_alloc: allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}

// function to free up dynamically allocated matrices
int mm_free(matrix* mat) {
    /*
     Notes:
     Reverse the allocation process, but you'll be calling free() instead
     */
    // free inner array first
    for (int i = 0; i < mat->rows; i++) {
        free(mat->data[i]);
    }
    // free outer array afterward
    free(mat->data);
    return 0;
}

// printing matrices
int mm_print(matrix* mat) {
    /*
     Notes:
     Print all the data values in your matrix
     Go row by row
     At the end of each row, print a newline
     Between each data value, insert a tab
     Use 2 digits of precision after the decimal (use "%10.2lf\t" as format string)
     See output.txt for expected output formatting
     */
    printf("\n/******** START of 2-D Matrix **********************************/\n");
    // looping over 2D array of doubles and printing it
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%10.2lf\t", mat->data[i][j]);
        }
        printf("\n");
    }
    printf("/******** END of 2-D Matrix ************************************/\n");
    return 0;
}

// initializing matrix values with data from file being read in
int mm_read(char* filename, matrix* mat) {
    FILE* my_file = NULL;
    char buffer[256];
    int numMatrixEntriesInitialized = 0;
    int numMatrixEntriesAbsolute = 0;
    int i = 0;
    int j = 0;
    // checking for ability to open file for reading
    if (NULL == (my_file = fopen(filename, "r"))) {
        printf("mm_read: failed to open file.\n");
        exit(EXIT_FAILURE);
    }
    // checking if file is empty
    if (fgets(buffer, 256, my_file) == NULL) {
        printf("mm_read: failed to read from file.\n");
        exit(EXIT_FAILURE);
    } else {
        // checking if two integers are supplied for the matrix dimensions
        if (2 != sscanf(buffer, "%d %d", &mat->rows, &mat->cols)) {
            printf("mm_read: failed to read matrix dimensions.\n");
            exit(EXIT_FAILURE);
        }
        // allocating matrix dynamically
        mm_alloc(mat);
        numMatrixEntriesAbsolute = mat->rows * mat->cols;
        // reading all values in the file into the matrix 2D array of doubles
        while (fgets(buffer, 256, my_file) != NULL) {
            sscanf(buffer, "%lf", &mat->data[i][j]);
            numMatrixEntriesInitialized++;
            if (j == mat->cols - 1) {
                i++;
                j = 0;
            } else {
                j++;
            }
        }
    }
    // checking if correct number of entries supplied to initialize matrix
    if (numMatrixEntriesInitialized != numMatrixEntriesAbsolute) {
        printf("mm_read: failed to read matrix values.\n");
        exit(EXIT_FAILURE);
    }
    // closing file since we are finished reading it
    fclose(my_file);
    return 0;
}

// function to multiply two matrices together
matrix* mm_matrix_mult(matrix* mat1, matrix* mat2) {
    /*
     Notes:
     First check that it is actually possible to multiply the two inputs
     If so, construct a result matrix
     Then perform the multiplication
     Then return a pointer to your result matrix
     */
    double answer = 0.0;
    int length = mat1->cols;
    matrix* result_matrix = (matrix*)malloc(sizeof(matrix));
    // checking if two matrices can be multiplied or not
    if (mat1->cols == mat2->rows) {
        result_matrix->rows = mat1->rows;
        result_matrix->cols = mat2->cols;
        // dynamically allocating result_matrix
        mm_alloc(result_matrix);
        // looping over entries in result_matrix and initializing them
        for (int i = 0; i < result_matrix->rows; i++) {
            for (int j = 0; j < result_matrix->cols; j++) {
                /* multiplying individual pairs of entries in rows and columns
                   and subsequently computing the sum of those pairs */
                for (int k = 0; k < length; k++) {
                    answer += (mat1->data[i][k])*(mat2->data[k][j]);
                }
                result_matrix->data[i][j] = answer;
                answer = 0.0;
            }
        }
    } else {
        printf("mm_matrix_mult: dimension mismatch between matrices.\n");
        exit(EXIT_FAILURE);
    }
    return result_matrix;
}

int main() {
    /*
     * you must keep this function AS IS.
     * you cannot modify it!
     */
    char filename_A[256];
    char filename_B[256];
    matrix* A = (matrix*)malloc(sizeof(matrix));
    matrix* B = (matrix*)malloc(sizeof(matrix));
    matrix* C = NULL;
    
    printf("Please enter the matrix A data file name: ");
    if (1 != scanf("%s", filename_A)) {
        printf("main: scanf failed to read filename\n");
        exit(-1);
    }
    mm_read(filename_A, A);
    mm_print(A);
    
    printf("Please enter the matrix B data file name: ");
    if (1 != scanf("%s", filename_B)) {
        printf("main: scanf failed to read filename\n");
        exit(-1);
    }
    mm_read(filename_B, B);
    mm_print(B);
    
    C = mm_matrix_mult(A, B);
    mm_print(C);
    
    mm_free(A);
    mm_free(B);
    mm_free(C);
    free(A);
    free(B);
    free(C);
    
    return EXIT_SUCCESS;
}
