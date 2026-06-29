#include <s7codec/s7.hpp>

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>

struct __attribute__((packed)) TestDB
{
    S7_BIT_GROUP_START
        S7_BIT(run);
        S7_BIT(fault);
        S7_BIT(manual);
        S7_BIT(auto_mode);
        S7_BIT(alarm1);
        S7_BIT(alarm2);
        S7_BIT(alarm3);
        S7_BIT(alarm4);
    S7_BIT_GROUP_END bits;

    s7codec::Int   speed;
    s7codec::UInt  pressure;
    s7codec::DInt  counter;
    s7codec::Real  temperature;
    s7codec::LReal energy;
};

static void dumpHex(const void* p, size_t n)
{
    auto b = static_cast<const uint8_t*>(p);

    for (size_t i = 0; i < n; ++i)
    {
        if (i % 16 == 0)
            printf("%04zx: ", i);

        printf("%02X ", b[i]);

        if ((i + 1) % 16 == 0)
            printf("\n");
    }

    if (n % 16)
        printf("\n");
}

int main()
{
    std::cout << "=== S7 Layout Test ===\n";

    TestDB db{};

    //
    // Deterministic values
    //
    // First byte = 0xAD
    //
    db.bits.run       = true;
    db.bits.fault     = false;
    db.bits.manual    = true;
    db.bits.auto_mode = true;
    db.bits.alarm1    = false;
    db.bits.alarm2    = true;
    db.bits.alarm3    = false;
    db.bits.alarm4    = true;

    db.speed       = 1500;
    db.pressure    = 300;
    db.counter     = 123456789;
    db.temperature = 45.5f;
    db.energy      = 1234.5678;

    std::cout << "\nsizeof(TestDB) = "
              << sizeof(TestDB) << "\n";

    std::cout << "\nOffsets:\n";
    std::cout << "speed       : "
              << offsetof(TestDB, speed) << "\n";
    std::cout << "pressure    : "
              << offsetof(TestDB, pressure) << "\n";
    std::cout << "counter     : "
              << offsetof(TestDB, counter) << "\n";
    std::cout << "temperature : "
              << offsetof(TestDB, temperature) << "\n";
    std::cout << "energy      : "
              << offsetof(TestDB, energy) << "\n";

    auto* bytes =
        reinterpret_cast<const uint8_t*>(&db);

    std::cout << "\n=== Raw DB Image ===\n";
    dumpHex(bytes, sizeof(db));

    std::cout << "\n=== DBB View ===\n";
    for (size_t i = 0; i < sizeof(db); ++i)
        printf("DBB%-2zu = %02X\n", i, bytes[i]);

    //
    // Decode again
    //
    auto run = s7codec::decodeScalar(
        s7codec::Type::Bool,
        bytes,
        sizeof(db),
        0);

    auto fault = s7codec::decodeScalar(
        s7codec::Type::Bool,
        bytes,
        sizeof(db),
        1);

    auto manual = s7codec::decodeScalar(
        s7codec::Type::Bool,
        bytes,
        sizeof(db),
        2);

    auto auto_mode = s7codec::decodeScalar(
        s7codec::Type::Bool,
        bytes,
        sizeof(db),
        3);

    auto alarm1 = s7codec::decodeScalar(
        s7codec::Type::Bool,
        bytes,
        sizeof(db),
        4);

    auto alarm2 = s7codec::decodeScalar(
        s7codec::Type::Bool,
        bytes,
        sizeof(db),
        5);

    auto alarm3 = s7codec::decodeScalar(
        s7codec::Type::Bool,
        bytes,
        sizeof(db),
        6);

    auto alarm4 = s7codec::decodeScalar(
        s7codec::Type::Bool,
        bytes,
        sizeof(db),
        7);

    auto speed = s7codec::decodeScalar(
        s7codec::Type::Int,
        bytes + offsetof(TestDB, speed),
        sizeof(db) - offsetof(TestDB, speed));

    auto pressure = s7codec::decodeScalar(
        s7codec::Type::UInt,
        bytes + offsetof(TestDB, pressure),
        sizeof(db) - offsetof(TestDB, pressure));

    auto counter = s7codec::decodeScalar(
        s7codec::Type::DInt,
        bytes + offsetof(TestDB, counter),
        sizeof(db) - offsetof(TestDB, counter));

    auto temperature = s7codec::decodeScalar(
        s7codec::Type::Real,
        bytes + offsetof(TestDB, temperature),
        sizeof(db) - offsetof(TestDB, temperature));

    auto energy = s7codec::decodeScalar(
        s7codec::Type::LReal,
        bytes + offsetof(TestDB, energy),
        sizeof(db) - offsetof(TestDB, energy));

    std::cout << "\n=== Comparison ===\n";

    std::cout
        << "run         : "
        << db.bits.run
        << " -> "
        << run.b
        << '\n';

    std::cout
        << "fault       : "
        << db.bits.fault
        << " -> "
        << fault.b
        << '\n';

    std::cout
        << "manual      : "
        << db.bits.manual
        << " -> "
        << manual.b
        << '\n';

    std::cout
        << "auto_mode   : "
        << db.bits.auto_mode
        << " -> "
        << auto_mode.b
        << '\n';

    std::cout
        << "alarm1      : "
        << db.bits.alarm1
        << " -> "
        << alarm1.b
        << '\n';

    std::cout
        << "alarm2      : "
        << db.bits.alarm2
        << " -> "
        << alarm2.b
        << '\n';

    std::cout
        << "alarm3      : "
        << db.bits.alarm3
        << " -> "
        << alarm3.b
        << '\n';

    std::cout
        << "alarm4      : "
        << db.bits.alarm4
        << " -> "
        << alarm4.b
        << '\n';

    std::cout
        << "speed       : "
        << static_cast<int16_t>(db.speed)
        << " -> "
        << speed.i
        << '\n';

    std::cout
        << "pressure    : "
        << static_cast<uint16_t>(db.pressure)
        << " -> "
        << pressure.i
        << '\n';

    std::cout
        << "counter     : "
        << static_cast<int32_t>(db.counter)
        << " -> "
        << counter.i
        << '\n';

    std::cout
        << "temperature : "
        << static_cast<float>(db.temperature)
        << " -> "
        << temperature.f
        << '\n';

    std::cout
        << "energy      : "
        << static_cast<double>(db.energy)
        << " -> "
        << energy.d
        << '\n';

    return 0;
}
