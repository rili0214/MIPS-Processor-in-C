/* 
Class Project: The logical conclusion (v1.1)
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

// useful constants
BIT ONE[32] = {TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, 
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT ZERO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT REG_THIRTY_ONE[5] = {TRUE, TRUE, TRUE, TRUE, TRUE};
BIT THIRTY_TWO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, 
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};


/******************************************************************************/
/* Function prototypes */
/******************************************************************************/
BIT not_gate(BIT A);
BIT or_gate(BIT A, BIT B);
BIT or_gate3(BIT A, BIT B, BIT C);
BIT and_gate(BIT A, BIT B);
BIT and_gate3(BIT A, BIT B, BIT C);
BIT xor_gate(BIT A, BIT B);
BIT nor_gate(BIT A, BIT B);
BIT nand_gate(BIT A, BIT B);

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3);
BIT multiplexor2(BIT S, BIT I0, BIT I1);
void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output);
BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3);
void decoder5(BIT* I, BIT* O);
void decoder3(BIT* I, BIT EN, BIT* O);

void copy_bits(BIT* A, BIT* B);
void print_binary(BIT* A);
void convert_to_binary(int a, BIT* A, int length);
void convert_to_binary_char(int a, char* A, int length);
int binary_to_integer(BIT* A);

int get_instructions(BIT Instructions[512][32]);

void Instruction_Memory(BIT* ReadAddress, BIT* Instruction);
void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite, BIT* funct);
void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2);
void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData);
void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl);
void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result);
void Data_Memory(BIT MemWrite, BIT MemRead, 
  BIT* Address, BIT* WriteData, BIT* ReadData);
void Extend_Sign16(BIT* Input, BIT* Output);
void updateState();
  
/******************************************************************************/
/* Functions provided for your convenience */
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

BIT or_gate3(BIT A, BIT B, BIT C)
{
  return or_gate(A, or_gate(B, C));
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

BIT nor_gate(BIT A, BIT B)
{
  return not_gate(or_gate(A, B));
}

BIT nand_gate(BIT A, BIT B)
{
  return not_gate(and_gate(A, B));
}

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3)
{
  // Note: The input -> output mapping is slightly modified from before
  *O0 = and_gate(not_gate(I1), not_gate(I0));
  *O1 = and_gate(not_gate(I1), I0);
  *O2 = and_gate(I1, not_gate(I0));
  *O3 = and_gate(I1, I0);
  
  return;
}

BIT multiplexor2(BIT S, BIT I0, BIT I1)
{
  BIT x0 = and_gate(not_gate(S), I0);
  BIT x1 = and_gate(S, I1);
  return or_gate(x0, x1);  
}

void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output)
{
  for (int i = 0; i < 32; ++i) {
    BIT x0 = and_gate(not_gate(S), I0[i]);
    BIT x1 = and_gate(S, I1[i]);
    Output[i] = or_gate(x0, x1);
  }
}

void decoder3(BIT* I, BIT EN, BIT* O)
{ 
  if(EN == FALSE){
    int i = 0;
    for(i =0; i< 8; i++){
      O[i] = 0;
    }
  }else{
      O[0] = and_gate(and_gate(not_gate(I[0]), not_gate(I[1])),not_gate(I[2]));
      O[1] = and_gate(and_gate(I[0], not_gate(I[1])),not_gate(I[2]));
      O[2] = and_gate(and_gate(not_gate(I[0]), I[1]),not_gate(I[2]));
      O[3] = and_gate(and_gate(I[0], I[1]),not_gate(I[2]));
      O[4] = and_gate(and_gate(not_gate(I[0]), not_gate(I[1])),I[2]);
      O[5] = and_gate(and_gate(I[0], not_gate(I[1])),I[2]);
      O[6] = and_gate(and_gate(not_gate(I[0]), I[1]),I[2]);
      O[7] = and_gate(and_gate(I[0], I[1]),I[2]);
    }

  return;

  }

