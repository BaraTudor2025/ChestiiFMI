.data
	n: .space 4
	v: .space 40
	sp: .asciiz " "
.text
main:
	li $v0, 5
	syscall
	sw $v0, n
	
	lw $t0, n
	li $t1, 0 #i
	li $t2, 0 #int_mem_offset
	
read_loop:
	bge $t1, $t0, init_print
	li $v0, 5
	syscall 
	sw $v0, v($t2)
	addi $t1, $t1, 1
	addi $t2, $t2, 4
	j read_loop

init_print:
	li $t1, 0
	li $t2, 0
	j print_loop

increment:
	addi $t1, $t1, 1
	addi $t2, $t2, 4

print_loop:
	bge $t1, $t0, exit
	lw $t3, v($t2)
	and $t4, $t3, 1
	bne $t4, 0, increment

	# print
	li $v0, 1
	move $a0, $t3
	syscall

	li $v0, 4
	la $a0, sp
	syscall
	j increment
exit:
	li $v0, 10
	syscall
