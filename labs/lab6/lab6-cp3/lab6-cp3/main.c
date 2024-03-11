/*
Lab 6: Decoding the instructions
CSCI-2500 Fall 2021
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

BIT ONE[32] = {TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT ZERO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};


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

void decoder2(BIT I0, BIT I1,BIT E, BIT* O0, BIT* O1, BIT* O2, BIT* O3)
{
  *O0 = and_gate(E,and_gate(not_gate(I1), not_gate(I0)));
  *O1 = and_gate(E,and_gate(not_gate(I1), I0));
  *O2 = and_gate(E,and_gate(I1, not_gate(I0)));
  *O3 = and_gate(E,and_gate(I1, I0));
  
  return;
}

BIT multiplexor2(BIT S, BIT I0, BIT I1)
{
  BIT x0 = and_gate(not_gate(S), I0);
  BIT x1 = and_gate(S, I1);
  return or_gate(x0, x1);
}

BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3)
{
  BIT x0, x1, x2, x3 = FALSE;
  decoder2(S0, S1, TRUE, &x0, &x1, &x2, &x3);
  
  BIT y0 = and_gate(x0, I0);
  BIT y1 = and_gate(x1, I1);
  BIT y2 = and_gate(x2, I2);
  BIT y3 = and_gate(x3, I3);
  
  BIT z0 = or_gate(y0, y1);
  BIT z1 = or_gate(y2, y3);
  
  return or_gate(z0, z1);
}


/******************************************************************************/
/* Checkpoint 1 functions */
/* Add helper functions as necessary. I did:
  - One for converting integer->2's complement binary
  - One for converting register string to binary representation */
