// TemeFMI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

const int n = 4;

const int inCript[n] = {0, 2, 0, 3};
int outCript[n];

const int inDecript[n] = {1, 2, 1, 6};
int outDecript[n];

//const char* alfabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

#if 0
int schita_tema_1_asc()
{
	int p = 7;

	if (p == 0 || p == 1 || !(p & 1))
		goto EXIT_PRIM;

	for (int d = 3; d < p / 2; d += 2)
		if (p % d == 0)
			goto EXIT_PRIM;

	for (int g = 2; g < p; g++) {
		//int inCript[], inDecript[], outCript[], outDecript[];
		int criptSize, decriptSize;
		//int p, g;

		int elem = 1;
		int power = 0;
		do {
			for (int i = 0; i < criptSize; i++) {
				if (inCript[i] == power) {
					outCript[i] = elem;
				}
			}
			for (int i = 0; i < decriptSize; i++) {
				if (inDecript[i] == elem) {
					outDecript[i] = power;
				}
			}
			power += 1;
			elem *= g;
			elem %= p;
		} while (elem != 1);

		if (power == p - 1) {
			std::printf("gen: %d\n", g);
			goto PRINT;
		}
	}

PRINT:

	std::printf("\ncript: \n");
	for (int e : outCript)
		std::cout << e << ' ';

	std::printf("\ndecript: \n");
	for (int e : outDecript)
		std::cout << e << ' ';

	return 0;

EXIT_PRIM:
	std::cout << "p nu e prin";
	return 0;

}
#endif

/*

.data
	p: .space 4
	generator: .space 4
	n_cript:      .word 20
	n_decript:    .word 20
	text_cript:   .space 20
	text_decript: .space 20

	msg_err_prim: .asciiz "'P' nu este prim"
	msg_gen:      .asciiz "Generatorul g este: "
	msg_print_cript:    .asciiz "Mesajul criptat: "
	msg_print_decript:  .asciiz "Mesajul decriptat"
	alfabet:      .asciiz "ABCDEFGHIJKLMNOPQRSTUVWXYZ"


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
	li $t0, 3		# t0 = divizor
	lw $t1, p		# t1 = p
	div $t2, $t1, 2		# t2 = sentinel(p/2)
loop_prim:
	rem $t3, $t1, $t0		# t3 = p % div
	beqz $t3, exit_err_prim 	# nu e prim
	bge $t0, $t2, read_msg		# am depasit sentinela si trecem la citirea stringurilor
	addi $t0, $t0, 2
	j loop_prim



# CITIRE MESAJE
read_msg:
	# citim cuvantul ce va fi criptat
	li $v0, 8
	la $a0, text_cript
	lw $a1, n_cript
	syscall

	# citim cuvantul ce va fi decriptat
	li $v0, 8
	la $a0, text_decript
	lw $a1, n_decript
	syscall


# CALNULAM STRLEN
	#la $t0, text_cript
loop_cript_msg:
	li $t1, 0
	#beq ($t1)
	#beq , , loop_cript_msg



# AFLAM GENERATORUL
	lw $t0, p	# p, folosit pentru (mod p) si sentinela
	li $t1, 2	# generatorul
loop_gen:
	li $t2, 1	# elementul din generator(elem)
	li $t3, 0	# puterea elementului (pow)

	loop_pow:
		# avansam la urmatorul element din generator
		addi $t3, $t3, 1	# pow += 1
		mul $t2, $t2, $t1	# elem *= g
		rem $t2, $t2, $t0	# elem %= p
		bne $t2, 1, loop_pow	# do{...} while(elem != 1)

	# daca generatorul a fost gasit atunci iesim din loop
	move $t4, $t0
	sub $t4, $t4, 1
	sw $t1, generator
	beq $t3, $t4, print_gen	# if (pow == p-1) goto print_gen

	add $t1, $t1, 1		# g += 1
	blt $t1, $t0, loop_gen	# while(g < p)


print_gen:
	li $v0, 4
	la $a0, msg_gen
	syscall

	li $v0, 1
	lw $a0, generator
	syscall
	j exit


# (DE)CRIPTARE
	lw $t0, p	# p
	lw $t1, generator
	li $t2, 1	# elem
	li $t3, 0	# pow
loop_gen_pow:

	li $t4, 0	# index
	loop_cript:
		lb $t5, text_cript($t4) # t5 = alfabet[i]
		sub $t5, $t5, 65     # indexul in alfabet
		add $t4, $t4, 1
		lb $t6, n_cript
		blt $t4, $t6, loop_cript #TODO: poate fac 'ble'


	loop_decript:


	# avansam la urmatorul element din generator
	addi $t3, $t3, 1	# pow += 1
	mul $t2, $t2, $t1	# elem *= g
	rem $t2, $t2, $t0	# elem %= p
	bne $t2, 1, loop_pow	# do{...} while(elem != 1)



loop_print_cript:
loop_print_decript:
	j exit



exit_err_prim:
	li $v0, 4
	la $a0, msg_err_prim
	syscall
	j exit

exit:
	li $v0, 10
	syscall



*/