void decoder5(BIT* I, BIT* O)
{
  // TODO: implement 5-to-32 decoder using 2-to-4 and 3-to-8 decoders
  // https://fci.stafpu.bu.edu.eg/Computer%20Science/4887/crs-12801/Files/hw4-solution.pdf
   BIT O1 = FALSE;
  BIT O2 = FALSE;
  BIT O3 = FALSE;
  BIT O4 = FALSE;
  decoder2(I[3],I[4],&O1,&O2,&O3,&O4);

  // call decoder3 4 times
  BIT inpot_3[3];
  int i = 0;
  for(i = 0; i < 3; i++){
    inpot_3[i] = I[i];
  }
  // from 4 output
  BIT out_put1[8] = {FALSE};

  BIT out_put2[8] = {FALSE};

  BIT out_put3[8] = {FALSE};

  BIT out_put4[8] = {FALSE};

  decoder3(inpot_3,O1,out_put1);
  decoder3(inpot_3,O2,out_put2);
  decoder3(inpot_3,O3,out_put3);
  decoder3(inpot_3,O4,out_put4);
  int count = 0;
  for(i = 0; i < 8; i ++){
    O[i] = out_put1[count];
    count++;
  }
  count = 0;
  for(i = 8; i < 16; i ++){
    O[i] = out_put2[count];
    count++;
  }
  count = 0;
  for(i = 16; i < 24; i ++){
    O[i] = out_put3[count];
    count++;
  }
  count = 0;
  for(i = 24; i < 32; i ++){
    O[i] = out_put4[count];
    count++;
  }
  // TODO: implement 5-to-32 decoder using 2-to-4 and 3-to-8 decoders
  // https://fci.stafpu.bu.edu.eg/Computer%20Science/4887/crs-12801/Files/hw4-solution.pdf
  
  return;
}

BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3)
{
  BIT x0, x1, x2, x3 = FALSE;
  decoder2(S0, S1, &x0, &x1, &x2, &x3);
  
  BIT y0 = and_gate(x0, I0);
  BIT y1 = and_gate(x1, I1);
  BIT y2 = and_gate(x2, I2);
  BIT y3 = and_gate(x3, I3);
  
  BIT z0 = or_gate(y0, y1);
  BIT z1 = or_gate(y2, y3);
  
  return or_gate(z0, z1);  
}


/******************************************************************************/
/* Helper functions */
/******************************************************************************/
void copy_bits(BIT* A, BIT* B)
{
  for (int i = 0; i < 32; ++i)
    B[i] = A[i]; 
}

void print_binary(BIT* A)
{
  for (int i = 31; i >= 0; --i)
    printf("%d", A[i]); 
}

void convert_to_binary(int a, BIT* A, int length)
{
  if (a >= 0) {
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 1 ? TRUE : FALSE);
      a /= 2;
    }
  } else {
    a += 1;
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 0 ? TRUE : FALSE);
      a /= 2;
    }
  }
}

void convert_to_binary_char(int a, char* A, int length)
{
  if (a >= 0) {
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 1 ? '1' : '0');
      a /= 2;
    }
  } else {
    a += 1;
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 0 ? '1' : '0');
      a /= 2;
    }
  }
}
  
int binary_to_integer(BIT* A)
{
  unsigned a = 0;
  unsigned mult = 1;
  
  for (int i = 0; i < 32; ++i) {
    a += A[i]*mult;
    mult *= 2;
  }
  
  return (int)a;
}

void I_type(BIT Instructions[32][32], int instruction_count, char reg1[64], char reg2[64], char reg3[64], BIT beq){
  BIT reg_num1[5];
  BIT reg_num2[5];
  BIT reg_num3[16];
  // Find the register number according to their name and convert them into binary
  if (strcmp(reg1,"zero")==0){
    convert_to_binary(0,reg_num1,5);
  }
  else if (strcmp(reg1,"v0")==0){
    convert_to_binary(2,reg_num1,5);
  }
  else if (strcmp(reg1,"a0")==0){
    convert_to_binary(4,reg_num1,5);
  }
  else if (strcmp(reg1,"t0")==0){
    convert_to_binary(8,reg_num1,5);
  }
  else if (strcmp(reg1,"t1")==0){
    convert_to_binary(9,reg_num1,5);
  }
  else if (strcmp(reg1,"s0")==0){
    convert_to_binary(16,reg_num1,5);
  }
  else if (strcmp(reg1,"s1")==0){
    convert_to_binary(17,reg_num1,5);
  }
  else if (strcmp(reg1,"sp")==0){
    convert_to_binary(29,reg_num1,5);
  }
  else if (strcmp(reg1,"ra")==0){
    convert_to_binary(31,reg_num1,5);
  }

  if (strcmp(reg2,"zero")==0){
    convert_to_binary(0,reg_num2,5);
  }
  else if (strcmp(reg2,"v0")==0){
    convert_to_binary(2,reg_num2,5);
  }
  else if (strcmp(reg2,"a0")==0){
    convert_to_binary(4,reg_num2,5);
  }
  else if (strcmp(reg2,"t0")==0){
    convert_to_binary(8,reg_num2,5);
  }
  else if (strcmp(reg2,"t1")==0){
    convert_to_binary(9,reg_num2,5);
  }
  else if (strcmp(reg2,"s0")==0){
    convert_to_binary(16,reg_num2,5);
  }
  else if (strcmp(reg2,"s1")==0){
    convert_to_binary(17,reg_num2,5);
  }
  else if (strcmp(reg2,"sp")==0){
    convert_to_binary(29,reg_num2,5);
  }
  else if (strcmp(reg2,"ra")==0){
    convert_to_binary(31,reg_num2,5);
  }
  // Convert constant into binary
  int i;
  if (strcmp(reg3,"zero")==0){
    i=0;
  }
  else{
    i=atoi(reg3);
  }
  convert_to_binary(i,reg_num3,16);

  int j=16;
  int k=21;
  if (beq==TRUE){
    // If it's beq, change position
    j=21;
    k=16;
  }

  int m=0;
  // Store binary bit into instruction
  for (int i=0;i<5;i++,j++,k++) {
    Instructions[instruction_count][j]=reg_num1[i];
    Instructions[instruction_count][k]=reg_num2[i];
  }
  for (int i=0;i<16;i++,m++){
    Instructions[instruction_count][m]=reg_num3[i];
  }
}

