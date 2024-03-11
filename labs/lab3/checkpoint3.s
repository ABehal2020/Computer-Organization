        .data
input_msg:  .asciiz "\nEnter two positive integers that are each less than 32768:\n";
output_msg: .asciiz "\nThe gcd of those two positive integers is: ";

        .text
        .globl main
main:   li $v0, 4
        la $a0, input_msg
        syscall
        li $v0, 5
        syscall
        move $t0, $v0
        li $v0, 5
        syscall 
        move $t1, $v0
        j gcd

gcd:    bne $t0, $t1, loop
        li $v0, 4
        la $a0, output_msg
        syscall
        li $v0 1    
        move $a0, $t0
        syscall
        jr $ra    

loop:   bgt $t0, $t1, greater
        sub $t1, $t1, $t0
        j gcd

greater:        sub $t0, $t0, $t1
                j gcd
                jr $ra