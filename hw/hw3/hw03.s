################################################################################
# Homework 3: Matrix-matrix multiplication
# CSCI-2500 Fall 2021
################################################################################
## Data segment
################################################################################
.data
  newline:  .asciiz "\n"
  tab:      .asciiz "\t"

################################################################################
## Text segment
################################################################################
.text
  .align 2          # instructions must be on word boundaries
  .globl main       # main is a global label
  .globl mm_read    # read in NxM matrix
  .globl mm_alloc   # allocate space for NxM matrix
  .globl mm_print   # print out formatted NxM matrix
  .globl mm_mult    # multiple two non-square matrices

################################################################################
# Main segment
################################################################################
main:
  # save return address on the stack
  sub $sp, $sp, 4   
  sw  $ra, 0($sp)

  # Read in a matrix 1 of some NxM size
  # Return in $v0 - N
  # Return in $v1 - M
  # Return in 0($sp) - base address of matrix
  sub $sp, $sp, 4   # make space on stack for return value
  jal mm_read

  # save matrix 1 values
  move  $s0, $v0
  move  $s1, $v1
  lw    $s2, 0($sp)
  add   $sp, $sp, 4   # restore stack

  # Setup arguments and print out matrix 1
  move  $a0, $s0
  move  $a1, $s1
  move  $a2, $s2
  jal mm_print

  # Read in matrix 2 of some NxM size
  sub $sp, $sp, 4   # make space on stack for return value
  jal mm_read

  # save matrix 2 values
  move  $s3, $v0
  move  $s4, $v1
  lw    $s5, 0($sp)
  add   $sp, $sp, 4   # restore stack

  # Setup arguments and print out matrix 2
  move  $a0, $s3
  move  $a1, $s4
  move  $a2, $s5
  jal mm_print

  # Setup arguments and call mm_mult
  # Note: 5 arguments, so we need to use stack
  # Note: num cols matrix 1 == num rows matrix 2
  move  $a0, $s0    # num rows matrix1
  move  $a1, $s1    # num cols matrix1
  move  $a2, $s2    # address of matrix 1
  move  $a3, $s4    # num cols matrix2
  sub   $sp, $sp, 4   
  sw    $s5, 0($sp) # address of matrix 2
  jal mm_mult
  add   $sp, $sp, 4

  # print the result
  move $a0, $s0 
  move $a1, $s4
  move $a2, $v0
  jal mm_print

  # restore $ra, free stack space, and return
  lw  $ra, 0($sp)
  add $sp, $sp, 4
  jr  $ra

################################################################################
# mm_read: Read in a NxM matrix from standard input
# Note: You will be calling mm_alloc from this function, so you'll need to save 
#       $ra on the stack. Also, you'll be returning 3 values back to main, one
#       value (the matrix base address), will need to be saved on the stack.
#       Matrix format is:
#         First line:  N = num rows
#         Second line: M = num cols
#         Subsequent lines contain one value each, filling each row in turn
#         (same format as hw01, except N,M on separate lines)  
# Input:
#   None
# Return:
#   v0 = N
#   v1 = M
#   0($sp) = base address of matrix
################################################################################
mm_read:
  # save return address and any saved registers on the stack, if necessary
  sub $sp, $sp, 4
  sw $ra, 0($sp)

  # get N
  li $v0, 5
  syscall
  # a0 has N
  move $a0, $v0

  # get M
  li $v0, 5
  syscall
  # a1 has M
  move $a1, $v0

  # Setup up arguments and call mm_alloc - v0 is returned as base address
  jal mm_alloc
  # lw $ra, 0($sp)

  # store base address in $t0 and $t1
  move $t0, $v0
  move $t1, $v0
  
  # do nested loops to read in values
  # for (int i = 0; i < rows; i++) {
    # for (int j = 0; j < cols; j++) {
      # resultMatrix[i][j] = value;
    # }
  # }
  li $t2, 0 # int i = 0

row_read:
  beq $t2, $a0, row_end # if i = rows
  li $t3, 0 # int j = 0

col_read:
  beq $t3, $a1, col_end # if j = cols
  li $v0, 5
  syscall
  sw $v0, 0($t1) # storing number in matrix
  addi $t1, $t1, 4 # going to next element in matrix
  addi $t3, $t3, 1 # j++
  j col_read

col_end:
  addi $t2, $t2, 1 # i++
  j row_read

row_end:
  # setup up return values
  # v0 = N
  move $v0, $a0

  # v1 = M
  move $v1, $a1

  # restore stack, ra, and any saved registers, if necessary
  lw $ra, 0($sp)
  addi $sp, $sp, 4

  # Note: third return value goes on the stack *after* restoration below
  # 0($sp) has base address
  sw $t0, 0($sp)
  
  # return to main
  jr  $ra

################################################################################
# mm_alloc: Allocate an NxM matrix
# Note: You can just allocate a single array of length N*M.
# Input:
#   a0 = N
#   a1 = M
# Return:
#   v0 = base address of allocated space
################################################################################
mm_alloc:
  # save return address and any saved registers on the stack, if necessary
  sub $sp, $sp, 12
  sw $ra, 0($sp)
  sw $a0, 4($sp)
  sw $a1, 8($sp)

  # Allocate sufficient space and set return value
  mul $t0, $a0, $a1
  li $v0, 9
  sll $a0, $t0, 2
  syscall

  # restore stack, ra, and any saved registers, if necessary
  lw $ra, 0($sp)
  lw $a0, 4($sp)
  lw $a1, 8($sp)
  addi $sp, $sp, 12
  
  # return to main
  jr  $ra


