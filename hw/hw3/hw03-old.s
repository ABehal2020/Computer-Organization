################################################################################
# Homework 3: Matrix-matrix multiplication
# CSCI-2500 Fall 2021
################################################################################
## Data segment
################################################################################
.data
  newline:  .asciiz "\n"
  tab:      .asciiz "\t"
  message:  .asciiz "reading\n"
  exit:  .asciiz "exiting\n"
  exit2:  .asciiz "exiting2\n"
  exit3:  .asciiz "exiting3\n"
  exit4:  .asciiz "exiting4\n"
  exit5:  .asciiz "exiting5\n"
  exit6:  .asciiz "exiting6\n"
  exit7:  .asciiz "terminating\n"

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
  .globl outer_loop
  .globl inner_loop
  .globl print_rows
  .globl print_cols
  .globl loop_rows
  .globl loop_cols
  .globl loop_sum_answer

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

  la $a0, exit4
  li $v0, 4
  syscall
  move $a0, $t0

  # save matrix 1 values
  move  $s0, $v0
  move  $s1, $v1
  lw    $s2, 0($sp)
  add   $sp, $sp, 4   # restore stack

  la $a0, exit5
  li $v0, 4
  syscall
  move $a0, $t0

  # Setup arguments and print out matrix 1
  move  $a0, $s0
  move  $a1, $s1
  move  $a2, $s2
  jal mm_print

  la $a0, exit6
  li $v0, 4
  syscall
  move $a0, $t0

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
  sub $sp, $sp, 24
  sw $ra, 0($sp)
  sw $s0, 4($sp)
  sw $s1, 8($sp)
  sw $s2, 12($sp)
  sw $t0, 16($sp)
  sw $t1, 20($sp)

  # get N
  li $v0, 5
  syscall

  move $s0, $v0

  # get M
  li $v0, 5
  syscall

  move $s1, $v0

  # Setup up arguments and call mm_alloc - v0 is returned as base address
  move $a0, $s0
  move $a1, $s1
  jal mm_alloc
  move $t6, $v0
  move $s0, $a0
  move $s1, $a1
  # do nested loops to read in values
  move $a0, $v0
  move $a1, $s0
  move $a2, $zero
  move $a3, $s1
  jal outer_loop

  # setup up return values
  # Note: third return value goes on the stack *after* restoration below
  move $v0, $s0
  move $v1, $s1

  la $a0, exit3
  li $v0, 4
  syscall
  move $a0, $t0

  # restore stack, ra, and any saved registers, if necessary
  lw $ra, 0($sp)
  lw $s0, 4($sp)
  lw $s1, 8($sp)
  lw $s2, 12($sp)
  lw $t0, 16($sp)
  lw $t1, 20($sp)
  addi $sp, $sp, 24

  sw $t6, 0($sp)

  # return to main
  jr  $ra
################################################################################
# outer_loop: Read in elements of matrix (counter is for rows)
# Input:
#   a0 = base address of dynamically allocated matrix
#   a1 = number of rows in matrix
#   a2 = number of iterations of loop
#   a3 = number of columns in matrix 
# Return:
#   None
################################################################################
outer_loop:
  sub $sp, $sp, 12
  sw $ra, 0($sp)
  sw $s0, 4($sp)
  sw $s1, 8($sp)
  move $s0, $a2
  li $a2, 0
  jal inner_loop
  move $a2, $s0
  addi $a2, $a2, 1
  lw $ra, 0($sp)
  lw $s0, 4($sp)
  lw $s1, 8($sp)
  addi $sp, $sp, 4

  # move $t0, $a0
  # la $a0, exit2
  # li $v0, 4
  # syscall
  # move $a0, $t0

  bne $a1, $a3, outer_loop
  # beq $a1, $a2, terminate

  jr $ra
