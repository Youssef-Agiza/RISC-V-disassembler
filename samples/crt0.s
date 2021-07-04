.text
.global _start
_start:
    la sp, _stack_start
    call main
loop:
    j loop