#include "../headers/disassembler.h"

Disassembler::Disassembler() : decoder_(new RV32I()), pc_(0x4), memory_(new char[8 * 1024]) {} // only 8KB of memory located at address 0

Disassembler::~Disassembler()
{
    delete memory_;
    if (decoder_)
        delete decoder_;
}
void Disassembler::read_file(char *file_name)
{
    std::ifstream inFile;
    inFile.open(file_name, std::ios::in | std::ios::binary | std::ios::ate);

    if (!inFile.is_open())
        emit_error("Cannot access input file\n");

    fsize_ = inFile.tellg();
    inFile.seekg(0, inFile.beg);

    if (!inFile.read((char *)memory_, fsize_))
        emit_error("Cannot read from input file\n");
}

void Disassembler::check_inst_type()
{
    unsigned int opcode_initial = memory_[pc_] & 0x3;
    int decoder_type = decoder_->get_decoder_type();

    // std::cout << "opcode: " << opcode_initial << "\t type:" << decoder_type << "\tmemory: " << std::hex << memory_[pc_] << "\n";
    if (opcode_initial == 0x3 && decoder_type != 1)
        change_decoder(new RV32I());

    if (opcode_initial != 0x3 && decoder_type != 2)
        change_decoder(new RV32C());
}

void Disassembler::disassemble(char *file_name)
{
    read_file(file_name);

    while (true)
    {
        check_inst_type();

        decoder_->read_instW(memory_, pc_);
        decoder_->decode_word(pc_);
        pc_ += decoder_->get_inst_size();

        if (pc_ >= fsize_)
            break;
    }
}

void Disassembler::change_decoder(RVDecoder *decoder)
{
    std::cout << "Changing decoder\n";
    if (this->decoder_ != nullptr)
        delete this->decoder_;

    this->decoder_ = decoder;
}