################################################################################
# inner_loop: Read in elements of matrix (counter is for columns)
# Input:
#   a0 = base address of dynamically allocated matrix
#   a1 = number of rows in matrix
#   a2 = number of iterations of loop
#   a3 = number of columns in matrix 
# Return:
#   None
################################################################################
inner_loop:
  sub $sp, $sp, 8
  sw $s0, 0($sp)
  sw $s1, 4($sp)
  
  move $t0, $a0
  la $a0, message
  li $v0, 4
  syscall
  move $a0, $t0

  li $v0, 5
  syscall

  # calculating offset (rowIndex)(4*# of columns) + 4*column index
  # sll $t0, $s0, 2
  # add $t0, $t0, $a2
  # mul $t0, $t0, 4
  # sw $v0, $t0($a0)
  sw $v0, 0($a0)
  addi $a0, $a0, 4
  # addi $a2, $a2, 1
  addi $a2, $a2, 1

  lw $s0, 0($sp)
  lw $s1, 4($sp)
  addi $sp, $sp, 8

  # bne $a2, $a3, inner_loop
  bne $a2, $a3, inner_loop
  # beq $a2, $a3, terminate

  move $t0, $a0
  la $a0, exit
  li $v0, 4
  syscall
  move $a0, $t0

  jr $ra

# terminate:
  # li $v0, 4
  # la $a0, exit7
  # syscall
  # jr $ra

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
  # registers used: a0, a1, ra
  sub $sp, $sp, 12
  sw $s0, 0($sp)
  sw $s1, 4($sp)
  sw $ra, 8($sp)

  # Allocate sufficient space and set return value
  mul $a0, $a0, $a1
  sll $a0, $a0, 2
  li $v0, 9
  syscall

  # restore stack, ra, and any saved registers, if necessary
  lw $s0, 0($sp)
  lw $s1, 4($sp)
  lw $ra, 8($sp)
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
  # do nested loops to print out values
  # move $v0, $a2
  li $a3, 0
  jal print_rows

  # restore stack, ra, and any saved registers, if necessary
  lw $ra, 0($sp)
  addi $sp, $sp, 4

  # return to main
  jr  $ra

################################################################################
# print_rows: Output the formatted contents of NxM matrix
# Note: Within a row, separate each value by tab
#       End each row with a newline
# Input:
#   a0 = N
#   a1 = M
#   a2 = base pointer
#   a3 = number of iterations of loop
# Return:
#   None
################################################################################
print_rows:
  sub $sp, $sp, 4
  sw $ra, 0($sp)
  move $s0, $a3
  li $a3, 0
  jal print_cols
  move $a3, $s0
  # move $t0, $a0
  li $v0, 4
  la $a0, newline
  syscall
  # move $a0, $t0
  addi $a3, $a3, 1
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  bne $a0, $a3, print_rows
  jr $ra

################################################################################
# print_cols: Output the formatted contents of NxM matrix
# Note: Within a row, separate each value by tab
#       End each row with a newline
# Input:
#   a0 = N
#   a1 = M
#   a2 = base pointer
#   a3 = number of iterations of loop
# Return:
#   None
################################################################################
print_cols:
  sub $sp, $sp, 4
  sw $a0, 0($sp)

  # calculating offset
  li $v0, 1
  la $a0, 0($a2)
  syscall
  li $v0, 4
  la $a0, tab
  syscall
  addi $a2, $a2, 4

  lw $a0, 0($sp)
  addi $sp, $sp, 4

  bne $a1, $a3, print_cols

  jr $ra

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
mm_mult:
  # save return address and any saved registers on the stack, if necessary
  lw $s0, 0($sp)
  sub $sp, $sp, 20
  lw $ra, 0($sp)
  lw $a0, 4($sp)
  lw $a1, 8($sp)
  lw $a2, 12($sp)
  lw $a3, 16($sp)

  # set up and call mm_alloc for result matrix
  move $a1, $a3
  jal mm_alloc
  sw $a0, 4($sp)
  sw $a1, 8($sp)
  sw $a2, 12($sp)
  sw $a3, 16($sp)
  addi $sp, $sp, 16

  # three nested loops for matrix-matrix multiplication
  li $s1, 0
  jal loop_rows

  # set return value

  # restore stack, ra, and any saved registers, if necessary
  sw $ra, 0($sp)
  addi $sp, $sp, 4

  # return to main
  jr  $ra