void R_type(BIT Instructions[32][32], int instruction_count, char reg1[64], char reg2[64], char reg3[64]){
  BIT reg_num1[5];
  BIT reg_num2[5];
  BIT reg_num3[5];
  // Find the register number according to their name and convert them into binary
  if (strcmp(reg1,"zero")==0){
    convert_to_binary(0,reg_num1,5);
  }
  else if (strcmp(reg1,"v0")==0){
    convert_to_binary(2,reg_num1,5);
  }
  else if (strcmp(reg1,"a0")==0){
    convert_to_binary(4,reg_num1,5);
  }
  else if (strcmp(reg1,"t0")==0){
    convert_to_binary(8,reg_num1,5);
  }
  else if (strcmp(reg1,"t1")==0){
    convert_to_binary(9,reg_num1,5);
  }
  else if (strcmp(reg1,"s0")==0){
    convert_to_binary(16,reg_num1,5);
  }
  else if (strcmp(reg1,"s1")==0){
    convert_to_binary(17,reg_num1,5);
  }

  if (strcmp(reg2,"zero")==0){
    convert_to_binary(0,reg_num2,5);
  }
  else if (strcmp(reg2,"v0")==0){
    convert_to_binary(2,reg_num2,5);
  }
  else if (strcmp(reg2,"a0")==0){
    convert_to_binary(4,reg_num2,5);
  }
  else if (strcmp(reg2,"t0")==0){
    convert_to_binary(8,reg_num2,5);
  }
  else if (strcmp(reg2,"t1")==0){
    convert_to_binary(9,reg_num2,5);
  }
  else if (strcmp(reg2,"s0")==0){
    convert_to_binary(16,reg_num2,5);
  }
  else if (strcmp(reg2,"s1")==0){
    convert_to_binary(17,reg_num2,5);
  }

  if (strcmp(reg3,"zero")==0){
    convert_to_binary(0,reg_num3,5);
  }
  else if (strcmp(reg3,"v0")==0){
    convert_to_binary(2,reg_num3,5);
  }
  else if (strcmp(reg3,"a0")==0){
    convert_to_binary(4,reg_num3,5);
  }
  else if (strcmp(reg3,"t0")==0){
    convert_to_binary(8,reg_num3,5);
  }
  else if (strcmp(reg3,"t1")==0){
    convert_to_binary(9,reg_num3,5);
  }
  else if (strcmp(reg3,"s0")==0){
    convert_to_binary(16,reg_num3,5);
  }
  else if (strcmp(reg3,"s1")==0){
    convert_to_binary(17,reg_num3,5);
  }
  int j=11;
  int k=21;
  int m=16;
  // Store binary bit into instruction
  for (int i=0;i<5;i++,j++,k++,m++) {
    Instructions[instruction_count][j]=reg_num1[i];
    Instructions[instruction_count][k]=reg_num2[i];
    Instructions[instruction_count][m]=reg_num3[i];
  }
}

void J_type(BIT Instructions[32][32], int instruction_count, char reg1[64]){
    BIT reg_num1[32];
    int i;
    int j=0;
    // Convert constant into binary
    if (strcmp(reg1,"zero")==0){
      i=0;
    }
    else{
      i=atoi(reg1);
    }
    convert_to_binary(i,reg_num1,26);
    // Store binary bit into instruction 
    for (int i=0;i<26;i++,j++) {
      Instructions[instruction_count][j]=reg_num1[i];
    }
}


/******************************************************************************/
/* Parsing functions */
/******************************************************************************/

// TODO: Implement any helper functions to assist with parsing

