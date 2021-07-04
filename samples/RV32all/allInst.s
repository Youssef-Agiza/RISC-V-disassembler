.data
	input: .asciz "Hello World!"

.globl main
.text

#R Instructions
main: 
li t1, 7
add t1, t1, t1
sub t1, t1, t1
li t2, 1
xor t1, t1, t2
or t2, t1, zero
and t2, t1, t2
sll t3, t2, t1 #t3=2
srl t3, t3, t1 #t3=1
sra t3, t3, t1 #t3=0
slt t2, t3, t1 #t2=1
sltu t1, t3, t2 #t1=1
li t1, 0
li t2, 0
li t3, 0

#I&S Instructions

addi t0, zero, 7 #t0=7
addi t0, zero, 0 #t0=0
xori t0, zero, 1 #t0=1
ori t0, t0, 0 #t0=1
li t0, 1 #t0=1
andi t0, t0, 1 #t0=1
slli t0, t0, 1 #t0=2
srli t0, t0, 1 #t0=1
srai t0, t0, 1 #t0=0
slti t0, t0, 1 #t0=1
sltiu t0, t0, 1 #t0=0

li t0, 9
addi sp, sp, -4
sw t0, (sp)
lw s0, (sp)
addi sp, sp, 4

la s0, input
lb s3, (s0)
addi sp, sp, -4
sb s3, (sp)
addi sp, sp, 4
addi s0, s0, 1
lbu s3, (s0)
addi s0, s0, 1
lh s3, (s0)
addi sp, sp, -4
sh s3, (sp)
addi sp, sp, 4
addi s0, s0, 2
lhu s3, (s0)
jalr x0, ra, 0
ecall
ebreak

#B&U&J Instructions
lui t1, 8
auipc t1, 8
beq x0, x0, FILE
li t0, 1
FILE: bne zero, t0, FILO
FILO: blt zero, t0, FILI
FILI: bge t0, zero, FILA
FILA: bltu zero, t0, FILU
FILU: bgeu t0, zero, FILK
FILK: jal Label
Label:jr ra
