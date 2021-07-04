#include "RVDecoder.h"

RVDecoder::RVDecoder() : lbl_cntr(1) {}
RVDecoder::~RVDecoder() {}

void RVDecoder::decode_word(unsigned int instW, unsigned int pc)
{
    /*parsing instWord*/
    extract_opcode(instW);
    extract_functs(instW);
    extract_regs(instW);
    extract_immediates(instW);

    print_prefix(pc, instW);

    if (!validate())
        return;

    print_instruction(pc);
}
