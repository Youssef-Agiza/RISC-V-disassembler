main:
    li a7, 5        # enter the value
    ecall

    #mv a2, a0
    #auipc a1, 0x0
    jal sum

    li a7, 1
    ecall

    li a7, 10
    ecall

sum:
    li s0, 0
loop:
    ble a0, zero, stop
    add s0, s0, a0
    addi a0, a0, -1
    j loop
stop:
    mv	a0, s0
    ret

# should print the summation the values: enter 4 prints 10
