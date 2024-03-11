# {
# int x = 2;
# if ( x < 7 )
# {
# x -= 4;
# }

      .data
bef:  .asciiz "\nBefore: "
aft:  .asciiz "\nAfter: "

      .text
      .globl main
main: li    $s0, 8
      li    $t0, 7
      li    $v0, 4
      la    $a0, bef
      syscall
      li    $v0, 1
      move  $a0, $s0
      syscall
      blt   $s0, $t0, update

finish:
      li    $v0, 4
      la    $a0, aft
      syscall
      li    $v0, 1
      move  $a0, $s0
      syscall
      jr    $ra

update: addi    $s0, $s0, -4
        j finish