int get_instructions(BIT Instructions[32][32])
{
  char line[256] = {0};
  char ins[64];
  char reg1[64];
  char reg2[64];
  char reg3[64];
  int instruction_count = 0;
  while (fgets(line, 256, stdin) != NULL) {        
    // TODO: perform conversion of instructions to binary
    // Input: coming from stdin via: ./a.out < input.txt
    // Output: Convert instructions to binary in the instruction memory
    // Return: Total number of instructions
    // Note: you are free to use if-else and external libraries here
    // Note: you don't need to implement circuits for saving to inst. mem.
    // My approach:
    // - Use sscanf on line to get strings for instruction and registers
    // - Use instructions to determine op code, funct, and shamt fields
    // - Convert immediate field and jump address field to binary
    // - Use registers to get rt, rd, rs fields
    // Note: I parse everything as strings, then convert to BIT array at end
    sscanf(line,"%s %s %s %s",ins,reg1,reg2,reg3);
    if(strcmp(ins,"lw")==0){
      // opcode for "lw" is 100011, 31-26. 
      Instructions[instruction_count][31]=TRUE;
      Instructions[instruction_count][27]=TRUE;
      Instructions[instruction_count][26]=TRUE;
      I_type(Instructions,instruction_count,reg1,reg2,reg3,FALSE);
      instruction_count++;
    }
    else if(strcmp(ins,"sw")==0){
      // opcode for "sw" is 101011, 31-26. 
      Instructions[instruction_count][31]=TRUE;
      Instructions[instruction_count][29]=TRUE;
      Instructions[instruction_count][27]=TRUE;
      Instructions[instruction_count][26]=TRUE;
      I_type(Instructions,instruction_count,reg1,reg2,reg3,FALSE);
      instruction_count++;
    }
    else if(strcmp(ins,"beq")==0){
      // opcode for "beq" is 000100, 31-26. 
      Instructions[instruction_count][28]=TRUE;
      I_type(Instructions,instruction_count,reg1,reg2,reg3,TRUE);
      instruction_count++;
    }
    else if(strcmp(ins,"addi")==0){
      // opcode for "addi" is 001000, 31-26. 
      Instructions[instruction_count][29]=TRUE;
      I_type(Instructions,instruction_count,reg1,reg2,reg3,FALSE);
      instruction_count++;
    }
    else if(strcmp(ins,"and")==0){
      // Function for "and" is 100100, 0-5. 
      Instructions[instruction_count][5]=TRUE;
      Instructions[instruction_count][2]=TRUE;
      R_type(Instructions,instruction_count,reg1,reg2,reg3);
      instruction_count++;
    }
    else if(strcmp(ins,"or")==0){
      // Function for "or" is 100101, 0-5. 
      Instructions[instruction_count][5]=TRUE;
      Instructions[instruction_count][2]=TRUE;
      Instructions[instruction_count][0]=TRUE;
      R_type(Instructions,instruction_count,reg1,reg2,reg3);
      instruction_count++;
    }
    else if(strcmp(ins,"add")==0){
      // Function for "add" is 100000, 0-5. 
      Instructions[instruction_count][5]=TRUE;
      R_type(Instructions,instruction_count,reg1,reg2,reg3);
      instruction_count++;
    }
    else if(strcmp(ins,"sub")==0){
      // Function for "sub" is 100010, 0-5. 
      Instructions[instruction_count][5]=TRUE;
      Instructions[instruction_count][1]=TRUE;
      R_type(Instructions,instruction_count,reg1,reg2,reg3);
      instruction_count++;
    }
    else if(strcmp(ins,"slt")==0){
      // Function for "slt" is 101010, 0-5. 
      Instructions[instruction_count][5]=TRUE;
      Instructions[instruction_count][3]=TRUE;
      Instructions[instruction_count][1]=TRUE;
      R_type(Instructions,instruction_count,reg1,reg2,reg3);
      instruction_count++;
    }
    else if(strcmp(ins,"j")==0){
      // opcode for "j" is 000010, 31-26. 
      Instructions[instruction_count][27]=TRUE;
      J_type(Instructions,instruction_count,reg1);
      instruction_count++;
    }
    else if(strcmp(ins,"jal")==0){
      // opcode for "jal" is 000011, 31-26. 
      Instructions[instruction_count][27]=TRUE;
      Instructions[instruction_count][26]=TRUE;
      J_type(Instructions,instruction_count,reg1);
      instruction_count++;
    }
    else if(strcmp(ins,"jr")==0){
      // Function for "jr" is 001000, 0-5. 
      Instructions[instruction_count][3]=TRUE;
      // bits for "ra".
      for (int i=21;i<26;i++){
        Instructions[instruction_count][i]=TRUE;
      }
      instruction_count++;
    }
  }
  return instruction_count;
}


