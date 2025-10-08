#include "Memory/DMEM.h"
#include <fstream>

void DMem::load_code(const std::vector<uint8_t> &bytes, uint32_t offset)
{
    if (offset + bytes.size() > size)
        throw std::out_of_range("Code too large to load");

    for (size_t i = 0; i < bytes.size(); ++i)
    {
        data[offset + i] = bytes[i];
    }
}

void DMem::load_code32(const std::vector<uint32_t> &instrs, uint32_t offset)
{
    for (size_t i = 0; i < instrs.size(); ++i)
    {
        write32(base_addr + offset + i * 4, instrs[i]);
    }
}
void DMem::load_code_from_file(const std::string &path, uint32_t offset = 0)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file)
        throw std::runtime_error("Cannot open binary file: " + path);

    std::streamsize size_in_bytes = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size_in_bytes);
    if (!file.read(reinterpret_cast<char *>(buffer.data()), size_in_bytes))
        throw std::runtime_error("Failed to read binary content");

    load_code(buffer, offset);

    // Set end_address
    end_address = base_addr + offset + static_cast<uint32_t>(size_in_bytes);
}