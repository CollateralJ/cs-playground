#    Description: MIPS program testing functions and recursion


.data
nArray:    .word 5, 6, 7, 8
rArray:    .word 2, 3, 3, 2
len:       .word 4

combination:.space 16

msgChoose: .asciiz " choose "
msgEqual:  .asciiz " = "
newline:    .asciiz "\n"
 
.text
.globl main
main:

li $s0, 0
lw $s1, len
li $s2, 0
# i in s0
# len in s1
# offset in s2
loopNCK:

    lw $s3, nArray($s2) # s3 holds n[i]
    lw $s4, rArray($s2) # s4 holds r[i]
    add $s2, $s2, 4 # add to index offset for grabbing next value

    # compute n!
    move $a0, $s3
    jal nFactorial
    move $s5, $a0 # s5 hold n!

    # compute (n - r)!
    sub $a0, $s3, $s4
    jal nFactorial
    move $s6, $a0 # s6 hold (n - r)!

    # compute r!
    move $a0, $s4
    jal nFactorial
    move $s7, $a0 # s7 hold r!

    # combine denominator
    mul $s6, $s6, $s7 # s6 = (n - r)! * r!. s7 now free

    # for this assignment's division, check how many times we can subtract denominator from numerator.
    li $s7, 0 # s7 will be counter/quotient
    loopDivision:

        blt $s5, $s6, endDivision # when numerator is smaller, leave loop
        sub $s5, $s5, $s6
        add $s7, $s7, 1
        j loopDivision

    endDivision:

    move $a0, $s3
    move $a1, $s4
    move $a2, $s7
    jal printNCK

    add $s0, $s0, 1 # add to i to compare with len
    blt $s0, $s1, loopNCK # loop if less than len


exit:
    li $v0, 10
    syscall



# --- functions ---


.globl nFactorial
# nFactorial function
# a0 = n before call and n! after call
nFactorial:
ble $a0, 1, endFactorial # base case

sub $sp, $sp, 8
sw $ra, 4($sp)       # push ra
sw $a0, ($sp)       # push a0

sub $a0, $a0, 1 # a0 = n - 1

# recursive call (a0 already has n - 1)
jal nFactorial

lw $a1, ($sp)       # pop the og n value into a1
lw $ra, 4($sp)       # pop ra
add $sp, $sp, 8

# a0 now has (n - 1)! and a1 has n
mul $a0, $a0, $a1 # a0 = n * (n-1)!

endFactorial:
jr $ra




.globl printNCK
# prints: "a0 choose a1 = a2\n"
printNCK:

# preserving s0, s1, s2, ra
sub $sp, $sp, 16
sw $s2, 12($sp)
sw $s1, 8($sp)
sw $s0, 4($sp)
sw $ra, ($sp)

# saving arguments
move $s0, $a0
move $s1, $a1
move $s2, $a2

# printing a0
li $v0, 1
move $a0, $s0
syscall

# printing choose
li $v0, 4
la $a0, msgChoose
syscall

# printing a1
li $v0, 1
move $a0, $s1
syscall

# printing equal
li $v0, 4
la $a0, msgEqual
syscall

# printing a2
li $v0, 1
move $a0, $s2
syscall

# printing \n
li $v0, 4
la $a0, newline
syscall

# restoring s0, s1, s2, ra
lw $ra, ($sp)
lw $s0, 4($sp)
lw $s1, 8($sp)
lw $s2, 12($sp)
add $sp, $sp, 16

jr $ra