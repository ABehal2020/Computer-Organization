/*
 Lab 6: Decoding the instructions
 CSCI-2500 Spring 2021
 Prof. Slota
 */

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// define BIT type as a char (i.e., one byte)
typedef char BIT;
#define TRUE 1
#define FALSE 0
#define UNDEF -1

#define LW_OP 35
#define ADD_OP 0
#define ADD_FUNCT 32
#define J_OP 2

/******************************************************************************/
/* Normal circuits */
/* Modify/add additional circuits as necessary (e.g., 3-input AND gate) */
/******************************************************************************/
BIT not_gate(BIT A)
{
    if (A)
        return FALSE;
    else
        return TRUE;
}

BIT or_gate(BIT A, BIT B)
{
    if (A || B)
        return TRUE;
    else
        return FALSE;
}

BIT and_gate(BIT A, BIT B)
{
    if (A && B)
        return TRUE;
    else
        return FALSE;
}

BIT and_gate3(BIT A, BIT B, BIT C)
{
    return and_gate(A, and_gate(B, C));
}

BIT xor_gate(BIT A, BIT B)
{
    if (A ^ B)
        return TRUE;
    else
        return FALSE;
}

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3)
{
    *O0 = and_gate(not_gate(I1), not_gate(I0));
    *O1 = and_gate(not_gate(I1), I0);
    *O2 = and_gate(I1, not_gate(I0));
    *O3 = and_gate(I1, I0);
    
    return;
}


/******************************************************************************/
/* Checkpoint 1 functions */
/* Add helper functions as necessary. I did:
 - One for converting integer->2's complement binary
 - One for converting register string to binary representation */
/******************************************************************************/

int get_reg_id(char* reg) {
    if (strcmp(reg, "s0") == 0) {
        return 16;
    } else if (strcmp(reg, "t0") == 0) {
        return 8;
    }
    
    return -1;
}

int from_front(int i) {
    return 32 - i;
}

void convert_bin(BIT* instruction, int start, int len, int value) {
    if (value < 0) {
        convert_bin(instruction, start, len, -value);
        
        // Flip bits
        for (int i = start; i < start + len; i++) {
            instruction[i] = not_gate(instruction[i]);
        }
        
        // Add one
        for (int i = start; i < start + len; i++) {
            if (instruction[i] + 1 > 1) {
                instruction[i] = FALSE;
            } else {
                instruction[i] = TRUE;
                break;
            }
        }
        
        return;
    }
    
    
    for(int i = start; value > 0 && i - start < len; i++){
        instruction[i] = value % 2;
        value = value / 2;
    }
}

void set_op_code(BIT* Output, int value) {
    convert_bin(Output, 26, 6, value);
}


void set_rs(BIT* Output, int value) {
    convert_bin(Output, 21, 5, value);
}


void set_rt(BIT* Output, int value) {
    convert_bin(Output, 16, 5, value);
}

void set_rd(BIT* Output, int value) {
    convert_bin(Output, 11, 6, value);
}

void set_shamt(BIT* Output, int value) {
    convert_bin(Output, 6, 5, value);
}

void set_funct(BIT* Output, int value) {
    convert_bin(Output, 0, 6, value);
}

void set_address(BIT* Output, int value) {
    convert_bin(Output, 0, 26, value);
}

void set_immediate(BIT* Output, int value) {
    convert_bin(Output, 0, 16, value);
}

#define ADD 0
#define LW 1
#define J 2

int set_instr(BIT* Output, char* instr) {
    if (strcmp(instr, "add") == 0) {
        set_op_code(Output, ADD_OP);
        set_shamt(Output, 0);
        set_funct(Output, ADD_FUNCT);
        return ADD;
    }
    
    if (strcmp(instr, "lw") == 0) {
        set_op_code(Output, LW_OP);
        return LW;
    }
    
    set_op_code(Output, J_OP);
    return J;
}

int parse_string_to_int(char* str) {
    return atoi(str);
}

void set_add_args(BIT* Output, char* rs, char* rt, char* rd) {
    set_rs(Output, get_reg_id(rs));
    set_rt(Output, get_reg_id(rd));
    set_rd(Output, get_reg_id(rt));
}

void set_lw_args(BIT* Output, char* rs, char* rt, char* rd) {
    set_rs(Output, get_reg_id(rt));
    set_rt(Output, get_reg_id(rs));
    
    set_immediate(Output, parse_string_to_int(rd));
}

