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

    # find offset of mat1[i][k]
    mul $t8, $s1, $t2
    add $t8, $t8, $t4
    sll $t8, $t8, 2
    add $t8, $t8, $s4      

    # find offset of mat2[k][j]
    mul $t7, $s2, $t4
    add $t7, $t7, $t3
    sll $t7, $t7, 2
    add $t7, $t7, $t1                  

    # get answer: mat1[i][k]*mat2[k][j]
    mul $t7, 0($t8), 0($t7)
    add $t6, $t6, $t7
    
    addi $t4, $t4, 1
    j a_loop
    

a_done:
    # resultMatrix[i][j]
    mul $t9, $s2, $t2
    add $t9, $t9, $t3
    sll $t9, $t9, 2
    add $t9, $t9, $t0              

    sw $t6, 0($t9) # storing answer in resultMatrix
    addi $t3, $t3, 1
    
    j c_loop

c_done:
    addi $t2, $t2, 1
    j r_loop

r_done:
    # saving base address of result matrix in $v0
    move $v0, $t0 
    lw $ra, 0($sp)
    lw $s0, 4($sp)
    lw $s1, 8($sp)
    lw $s2, 12($sp)
    lw $t0, 16($sp)
    lw $s4, 20($sp)
    addi $sp, $sp 24
    jr  $ra
