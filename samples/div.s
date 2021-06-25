main:
    addi	a0, zero, 8
    addi	a1, zero, 2
    jal   mydiv

    li	a7, 1
    ecall
    li	a7, 10			 # calls exit command (code 10)
    eCALL				     # end of program

## Divides two numbers, storing integer result on t0 and rest on t1
# a0	Number we will divide
# a1	Number we will divide for
mydiv:
    add   t1, zero, zero	# i = 0

mydiv_test:
    slt	  t0, a0, a1		# if ( a < b )
    bne	  t0, zero, mydiv_end	# then get out of here
    sub	  a0, a0, a1		# else, a = a - b
    addi	t1, t1, 1		# and   i = i + 1
    j	  mydiv_test		# let's test again

mydiv_end:
    add	a1, zero, a0		# rest   = a
    add	a0, zero, t1		# result = i
    ret
