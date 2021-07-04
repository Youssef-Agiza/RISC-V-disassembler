ENTRY(_start)
MEMORY {
ram : ORIGIN = 0, LENGTH = 0xC000
stack : ORIGIN = 0xC000, LENGTH = 0x4000
}
SECTIONS {
. = 0x00;
.text :
{
*(.text)
} > ram
.data :
{
*(.data)
} > ram
.bss :
{
*(.bss)
} > ram
_end = .;
}
PROVIDE(_stack_start = ORIGIN(stack) + LENGTH(stack));
