#include "../headers/disassembler.h"

void Disassembler::emitError(const char *message) //static
{
    std::cout << message;
    exit(1);
}

Disassembler::Disassembler() : decoder_(nullptr), pc_(0x0), memory_(new char[8 * 1024]) {} // only 8KB of memory located at address 0

Disassembler::~Disassembler()
{
    delete memory_;
    if (decoder_)
        delete decoder_;
}
void Disassembler::readFile(char *file_name)
{
    std::ifstream inFile;
    inFile.open(file_name, std::ios::in | std::ios::binary | std::ios::ate);

    if (!inFile.is_open())
        emitError("Cannot access input file\n");

    fsize_ = inFile.tellg();
    inFile.seekg(0, inFile.beg);
    if (!inFile.read((char *)memory_, fsize_))
        emitError("Cannot read from input file\n");
}

void Disassembler::disassemble(char *file_name)
{
    readFile(file_name);

    changeDecoder(new RV32I());

    unsigned int instWord;
    while (true)
    {
        instWord = (unsigned char)memory_[pc_] |
                   (((unsigned char)memory_[pc_ + 1]) << 8) |
                   (((unsigned char)memory_[pc_ + 2]) << 16) |
                   (((unsigned char)memory_[pc_ + 3]) << 24);
        pc_ += decoder_->getInstSize();
        if (pc_ >= fsize_ + 4)
            break;
        decoder_->decodeWord(instWord, pc_);
    }
}

void Disassembler::changeDecoder(RVDecoder *decoder)
{
    if (this->decoder_ != nullptr)
        delete this->decoder_;

    this->decoder_ = decoder;
}