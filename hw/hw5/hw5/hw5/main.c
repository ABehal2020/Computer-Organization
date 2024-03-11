//
//  main.c
//  hw5
//
//  Created by Aditya Behal on 11/12/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

// find log base 2 of a number
int pow2(int n) {
    int ans = log2(n);
    if (ans == floor(ans)) {
        return ans;
    } else {
        return 0;
    }
}

// initialize register
void initialize(char** expression, int regNum) {
    printf("li $s%d,%s\n", regNum, expression[2]);
}

// check if an argument is an integer constant
bool isNumber(char* arg) {
    if (arg[0] == '-') {
        return isdigit(arg[1]);
    } else {
        return isdigit(arg[0]);
    }
}

// addition
void addition(char* arg1, char* arg2, char* arg3) {
    if (isNumber(arg2) || isNumber(arg3)) {
        printf("addi %s,%s,%s\n", arg1, arg2, arg3);
    } else {
        printf("add %s,%s,%s\n", arg1, arg2, arg3);
    }
}

// subtraction
void subtraction(char* arg1, char* arg2, char* arg3) {
    printf("sub %s,%s,%s\n", arg1, arg2, arg3);
}

// sub-cases for multiplication
void times0(char* arg1, char* arg2, char* arg3, int* tTracker, char** tempRegs, int* val) {
    printf("li %s,%d\n", arg1, 0);
}

void times1(char* arg1, char* arg2, char* arg3, int* tTracker, char** tempRegs, int* val) {
    printf("move %s,%s\n", tempRegs[*tTracker], arg2);
    printf("move %s,%s\n", arg1, tempRegs[*tTracker]);
    if (*tTracker <= 9) {
        (*tTracker)++;
    } else {
        *val = 1;
        *tTracker = 0;
    }
}

void timesNeg1(char* arg1, char* arg2, char* arg3, int* tTracker, char** tempRegs, int* val) {
    printf("move %s,%s\n", tempRegs[*tTracker], arg2);
    printf("sub %s,$zero,%s\n", arg1, tempRegs[*tTracker]);
    if (*tTracker <= 9) {
        (*tTracker)++;
    } else {
        *val = 1;
        *tTracker = 0;
    }
}

void break2(char* arg1, char* arg2, char* arg3, int* tTracker, char** tempRegs, int* val) {
    // break into powers of 2
    int temp = atoi(arg3);
    temp = abs(temp);
    int total = 0;
    int bin[30];
    int check = 0;
    while(temp != 0) {
        if (temp % 2 == 1) {
            bin[total] = 1;
        } else {
            bin[total] = 0;
        }
        temp = temp / 2;
        total++;
    }
    int i = total - 1;
    while (i >= 1) {
        if (check == 0 && bin[i] == 1) {
            printf("sll %s,%s,%d\n", tempRegs[*tTracker], arg2, i);
            printf("move %s,%s\n", tempRegs[*tTracker + 1], tempRegs[*tTracker]);
            check = 1;
        } else if (bin[i] == 1){
            printf("sll %s,%s,%d\n", tempRegs[*tTracker], arg2, i);
            printf("add %s,%s,%s\n", tempRegs[*tTracker + 1], tempRegs[*tTracker + 1], tempRegs[*tTracker]);
        }
        i--;
    }
    printf("add %s,%s,%s\n", tempRegs[*tTracker + 1], tempRegs[*tTracker + 1], arg2);
    if (arg3[0] == '-') {
        if (arg1[1] == 't') {
            printf("sub %s,$zero,%s\n", tempRegs[*tTracker + 2],tempRegs[*tTracker + 1]);
        } else {
            printf("sub %s,$zero,%s\n", arg1,tempRegs[*tTracker + 1]);
        }
    } else {
        printf("move %s,%s\n", arg1,tempRegs[*tTracker + 1]);
    }
    *tTracker += 3;
}