################################################################################
# mm_print: Output the formatted contents of NxM matrix
# Note: Within a row, separate each value by tab
#       End each row with a newline
# Input:
#   a0 = N
#   a1 = M
#   a2 = base pointer
# Return:
#   None
################################################################################
mm_print:
  # save return address and any saved registers on the stack, if necessary
  sub $sp, $sp, 4
  sw $ra, 0($sp)

  # save $a0 in $t0
  move $t0, $a0

  # for (int i = 0; i < rows; i++) {
    # for (int j = 0; j < cols; j++) {
      # print(matrix[i][j])
    # }
  # }
  # do nested loops to print out values
  li $t1, 0 # int i = 0

print_rows:
  beq $t1, $t0, finish_rows # if i = rows
  li $t2, 0 # int j = 0

print_cols:
  beq $t2, $a1, finish_cols # if j = cols
  # print number
  li $v0, 1
  lw $a0, 0($a2)
  syscall
  # print tab
  li $v0, 4
  la $a0, tab
  syscall
  # move to next element in matrix
  addi $a2, $a2, 4
  addi $t2, $t2, 1 # j++
  j print_cols

finish_cols:
  li $v0, 4
  la $a0, newline
  syscall
  addi $t1, $t1, 1 # i++
  j print_rows

finish_rows:
  # restore stack, ra, and any saved registers, if necessary
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  # restore original value of $a0 using $t0
  move $a0, $t0
  # return to main
  jr  $ra


################################################################################
# mm_mult: Perform the actual NxM matrix multiplication
# Input:
#   a0 = N for matrix 1
#   a1 = M for matrix 1 <-> N for matrix 2
#   a2 = base address for matrix 1
#   a3 = M for matrix 2
#   0($sp) = base address for matrix 2
# Return:
#   v0 = base address for result matrix
################################################################################
# double answer = 0.0;
# int length = mat1->cols;
# for (int i = 0; i < result_matrix->rows; i++) {
  # for (int j = 0; j < result_matrix->cols; j++) {
    # for (int k = 0; k < length; k++) {
      # answer += (mat1->data[i][k])*(mat2->data[k][j]);
    # }
  # result_matrix->data[i][j] = answer;
  # answer = 0.0;
  # }
# }
mm_mult:
    # get base address of matrix 2
    lw $t1, 0($sp)        

    # save return address and any saved registers on the stack, if necessary
    addi $sp, $sp -24
    sw $ra, 0($sp)
    sw $s0, 4($sp)
    sw $s1, 8($sp)
    sw $s2, 12($sp)
    sw $t0, 16($sp)
    sw $s4, 20($sp)

result_alloc:
    move $s0, $a0 # rows of matrix 1
    move $s1, $a1 # colmns of matrix 1, rows of matrix 2
    move $s2, $a3 # columns of matrix 2
    move $s4, $a2 # base address of matrix 1

    move $a1, $a3 # columns of matrix 1
    jal mm_alloc
    move $t0, $v0 # storing result of base matrix

    li $t2, 0 # i = 0

r_loop:
    beq $t2, $s0, r_done    # if i = # of rows in result matrix
    li $t3, 0               # j = 0

c_loop:
    beq $t3, $s2, c_done    # if = # of columns in result matrix
    li $t4, 0               # k = 0
    li $t6, 0               # answer = 0

a_loop:
    beq $t4, $s1, a_done    # if k = # of columns in matrix 1

offset1:
    # find offset of mat1[i][k]
    mul $t8, $s1, $t2
    add $t8, $t8, $t4
    sll $t8, $t8, 2
    add $t8, $t8, $s4     

    # get mat1[i][k]
    lw $t9, 0($t8)        

offset2:
    # find offset of mat2[k][j]
    mul $t7, $s2, $t4
    add $t7, $t7, $t3
    sll $t7, $t7, 2
    add $t7, $t7, $t1       

     # get mat2[k][j]       
    lw $t5, 0($t7)             

calc_answer:
    mul $t7, $t9, $t5
    add $t6, $t6, $t7
    
    addi $t4, $t4, 1 # k++
    j a_loop
    

a_done:
    # resultMatrix[i][j]
    mul $t9, $s2, $t2
    add $t9, $t9, $t3
    sll $t9, $t9, 2
    add $t9, $t9, $t0              

store_result:
    sw $t6, 0($t9) # storing answer in resultMatrix
    addi $t3, $t3, 1
    
    j c_loop

c_done:
    addi $t2, $t2, 1
    j r_loop

r_done:
    # saving base address of result matrix in $v0
    move $v0, $t0 

restore_variables:
    lw $ra, 0($sp)
    lw $s0, 4($sp)
    lw $s1, 8($sp)
    lw $s2, 12($sp)
    lw $t0, 16($sp)
    lw $s4, 20($sp)
    addi $sp, $sp 24
    jr  $ra
