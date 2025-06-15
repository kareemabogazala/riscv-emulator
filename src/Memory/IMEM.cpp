#include "Memory/IMEM.h"

void IMem::load_code(const std::vector<uint8_t> &bytes, uint32_t offset)
{
    if (offset + bytes.size() > size)
        throw std::out_of_range("Code too large to load");

    for (size_t i = 0; i < bytes.size(); ++i)
    {
        data[offset + i] = bytes[i];
    }
}
