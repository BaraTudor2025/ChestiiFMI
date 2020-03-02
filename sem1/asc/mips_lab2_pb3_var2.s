.data
	n: .space 4
	sp: .asciiz " "
.text
main:
	#citeste n
	li $v0, 5
	syscall
	sw $v0, n

	lw $t0, n
	li $t1, 0 #i

loop:
	bge $t1, $t0, exit
	addi $t1, $t1, 1
	li $v0, 5
	syscall
	move $t2, $v0
	and $t3, $t2, 1
	bne $t3, 0, loop
# print
	li $v0, 1
	move $a0, $t2
	syscall

	li $v0, 4
	la $a0, sp
	syscall
	j loop

exit:
	li $v0, 10
	syscall
