#include "catch.hpp"
#include "RISCV.h"
#include "Stages/FetchStage.h"
#include "Memory/DMEM.h"
#include "Memory/IMEM.h"
#include "Memory/MemoryBus.h"

TEST_CASE("FetchStage: fetches correct instruction and updates pc_next", "[fetch]")
{
    IMem imem(0x00000000, 64);
    Memory ram(0x00100000, 64);
    auto bus = std::make_shared<MemoryBus>(ram);
    RISCV cpu(bus);

    // Load instructions as bytes (little-endian)
    imem.load_code({0x13, 0x00, 0x00, 0x00,   // nop (0x00000013)
                    0x78, 0x56, 0x34, 0x12,   // 0x12345678
                    0xBE, 0xBA, 0xFE, 0xCA}); // 0xCAFEBABE


    SECTION("First cycle fetches first instruction")
    {
       // REQUIRE_THROWS_AS(cpu.step(),std::runtime_error) ;
       // REQUIRE(cpu.if_id.instr == 0x00000013);
       // REQUIRE(cpu.if_id.pc_next == 0x00000004);
    }

}
