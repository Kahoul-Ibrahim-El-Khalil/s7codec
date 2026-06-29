# s7codec — Header-Only S7 Type Library

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

A freestanding, dependency-free C++17 header-only library that defines the complete S7 type system (27+ types) and provides portable, bidirectional encode/decode routines for all industrial scalar, composite, and temporal types.

Note: This library simply decodes and encodes scalar values, and it is used in my own proprietary semantic engine that is under development.

## Why this exists?

In modern industrial software (IIoT, SCADA, Edge Computing), a major bottleneck is the S7 Semantic Opacity. Raw PLC memory is big-endian and follows specific alignment rules that are opaque to standard IT systems.

This library bridges that gap by providing:
1.  Hardware Agnosticism: The same C++ code runs on high-performance AArch64/x86 gateways and resource-constrained ESP32/Arduino field devices.
2.  Semantic Fidelity: Full support for complex types like DTL (TIA Portal Date and Time Long), BCD, and S7 String.
3.  Deployment Optimization: By using these headers, developers can define "Logical Shadows" of PLC Data Blocks as packed C++ structs, automatically handling big-endian conversion and alignment.

## Bit-Packing and Zero-Copy

For high-performance applications, `s7codec` provides macros to define bit-packed groups that mirror the internal memory layout of an S7 PLC exactly. This allows for "Zero-Copy" access using `reinterpret_cast`.

```cpp
#include <s7codec/s7.hpp>

// Define a Data Block that matches S7-1500 bit-packing rules
DATABLOCK(MotorDB) {
    // S7_BIT_GROUP packs individual bits into bytes (8 bits per byte)
    // Using an anonymous struct ensures bits are direct members of MotorDB
    S7_BIT_GROUP_START
        S7_BIT(running);    // Offset 0.0
        S7_BIT(fault);      // Offset 0.1
        S7_BIT(manual);     // Offset 0.2
    S7_BIT_GROUP_END;       // Compiler dynamically pads remaining 5 bits

    s7codec::Int speed;     // Offset 2.0 (Automatic S7 word alignment)
};

void process(uint8_t* raw_buffer) {
    auto* db = reinterpret_cast<MotorDB*>(raw_buffer);
    
    // Direct access to bits!
    if (db->running && !db->fault) {
        db->speed = 1500; // Automatically handles Big-Endian conversion
    }
}
```

## Features

## Testing

A standalone example test is provided in `example_test.cpp`. To run it:

```bash
g++ -Iinclude example_test.cpp -o s7_test
./s7_test
```

## Usage

### Scalar Decoding
```cpp
#include <s7codec/s7.hpp>

// Decode a 16-bit signed integer from a big-endian PLC buffer
uint8_t buffer[] = {0x00, 0x2A};
auto val = s7codec::decodeScalar(s7codec::Type::Int, buffer);
// val.i == 42
```

## License

This library is licensed under the GNU General Public License v3.0 (GPLv3). 

---
*Developed by Kahoul Ibrahim El-Khalil.*