/******************************************************************************/
/* Program state - memory spaces, PC, and control */
/******************************************************************************/
BIT PC[32]                  = {FALSE};
BIT MEM_Instruction[32][32] = {FALSE};
BIT MEM_Data[32][32]        = {FALSE};
BIT MEM_Register[32][32]    = {FALSE};

BIT RegDst    = FALSE;
BIT Jump      = FALSE;
BIT Branch    = FALSE;
BIT MemRead   = FALSE;
BIT MemToReg  = FALSE;
BIT ALUOp[2]  = {FALSE};
BIT MemWrite  = FALSE;
BIT ALUSrc    = FALSE;
BIT RegWrite  = FALSE;
BIT Zero      = FALSE;
BIT ALUControl[4] = {FALSE};

void print_instruction()
{
  unsigned pc = binary_to_integer(PC);
  printf("PC: %d\n", pc);
  printf("Instruction: ");
  print_binary(MEM_Instruction[pc]);
  printf("\n");
}

void print_state()
{
  printf("Data: ");
  for (int i = 0; i < 32; ++i) {
    printf("%d ", binary_to_integer(MEM_Data[i]));
  } 
  printf("\n");  
  
  printf("Register: ");
  for (int i = 0; i < 32; ++i) {
    printf("%d ", binary_to_integer(MEM_Register[i]));
  } 
  printf("\n");
}


/******************************************************************************/
/* Functions that you will implement */
/******************************************************************************/
void Instruction_Memory(BIT* ReadAddress, BIT* Instruction)
{
  // TODO: Implement instruction memory
  // Input: 32-bit instruction address
  // Output: 32-bit binary instruction
  // Note: Useful to use a 5-to-32 decoder here
 BIT temp[32] = {FALSE};
  // gives 0 0 0 1 0 0 0 0 as a example
  // the 1 is which line we should read
  int i = 0;
  decoder5(ReadAddress,temp);
  for(i = 0; i < 32; i++){
    multiplexor2_32(temp[i],Instruction,MEM_Instruction[i],Instruction);
  }
}

void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite, BIT* funct)
{
  // TODO: Set control bits for everything
  // Input: opcode field from the instruction
  // OUtput: all control lines get set 
  // Note: Can use SOP or similar approaches to determine bits
  /*
                                                   Control Truth Table:
                                                       Instruction
                      R-format      lw         sw         beq       addi         j         jal        jr
  opcode            |  000000  |  100011  |  101011  |  000100  |  001000  |  000010  |  000011  |  001000  |
          ---------------------------------------------------------------------------------------------------
          RegDst    |    1     |    0     |     X    |     X    |     0    |     X    |     X    |     X    | 
          Jump      |    0     |    0     |     0    |     0    |     0    |     1    |     1    |     1    |
          Branch    |    0     |    0     |     0    |     1    |     0    |     X    |     X    |     X    |
          MemRead   |    0     |    1     |     0    |     0    |     0    |     0    |     0    |     0    |
  output  MemtoReg  |    0     |    1     |     X    |     X    |     0    |     X    |     X    |     X    |
          ALUOp1    |    1     |    0     |     0    |     0    |     0    |     X    |     X    |     X    |
          ALUOp0    |    0     |    0     |     0    |     1    |     0    |     X    |     X    |     X    |
          MemWrite  |    0     |    0     |     1    |     0    |     0    |     0    |     0    |     0    |
          ALUSrc    |    0     |    1     |     1    |     0    |     1    |     X    |     X    |     X    |
          RegWrite  |    1     |    1     |     0    |     0    |     1    |     0    |     1    |     0    |
  */
  *RegDst = not_gate(or_gate3(or_gate(OpCode[5],OpCode[4]), or_gate(OpCode[3],OpCode[2]), or_gate(OpCode[1],OpCode[0])));
  BIT JmpReg = and_gate(and_gate(not_gate(or_gate3(funct[0], funct[1], funct[2])), funct[3]), 
    not_gate(and_gate(funct[4], funct[5])));
  *Jump = or_gate(and_gate(OpCode[1], not_gate(OpCode[5])), JmpReg);
  *Branch = OpCode[2];
  *MemRead = and_gate(and_gate3(OpCode[0],OpCode[1],OpCode[5]), not_gate(or_gate3(OpCode[2],OpCode[3],OpCode[4])));
  *MemToReg = and_gate(and_gate3(OpCode[0], OpCode[1], OpCode[5]), not_gate(OpCode[3]));
  ALUOp[1] = RegDst[0];
  ALUOp[0] = Branch[0];
  *MemWrite = and_gate(and_gate(OpCode[0], OpCode[1]), and_gate(OpCode[3], OpCode[5]));
  *ALUSrc = or_gate(and_gate(OpCode[1], OpCode[0]), OpCode[3]);
  *RegWrite = and_gate(or_gate(xor_gate(OpCode[3], and_gate(OpCode[1], OpCode[0])), *RegDst), not_gate(JmpReg));
}

