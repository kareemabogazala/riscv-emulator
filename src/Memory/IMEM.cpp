#include "Memory/IMEM.h"
#include <fstream>

void IMem::load_code(const std::vector<uint8_t> &bytes, uint32_t offset)
{
    if (offset + bytes.size() > size)
        throw std::out_of_range("Code too large to load");

    for (size_t i = 0; i < bytes.size(); ++i)
    {
        data[offset + i] = bytes[i];
    }
}

void IMem::load_code32(const std::vector<uint32_t> &instrs, uint32_t offset)
{
    for (size_t i = 0; i < instrs.size(); ++i)
    {
        write32(base_addr + offset + i * 4, instrs[i]);
    }
}
void IMem::load_code_from_file(const std::string &path, uint32_t offset = 0)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
        throw std::runtime_error("Cannot open binary file: " + path);

    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(file), {});
    load_code(buffer, offset);
}