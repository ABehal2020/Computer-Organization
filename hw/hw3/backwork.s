################################################################################
# Homework 3: Matrix-matrix multiplication
# CSCI-2500 Spring 2021
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
  .globl print_cols
  .globl print_row
  .globl iterate_rows
  .globl iterate_col_sum
  .globl iterate_dot_product

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
    # needed regs s0, t0, t1, ra, s1, s2
    # t6 is overwritten by this function
    sub $sp, $sp, 24
    sw $s0, 20($sp)
    sw $t0, 16($sp)
    sw $t1, 12($sp)
    sw $ra, 8($sp)
    sw $s1, 4($sp)
    sw $s2, 0($sp)

    # get N
    li $v0, 5
    syscall

    move $s1, $v0

    # get M
    li $v0, 5
    syscall

    move $s0, $v0

    

    # Setup up arguments and call mm_alloc - v0 is returned as base address
    move $a1, $s0
    move $a0, $s1
    jal mm_alloc
    move $t6, $v0 # start of mat
    move $s1, $a0 # n 
    move $s2, $a1 # m

    # do nested loops to read in values


    add $a0, $zero, $s1
    mult $a0, $s2 # multiply n * m
    mflo $a0
    
    add $a1, $zero, $t6 
    li $a2, 0


    jal read_loop

    # setup up return values
    # Note: third return value goes on the stack *after* restoration below

    move $v0, $s1
    move $v1, $s2

    # restore stack, ra, and any saved registers, if necessary
    lw $s0, 20($sp)
    lw $t0, 16($sp)
    lw $t1, 12($sp)
    lw $ra, 8($sp)
    lw $s1, 4($sp)
    lw $s2, 0($sp)
    addi $sp, $sp, 24


    sw $t6, 0($sp)
    # return to main
    jr  $ra

################################################################################
# read_loop: Read all elements of the matrix
# Input:
#   a0 = total length
#   a1 = address to write to 
#   a2 = iterations
# Return:
#   None
################################################################################
read_loop:
    # t0, v0, 
    move $t0, $a0

    li $v0, 5
    syscall

    sw $v0, 0($a1)
    move $a0, $t0

    addi $a1, $a1, 4

    addi $a2, $a2, 1

    bne $a2, $a0, read_loop

    jr $ra

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
    # needed regs: ra, a0, a1
    sub $sp, $sp, 12
    sw $ra, 8($sp)
    sw $a0, 4($sp)
    sw $a1, 0($sp)


    # Allocate sufficient space
    mul $a0, $a0, 4
    mul $a0, $a0, $a1
    li $v0, 9
    syscall # v0 get set to matrix pointer
    # set return value

    # restore stack, ra, and any saved registers, if necessary
    lw $ra, 8($sp)
    lw $a0, 4($sp)
    lw $a1, 0($sp)
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
    # SAVE: ra, v0
    sub $sp, $sp, 8
    sw $ra, 4($sp)
    sw $v0, 0($sp)
    # do nested loops to print out values

    move $v0, $a2
    jal print_cols

    # restore stack, ra, and any saved registers, if necessary

    # return to main
    lw $ra, 4($sp)
    lw $v0, 0($sp)
    addi $sp, $sp, 8
    jr  $ra

################################################################################
# mm_print: Output the formatted contents of NxM matrix
# Note: Within a row, separate each value by tab
#       End each row with a newline
# Input:
#   a0 = cols to print
#   a1 = M
#   a2 = base pointer
# Return:
#   None
################################################################################
print_cols:
    # SAVE: ra, s0, s1 
    sub $sp, $sp, 12
    sw $ra, 8($sp)
    sw $s0, 4($sp)
    sw $s1, 0($sp)


    sub $a0, $a0, 1

    move $s0, $a0
    move $s1, $a1 

    add $a0, $zero, $s1 
    li $a1, 0

    jal print_row

    move $a0, $s0
    move $a1, $s1

    lw $ra, 8($sp)
    lw $s0, 4($sp)
    lw $s1, 0($sp)
    addi $sp, $sp, 12

    bne $a0, $zero, print_cols


    jr $ra


