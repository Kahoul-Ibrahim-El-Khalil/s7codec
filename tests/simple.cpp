#include <s7codec/s7.hpp>
#include <iostream>
#include <cassert>
#include <array>

int main() {
    std::cout << "Running S7 Codec Tests..." << std::endl;

    // Test 1: Scalar Decoding (Int)
    std::array<uint8_t,2> int_buffer = {0x00, 0x2A}; // 42 in big-endian
    auto int_val = s7codec::decodeScalar(s7codec::Type::Int, int_buffer.data(), int_buffer.size());
    assert(int_val.i == 42);
    std::cout << "[PASS] Scalar Int Decoding" << std::endl;

    // Test 2: Real Decoding
    std::array<uint8_t, 4> real_buffer = {0x41, 0xBC, 0x00, 0x00}; // 23.5 in IEEE 754 Big-Endian
    auto real_val = s7codec::decodeScalar(s7codec::Type::Real, real_buffer.data(), real_buffer.size()) ;
    assert(real_val.f > 23.49f && real_val.f < 23.51f);
    std::cout << "[PASS] Scalar Real Decoding" << std::endl;

    // Test 3: Structural Mirroring
    struct __attribute__((packed)) TestDB {
        s7codec::Int   speed;
        s7codec::Real  temp;
    };

    uint8_t db_buffer[6];
    auto* db = reinterpret_cast<TestDB*>(db_buffer);
    db->speed = 1500;
    db->temp = 45.5f;

    assert(db_buffer[0] == 0x05 && db_buffer[1] == 0xDC); // 1500 is 0x05DC
    std::cout << "[PASS] Structural Mirroring (Endianness)" << std::endl;

    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}