// multiplication
void multiplication(char* arg1, char* arg2, char* arg3, int* tTracker, char** tempRegs, int* val) {
    if (arg2[0] == '0' || arg3[0] == '0') {
        times0(arg1, arg2, arg3, tTracker, tempRegs, val);
        return;
    } else if (arg3[0] == '1' && arg3[1] == '\0') {
        times1(arg1, arg2, arg3, tTracker, tempRegs, val);
        return;
    } else if (arg3[0] == '-' && arg3[1] == '1' && arg3[2] == '\0') {
        timesNeg1(arg1, arg2, arg3, tTracker, tempRegs, val);
        return;
    }
    if (arg3[0] == '-' || isNumber(arg3)) {
        break2(arg1, arg2, arg3, tTracker, tempRegs, val);
        return;
    }
    printf("mult %s,%s\n", arg2 ,arg3);
    printf("mflo %s\n", arg1);
    if (arg1[1] == 't') {
        if (*tTracker >= 10) {
            *val = 1;
            *tTracker = 0;
        } else {
            (*tTracker)++;
        }
    }
}

// sub cases for division
void base1(char* arg1, char* arg2) {
    printf("move %s,%s\n", arg1, arg2);
}

void base2(char* arg1, char* arg2) {
    printf("sub %s,$zero,%s\n", arg1, arg2);
}

void finish(char* arg1, char* arg2, char* arg3) {
    printf("div %s,%s\n", arg2 ,arg3);
    printf("mflo %s\n", arg1);
}

void branch(char* arg1, char* arg2, char* arg3, int* tTracker, char** tempRegs, int* remC) {
    if (arg1[1] == 't') {
        printf("li %s,%s\n", arg1, arg3);
        printf("div %s,%s\n", arg2, arg1);
        printf("mflo %s\n", tempRegs[*tTracker + 1]);
        (*tTracker)++;
    } else {
        printf("li %s,%s\n", tempRegs[*tTracker], arg3);
        printf("div %s,%s\n", arg2, tempRegs[*tTracker]);
        printf("mflo %s\n", arg1);
        (*tTracker)++;
    }
}

void bltz(char* arg1, char* arg2, char* arg3, int* tTracker, char** tempRegs, int* remC) {
    int temp = atoi(arg3);
    int shiftBits;
    if ((shiftBits = pow2(abs(temp))) > 0) {
        printf("bltz %s,L%d\n", arg2, *remC);
        printf("srl %s,%s,%d\n", arg1, arg2, shiftBits);
        if (arg3[0] == '-') {
            printf("sub %s,$zero,%s\n", arg1, arg1);
        }
        printf("j L%d\n", *remC + 1);
        printf("L%d:\n", *remC);
        printf("li %s,%s\n", tempRegs[*tTracker], arg3);
        printf("div %s,%s\n", arg2, tempRegs[*tTracker]);
        printf("mflo %s\n", arg1);
        printf("L%d:\n", *remC + 1);
        *remC += 2;
        (*tTracker)++;
    } else {
        branch(arg1, arg2, arg3, tTracker, tempRegs, remC);
    }
}

// division
void division(char* arg1, char* arg2, char* arg3, int* tTracker, char** tempRegs, int* remC) {
    if (arg3[0] == '1' && arg3[1] == '\0') {
        base1(arg1, arg2);
        return;
    } else if (arg3[0] == '-' && arg3[1] == '1' && arg3[2] == '\0') {
        base2(arg1, arg2);
        return;
    }
    if (arg3[0] == '-' || isNumber(arg3)) {
        bltz(arg1, arg2, arg3, tTracker, tempRegs, remC);
        return;
    }
    finish(arg1, arg2, arg3);
}

// modulus
void modulus(char* arg1, char* arg2, char* arg3) {
    printf("div %s,%s\n", arg2, arg3);
    printf("mfhi %s\n", arg1);
}