################################################################################
# print_row: Output the formatted contents of NxM matrix
# Note: Within a row, separate each value by tab
#       End each row with a newline
# Input:
#   a0 = len
#   a1 = iterations
#   a2 = base pointer
# Return:
#   v0 = pointer after iteration
################################################################################
print_row:
    # print number
    move $t0, $a0
    lw $a0, 0($v0)
    move $t1, $v0
    li $v0, 1
    syscall

    # print tab
    la $a0, tab # load tab

    li $v0, 4
    syscall 

    move $a0, $t0

    move $v0, $t1 

    addi $v0, $v0, 4
    addi $a1, $a1, 1

    bne $a1, $a0, print_row

    # print new line
    move $t0, $a0
    move $t1, $v0
    la $a0, newline
    li $v0, 4
    syscall

    move $a0, $t0
    move $v0, $t1


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
    lw $t0, 0($sp)

    # save ra, s0
    sub $sp, $sp, 8
    sw $ra, 4($sp)
    sw $s0, 0($sp)

    move $s0, $t0

    # set up and call mm_alloc for result matrix
    # save all of the arguments
    # a0, a1, a2, a3

    sub $sp, $sp, 16
    sw $a0, 12($sp)
    sw $a1, 8($sp)
    sw $a2, 4($sp)
    sw $a3, 0($sp)

    # move $a0, $a0   # move n 
    move $a1, $a3   # move m
    jal mm_alloc # returns v0

    # restore arguments
    lw $a0, 12($sp)
    lw $a1, 8($sp)
    lw $a2, 4($sp)
    lw $a3, 0($sp)
    addi $sp, $sp, 16
 
    # three nested loops for matrix-matrix multiplication

    # resave args + v0 
    sub $sp, $sp, 20
    sw $a0, 16($sp)
    sw $a1, 12($sp)
    sw $a2, 8($sp)
    sw $a3, 4($sp)
    sw $v0, 0($sp)

    # # set args
    sub $sp, $sp, 12
    sw $a0, 8($sp)
    sw $a3, 4($sp)
    sw $s0, 0($sp)
    li $a0, 0
    move $a3, $v0


    jal iterate_rows

    addi $sp, $sp, 12
    # restore args
    # set return value
    lw $a0, 16($sp)
    lw $a1, 12($sp)
    lw $a2, 8($sp)
    lw $a3, 4($sp)
    lw $v0, 0($sp)
    addi $sp, $sp, 20


    # restore stack, ra, and any saved registers, if necessary
    lw $ra, 4($sp)
    lw $s0, 0($sp)
    addi $sp, $sp, 8

    # return to main
    jr  $ra

################################################################################
# mm_mult: Perform the actual NxM matrix multiplication
# Input:
#   a0 = itrs (rows)
#   a1 = M for matrix 1 <-> N for matrix 2
#   a2 = base address for matrix 1
#   a3 = address of new matrix
#   0($sp) = base address for matrix 2
#   4($sp) = m of matrix 2
#   8($sp) = n mat 1 <-> n output mat
# Return:
#   none
################################################################################
iterate_rows:
    lw $t0, 0($sp)
    lw $t1, 4($sp)

    # save regs: ra, s0, s1
    sub $sp, $sp, 12
    sw $ra, 8($sp)
    sw $s0, 4($sp)
    sw $s5, 0($sp)

    move $s0, $t0
    move $s5, $t1

    # set args
    move $v0, $a3 
    li $a3, 0 

    sw $s0, 0($sp)
    sw $s5, 4($sp)   
    jal iterate_col_sum
    move $a3, $v0

    addi $a0, $a0, 1

    move $t0, $s0
    move $t1, $s5

    # restore regs
    lw $ra, 8($sp)
    lw $s0, 4($sp)
    lw $s5, 0($sp)
    addi $sp, $sp, 12

    sw $t0, 0($sp)
    sw $t1, 4($sp)

    lw $t2, 8($sp)

    bne $a0, $t2, iterate_rows

    jr $ra

