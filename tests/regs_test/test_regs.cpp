#include "catch.hpp"
#include "regs.h"

TEST_CASE("Regs Basic Read/Write", "[regs]")
{
    Regs r;

    SECTION("Write and read a general register")
    {
        r.write(5, 123);
        REQUIRE(r.read(5) == 123);
    }

    SECTION("Multiple writes to same reg")
    {
        r.write(10, 1);
        r.write(10, 99);
        REQUIRE(r.read(10) == 99);
    }

    SECTION("All registers initialized to zero")
    {
        for (int i = 0; i < 32; ++i)
        {
            REQUIRE(r.read(i) == 0);
        }
    }
}

TEST_CASE("Regs x0 Behavior", "[regs][zero]")
{
    Regs r;

    SECTION("x0 always reads as 0")
    {
        r.write(0, 999);
        REQUIRE(r.read(0) == 0);
    }

    SECTION("Writing to x0 has no effect")
    {
        r.write(0, 12345);
        r.write(1, 888);
        REQUIRE(r.read(0) == 0);
        REQUIRE(r.read(1) == 888); // sanity check
    }
}

TEST_CASE("Regs Bounds Checking", "[regs][errors]")
{
    Regs r;

    SECTION("Reading out of bounds throws exception")
    {
        REQUIRE_THROWS_AS(r.read(32), std::out_of_range);
        REQUIRE_THROWS_AS(r.read(-1), std::out_of_range);
    }

    SECTION("Writing out of bounds throws exception")
    {
        REQUIRE_THROWS_AS(r.write(32, 42), std::out_of_range);
        REQUIRE_THROWS_AS(r.write(-1, 42), std::out_of_range);
    }
}

TEST_CASE("Regs Comprehensive Mixed-Type and Format Test", "[regs][complex]")
{
    Regs r;

    SECTION("Write and read various values in different forms")
    {
        // Integers
        r.write(1, 42);                        // decimal int
        r.write(2, 0xDEADBEEF);                // hex literal
        r.write(3, 0755);                      // octal literal (=> decimal 493)
        r.write(4, static_cast<uint32_t>(-1)); // 0xFFFFFFFF

        // Typed constants
        uint8_t u8 = 0xAB;
        uint16_t u16 = 0x1234;
        uint32_t u32 = 0xCAFEBABE;
        int32_t i32 = -999;

        r.write(5, u8);
        r.write(6, u16);
        r.write(7, u32);
        r.write(8, static_cast<uint32_t>(i32));

        // Overwrite registers to verify update
        r.write(1, 0x11111111);
        r.write(2, 0x22222222);

        // Read back and check
        REQUIRE(r.read(1) == 0x11111111);
        REQUIRE(r.read(2) == 0x22222222);
        REQUIRE(r.read(3) == 493); // Octal 0755
        REQUIRE(r.read(4) == 0xFFFFFFFF);
        REQUIRE(r.read(5) == 0xAB);
        REQUIRE(r.read(6) == 0x1234);
        REQUIRE(r.read(7) == 0xCAFEBABE);
        REQUIRE(r.read(8) == static_cast<uint32_t>(-999));

        // Random large indices in valid range
        for (int i = 9; i < 32; ++i)
        {
            r.write(i, i * 1000 + 0xAB);
            REQUIRE(r.read(i) == static_cast<uint32_t>(i * 1000 + 0xAB));
        }
    }
}

#include <random>

TEST_CASE("Regs Randomized Stress Test", "[regs][random]")
{
    Regs r;

    std::mt19937 rng(1234);                             // Fixed seed for reproducibility
    std::uniform_int_distribution<int> reg_dist(1, 31); // avoid x0
    std::uniform_int_distribution<uint32_t> val_dist(0, UINT32_MAX);

    const int trials = 100;

    SECTION("Randomized writes and reads")
    {
        std::vector<std::pair<int, uint32_t>> written;

        for (int i = 0; i < trials; ++i)
        {
            int reg = reg_dist(rng);
            uint32_t val = val_dist(rng);
            r.write(reg, val);
            written.emplace_back(reg, val);
        }

        // Validate that the last written value to each register was preserved
        std::unordered_map<int, uint32_t> last_written;
        for (const auto &[reg, val] : written)
        {
            last_written[reg] = val;
        }

        for (const auto &[reg, val] : last_written)
        {
            REQUIRE(r.read(reg) == val);
        }
    }

    SECTION("Random writes to x0 are ignored")
    {
        for (int i = 0; i < 50; ++i)
        {
            uint32_t val = val_dist(rng);
            r.write(0, val);
            REQUIRE(r.read(0) == 0);
        }
    }
}

TEST_CASE("Regs handles values larger than 2^32", "[regs][overflow]")
{
    Regs r;

    SECTION("Value wraps modulo 2^32 when written")
    {
        uint64_t large_value = static_cast<uint64_t>(1) << 33;  // 2^33
        uint32_t expected = static_cast<uint32_t>(large_value); // lower 32 bits (== 0)

        r.write(10, static_cast<uint32_t>(large_value));
        REQUIRE(r.read(10) == expected);

        // More examples
        large_value = (static_cast<uint64_t>(1) << 32) + 12345; // 2^32 + 12345
        expected = static_cast<uint32_t>(large_value);          // Should be 12345

        r.write(11, static_cast<uint32_t>(large_value));
        REQUIRE(r.read(11) == 12345);
    }
}