/******************************************************************************/
void tobinary(int data, char* bin_str){
  const size_t BITS = 16;

for(unsigned int i=0; i<BITS; i++)
{
  unsigned int mask = 1u << (BITS - 1 - i);
  bin_str[i] = (data & mask) ? '1' : '0';
}
}
void tobinary2(unsigned int data, char* bin_str){
  const size_t BITS = 26;

for(unsigned int i=0; i<BITS; i++)
{
  unsigned int mask = 1u << (BITS - 1 - i);
  bin_str[i] = (data & mask) ? '1' : '0';
}
}
void convert_instructions()
{
  char line[256] = {0};
  while (fgets(line, 256, stdin) != NULL) {
    BIT Output[32] = {FALSE};
    char op[4];
    char r1[260];
    char r2[4];
    char r3[260];
    sscanf(line,"%s %s %s %s", &op, &r1, &r2,&r3);
    if(op[0]=='a'){
      char *opc="000000";
      char *shamt="00000";
      char * func="000001";
      char * t0="01000";
      char * s0="10000";
      printf("funct: 100000\n");
      for(int i=5; i>=0; --i){
        Output[i]= func[i]=='1' ? 1: 0;
      }
      printf("shamt: 00000\n");
      for(int i=10; i>=6; --i){
        Output[i]=shamt[abs(i-10)]=='1' ? 1: 0;
      }
      switch (r1[0])
      {
      case 't':
        printf("rd: %s\n",t0);
        for(int i=15; i>=11; --i){
        Output[i]=t0[abs(i-15)]=='1' ? 1: 0;
      }
        break;
      case 's':
        printf("rd: %s\n",s0);
        for(int i=15; i>=11; --i){
        Output[i]=s0[abs(i-15)]=='1' ? 1: 0;
      }
        break;
      }
      switch (r3[0])
      {
      case 't':
        printf("rt: %s\n",t0);
        for(int i=20; i>=16; --i){
        Output[i]=t0[abs(i-20)]=='1' ? 1: 0;
      }
        break;
      case 's':
        printf("rt: %s\n",s0);
        for(int i=20; i>=16; --i){
        Output[i]=s0[abs(i-20)]=='1' ? 1: 0;
      }
        break;
      }
      switch (r2[0])
      {
      case 't':
        printf("rs: %s\n",t0);
        for(int i=25; i>=21; --i){
        Output[i]=t0[abs(i-25)]=='1' ? 1: 0;
      }
        break;
      case 's':
        printf("rs: %s\n",s0);
        for(int i=25; i>=21; --i){
        Output[i]=s0[abs(i-25)]=='1' ? 1: 0;
      }
        break;
      }
      printf("opcode: 000000\n");
      for(int i=31;i>=26;--i){
        Output[i]=opc[abs(i-31)]=='1' ? 1: 0;

      }
    }
    if(op[0]=='l'){
      char *opc="100001";
      char * t0="01000";
      char * s0="10000";
      char * con= malloc(sizeof(char)*16);
      int data;
      sscanf(r3,"%d",&data);
      tobinary(data,con);
      printf("immideate: %s\n",con);
      for(int i=15; i>=0; --i){
        Output[i]= con[abs(i-15)]=='1' ? 1: 0;
      }
      switch (r1[0])
      {
      case 't':
        printf("rt: %s\n",t0);
        for(int i=20; i>=16; --i){
        Output[i]=t0[abs(i-20)]=='1' ? 1: 0;
      }
        break;
      case 's':
        printf("rt: %s\n",s0);
        for(int i=20; i>=16; --i){
        Output[i]=s0[abs(i-20)]=='1' ? 1: 0;
      }
        break;
      }
      switch (r2[0])
      {
      case 't':
        printf("rs: %s\n",t0);
        for(int i=25; i>=21; --i){
        Output[i]=t0[abs(i-25)]=='1' ? 1: 0;
      }
        break;
      case 's':
        printf("rs: %s\n",s0);
        for(int i=25; i>=21; --i){
        Output[i]=s0[abs(i-25)]=='1' ? 1: 0;
      }
        break;
      }
      printf("opcode: 100001\n");
      for(int i=31;i>=26;--i){
        Output[i]=opc[abs(i-31)]=='1' ? 1: 0;

      }
      
        
    }
    if(op[0]=='s'){
      char *opc="101011";
      char * t0="01000";
      char * s0="10000";
      char * con= malloc(sizeof(char)*16);
      int data;
      sscanf(r3,"%d",&data);
      tobinary(data,con);
      printf("immideate: %s\n",con);
      for(int i=15; i>=0; --i){
        Output[i]= con[abs(i-15)]=='1' ? 1: 0;
      }
      switch (r1[0])
      {
      case 't':
        printf("rt: %s\n",t0);
        for(int i=20; i>=16; --i){
        Output[i]=t0[abs(i-20)]=='1' ? 1: 0;
      }
        break;
      case 's':
        printf("rt: %s\n",s0);
        for(int i=20; i>=16; --i){
        Output[i]=s0[abs(i-20)]=='1' ? 1: 0;
      }
        break;
      }
      switch (r2[0])
      {
      case 't':
        printf("rs: %s\n",t0);
        for(int i=25; i>=21; --i){
        Output[i]=t0[abs(i-25)]=='1' ? 1: 0;
      }
        break;
      case 's':
        printf("rs: %s\n",s0);
        for(int i=25; i>=21; --i){
        Output[i]=s0[abs(i-25)]=='1' ? 1: 0;
      }
        break;
      }
      printf("opcode: 101011\n");
      for(int i=31;i>=26;--i){
        Output[i]=opc[abs(i-31)]=='1' ? 1: 0;

      }
    }
    if(op[0]=='j'){
      char *opc="000010";
      char * con= malloc(sizeof(char)*26);
      unsigned int data;
      sscanf(r1,"%u",&data);
      tobinary2(data,con);
      printf("adress: %s\n",con);
      for(int i=25; i>=0; --i){
        Output[i]= con[abs(i-25)]=='1' ? 1: 0;
      }
      printf("opcode: 000010\n");
      for(int i=31;i>=26;--i){
        Output[i]=opc[abs(i-31)]=='1' ? 1: 0;

      }
    }

        
    // TODO: perform conversion
    // My approach:
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
  if(not_gate(EN)){
    for(int i=0;i<8;i++){
      O[i]=FALSE;
    }
    return;
  }
  decoder2(I[0],I[1],not_gate(I[2]),&O[0],&O[1],&O[2],&O[3]);
  decoder2(I[0],I[1],I[2],&O[4],&O[5],&O[6],&O[7]);


  // TODO: implement 3-to-8 decoder using gates
  // See lecture slides, book, and/or online resources for logic designs
  
  return;
}

void decoder5(BIT* I, BIT* O)
{
  BIT c;
  BIT d;
  BIT e;
  BIT f;
  decoder2(I[3],I[4],TRUE,&c,&d,&e,&f);
  decoder3(I,c,O);
  decoder3(I,d,O+4);
  decoder3(I,e,O+8);
  decoder3(I,f,O+16);
  // TODO: implement 5-to-32 decoder using 2-to-4 and 3-to-8 decoders
  // https://fci.stafpu.bu.edu.eg/Computer%20Science/4887/crs-12801/Files/hw4-solution.pdf
  
  return;
}


/******************************************************************************/
/* Checkpoint 3 circuits */
/******************************************************************************/

void convert_to_binary(int a, BIT* A)
{
  const size_t BITS = 32;

for(unsigned int i=0; i<BITS; i++)
{
  unsigned int mask = 1u << (BITS - 1 - i);
  A[i] = (a & mask) ? 1 : 0;
}
  // TODO: convert integer to 2's complement BIT representation
  // Note: A[0] is least significant bit and A[31] is most significant bit
  // For this function ONLY: you're allowed to use logical if-else statements
  
}

void adder1(BIT A, BIT B, BIT CarryIn, BIT* CarryOut, BIT* Sum)
{
  BIT x0 = xor_gate(A, B);
  *Sum = xor_gate(CarryIn, x0);
  
  BIT y0 = and_gate(x0, CarryIn);
  BIT y1 = and_gate(A, B);
  *CarryOut = or_gate(y0, y1);
  // TODO: implement a 1-bit adder
  // Note: you can probably copy+paste this from your (or my) Lab 5 solution
  
}


void LSALU1(BIT A, BIT B, BIT Binvert, BIT CarryIn, BIT Less,
  BIT Op0, BIT Op1, BIT* Result, BIT* CarryOut)
{
  BIT x0 = multiplexor2(or_gate(and_gate(Op0,Op1), Binvert), B, not_gate(B));

  BIT y0 = and_gate(A, x0);
  BIT y1 = or_gate(A, x0);
  BIT y2 = FALSE;
  adder1(A, x0, or_gate(CarryIn,and_gate(Op0,Op1)), CarryOut, &y2);
  *Result = multiplexor4(Op0, Op1, y0, y1, y2, Less);
  // TODO: implement a 1-bit ALU
  // Note: this will need modifications from Lab 5 to account for 'slt'
  // See slide "MSB ALU" in csci2500-f21-ch03a-slides.pdf
  
}
void MBALU1(BIT A, BIT B, BIT Binvert, BIT CarryIn, BIT Less,
  BIT Op0, BIT Op1, BIT* Result, BIT* CarryOut, BIT* Set)
{
  BIT x0 = multiplexor2(or_gate(and_gate(Op0,Op1), Binvert), B, not_gate(B));

  BIT y0 = and_gate(A, x0);
  BIT y1 = or_gate(A, x0);
  BIT y2 = FALSE;
  adder1(A, x0, or_gate(CarryIn,and_gate(Op0,Op1)), CarryOut, &y2);
  *Result = multiplexor4(Op0, Op1, y0, y1, y2, Less);
  *Set= multiplexor2(xor_gate(CarryIn,*CarryOut),y2,not_gate(y2));
  
  // TODO: implement a 1-bit ALU
  // Note: this will need modifications from Lab 5 to account for 'slt'
  // See slide "MSB ALU" in csci2500-f21-ch03a-slides.pdf
  
}



void ALU32(BIT* A, BIT* B, BIT Binvert, BIT CarryIn,
  BIT Op0, BIT Op1, BIT* Result, BIT* CarryOut)
{
  BIT set=FALSE;
  for(int i=0; i<31;i++){
    LSALU1(A[i],B[i],Binvert,CarryIn,set,Op0,Op1,&Result[i],CarryOut);
  }
  MBALU1(A[31],B[31],Binvert,CarryIn,set,Op0,Op1,&Result[31],CarryOut,&set);
  LSALU1(A[0],B[0],Binvert,CarryIn,set,Op0,Op1,&Result[0],CarryOut);
  // TODO: implement a 32-bit ALU
  // You'll need to essentially implement a 32-bit ripple adder here
  // See slide "New 32-bit ALU" in csci2500-f21-ch03a-slides.pdf
  
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