void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2)
{
  // TODO: Implement register read functionality
  // Input: two 5-bit register addresses
  // Output: the values of the specified registers in ReadData1 and ReadData2
  // Note: Implementation will be very similar to instruction memory circuit
  BIT temp1[32] = {FALSE};
  decoder5(ReadRegister1,temp1);
  int i = 0;
  for(i = 0; i < 32; i++){
      multiplexor2_32(temp1[i],ReadData1,MEM_Register[i],ReadData1);
  }
  BIT temp2[32] = {FALSE};
  decoder5(ReadRegister2,temp2);
  for(i = 0; i < 32; i++){
    //for(j = 0; j < 32; j++){
      // a sop for
      // R S F 
      // 0 0 0 0
      // 0 1 0 0
      // 1 0 0 1
      // 1 1 0 1
      // 0 0 1 0
      // 0 1 1 1
      // 1 1 1 1
      // 1 0 1 0
      multiplexor2_32(temp2[i],ReadData2,MEM_Register[i],ReadData2);
  }
  
}

void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData)
{
  // TODO: Implement register write functionality
  // Input: one 5-bit register address, data to write, and control bit
  // Output: None, but will modify register file
  // Note: Implementation will again be similar to those above
  BIT temp1[32] = {FALSE};
  decoder5(WriteRegister,temp1);
  int i = 0;
  // V M F
  // 0 0 0 0
  // 0 1 0 1
  // 1 0 0 0
  // 1 1 0 1
  // 0 0 1 0
  // 0 1 1 0
  // 1 0 1 1
  // 1 1 1 1
  // mean   ing only when F is 1
  // the modification happened to M
  for(i =0; i <32; i ++){
        multiplexor2_32(and_gate(RegWrite,temp1[i]),MEM_Register[i],WriteData,MEM_Register[i]);
  }
}

void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl)
{
  // TODO: Implement ALU Control circuit
  // Input: 2-bit ALUOp from main control circuit, 6-bit funct field from the
  //        binary instruction
  // Output:4-bit ALUControl for input into the ALU
  // Note: Can use SOP or similar approaches to determine bits
  /*
                          Truth Table:
      ALUOp               Funct field         Operation
  ALUOP1  ALUOp0  | F5  F4  F3  F2  F1  F0  |
  -----------------------------------------------------
     0       0    |  X   X   X   X   X   X  |   0010
     X       1    |  X   X   X   X   X   X  |   0110
     1       X    |  X   X   0   0   0   0  |   0010
     1       X    |  X   X   0   0   1   0  |   0110
     1       X    |  X   X   0   1   0   0  |   0000
     1       X    |  X   X   0   1   0   1  |   0001
     1       X    |  X   X   1   0   1   0  |   0111

  SOP:
  Oper0 = Op1 * F0 + Op1 * F3
  Oper1 = ~Op1 + op1 * ~F2
  Oper2 = ~Op1 * Op0 + Op1 * F1
  Oper3 = 0
  */
  ALUControl[0] = or_gate(and_gate(ALUOp[1], funct[0]), and_gate(ALUOp[1], funct[3]));
  ALUControl[1] = or_gate(not_gate(ALUOp[1]), and_gate(ALUOp[1], not_gate(funct[2])));
  ALUControl[2] = or_gate(and_gate(not_gate(ALUOp[1]), ALUOp[0]), and_gate(ALUOp[1], funct[1]));
  ALUControl[3] = FALSE;
}

void Extend_Sign16(BIT* ipt, BIT* opt){

  for (unsigned int i=0;i < 16;i++){
      opt[i] = ipt[i];
    }

  for (unsigned int i=16;i < 32;i++){
      opt[i] = ipt[15];
  }
}

void adder1(BIT A, BIT B, BIT CarryIn, BIT* CarryOut, BIT* Sum)
{
    // TODO: implement a 1-bit adder
    // Note: you can copy+paste this from your or my Lab 5

    BIT x0 = xor_gate(A, B);
    *Sum = xor_gate(CarryIn, x0);
    BIT y0 = and_gate(x0, CarryIn);
    BIT y1 = and_gate(A, B);
    *CarryOut = or_gate(y0, y1);
}

