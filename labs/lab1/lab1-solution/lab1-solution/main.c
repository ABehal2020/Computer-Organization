//
//  main.c
//  lab1-solution
//
//  Created by Aditya Behal on 9/8/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

double timer() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double) (tp.tv_sec) + 1e-6 * tp.tv_usec);
}


// Output the right triangle using nested loops
int output_triangle_nested_loops(int n) {
    int i, j;
    for (i = n - 1; i >= 0; --i) {
        int num_stars = i * 2 + 1;
        for (j = 0; j < num_stars; ++j) {
            printf("*");
        }
        printf("\n");
    }
    return 0;
}


// Output the right triangle using a single for loop with memset
int output_triangle_one_loop_memset(int n) {
    int i;
    char* output;
    output = (char*)malloc(n * 2);
    
    for (i = n - 1; i >= 0; --i) {
        int num_stars = i * 2 + 1;
        memset(output, '*', num_stars);
        output[num_stars] = '\0';
        printf("%s\n", output);
    }
    
    free(output);
    
    return 0;
}


// Use only one loop, but use a pointer instead of memset
int output_triangle_one_loop_ptr(int n) {
    int i;
    char* output;
    char* out_ptr;
    output = (char*)malloc(n * 2 + 2);
    out_ptr = output;
    *out_ptr++ = '*';
    *out_ptr = '\0';
    
    for (i = 0; i < 2 * n; ++i) {
        if (i < n) {
            *out_ptr++ = '*';
            *out_ptr++ = '*';
            *out_ptr = '\0';
        }
        else {
            *out_ptr--;
            *--out_ptr = '\0';
            printf("%s\n", output);
        }
    }
    
    free(output);
    
    return 0;
}

// Output the right triangle using a single while loop
int output_triangle_one_while_loop(int n) {
    int r = 0, c = 0;
    while (r < n) {
        printf("*");
        c++;
        if (c >= (n - r) * 2 - 1) {
            c = 0;
            r++;
            printf("\n");
        }
    }
    return 0;
}

// Output the right triangle using a single for loop
int output_triangle_one_for_loop(int n) {
    for (int r = n - 1, c = n * n; c >= 1; --c) {
        printf("*");
        if (c == r * r + 1) {
            r--;
            printf("\n");
        }
    }
    return 0;
}

// Calculate the length of the hypotenuse for the output triangles
// Note: a^2 = b^2 + c^2
float calculate_hypotenuse(int n) {
    float a, b, c;
    a = (float)n;
    b = (float)((n - 1) * 2 + 1);
    c = sqrt(a * a + b * b);
    
    return c;
}

long num = 0;
// Calculate Fibonacci number via recursion
long calc_fib_recursion(long n) {
    ++num;
    if (n < 2)
        return n;
    else
        return calc_fib_recursion(n-1) + calc_fib_recursion(n-2);
}


long num_fast = 0;

// Calculate Fibonacci number via faster recursion
// i.e. tail recursion
long calc_fib_recursion_faster(long n, int n_1, int n_2) {
    ++num_fast;
    
    if (n < 1)
        return n_2;
    else
        return calc_fib_recursion_faster(n - 1, n_1 + n_2, n_1);
}


// Calculate Fibonacci number via iteration
// Note similarity between tail recursion and iterative methods
// For fun:
// - compile in gcc with ~$ gcc lab01-soln.c -lm -O3 -fdump-tree-optimized
// - the '-O3' flag maximizes the optimization options for the compiler
//   (gcc lets us set optimization from '-O0' to '-O3')
// - '-fdump-tree-optimized' outputs optimized intermediate code used by
//   the compiler
// - compare the *.optimized function listings for
//   calc_fib_recursion_faster() and calc_fib_iteration()
// - Recompile using '-O0' instead of '-O3' and look again - what changed?
// Even more fun:
// - Try and figure out how calc_fib_recursion() has been optimized
// Check out https://gcc.gnu.org/onlinedocs/gccint/ for info on gcc internals
long calc_fib_iteration(long n) {
    long i;
    long n_1,n_2,fib;
    
    if (n < 2)
        return n;
    
    for (i = 2, n_2 = 0, n_1 = 1; i <= n; ++i) {
        fib = n_1 + n_2;
        n_2 = n_1;
        n_1 = fib;
    }
    
    return fib;
}


int main(int argc, const char* argv[]) {
    setbuf(stdout, NULL);
    int n;
    long f;
    double elt;
    
    printf("Enter an integer for triangle output: ");
    scanf( "%d", &n);
    
    printf("Nested loops:\n");
    output_triangle_nested_loops(n);
    
    printf("Single loop (using memset):\n");
    output_triangle_one_loop_memset(n);
    
    printf("Single loop (using pointer):\n");
    output_triangle_one_loop_ptr(n);
    
    printf("Single while loop:\n");
    output_triangle_one_while_loop(n);
    
    printf("Single for loop:\n");
    output_triangle_one_for_loop(n);
    
    printf("Length of hypotenuse: %.2f\n", calculate_hypotenuse(n));
    
    printf("Enter an integer for Fibonacci output: ");
    scanf("%li", &f);
    
    elt = timer();
    printf("Fib number (recursion): %li - %li\n", f, calc_fib_recursion(f));
    printf("Time: %10.10lf (s)\n", timer() - elt);
    
    elt = timer();
    printf("Fib number (tail recursion): %li - %li\n", f,
           calc_fib_recursion_faster(f, 1, 0));
    printf("Time: %10.10lf (s)\n", timer() - elt);
    
    elt = timer();
    printf("Fib number (iteration): %li - %li\n", f, calc_fib_iteration(f));
    printf("Time: %10.10lf (s)\n", timer() - elt);
    
    printf("%li calls\n", num);
    printf("%li calls fast\n", num_fast);
    
    return EXIT_SUCCESS;
}