// detecting if a character is an operator
bool opCheck(char x) {
    char* ops = "=+-*/%";
    for (int i = 0; i < 6; i++) {
        if (ops[i] == x) {
            return true;
        }
    }
    return false;
}

// compling c code into mips
char* mipsCompiled(char** registers, int size, int* lValuesElements, char* lValues, int* tempTracker, int* remC) {
    int r = *lValuesElements;
    for (int i = 0; i < size; i++) {
        if (opCheck(registers[i][0])) {
            continue;
        } else if (isNumber(registers[i]) || registers[i][0] == '$') {
            continue;
        } else {
            lValues[r] = registers[i][0];
            (*lValuesElements)++;
            for (int j = i + 1; j < size; j++) {
                if(registers[j][0] == registers[i][0]) {
                    sprintf(registers[j], "$s%d", r);
                }
            }
            sprintf(registers[i], "$s%d", r);
            r++;
        }
    }
    
    // printing
    char* tempRegisters[] = { "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9" };
    int t = *tempTracker;
    int LCount = *remC;
    int value = 0;
    int i = 2;
    while (i < size) {
        if (i + 1 < size) {
            // addition branch and sub cases
            if (registers[i + 1][0] == '+') {
                if (i + 4 < size && t == 0 && value == 1) {
                    addition(tempRegisters[t],tempRegisters[9], registers[i + 2]);
                    t++;
                } else if (i + 4 < size && t == 0) {
                    addition(tempRegisters[t],registers[i], registers[i + 2]);
                    t++;
                } else  if (i + 4 < size && t != 0) {
                    addition(tempRegisters[t],tempRegisters[t - 1], registers[i + 2]);
                    t++;
                } else if (t == 0) {
                    addition(registers[0],registers[i], registers[i + 2]);
                } else {
                    addition(registers[0],tempRegisters[t - 1], registers[i + 2]);
                }
            }
            // multiplication branch and sub cases
            if (registers[i + 1][0] == '*') {
                if (i == 2 && t != 0 && i + 4 < size) {
                    multiplication(tempRegisters[t],registers[i], registers[i + 2], &t, tempRegisters, &value);
                } else if (i == 2 && t != 0) {
                    multiplication(registers[0],registers[i], registers[i + 2], &t, tempRegisters, &value);
                }else if (i + 4 < size && t == 0 && value == 1) {
                    multiplication(tempRegisters[t],tempRegisters[9], registers[i + 2], &t, tempRegisters, &value);
                } else if (i + 4 > size && t == 0 && value == 1) {
                    multiplication(registers[0],tempRegisters[9], registers[i + 2], &t, tempRegisters, &value);
                } else if (i + 4 < size && t == 0) {
                    multiplication(tempRegisters[t],registers[i], registers[i + 2], &t, tempRegisters, &value);
                } else  if (i + 4 < size && t != 0) {
                    multiplication(tempRegisters[t],tempRegisters[t - 1], registers[i + 2], &t, tempRegisters, &value);
                } else if (t == 0) {
                    multiplication(registers[0],registers[i], registers[i + 2], &t, tempRegisters, &value);
                } else {
                    multiplication(registers[0],tempRegisters[t - 1], registers[i + 2], &t, tempRegisters, &value);
                }
            }
            // division branch and sub cases
            if (registers[i + 1][0] == '/') {
                if (i + 4 < size && t == 0 && value == 1) {
                    division(tempRegisters[t],tempRegisters[9], registers[i + 2], &t, tempRegisters, &LCount);
                    t++;
                } else if (i + 4 < size && t == 0) {
                    division(tempRegisters[t],registers[i], registers[i + 2], &t, tempRegisters, &LCount);
                    t++;
                } else  if (i + 4 < size && t != 0) {
                    division(tempRegisters[t],tempRegisters[t - 1], registers[i + 2], &t, tempRegisters, &LCount);
                    t++;
                } else if (t == 0) {
                    division(registers[0],registers[i], registers[i + 2], &t, tempRegisters, &LCount);
                } else {
                    division(registers[0],tempRegisters[t - 1], registers[i + 2], &t, tempRegisters, &LCount);
                }
            }
            // modulus branch and sub cases
            if (registers[i + 1][0] == '%') {
                if (i + 4 < size && t == 0 && value == 1) {
                    modulus(tempRegisters[t],tempRegisters[9], registers[i + 2]);
                    t++;
                } else if (i + 4 < size && t == 0) {
                    modulus(tempRegisters[t],registers[i], registers[i + 2]);
                    t++;
                } else  if (i + 4 < size && t != 0) {
                    modulus(tempRegisters[t],tempRegisters[t - 1], registers[i + 2]);
                    t++;
                } else if (t == 0) {
                    modulus(registers[0],registers[i], registers[i + 2]);
                } else {
                    modulus(registers[0],tempRegisters[t - 1], registers[i + 2]);
                }
            }
            if (registers[i + 1][0] == '-' && registers[i + 1][1] == '\0') {
                if (i + 4 < size && t == 0 && value == 1) {
                    subtraction(tempRegisters[t],tempRegisters[9], registers[i + 2]);
                    t++;
                } else if (i + 4 < size && t == 0) {
                    subtraction(tempRegisters[t],registers[i], registers[i + 2]);
                    t++;
                } else  if (i + 4 < size && t != 0) {
                    subtraction(tempRegisters[t],tempRegisters[t - 1], registers[i + 2]);
                    t++;
                } else if (t == 0) {
                    subtraction(registers[0],registers[i], registers[i + 2]);
                } else {
                    subtraction(registers[0],tempRegisters[t - 1], registers[i + 2]);
                }
            }
        }
        if (t >= 10) {
            value = 1;
            t = 0;
        }
        i++;
    }
    *remC = LCount;
    *tempTracker = t;
    return lValues;
}