void set_j_args(BIT* Output, char* rs) {
    set_address(Output, parse_string_to_int(rs));
}

void set_args(BIT* Output, int type, char* rs, char* rt, char* rd) {
    if (type == ADD) {
        set_add_args(Output, rs, rt, rd);
        return;
    }
    
    if (type == LW) {
        set_lw_args(Output, rs, rt, rd);
        return;
    }
    
    set_j_args(Output, rs);
}

void convert_instructions()
{
    char line[256] = {0};
    while (fgets(line, 256, stdin) != NULL) {
        BIT Output[32] = {FALSE};
        
        // convert_bin(Output, from_front(10), 10, -56);
        // TODO: perform conversion
        // My approach:
        char instr[256];
        char rs[256];
        char rt[256];
        char rd[256];
        sscanf(line, "%s %s %s %s", instr, rs, rt, rd);
        int type = set_instr(Output, instr);
        
        set_args(Output, type, rs, rt, rd);
        
        // - Use sscanf on line to get strings for instruction and registers
        // - Use instructions to determine op code, funct, and shamt fields
        // - Convert immediate field and jump address field to binary
        // - Use registers to get rt, rd, rs fields
        // Note: I parse everything as strings, then convert to BIT array at end
        
        
        for (int i = 31; i >= 0; --i) {
            printf("%d", Output[i]);
        }
        printf("\n");
    }
    
    return;
}

/******************************************************************************/
/* Checkpoint 2 circuits */
/******************************************************************************/

void decoder3(BIT* I, BIT EN, BIT* O)
{
    // TODO: implement 3-to-8 decoder using gates
    // See lecture slides, book, and/or online resources for logic designs
    BIT I0 = I[0];
    BIT I1 = I[1];
    BIT I2 = I[2];
    
    BIT NI0 = not_gate(I0);
    BIT NI1 = not_gate(I1);
    BIT NI2 = not_gate(I2);
    
    O[0] = and_gate(EN, and_gate3(NI2, NI1, NI0));
    O[1] = and_gate(EN, and_gate3(NI2, NI1, I0));
    O[2] = and_gate(EN, and_gate3(NI2, I1, NI0));
    O[3] = and_gate(EN, and_gate3(NI2, I1, I0));
    O[4] = and_gate(EN, and_gate3(I2, NI1, NI0));
    O[5] = and_gate(EN, and_gate3(I2, NI1, I0));
    O[6] = and_gate(EN, and_gate3(I2, I1, NI0));
    O[7] = and_gate(EN, and_gate3(I2, I1, I0));
    
    return;
}

void decoder5(BIT* I, BIT* O)
{
    // TODO: implement 5-to-32 decoder using 2-to-4 and 3-to-8 decoders
    // https://fci.stafpu.bu.edu.eg/Computer%20Science/4887/crs-12801/Files/hw4-solution.pdf
    
    BIT S[4];
    decoder2(I[3], I[4], S, S+1, S+2, S+3);
    
    for (int i = 0; i < 32; i += 8) {
        BIT dec3Wires[8];
        decoder3(I, S[i/8], dec3Wires);
        
        for (int j = 0; j < 8; j++) {
            O[j + i] = dec3Wires[j];
        }
    }
    
    return;
}

/******************************************************************************/
/* Insane in the membmain */
/******************************************************************************/
int main()
{
    // convert some arbitrary number of instructions from stdin
    convert_instructions();
    
    // Unit test for 5-to-32 decoder
    printf("\n===== Unit test for 5-to-32 decoder =====\n");
    printf("decoder( I4, I3, I2, I1, I0 ) | ( O31, O30, ..., O1, O0 )\n");
    printf("------------------------------|--------------------------------------------------------------------------------------------------\n" );
    BIT I[5] = {0};
    BIT O[32] = {0};
    for (I[4] = 0; I[4] < 2; ++I[4])
        for (I[3] = 0; I[3] < 2; ++I[3])
            for (I[2] = 0; I[2] < 2; ++I[2])
                for (I[1] = 0; I[1] < 2; ++I[1])
                    for (I[0] = 0; I[0] < 2; ++I[0]) {
                        decoder5(I, O);
                        printf( "decoder( %2d, %2d, %2d, %2d, %2d, ) | ( ",
                               I[4], I[3], I[2], I[1], I[0]);
                        printf("%2d", O[31]);
                        for (int i = 30; i >= 0; --i)
                            printf(", %2d", O[i]);
                        printf(" )\n");
                    }
    
    return 0;
}

