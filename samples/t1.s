main:
	li	x0, 1
	li	x1, 1
	li	x2, 1
	add	x3, x0, x1
	add	x4, x1, x2
	add	x5, x3, x4
	li	a7, 1
	mv	a0, x5
	ecall
	li	a7, 10			 # calls exit command (code 10)
    	ecall				 # end of program