################################################################################
# mm_mult: Perform the actual NxM matrix multiplication
# Input:
#   a0 = row
#   a1 = M for matrix 1 <-> N for matrix 2
#   a2 = base address for matrix 1
#   a3 = itrs (cols)
#   0($sp) = base address for matrix 2
#   4($sp) = m of matrix 2
# Return:
#   v0 = ptr of end iteration
################################################################################
iterate_col_sum:
    lw $t0, 0($sp) # base address mat 2 
    lw $t1, 4($sp) # m of matrix 2

    # save regs: ra, s0, s1, s2, s3
    sub $sp, $sp, 20
    sw $ra, 16($sp)
    sw $s0, 12($sp)
    sw $s1, 8($sp)
    sw $s2, 4($sp)
    sw $s3, 0($sp)

    move $s0, $t0
    move $s1, $t1

    # save v0
    move $s2, $v0
    move $s3, $a3

    # set args
    li $a3, 0
    sw $s0, 0($sp)
    sw $s1, 4($sp)
    sw $s3, 8($sp)
    li $v0, 0
    jal iterate_dot_product


    mult $a0, $s1 # row * m
    mflo $t0
    add $t0, $t0, $a3 # t + col
    li $t1, 4
    mult $t0, $t1 # 4 * index
    mflo $t0
    add $t0, $t0, $s2 # address of value to write to

    # sw $v0, 0($t0)
    sw $v0, 0($s2)
    move $v0, $s2

    move $a3, $s3 # <---------------------- HERE 
    addi $a3, $a3, 1
    addi $v0, $v0, 4

    move $t0, $s0
    move $t1, $s1

    # restore regs
    lw $ra, 16($sp)
    lw $s0, 12($sp)
    lw $s1, 8($sp)
    lw $s2, 4($sp)
    lw $s3, 0($sp)
    addi $sp, $sp, 20

    sw $t0, 0($sp)
    sw $t1, 4($sp)


    bne $t1, $a3, iterate_col_sum

    jr $ra


################################################################################
# iterate_dot_product: Perform the actual NxM matrix multiplication
# Input:
#   a0 = row mat 1
#   a1 = M for matrix 1 <-> N for matrix 2
#   a2 = base address for matrix 1
#   a3 = itrs
#   0($sp) = base address for matrix 2
#   4($sp) = m of matrix 2
#   8($sp) = col mat 2
# Return:
#   v0 = sum of dot product
################################################################################
iterate_dot_product:
    lw $t0, 0($sp) # base address of mat 2
    lw $t1, 4($sp) # m of mat 2
    lw $t4, 8($sp) # col of  mat 2

    # save regs: s0, s1, s2, s3, s4
    sub $sp, $sp, 20
    sw $s0, 16($sp)
    sw $s1, 12($sp)
    sw $s2, 8($sp)
    sw $s3, 4($sp)
    sw $s4, 0($sp)

    move $s0, $t0
    move $s1, $t1
    move $s4, $t4

    mult $a0, $a1
    mflo $t0 
    add $t0, $t0, $a3 # index of mat 1

    li $t1, 4
    mult $t0, $t1
    mflo $t0
    add $t0, $t0, $a2 # mem address of mat 1 index 
    move $s2, $t0


    mult $s1, $a3
    mflo $t1
    add $t1, $t1, $s4 # index of mat 2
    li $t2, 4
    mult $t1, $t2
    mflo $t1
    add $t1, $t1, $s0 # mem address of mat 2 index
    move $s3, $t1

    lw $t0, 0($s2) # get values
    lw $t1, 0($s3)

    mult $t0, $t1
    mflo $t0

    add $v0, $v0, $t0 # add current multiplication to current dot product

    move $t0, $s0 
    move $t1, $s1
    move $t4, $s4

    # restore regs
    lw $s0, 16($sp)
    lw $s1, 12($sp)
    lw $s2, 8($sp)
    lw $s3, 4($sp)
    lw $s4, 0($sp)
    addi $sp, $sp, 20

    sw $t0, 0($sp)
    sw $t1, 4($sp)
    sw $t4, 8($sp)

    addi $a3, $a3, 1

    bne $a3, $a1, iterate_dot_product

    jr $ra