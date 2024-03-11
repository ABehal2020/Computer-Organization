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
    addi $sp, $sp, -12
    sw $ra, 0($sp)
    sw $s0, 4($sp)
    sw $s1, 8($sp)

  # get N
    li $v0, 5
    syscall
    move $a0, $v0
  # get M
    li $v0, 5
    syscall
    move $a1, $v0

    move $s0, $a0
    move $s1, $a1
  # Setup up arguments and call mm_alloc - v0 is returned as base address
    jal mm_alloc
    move $t0, $v0
    move $t5, $v0
    lw $ra 0($sp)
  # do nested loops to read in values

    li $t1, 0
row_loop:
    beq $t1, $s0, row_done
    li $t2, 0
col_loop:
    beq $t2, $s1, col_done

    li $v0, 5
    syscall
    sw $v0, 0($t0)
    addi $t0, $t0, 4
    addi $t2, $t2, 1

    j col_loop
    
col_done:
    addi $t1, $t1, 1
    j row_loop

row_done:
    move $v0, $s0
    move $v1, $s1
    
    lw $ra, 0($sp)
    lw $s0, 4($sp)
    lw $s1, 8($sp)
    addi $sp, $sp, 12

    sw $t5, 0($sp)

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
    mul $t0, $a0, $a1
    
    li $v0, 9
    sll $a0, $t0, 2
    syscall
    
  # Allocate sufficient space

  # set return value

  # restore stack, ra, and any saved registers, if necessary

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
    li $t0, 0
    move $t2, $a2
    move $t3, $a0

row_print:
    beq $t0, $t3, finish_row
    li $t1, 0

col_print:
    beq $t1, $a1, finish_col
    
    li $v0, 1
    lw $a0, 0($t2)
    syscall
    
    li $v0, 4
    la $a0, tab
    syscall

    addi $t2, $t2, 4
    addi $t1, $t1, 1
    j col_print

finish_col:
    li $v0, 4
    la $a0, newline
    syscall
    
    addi $t0, $t0, 1
    j row_print

finish_row:
    move $a0, $t3
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
mm_mult:
  # save return address and any saved registers on the stack, if necessary
    lw $t1, 0($sp)         #base matrix 2

    addi $sp, $sp -24
    sw $ra, 0($sp)
    sw $s0, 4($sp)
    sw $s1, 8($sp)
    sw $s2, 12($sp)
    sw $s3, 16($sp)
    sw $s4, 20($sp)

    move $s0, $a0       #row matrix 1
    move $s1, $a1       #col matrix 1 + row matrix 2
    move $s2, $a3       #col matrix 3
    move $s4, $a2       #base of matrix 1

    move $a1, $a3       #a1 - matrix col of matrix 2
    jal mm_alloc
    move $s3, $v0       #s3 - base of result matrix

    li $t2, 0           #i
loop_i:
    beq $t2, $s0, i_done    # i compared with matrix 1 row
    li $t3, 0               #j

loop_j:
    beq $t3, $s2, j_done    # j compared with matrix 2 col
    li $t4, 0               #k
    li $t6, 0               #sum

loop_k:
    beq $t4, $s1, k_done    # k compared with matrix 1 col

    mul $t7, $s1, $t2
    add $t7, $t7, $t4
    sll $t7, $t7, 2
    add $t7, $t7, $s4           #t7 - offset of matrix 1

    mul $t8, $s2, $t4
    add $t8, $t8, $t3
    sll $t8, $t8, 2
    add $t8, $t8, $t1           #t8 - offset of matrix 2
    
    lw $t0, 0($t7)              #t0 - value of matrix 1 from the offset
    lw $t5, 0($t8)              #t5 - value of matrix 2 from the offset

    mul $t7, $t0, $t5
    add $t6, $t6, $t7
    
    addi $t4, $t4, 1
    j loop_k
    

k_done:
    mul $t9, $s2, $t2
    add $t9, $t9, $t3
    sll $t9, $t9, 2
    add $t9, $t9, $s3               #t9 offset value of result

    sw $t6, 0($t9)              #t6 - sum
    addi $t3, $t3, 1
    
    j loop_j

j_done:
    addi $t2, $t2, 1
    j loop_i

i_done:
    
    move $v0, $s3           #s3 - base of matrix result

    lw $ra, 0($sp)
    lw $s0, 4($sp)
    lw $s1, 8($sp)
    lw $s2, 12($sp)
    lw $s3, 16($sp)
    lw $s4, 20($sp)
    addi $sp, $sp 24

  jr  $ra
