.data
    n: .word 8
    v: .word 0, 1, 2, 3, 4, 5, 6, 7
    sp: .asciiz " "

.text
main:
    # argumentele v[], n pentru functia modifica
    subu $sp, 8
    # push {v, n}
    la $t0, v
    sw $t0, 0($sp)
    lw $t0, n
    sw $t0, 4($sp)

    jal modifica

# print array init
    li $t0, 0 # contor=0
    li $t1, 0 # index=0
    lw $t2, n

print_array_loop:
    # while(contor < n)
    beq $t0, $t2, exit

    # put(v[index])
    lw $a0, v($t1)
    li $v0, 1
    syscall

    # put(' ')
    la $a0, sp
    li $v0, 4
    syscall

    addi $t1, 4 # index += 4
    addi $t0, 1 # contor += 1
    j print_array_loop

exit:
    addu $sp, 8
    li $v0, 10
    syscall

modifica:
    # setup frame
    subu $sp, 4
    sw $fp, 0($sp)
    addi $fp, $sp, 4

    # push {$ra, $s0, $s1}
    subu $sp, 12
    sw $ra, -8($fp)
    sw $s0, -12($fp)
    sw $s1, -16($fp)

    # get args
    lw $s0, 0($fp) # int* v
    lw $s1, 4($fp) # int n

    # if (n == 0) return;
    beqz $s1, modifica_exit

    # *v = suma_patrate(*v)
    subu $sp, 4
    lw $t0, 0($s0)
    sw $t0, 0($sp)
    jal suma_patrate
    addi $sp, 4
    sw $v0, 0($s0)

    addi $s0, 4  # v += 4
    addi $s1, -1 # n -= 1

    # push args pentru apel recursiv
    subu $sp, 8
    sw $s0, 0($sp) # v
    sw $s1, 4($sp) # n
    jal modifica
    addu $sp, 8

modifica_exit:
    lw $s1, -16($fp)
    lw $s0, -12($fp)
    lw $ra, -8($fp)
    lw $fp, -4($fp)
    addu $sp, 16
    jr $ra


suma_patrate:
    # setup frame
    subu $sp, 4
    sw $fp, 0($sp)
    addi $fp, $sp, 4

    # push {$ra, $s0}
    subu $sp, 8
    sw $ra, -8($fp)
    sw $s0, -12($fp)

    # get arg
    lw $s0, 0($fp) # int num

    # if(num==1) return 0;
    ble $s0, 1, suma_patrate_ret_0

    subu $s0, 1    # num -= 1; 

    # push arg pentru apel recursiv
    subu $sp, 4
    sw $s0, 0($sp)
    jal suma_patrate
    addu $sp, 4

    mul $s0, $s0, $s0 # num *= num;
    add $v0, $v0, $s0 # return num * num + suma_patrate(num);

    j suma_patrate_exit

suma_patrate_ret_0:
    li $v0, 0
suma_patrate_exit:
    lw $s0, -12($fp)
    lw $ra, -8($fp)
    lw $fp, -4($fp)
    addu $sp, 12
    jr $ra
