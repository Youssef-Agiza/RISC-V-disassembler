# RISC-V-disassembler

A disassmbler covers all the base RV32IC instructions except of the instructions the FENCE and CSR instructions.

## Group members

- Omar K Hamed
- Sherif Elsamra
- Youssef Agiza

# Implementation

## implementation style:

We prefered to implement the project in _object-oriented design(OOP)_ to keep the code maintaible and scalable using OOP features such as polymorphism and inheritance.

## Design Idea

The design of the code was inspired by _state_ design pattern. We created an abstract class (RVDecoder) that implements the interface for decoding instructions. Then, we created Assembler class that kept pointer to RVDecoder. Finally, every decoding format was implemented in a child class of RVDecoder and whenever we need to change the deocding format, we only changed the child class at which Assembler points while keeping the same code everywhere else.

## Classes and their responsibility

1. RVDecoder(_abstract_).\
   An abstract class the defines the interface for a family of decoder classes.

2. RV32I.\
   Inherits from RVDecoder and this class is reponsible for decoding 32-bit base instructions.

3. RV32C.\
   Similar to RV32C but decodes 16-bit compressed instructions

4. Disassembler.\
   it reads the binary file and calls the decoder classes accordingly. It creates a pointer to RVDecoder and this makes it point to a child class(RV32I/RV32C) according to the instrucion word opcode.

## Important implementation details

1. Changing decoding mode: this is done in Assembler class which keeps a pointer the abstract interface class.

   - Before decoding a new instruction, Assembler checks the format in which the instruction word is in encded using the first to bits of the opcode.
   - If the format is different from the one that should be handled by the current decoder object, the object is deleted and the pointer points to a new decoder of the needed type.
   - e.g.

     if(opcode==0x3 && decoder is not RV32I)\
     {
     delete decoder;\
     decoder = new RV32I();}

   - Accordingly, we can have extend this project to support other exntensions anytime by implementing a child classes for the extension targeted and changing the pointer to point to the new type without breaking the code.

2. Maps data structure: maps were used in several parts of the code to map functions to their instrucion such that instruction can be retrieved without bloating the function with switch/if statements.

   - the maps are stored in a header file and included when needed.
   - In RV32I, there is a map for all the instructions that same opcode. The key to the function is sometimes function3 from the manual and other time a combination between function3 and function7(check the comments before each map).
   - In RV32C, we have less maps and more switch statements because the compressed format doesn't have clear patterns and we couldn't manage to create patterns without overcomplicating the code.

3. Adding labels to jump/branch instructions: this was again achieved using map that takes the address of the label as key and returns and label.
   - The function responsible for generating the label generate a new label if and only if the address passed is a new one.
   - The label name changes by keeping a static counter in the class that increments every time a new label is created.
   - This design choice was made to gaurantee that the code will give the same label name for the same address when called more than once.

# Limitations and Known Issues

In theory, this disassembler supports the compressed instructions. However, this claim require more testing as we faced some issues generating a binary file to test them. Accordingly, there may be mistakes in the code that we are unaware of in the implementation of the compressed format.

# Contributions

**Omar**

- managed to handle the R and I instructions in the program.
- He also,
  created the comprehensive test cases required to test all types of instructions
- He wrote the ReadMe file.

**Sherif**

- Handled the S and B instructions in the program
- collaborated with Omar in making the test cases

**Youssef:**

- Handled the U and J instructions in the base format.
- Implemented the compressed instructions
- Refactored the code to be object-oriented
- Revised the ReadMe file and wrote it in Markdown language.
