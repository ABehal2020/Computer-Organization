# num = 4
# for (i = 0; i < num; ++i)
#   print arr[0]
#   x = arr[0]
#   x = x + 2
#   arr[0] = x
#   print arr[0]
# return



      .data
bef:  .asciiz "\nBefore: "
aft:  .asciiz "\nAfter: "
arr:  .word 1, 2, 3, 4

      .text
      .globl main
main: li    $s0, 4            # num = 4
      li    $s1, 0            # i = 0
      la    $s2, arr          # base address of arr
loop: slt   $t0, $s1, $s0     # t1 = 1 if i < num
      beq   $t0, $zero, done  # break loop if !(i < num)
      sll   $t1, $s1, 2       # j = i *= 4
      add   $t2, $s2, $t1     # t2 = address of arr[i]
      lw    $t3, 0($t2)       # x = arr[i] = t3

      # print x before inc
      li    $v0, 4            # setup print_str syscall
      la    $a0, bef          # load bef string as argument
      syscall
      li    $v0, 1            # setup print_int syscall
      move  $a0, $t3          # load x as argument
      syscall

      addi  $t3, $t3, 2       # x = x + 2
      sw    $t3, 0($t2)       # arr[i] = x 

      # print x after inc
      li    $v0, 4            # setup print_str syscall
      la    $a0, aft          # load bef string as argument
      syscall
      li    $v0, 1            # setup print_int syscall
      move  $a0, $t3          # load x as argument
      syscall

      addi  $s1, $s1, 1       # ++i
      j loop                  # jump to start of loop

done: jr $ra                  # return to caller
