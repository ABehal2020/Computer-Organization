
            .data

newline:    .asciiz "\n"
space:      .asciiz " "
list:       .word 12, 100, -101, 5, 0, 123456789, 18
size:       .word 7

        .text
        .globl main

main:
    la $a0, list
    lw $a1, size
    jal numodds
    
    move $t6, $v0
    li $v0, 1
    move $a0, $t6
    syscall
    
    li $v0, 10
    syscall

numodds:
    addi $sp, $sp, -4
    sw $s0, 0($sp)
    li $t0, 0
    addi $t3, $t3, 1     
loop:
    beq $t0, $a1, exit
    
    sll $t1, $t0, 2
    add $t1, $t1, $a0
    lw $t2, 0($t1)
    
    and $t4, $t2, $t3
    beq $t4, $zero, next
    addi $s0, $s0, 1
next:
    addi $t0, $t0, 1
    j loop
    
exit:
    add $v0, $s0, $zero      
    lw $s0, 0($sp)     
    addi $sp, $sp, 4
    jr $ra
    
