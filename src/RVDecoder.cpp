#include "RVDecoder.h"

unsigned int RVDecoder::lbl_cntr_ = 1;

RVDecoder::RVDecoder() {}
RVDecoder::~RVDecoder() {}

void RVDecoder::decode_word(unsigned int pc)
{
    /*parsing instWord*/
    extract_opcode();
    extract_functs();
    extract_regs();
    extract_immediates();

    if (!validate())
        return;

    print_prefix(pc);
    print_instruction(pc);
}