void ALU1(BIT A, BIT B, BIT Binvert, BIT CarryIn, BIT Less, 
      BIT Op0, BIT Op1, BIT* Result, BIT* CarryOut, BIT* Set)
{
  // TODO: implement a 1-bit ALU 
  // Note: this will need modifications from Lab 5 to account for 'slt'
  // See slide "MSB ALU" in csci2500-f21-ch03a-slides.pdf
  
  BIT I0, I1, I2, I3;  
  Binvert = multiplexor2(Binvert, B, not_gate(B));
  I2 = FALSE;
  adder1(A, Binvert, CarryIn, CarryOut, &I2);
  I0 = and_gate(Binvert, A);
  I1 = or_gate(Binvert, A);
  *Set = I2;
  I3 = Less;
  *Result = multiplexor4(Op0, Op1, I0, I1, I2, I3);
}

void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result)
{   
  // TODO: Implement 32-bit ALU
  // Input: 4-bit ALUControl, two 32-bit inputs
  // Output: 32-bit result, and zero flag big
  // Note: Can re-use prior implementations (but need new circuitry for zero)
  
  BIT B_invert = and_gate(ALUControl[1], ALUControl[2]);   //sub, slt, beq
  BIT less = FALSE;
  BIT CarryIn = B_invert;     //set initial Cin to False
  BIT pre_Cout = FALSE;
  BIT t_set = FALSE;
  BIT Op0 = or_gate(and_gate(ALUControl[0],not_gate(ALUControl[2])), and_gate(ALUControl[0],ALUControl[2]));

  ALU1(Input1[0], Input2[0], B_invert, CarryIn, less, Op0, ALUControl[1], &Result[0], &pre_Cout, &t_set); 
  //*Zero[0] = and_gate(1, not_gate(Result[0]));       
  for (int i=1;i < 32;i++){
    ALU1(Input1[i], Input2[i], B_invert, pre_Cout, less, Op0, ALUControl[1], &Result[i], &pre_Cout, &t_set);  
    //*Zero[i] = and_gate(1, not_gate(Result[i])); 
  } 

  less = t_set;                      
  ALU1(Input1[0], Input2[0], B_invert, CarryIn, less, Op0, ALUControl[1], &Result[0], &pre_Cout, &t_set);
  //*Zero[0] = and_gate(1, not_gate(Result[0]));                      

  //if Result[i] is 0, then or gates will lead z_opt to zero among all 32 bits
  BIT z_opt = FALSE;
  for (int i=0;i < 32;i++){
    z_opt = or_gate(Result[i], z_opt);
  }
  *Zero = not_gate(z_opt);
}

void Data_Memory(BIT MemWrite, BIT MemRead, 
  BIT* Address, BIT* WriteData, BIT* ReadData)
{
  // TODO: Implement data memory
  // Input: 32-bit address, control flags for read/write, and data to write
  // Output: data read if processing a lw instruction
  // Note: Implementation similar as above

  // TODO: Implement data memory
  // MemWrite 1 writin to data
  // MemRead 1 read form the data
  
  BIT temp1[32] = {FALSE};
  decoder5(Address,temp1);
  int i = 0;
  for(i = 0; i < 32; i++){
      // a sop for
      // R M F 
      // 0 0 0 0
      // 0 1 0 0
      // 1 0 0 1
      // 1 1 0 1
      // 0 0 1 0
      // 0 1 1 1
      // 1 1 1 1
      // 1 0 1 0
      multiplexor2_32(and_gate( MemRead ,temp1[i]),ReadData,MEM_Data[i],ReadData);
  }
  //  W V M F   S
  //  0 0 0 0 0
  //  0 0 1 0 1 1
  //  0 1 0 0 0
  //  0 1 1 0 1 1
  //  0 0 0 1 0
  //  0 0 1 1 0
  //  0 1 0 1 1
  //  0 1 1 1 1

  //  1 0 0 0 0
  //  1 0 1 0 1
  //  1 1 0 0 0
  //  1 1 1 0 1
  //  1 0 0 1 0
  //  1 0 1 1 0
  //  1 1 0 1 1
  //  1 1 1 1 1
  // meaning only when F is 1
  // the modification happened to M
  for(i =0; i <32; i ++){
      multiplexor2_32(and_gate( MemWrite ,temp1[i]),MEM_Data[i],WriteData,MEM_Data[i]);
  }
}

