        .data
input_msg:  .asciiz "\nEnter an integer: \n";
odd_msg:    .asciiz "\nODD\n";
even_msg:    .asciiz "\nEVEN\n";

        .text
        .globl main
main:   sub $sp, $sp, 4
        sw $ra, 0($sp)
        li $v0, 4
        la $a0, input_msg
        syscall
        li $v0, 5
        syscall
        move $a0, $v0
        li $s0, 1
        jal isodd
        beq $v0, $s0, dispodd
        beq $v0, $zero, dispeven
        
exit:       
        lw $ra, 0($sp)
        add $sp, $sp, 4
        jr $ra

isodd:  
        and $v0, $a0, 1
        jr $ra

dispodd:    li $v0, 4
            la $a0, odd_msg
            syscall
            j exit

dispeven:   li $v0, 4
            la $a0, even_msg
            syscall
            j exit