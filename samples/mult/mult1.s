li s2, 20
li s1, 30
li s0, 0	# the product

whileLoop:

beq s2, x0, endWhile
add  s0, s0, s1
addi s2, s2, -1
jal x0, whileLoop
endWhile:
li a7, 1
mv a0, s0
ecall
li a7, 10
ecall
  