void updateState()
{
    // TODO: Implement the full datapath here
    // Essentially, you'll be figuring out the order in which to process each of 
    // the sub-circuits comprising the entire processor circuit. It makes it 
    // easier to consider the pipelined version of the process, and handle things
    // in order of the pipeline. The stages and operations are:
    // Fetch - load instruction from instruction memory
    // Decode - set control bits and read from the register file
    // Execute - process ALU
    // Memory - read/write data memory
    // Write Back - write to the register file
    // Update PC - determine the final PC value for the next instruction

    // Fetch - load instruction from instruction memory
   
    //for determine the jump address
    BIT zero1=FALSE;
    BIT add1[4]= {FALSE,TRUE,FALSE,FALSE};
    BIT temp_PC[32]={FALSE};
    ALU(add1, PC, ONE, &zero1, temp_PC);

    BIT temp1[32]={FALSE};
    Instruction_Memory(PC, temp1);

    BIT opcode[6]={FALSE};
    BIT rs[5]={FALSE};
    BIT rt[5]={FALSE};
    BIT rd[5]={FALSE};
    BIT funct[6]={FALSE};
    int j=0;
    BIT RegDst=FALSE;
    BIT Jump=FALSE;
    BIT Branch=FALSE;
    BIT MemRead=FALSE;
    BIT MemToReg=FALSE;
    BIT ALUOp[2]={FALSE};
    BIT MemWrite=FALSE;
    BIT ALUSrc=FALSE;
    BIT RegWrite=FALSE;
    BIT ALUControl[4]={FALSE};
    BIT Input1[32]={FALSE};
    BIT Input2[32]={FALSE};
    BIT Zero=FALSE;
    BIT Result[32]={FALSE};
    BIT immediate[16]={FALSE};
    BIT immediate_after_extension[32]={FALSE};
      
    for (int i=0; i<6; ++i) {
      funct[i]=temp1[j];
      ++j;
    }

    for (int i=0; i<5; ++i) {
      ++j;
    }

    for (int i=0; i<5; ++i) {
      rd[i]=temp1[j];
      ++j;
    }

    for (int i=0; i<5; ++i) {
      rt[i]=temp1[j];
      ++j;
    }

    for (int i=0; i<5; ++i) {
      rs[i]=temp1[j];
      ++j;
    }

    for (int i=0; i<6; ++i) {
      opcode[i]=temp1[j];
      ++j;
    }

    for (int i=0; i<16; ++i) {
    immediate[i]=temp1[i];
    }

    Extend_Sign16(immediate,immediate_after_extension);
    Control(opcode, &RegDst, &Jump, &Branch, &MemRead, &MemToReg, ALUOp, &MemWrite, &ALUSrc, &RegWrite,funct);
    ALU_Control(ALUOp, funct, ALUControl);

    //rs=Input1,rt=Input2
    Read_Register(rs, rt,Input1, Input2);
    BIT temp2[32]={FALSE};
    for (int i=0; i<32; ++i) {
        temp2[i]=multiplexor2(ALUSrc,Input2[i],immediate_after_extension[i]);
    }
    ALU(ALUControl, Input1, temp2, &Zero, Result);
    BIT ReadData_from_memory[32] = {FALSE};
    Data_Memory(MemWrite, MemRead, Result, Input2, ReadData_from_memory);
    BIT WriteRegister_for_write[5]={FALSE};
    for (int i = 0; i < 5; ++i){
        WriteRegister_for_write[i] = multiplexor2(RegDst, rt[i], rd[i]);
    }

    for (int i = 0; i < 5; ++i){
        WriteRegister_for_write[i] = multiplexor2(and_gate(Jump,RegWrite), WriteRegister_for_write[i], REG_THIRTY_ONE[i]);
    }
    BIT WriteData_for_write[32]={FALSE};

    multiplexor2_32(MemToReg, Result, ReadData_from_memory, WriteData_for_write);
    multiplexor2_32(and_gate(Jump,RegWrite), WriteData_for_write, temp_PC, WriteData_for_write);
    
    Write_Register(RegWrite, WriteRegister_for_write, WriteData_for_write);    
    //PC++
    copy_bits(temp_PC,PC);
    
    //beq
    ALU(add1, PC, immediate_after_extension, &zero1, temp_PC);
    multiplexor2_32(and_gate(Branch,Zero), PC, temp_PC, temp_PC);

    //for j and jal
    multiplexor2_32(Jump, temp_PC, immediate_after_extension, temp_PC);

    multiplexor2_32(and_gate(Jump,RegDst), temp_PC, Input1, temp_PC);
    //for jr

    copy_bits(temp_PC,PC);
}

/******************************************************************************/
/* Main */
/******************************************************************************/

int main()
{
  setbuf(stdout, NULL);
    
  // parse instructions into binary format
  int counter = get_instructions(MEM_Instruction);
  
  // load program and run
  copy_bits(ZERO, PC);
  copy_bits(THIRTY_TWO, MEM_Register[29]);
  
  while (binary_to_integer(PC) < counter) {
    print_instruction();
    updateState();
    print_state();
  }

  return 0;
}
