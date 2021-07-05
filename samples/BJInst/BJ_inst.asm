.data
	input: .asciz "Hello World!"
.globl main
.text

main:
#R Instructions


beq s1, s1, L1

li s0, 5

L1:  bne zero, s0, FILO
     addi s0, zero, -1	
     j L1 
	

FILO: blt zero, t0, FILI

FILI: bge t0, zero, FILA

FILA: bltu zero, t0, FILU

FILU: bgt t0, zero, FAAA

FAAA: bgtu t0, zero, FAAB

FAAB: ble zero, t0, FABA

FABA: bleu zero, t0, FILL

FILL: bgeu t0, zero, FILK

FILK: jal Label

Label:jr ra