/*

.data
	p: .space 4
	generator: .space 4
	in_cript:   .space 20
	in_decript: .space 20
	out_cript:    .space 20
	out_decript:  .space 20

	char_endl: .ascii "\n"

	msg_err_prim: .asciiz "'P' nu este prim"
	msg_gen:      .asciiz "Generatorul g este: "
	msg_cript:    .asciiz "\nMesajul criptat: "
	msg_decript:  .asciiz "\nMesajul decriptat: "
	#alfabet:      .asciiz "ABCDEFGHIJKLMNOPQRSTUVWXYZ"


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
	li $t0, 3		# t0 = divizor
	lw $t1, p		# t1 = p
	div $t2, $t1, 2		# t2 = sentinel(p/2)
loop_prim:
	rem $t3, $t1, $t0		# t3 = p % div
	beqz $t3, exit_err_prim 	# nu e prim
	bge $t0, $t2, read_msg		# am depasit sentinela si trecem la citirea stringurilor
	addi $t0, $t0, 2
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
	lw $t0, p	# folosit pentru (mod p) si ca sentinela
	li $t1, 2	# generatorul
loop_gen:
	li $t2, 1	# elementul din generator(elem)
	li $t3, 0	# puterea elementului (pow)

	loop_pow:
		# avansam la urmatorul element din generator
		addi $t3, $t3, 1	# pow += 1
		mul $t2, $t2, $t1	# elem *= g
		rem $t2, $t2, $t0	# elem %= p
		bne $t2, 1, loop_pow	# do{...} while(elem != 1)

	# daca generatorul a fost gasit atunci iesim din loop
	move $t4, $t0   # copie 'p'
	sub $t4, $t4, 1 # copie 'p' = p-1
	beq $t3, $t4, print_gen	# if (pow == p-1) goto print_gen

	add $t1, $t1, 1		# g += 1
	blt $t1, $t0, loop_gen	# while(g < p)

print_gen:
	sw $t1, generator # salvam generatorul
	li $v0, 4
	la $a0, msg_gen
	syscall

	li $v0, 1
	lw $a0, generator
	syscall


# (DE)CRIPTARE
	lw $t0, p	# p
	lw $t1, generator
	li $t2, 1	# elem
	li $t3, 0	# pow
loop_procesare:

	li $t4, 0	# index
	lb $t6, char_endl # sentinela
	loop_cript:
		lb $t5, in_cript($t4) # t5 = in_cript[i]
		beq $t5, $t6, end_loop_cript
		sub $t5, $t5, 65
		bne $t3, $t5, else_cript # if(pow == in_cript[i])
		add $t2, $t2, 65
		sb $t2, out_cript($t4)   # out_cript[i] = elem
		sub $t2, $t2, 65
	else_cript:
		add $t4, $t4, 1		# i += 1
		j loop_cript

	end_loop_cript:
		li $t4, 0
		lb $t6, char_endl
	loop_decript:
		lb $t5, in_decript($t4) # t5 = in_cript[i]
		beqz $t5, end_loop_decript
		beq $t5, $t6, end_loop_decript
		sub $t5, $t5, 65	# t5 = indexul din alfabet
		bne $t2, $t5, else_decript # if(elem == in_cript[i])
		add $t3, $t3, 65
		sb $t3, out_decript($t4)   # out_cript[i] = pow
		sub $t3, $t3, 65
	else_decript:
		add $t4, $t4, 1		# i += 1
		j loop_decript
	end_loop_decript:


	# avansam la urmatorul element din generator
	addi $t3, $t3, 1	# pow += 1
	mul $t2, $t2, $t1	# elem *= g
	rem $t2, $t2, $t0	# elem %= p
	bne $t2, 1, loop_procesare	# do{...} while(elem != 1)

print_cript:
	li $v0, 4
	la $a0, msg_cript
	syscall

	li $v0, 4
	la $a0, out_cript
	syscall

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


*/