################################################################################
# loop_rows: Loop over the rows
# Input:
#   a0 = N for matrix 1
#   a1 = M for matrix 1 <-> N for matrix 2
#   a2 = base address for matrix 1
#   a3 = M for matrix 2
#   s0 = base address for matrix 2
#   s1 = number of iterations of loop rows
#   v0 = base address for result matrix
#   
# Return:
#   None
################################################################################
loop_rows:
  sub $sp, $sp, 12
  lw $ra, 0($sp)
  lw $s0, 4($sp)
  lw $s1, 8($sp)
  li $s2, 0
  jal loop_cols
  addi $s1, $s1, 1
  sw $ra, 0($sp)
  sw $s0, 4($sp)
  sw $s1, 8($sp)
  addi $sp, $sp, 12
  bne $s1, $a0, loop_rows
  jr $ra
  

################################################################################
# loop_cols: Loop over the columns
# Input:
#   a0 = N for matrix 1
#   a1 = M for matrix 1 <-> N for matrix 2
#   a2 = base address for matrix 1
#   a3 = M for matrix 2
#   s0 = base address for matrix 2
#   s1 = number of iterations of loop rows
#   s2 = number of iterations of loop cols
#   v0 = base address for result matrix
# Return:
#   None
################################################################################
loop_cols:
  sub $sp, $sp, 16
  lw $ra, 0($sp)
  lw $s0, 4($sp)
  lw $s1, 8($sp)
  lw $s2, 12($sp)
  li $s3, 0
  li $v1, 0
  jal loop_sum_answer
  # calculating offset
  # sll $t0, $s1, 2
  # add $t0, $t0, $s2
  # mul $t0, $t0, 4
  # sw $t0($v0), $v1
  sll $t0, $a3, 2
  mul $t0, $t0, $s1
  mul $t1, $s2, 4
  add $t2, $t0, $t1
  add $v0, $v0, $t2
  move $v0, $v1
  addi $s2, $s2, 1
  sw $ra, 0($sp)
  sw $s0, 4($sp)
  sw $s1, 8($sp)
  sw $s2, 12($sp)
  addi $sp, $sp, 16
  bne $s2, $a3, loop_cols
  jr $ra

################################################################################
# loop_sum_answer: Calculate the answer
# Input:
#   a0 = N for matrix 1
#   a1 = M for matrix 1 <-> N for matrix 2
#   a2 = base address for matrix 1
#   a3 = M for matrix 2
#   s0 = base address for matrix 2
#   s1 = number of iterations of loop rows
#   s2 = number of iterations of loop cols
#   s3 = number of iterations of loop_sum_answer
#   v0 = base address for result matrix
# Return:
#   v1 = answer
################################################################################
loop_sum_answer:
  sub $sp, $sp, 16
  lw $s0, 0($sp)
  lw $s1, 4($sp)
  lw $s2, 8($sp)
  lw $s3, 12($sp)
  # calculate offset for matrix 1 (i)(k)
  sll $t0, $a3, 2
  mul $t0, $t0, $s1
  mul $t1, $s3, 4
  add $t2, $t0, $t1
  add $a2, $v0, $t2
  # calculate offset for matrix 2 (k)(j)
  sll $t0, $a3, 2
  mul $t0, $t0, $s3
  mul $t1, $s2, 4
  add $t2, $t0, $t1
  add $s0, $v0, $t2
  # multiply elements of matrix 1 and matrix 2
  mul $t3, $a2, $s0
  # add them to result 
  add $v1, $v1, $t3
  addi $s3, $s3, 1
  sw $s0, 0($sp)
  sw $s1, 4($sp)
  sw $s2, 8($sp)
  sw $s3, 12($sp)
  addi $sp, $sp, 16
  beq $s3, $a1, loop_sum_answer
  jr $ra