void freeMemory(char** registers, char* lValues) {
    int x = 0;
    
    while (x != 128) {
        free(registers[x]);
        x++;
    }
    
    free(registers);
    free(lValues);
}

int main( int argc, char * argv[] )
{
    int arg1 = 0;
    int arg2 = 0;
    FILE* fp = fopen(argv[1], "r");
    char* buffer = NULL;
    size_t length = 0;
    int regNum = 0;
    int registersIndex = 0;
    int numElements = 0;
    
    // keeping track of lValues and mapping it to registes
    char* lValues = calloc((128), sizeof(char ));
    int lValuesElements = 0;
    
    // storing registers in 2D array of chars
    char** registers = (char**) calloc((128), sizeof(char*));
    for (int i = 0; i < 128; i++) {
        registers[i] = (char*) calloc((10), sizeof(char));
    }
    
    // reading text file line by line
    while (getline(&buffer, &length, fp) != -1) {
        // printing line of C code that will be compiled to MIPS code
        printf("# %s", buffer);
        numElements = 0;
        registersIndex  = 0;
        
        // splitting string using spaces as delimiters
        char* split = strtok(buffer, " ");
        while (split != NULL) {
            strcpy(registers[registersIndex], split);
            numElements++;
            split = strtok (NULL, " ;\n");
            registersIndex++;
        }
        
        // this is a simple assignment operation
        if (numElements == 3) {
            initialize(registers, regNum);
            lValues[lValuesElements] = registers[0][0];
            lValuesElements++;
            regNum++;
        } else {
            for (int i = 0; i < numElements; i++) {
                for (int j = 0; j < lValuesElements; j++) {
                    if (lValues[j] == registers[i][0]) {
                        sprintf(registers[i], "$s%d", j);
                    }
                }
            }
            lValues = mipsCompiled(registers, numElements, &lValuesElements, lValues, &arg1, &arg2);
        }
    }
    
    freeMemory(registers, lValues);
    fclose(fp);
    return EXIT_SUCCESS;
}
