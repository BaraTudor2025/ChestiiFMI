
.data
	p: .space 4
	generator: .space 4
	in_cript:    .space 20
	in_decript:  .space 20
	out_cript:   .space 20
	out_decript: .space 20

	char_endl: .ascii "\n"

	msg_err_prim: .asciiz "'P' nu este prim"
	msg_gen:      .asciiz "Generatorul este: "
	msg_cript:    .asciiz "\nMesajul criptat: "
	msg_decript:  .asciiz "\nMesajul decriptat: "

.text
main:
# CITIM P
	li $v0, 5 
	syscall	
	sw $v0, p

# TEST DACA P ESTE PRIM

	# cazul 3, 0, 1, par
	lw $t0, p
	beq $t0, 3, read_msg
	beq $t0, 2, read_msg
	beqz $t0, exit_err_prim
	beq $t0, 1, exit_err_prim
	andi $t0, $t0, 1
	beqz $t0, exit_err_prim

	# cazul general
	li $t0, 3               # t0 = divizor
	lw $t1, p               # t1 = p
	div $t2, $t1, 2         # t2 = sentinel(p/2)
loop_prim:
	rem $t3, $t1, $t0       # t3 = p % div
	beqz $t3, exit_err_prim # nu e prim
	bge $t0, $t2, read_msg  # am depasit sentinela si trecem la citirea mesajelor
	addi $t0, $t0, 2        # div += 2
	j loop_prim

	
# CITIRE MESAJE
read_msg:
	# citim cuvantul ce va fi criptat
	li $v0, 8
	la $a0, in_cript
	li $a1, 20
	syscall
	
	# citim cuvantul ce va fi decriptat
	li $v0, 8
	la $a0, in_decript
	li $a1, 20
	syscall


# CAUTAM GENERATORUL IN RANGE-UL (2, p-1)
	lw $t0, p       # folosit pentru (mod p) si ca sentinela 
	li $t1, 2       # generatorul care va fi testat 
	beq $t0, $t1, print_gen # caz special, cand p=2 atunci g=2
loop_gen:
	li $t2, 1       # (elem) elementul din generator
	li $t3, 0       # (power) puterea elementului

	loop_power:
		# avansam la urmatorul element din generator
		addi $t3, $t3, 1       # power += 1
		mul $t2, $t2, $t1      # elem *= g
		rem $t2, $t2, $t0      # elem %= p
		bne $t2, 1, loop_power # if(elem != 1)
	
	# daca generatorul a fost gasit atunci iesim din loop
	move $t4, $t0           # t4 = copie 'p'
	sub $t4, $t4, 1         # copie 'p' = p-1
	beq $t3, $t4, print_gen	# if (power == p-1)

	add $t1, $t1, 1	        # g += 1
	blt $t1, $t0, loop_gen  # if (g < p)

print_gen:
	# salvam generatorul
	sw $t1, generator

	li $v0, 4
	la $a0, msg_gen
	syscall

	li $v0, 1
	lw $a0, generator
	syscall


# (DE)CRIPTARE
	# se vor genera din nou puterile generatorului
	lw $t0, p
	lw $t1, generator
	li $t2, 1         # elem
	li $t3, 0         # power
	lw $t8, p
	sub $t8, $t8, 1   # t8 = p-1
loop_procesare:
	li $t4, 0         # indexul in mesaj
	lb $t6, char_endl # sentinela
	loop_cript:
		lb $t5, in_cript($t4)        # t5 = in_cript[i]
		beq $t5, $t6, end_loop_cript # am terminat sirul
		sub $t5, $t5, 65             # scadem codul ascii si ramane indexul din alfabet(operatia asta apare de mai multe ori)
		bne $t3, $t5, else_cript     # if(power == in_cript[i])
		add $t2, $t2, 65             # adunam codul ascii la 'elem' pentru conversie
		sb $t2, out_cript($t4)       # out_cript[i] = elem
		sub $t2, $t2, 65             # restauram 'elem'
	else_cript:
		add $t4, $t4, 1              # i += 1
		j loop_cript

	end_loop_cript:
		li $t4, 0     # index
		lb $t6, char_endl 
	loop_decript:
		lb $t5, in_decript($t4)        # t5 = in_cript[i]
		beqz $t5, end_loop_decript     # am terminat sirul
		beq $t5, $t6, end_loop_decript # am terminat sirul
		sub $t5, $t5, 65
		bne $t2, $t5, else_decript     # if(elem == in_cript[i])
		add $t3, $t3, 65
		sb $t3, out_decript($t4)       # out_cript[i] = power
		sub $t3, $t3, 65
	else_decript:
		add $t4, $t4, 1   # i += 1
		j loop_decript
	end_loop_decript:


	# avansam la urmatorul element din generator
	addi $t3, $t3, 1           # power += 1
	mul $t2, $t2, $t1          # elem *= g
	rem $t2, $t2, $t0          # elem %= p
	bne $t3, $t8, loop_procesare # if(power != p-1)

print_cript:
	# afisarea mesajului care s-a criptat
	li $v0, 4
	la $a0, msg_cript
	syscall
	
	li $v0, 4
	la $a0, out_cript
	syscall

	# afisarea mesajului care s-a decriptat
	li $v0, 4
	la $a0, msg_decript
	syscall
	
	li $v0, 4
	la $a0, out_decript
	syscall

	j exit

	

exit_err_prim:
	li $v0, 4
	la $a0, msg_err_prim 
	syscall
	j exit

exit:
	li $v0, 10
	syscall
	
	