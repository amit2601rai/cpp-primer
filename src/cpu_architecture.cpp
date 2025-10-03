/**
 * @file cpu_architecture.cpp
 * @author GitHub Copilot
 * @brief Tutorial code for CPU architectures, instruction sets, endianness, and computer architecture concepts.
 */

// This program provides a comprehensive overview of computer architecture including:
// - CPU architectures (x86, ARM, RISC-V)
// - Instruction set architectures (CISC vs RISC)
// - Endianness (Little Endian vs Big Endian)
// - CPU pipeline and execution models
// - Memory hierarchies and cache systems
// - Assembly language basics
// - Performance characteristics and trade-offs

#include <iostream>
#include <vector>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <chrono>

// Union for demonstrating endianness
union EndianTest {
    uint32_t value;
    uint8_t bytes[4];
};

void demonstrate_cpu_architecture_overview() {
    std::cout << "=== CPU ARCHITECTURE OVERVIEW ===" << std::endl;
    
    // CPU Architecture Comparison:
    // ┌─ MAJOR CPU ARCHITECTURES ───────────────────────────────┐
    // │                                                         │
    // │ x86-64 (Intel/AMD):        ARM (ARM Holdings):         │
    // │ ┌─────────────────────────┐ ┌─────────────────────────┐ │
    // │ │ CISC Architecture       │ │ RISC Architecture       │ │
    // │ │ ┌─ Characteristics ───┐ │ │ ┌─ Characteristics ───┐ │ │
    // │ │ │ • Complex instruct. │ │ │ │ • Simple instruct.  │ │ │
    // │ │ │ • Variable length   │ │ │ │ • Fixed length      │ │ │
    // │ │ │ • Many addressing   │ │ │ │ • Load/Store model  │ │ │
    // │ │ │   modes             │ │ │ │ • Large register    │ │ │
    // │ │ │ • Backward compat.  │ │ │ │   file              │ │ │
    // │ │ └─────────────────────┘ │ │ └─────────────────────┘ │ │
    // │ │                         │ │                         │ │
    // │ │ ┌─ Register Set ──────┐ │ │ ┌─ Register Set ──────┐ │ │
    // │ │ │ RAX, RBX, RCX, RDX  │ │ │ │ R0-R30 (31 GP regs)│ │ │
    // │ │ │ RSI, RDI, RBP, RSP  │ │ │ │ SP (Stack Pointer) │ │ │
    // │ │ │ R8-R15 (x86-64)     │ │ │ │ PC (Program Counter)│ │ │
    // │ │ │ RIP (Instr. Pointer)│ │ │ │ NZCV (Status)      │ │ │
    // │ │ └─────────────────────┘ │ │ └─────────────────────┘ │ │
    // │ └─────────────────────────┘ └─────────────────────────┘ │
    // │                                                         │
    // │ RISC-V (Open Source):       MIPS (Legacy):             │
    // │ ┌─────────────────────────┐ ┌─────────────────────────┐ │
    // │ │ Open RISC Architecture  │ │ Classic RISC Design     │ │
    // │ │ ┌─ Characteristics ───┐ │ │ ┌─ Characteristics ───┐ │ │
    // │ │ │ • Modular design    │ │ │ │ • Pipeline friendly │ │ │
    // │ │ │ • Clean ISA         │ │ │ │ • Delay slots       │ │ │
    // │ │ │ • Extensible        │ │ │ │ • 32 registers      │ │ │
    // │ │ │ • Open standard     │ │ │ │ • Big endian orig.  │ │ │
    // │ │ └─────────────────────┘ │ │ └─────────────────────┘ │ │
    // │ └─────────────────────────┘ └─────────────────────────┘ │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- CPU Architecture Families ---" << std::endl;
    std::cout << "x86-64 (Intel/AMD):" << std::endl;
    std::cout << "• CISC (Complex Instruction Set Computer)" << std::endl;
    std::cout << "• Variable-length instructions (1-15 bytes)" << std::endl;
    std::cout << "• Rich addressing modes" << std::endl;
    std::cout << "• Backward compatibility to 8086 (1978)" << std::endl;
    std::cout << "• Used in: Desktop, Server, Laptop computers" << std::endl;
    
    std::cout << "\nARM (Advanced RISC Machine):" << std::endl;
    std::cout << "• RISC (Reduced Instruction Set Computer)" << std::endl;
    std::cout << "• Fixed-length instructions (32-bit)" << std::endl;
    std::cout << "• Load/Store architecture" << std::endl;
    std::cout << "• Power efficient design" << std::endl;
    std::cout << "• Used in: Mobile devices, embedded systems, Apple M1/M2" << std::endl;
    
    std::cout << "\nRISC-V:" << std::endl;
    std::cout << "• Open-source RISC architecture" << std::endl;
    std::cout << "• Modular and extensible" << std::endl;
    std::cout << "• Clean, simple design" << std::endl;
    std::cout << "• Used in: Research, embedded systems, some servers" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_instruction_sets() {
    std::cout << "=== INSTRUCTION SET ARCHITECTURES ===" << std::endl;
    
    // CISC vs RISC Instruction Comparison:
    // ┌─ CISC vs RISC INSTRUCTION COMPARISON ───────────────────┐
    // │                                                         │
    // │ CISC (x86) Example:         RISC (ARM) Example:        │
    // │ ┌─────────────────────────┐ ┌─────────────────────────┐ │
    // │ │ ADD [EBX+4], EAX       │ │ LDR R1, [R2, #4]       │ │
    // │ │ (Add EAX to memory     │ │ MOV R3, R1              │ │
    // │ │  at EBX+4)             │ │ ADD R3, R3, R0          │ │
    // │ │                        │ │ STR R3, [R2, #4]       │ │
    // │ │ Single Complex Instr.  │ │ Multiple Simple Instr.  │ │
    // │ └─────────────────────────┘ └─────────────────────────┘ │
    // │                                                         │
    // │ Instruction Breakdown:                                  │
    // │ ┌─ CISC Execution ────────┐ ┌─ RISC Execution ────────┐ │
    // │ │ 1. Decode complex       │ │ 1. Load from memory     │ │
    // │ │    instruction          │ │ 2. Move to temp reg     │ │
    // │ │ 2. Calculate address    │ │ 3. Add registers        │ │
    // │ │ 3. Load from memory     │ │ 4. Store to memory      │ │
    // │ │ 4. Perform addition     │ │                         │ │
    // │ │ 5. Store to memory      │ │ Each step is simple     │ │
    // │ │                         │ │ and predictable         │ │
    // │ │ Complex but fewer       │ │                         │ │
    // │ │ instructions            │ │                         │ │
    // │ └─────────────────────────┘ └─────────────────────────┘ │
    // └─────────────────────────────────────────────────────────┘
    
    // Instruction Format Comparison:
    // ┌─ INSTRUCTION FORMATS ───────────────────────────────────┐
    // │                                                         │
    // │ x86 Variable Length Instructions:                       │
    // │ ┌─────────────────────────────────────────────────────┐ │
    // │ │ Prefix │ Opcode │ ModR/M │ SIB │ Disp │ Immediate │ │
    // │ │ 0-4 B  │ 1-3 B  │ 0-1 B  │0-1B │ 0-4B │  0-4 B    │ │
    // │ │                                                     │ │
    // │ │ Examples:                                           │ │
    // │ │ MOV EAX, EBX     → 89 D8          (2 bytes)        │ │
    // │ │ MOV EAX, [EBX+8] → 8B 43 08       (3 bytes)        │ │
    // │ │ MOV EAX, 0x1234  → B8 34 12 00 00 (5 bytes)        │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ ARM Fixed Length Instructions (32-bit):                │
    // │ ┌─────────────────────────────────────────────────────┐ │
    // │ │ Cond │ 00 │ I │OpCode│ S │  Rn  │  Rd  │ Operand2 │ │
    // │ │  4b  │ 2b │1b│  4b  │1b│  4b  │  4b  │   12b     │ │
    // │ │                                                     │ │
    // │ │ Examples (all 32-bit/4 bytes):                      │ │
    // │ │ ADD R0, R1, R2   → E0810002                         │ │
    // │ │ MOV R0, #42      → E3A0002A                         │ │
    // │ │ LDR R0, [R1]     → E5910000                         │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- CISC vs RISC Philosophy ---" << std::endl;
    std::cout << "CISC (Complex Instruction Set Computer):" << std::endl;
    std::cout << "• Rich, powerful instructions" << std::endl;
    std::cout << "• Variable instruction length" << std::endl;
    std::cout << "• Memory-to-memory operations" << std::endl;
    std::cout << "• Fewer instructions per program" << std::endl;
    std::cout << "• Complex decoding logic" << std::endl;
    std::cout << "• Example: x86, x86-64" << std::endl;
    
    std::cout << "\nRISC (Reduced Instruction Set Computer):" << std::endl;
    std::cout << "• Simple, uniform instructions" << std::endl;
    std::cout << "• Fixed instruction length" << std::endl;
    std::cout << "• Load/Store architecture" << std::endl;
    std::cout << "• More instructions per program" << std::endl;
    std::cout << "• Simple decoding, easier pipelining" << std::endl;
    std::cout << "• Examples: ARM, RISC-V, MIPS" << std::endl;
    
    // Demonstrate instruction encoding
    std::cout << "\n--- Instruction Encoding Example ---" << std::endl;
    std::cout << "C code: result = a + b;" << std::endl;
    std::cout << "\nx86 Assembly (CISC):" << std::endl;
    std::cout << "  mov eax, [a]    ; Load 'a' into EAX" << std::endl;
    std::cout << "  add eax, [b]    ; Add 'b' to EAX (result in EAX)" << std::endl;
    std::cout << "  mov [result], eax ; Store result" << std::endl;
    
    std::cout << "\nARM Assembly (RISC):" << std::endl;
    std::cout << "  ldr r0, [a_addr]  ; Load address of 'a'" << std::endl;
    std::cout << "  ldr r1, [r0]      ; Load value of 'a' into R1" << std::endl;
    std::cout << "  ldr r0, [b_addr]  ; Load address of 'b'" << std::endl;
    std::cout << "  ldr r2, [r0]      ; Load value of 'b' into R2" << std::endl;
    std::cout << "  add r3, r1, r2    ; Add R1 and R2, store in R3" << std::endl;
    std::cout << "  ldr r0, [result_addr] ; Load address of result" << std::endl;
    std::cout << "  str r3, [r0]      ; Store R3 to result" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_endianness() {
    std::cout << "=== ENDIANNESS: BYTE ORDER IN MEMORY ===" << std::endl;
    
    // Endianness Visualization:
    // ┌─ ENDIANNESS COMPARISON ─────────────────────────────────┐
    // │                                                         │
    // │ 32-bit value: 0x12345678                               │
    // │                                                         │
    // │ LITTLE ENDIAN (x86, ARM-typical):                      │
    // │ ┌─ Memory Layout ─────────────────────────────────────┐ │
    // │ │ Address: │ 0x1000 │ 0x1001 │ 0x1002 │ 0x1003 │   │ │
    // │ │ Value:   │  0x78  │  0x56  │  0x34  │  0x12  │   │ │
    // │ │          │  LSB   │        │        │  MSB   │   │ │
    // │ │          │(Least  │        │        │ (Most  │   │ │
    // │ │          │Signif. │        │        │Signif. │   │ │
    // │ │          │ Byte)  │        │        │ Byte)  │   │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ BIG ENDIAN (MIPS, Network byte order):                 │
    // │ ┌─ Memory Layout ─────────────────────────────────────┐ │
    // │ │ Address: │ 0x1000 │ 0x1001 │ 0x1002 │ 0x1003 │   │ │
    // │ │ Value:   │  0x12  │  0x34  │  0x56  │  0x78  │   │ │
    // │ │          │  MSB   │        │        │  LSB   │   │ │
    // │ │          │ (Most  │        │        │(Least  │   │ │
    // │ │          │Signif. │        │        │Signif. │   │ │
    // │ │          │ Byte)  │        │        │ Byte)  │   │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ Memory Access Implications:                             │
    // │ ┌─ Pointer Casting ───────────────────────────────────┐ │
    // │ │ uint32_t value = 0x12345678;                        │ │
    // │ │ uint8_t* byte_ptr = (uint8_t*)&value;               │ │
    // │ │                                                     │ │
    // │ │ Little Endian: byte_ptr[0] = 0x78 (LSB first)      │ │
    // │ │ Big Endian:    byte_ptr[0] = 0x12 (MSB first)      │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // └─────────────────────────────────────────────────────────┘
    
    // Hardware Implementation Details:
    // ┌─ HARDWARE PERSPECTIVE ──────────────────────────────────┐
    // │                                                         │
    // │ CPU Register to Memory Bus Transaction:                 │
    // │                                                         │
    // │ CPU Register: [0x12345678] (32-bit value)              │
    // │                                                         │
    // │ LITTLE ENDIAN Bus Transactions:                        │
    // │ ┌─ Memory Controller ─────────────────────────────────┐ │
    // │ │ Store Address 0x1000 → Data Bus: 0x78 (LSB first) │ │
    // │ │ Store Address 0x1001 → Data Bus: 0x56             │ │
    // │ │ Store Address 0x1002 → Data Bus: 0x34             │ │
    // │ │ Store Address 0x1003 → Data Bus: 0x12 (MSB last)  │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ BIG ENDIAN Bus Transactions:                           │
    // │ ┌─ Memory Controller ─────────────────────────────────┐ │
    // │ │ Store Address 0x1000 → Data Bus: 0x12 (MSB first) │ │
    // │ │ Store Address 0x1001 → Data Bus: 0x34             │ │
    // │ │ Store Address 0x1002 → Data Bus: 0x56             │ │
    // │ │ Store Address 0x1003 → Data Bus: 0x78 (LSB last)  │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ Key Hardware Insight:                                   │
    // │ Endianness determines which BYTE goes to which ADDRESS │
    // │ NOT just visual positioning - it's about memory layout │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- Detecting System Endianness ---" << std::endl;
    
    EndianTest test;
    test.value = 0x12345678;
    
    std::cout << "32-bit value: 0x" << std::hex << test.value << std::dec << std::endl;
    std::cout << "Byte layout in memory:" << std::endl;
    
    for (int i = 0; i < 4; ++i) {
        std::cout << "  Byte " << i << ": 0x" 
                  << std::hex << std::setfill('0') << std::setw(2) 
                  << static_cast<int>(test.bytes[i]) << std::dec << std::endl;
    }
    
    if (test.bytes[0] == 0x78) {
        std::cout << "System is LITTLE ENDIAN (LSB at lowest address)" << std::endl;
    } else if (test.bytes[0] == 0x12) {
        std::cout << "System is BIG ENDIAN (MSB at lowest address)" << std::endl;
    }
    
    // Demonstrate endianness conversion
    std::cout << "\n--- Endianness Conversion Functions ---" << std::endl;
    
    auto swap_bytes_32 = [](uint32_t value) -> uint32_t {
        return ((value & 0x000000FF) << 24) |
               ((value & 0x0000FF00) << 8)  |
               ((value & 0x00FF0000) >> 8)  |
               ((value & 0xFF000000) >> 24);
    };
    
    uint32_t original = 0x12345678;
    uint32_t swapped = swap_bytes_32(original);
    
    std::cout << "Original: 0x" << std::hex << original << std::dec << std::endl;
    std::cout << "Swapped:  0x" << std::hex << swapped << std::dec << std::endl;
    
    // Network byte order example
    std::cout << "\n--- Network Byte Order Example ---" << std::endl;
    std::cout << "Network protocols use Big Endian (network byte order)" << std::endl;
    std::cout << "Functions like htonl() convert host to network byte order" << std::endl;
    
    // Demonstrate with a practical example
    uint32_t host_ip = 0xC0A80001; // 192.168.0.1 in hex
    std::cout << "IP Address 192.168.0.1 as host order: 0x" << std::hex << host_ip << std::dec << std::endl;
    
    // Manual conversion (since htonl might not be available)
    uint32_t network_ip = swap_bytes_32(host_ip);
    std::cout << "Same IP as network order: 0x" << std::hex << network_ip << std::dec << std::endl;
    
    std::cout << "\n--- Hardware Implementation Details ---" << std::endl;
    
    // Hardware Endianness Examples:
    // ┌─ REAL HARDWARE ARCHITECTURES ───────────────────────────┐
    // │                                                         │
    // │ INTEL x86/x64: LITTLE ENDIAN                           │
    // │ ┌─ Historical Reason ─────────────────────────────────┐ │
    // │ │ • Inherited from 8080/8086 processors (1978)       │ │
    // │ │ • Optimized for arithmetic operations               │ │
    // │ │ • LSB accessible at lowest address for efficiency   │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ ARM: CONFIGURABLE (mostly Little Endian now)           │
    // │ ┌─ Mode Selection ────────────────────────────────────┐ │
    // │ │ • SCTLR.EE bit controls data endianness            │ │
    // │ │ • SCTLR.IE bit controls instruction endianness     │ │
    // │ │ • Apple M1/M2: Fixed little-endian mode            │ │
    // │ │ • Can be set at boot/reset time                    │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ MIPS: CONFIGURABLE                                     │
    // │ ┌─ Configuration Methods ─────────────────────────────┐ │
    // │ │ • Config register bit 15 (BE bit)                  │ │
    // │ │ • Hardware pin configuration                        │ │
    // │ │ • Can switch dynamically in some implementations   │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ POWERPC: BIG ENDIAN (traditionally)                    │
    // │ ┌─ Enterprise Usage ──────────────────────────────────┐ │
    // │ │ • IBM Power servers typically big-endian           │ │
    // │ │ • Some embedded PowerPC configurable               │ │
    // │ │ • AIX, Linux on Power use big-endian               │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "Hardware Architecture Endianness:" << std::endl;
    std::cout << "• Intel x86/x64: Always Little Endian" << std::endl;
    std::cout << "• ARM: Configurable, mostly Little Endian in modern systems" << std::endl;
    std::cout << "• MIPS: Configurable via hardware pin or register" << std::endl;
    std::cout << "• PowerPC: Traditionally Big Endian" << std::endl;
    std::cout << "• RISC-V: Configurable, but typically Little Endian" << std::endl;
    
    std::cout << "\n--- Cross-Platform Data Exchange ---" << std::endl;
    
    // Demonstrate endianness conversion for cross-platform data
    std::cout << "File/Network Data Exchange Example:" << std::endl;
    
    // Simulate reading from a big-endian source (like network or file)
    uint32_t network_data = 0x12345678;  // Big-endian format
    std::cout << "Data from network/file (big-endian): 0x" << std::hex << network_data << std::dec << std::endl;
    
    // On little-endian system, convert to host byte order
    auto convert_big_to_host = [](uint32_t big_endian_value) -> uint32_t {
        // Simulate ntohl() function
        return ((big_endian_value & 0xFF000000) >> 24) |
               ((big_endian_value & 0x00FF0000) >> 8)  |
               ((big_endian_value & 0x0000FF00) << 8)  |
               ((big_endian_value & 0x000000FF) << 24);
    };
    
    uint32_t host_data = convert_big_to_host(network_data);
    std::cout << "Converted to host byte order: 0x" << std::hex << host_data << std::dec << std::endl;
    
    std::cout << "\n--- Memory-Mapped I/O Hardware Registers ---" << std::endl;
    
    // Memory-Mapped I/O Impact:
    // ┌─ HARDWARE REGISTER ACCESS ──────────────────────────────┐
    // │                                                         │
    // │ Hardware Register Layout (Big-Endian Device):          │
    // │ ┌─ 32-bit Control Register at 0x40000000 ─────────────┐ │
    // │ │ Bits: │31-24│23-16│15-8 │7-0  │                   │ │
    // │ │ Field:│ CMD │STAT │CTRL │DATA │                   │ │
    // │ │ Value:│ 0x12│0x34 │0x56 │0x78 │                   │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ Little-Endian CPU Access:                              │
    // │ ┌─ Memory Layout ─────────────────────────────────────┐ │
    // │ │ Address│0x40000000│0x40000001│0x40000002│0x40000003││ │
    // │ │ CPU    │   0x78   │   0x56   │   0x34   │   0x12   ││ │
    // │ │ Device │   0x12   │   0x34   │   0x56   │   0x78   ││ │
    // │ │        │   ↑      │    ↑     │    ↑     │    ↑     ││ │
    // │ │        │ MISMATCH │ MISMATCH │ MISMATCH │ MISMATCH ││ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ Solution: Byte swapping required for hardware access   │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "Hardware Register Access Considerations:" << std::endl;
    std::cout << "• Network interfaces: Always big-endian (network byte order)" << std::endl;
    std::cout << "• Graphics hardware: May have different endianness than CPU" << std::endl;
    std::cout << "• Storage controllers: File formats specify endianness" << std::endl;
    std::cout << "• Embedded peripherals: Often have fixed endianness" << std::endl;
    
    // Example of hardware register access
    std::cout << "\nExample: Writing to big-endian hardware register" << std::endl;
    uint32_t control_value = 0x12345678;
    std::cout << "CPU value: 0x" << std::hex << control_value << std::dec << std::endl;
    
    // Simulate what would be needed for big-endian hardware
    uint32_t hw_value = swap_bytes_32(control_value);
    std::cout << "Value for big-endian hardware: 0x" << std::hex << hw_value << std::dec << std::endl;
    
    std::cout << "\n--- Multi-Byte Instruction Encoding ---" << std::endl;
    
    // Instruction Encoding Impact:
    // ┌─ INSTRUCTION ENCODING ENDIANNESS ───────────────────────┐
    // │                                                         │
    // │ x86 Instruction: MOV EAX, 0x12345678                   │
    // │                                                         │
    // │ Machine Code (Little-Endian):                          │
    // │ ┌─ Byte Sequence ─────────────────────────────────────┐ │
    // │ │ B8 78 56 34 12                                      │ │
    // │ │ │  └─────┬─────┘                                    │ │
    // │ │ │   Immediate value stored little-endian            │ │
    // │ │ └─ MOV EAX, imm32 opcode                            │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ If x86 were Big-Endian (hypothetical):                │
    // │ ┌─ Byte Sequence ─────────────────────────────────────┐ │
    // │ │ B8 12 34 56 78                                      │ │
    // │ │ │  └─────┬─────┘                                    │ │
    // │ │ │   Immediate value stored big-endian               │ │
    // │ │ └─ MOV EAX, imm32 opcode                            │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "Instruction Encoding Examples:" << std::endl;
    std::cout << "x86 'MOV EAX, 0x12345678' encoding:" << std::endl;
    std::cout << "  Machine code: B8 78 56 34 12 (immediate is little-endian)" << std::endl;
    std::cout << "ARM 'MOV R0, #0x12345678' encoding:" << std::endl;
    std::cout << "  Machine code: E3 A0 xxxx (immediate encoding varies)" << std::endl;
    
    std::cout << "\nKey Hardware Insight:" << std::endl;
    std::cout << "Endianness is about MEMORY ADDRESS ORDERING, not visual positioning!" << std::endl;
    std::cout << "It determines which byte of a multi-byte value goes to which memory address." << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_cpu_pipeline() {
    std::cout << "=== CPU PIPELINE AND EXECUTION MODEL ===" << std::endl;
    
    // CPU Pipeline Visualization:
    // ┌─ 5-STAGE RISC PIPELINE ─────────────────────────────────┐
    // │                                                         │
    // │ Clock Cycle: 1    2    3    4    5    6    7    8      │
    // │ ┌─────────────────────────────────────────────────────┐ │
    // │ │ Instruction 1:                                      │ │
    // │ │ │IF │ID │EX │MEM│WB │    │    │    │               │ │
    // │ │                                                     │ │
    // │ │ Instruction 2:                                      │ │
    // │ │ │   │IF │ID │EX │MEM│WB │    │    │               │ │
    // │ │                                                     │ │
    // │ │ Instruction 3:                                      │ │
    // │ │ │   │   │IF │ID │EX │MEM│WB │    │               │ │
    // │ │                                                     │ │
    // │ │ Instruction 4:                                      │ │
    // │ │ │   │   │   │IF │ID │EX │MEM│WB │               │ │
    // │ │                                                     │ │
    // │ │ Instruction 5:                                      │ │
    // │ │ │   │   │   │   │IF │ID │EX │MEM│               │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ Pipeline Stages:                                        │
    // │ IF  = Instruction Fetch                                 │
    // │ ID  = Instruction Decode                                │
    // │ EX  = Execute                                           │
    // │ MEM = Memory Access                                     │
    // │ WB  = Write Back                                        │
    // │                                                         │
    // │ Throughput: 1 instruction per clock cycle (ideally)    │
    // │ Latency: 5 clock cycles per instruction                │
    // └─────────────────────────────────────────────────────────┘
    
    // Pipeline Hazards:
    // ┌─ PIPELINE HAZARDS ──────────────────────────────────────┐
    // │                                                         │
    // │ 1. STRUCTURAL HAZARDS:                                  │
    // │ ┌─ Resource Conflict ─────────────────────────────────┐ │
    // │ │ IF stage needs memory  │  MEM stage needs memory   │ │
    // │ │ at same time           │  at same time             │ │
    // │ │ ┌─────────────────────┐│ ┌─────────────────────────┐│ │
    // │ │ │ Fetch Instruction   ││ │ Load/Store Data         ││ │
    // │ │ └─────────────────────┘│ └─────────────────────────┘│ │
    // │ │           └──────────────┬──────────────┘           │ │
    // │ │                   MEMORY CONFLICT                   │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ 2. DATA HAZARDS:                                        │
    // │ ┌─ Read After Write (RAW) ────────────────────────────┐ │
    // │ │ ADD R1, R2, R3  ; R1 = R2 + R3                     │ │
    // │ │ SUB R4, R1, R5  ; R4 = R1 - R5 (needs R1!)         │ │
    // │ │                                                     │ │
    // │ │ Clock: 1   2   3   4   5   6   7                   │ │
    // │ │ ADD:  │IF │ID │EX │MEM│WB │   │   │               │ │
    // │ │ SUB:  │   │IF │ID │ ? │   │   │   │ (R1 not ready)│ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ 3. CONTROL HAZARDS:                                     │
    // │ ┌─ Branch Prediction ─────────────────────────────────┐ │
    // │ │ BEQ R1, R2, LABEL ; Branch if equal                │ │
    // │ │ ADD R3, R4, R5    ; Next instruction               │ │
    // │ │                                                     │ │
    // │ │ Problem: Don't know if branch taken until EX stage │ │
    // │ │ Solution: Branch prediction, speculative execution │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- Pipeline Concepts ---" << std::endl;
    std::cout << "CPU Pipeline stages (RISC model):" << std::endl;
    std::cout << "1. IF (Instruction Fetch): Get instruction from memory" << std::endl;
    std::cout << "2. ID (Instruction Decode): Decode and read registers" << std::endl;
    std::cout << "3. EX (Execute): Perform ALU operations" << std::endl;
    std::cout << "4. MEM (Memory): Access data memory if needed" << std::endl;
    std::cout << "5. WB (Write Back): Write result to register" << std::endl;
    
    std::cout << "\n--- Pipeline Benefits ---" << std::endl;
    std::cout << "• Increased throughput (instructions per second)" << std::endl;
    std::cout << "• Better resource utilization" << std::endl;
    std::cout << "• Overlapped execution" << std::endl;
    
    std::cout << "\n--- Pipeline Challenges ---" << std::endl;
    std::cout << "• Data dependencies (RAW, WAR, WAW hazards)" << std::endl;
    std::cout << "• Control hazards (branches)" << std::endl;
    std::cout << "• Structural hazards (resource conflicts)" << std::endl;
    std::cout << "• Pipeline stalls and bubbles" << std::endl;
    
    // Demonstrate simple pipeline simulation
    std::cout << "\n--- Simple Pipeline Simulation ---" << std::endl;
    std::vector<std::string> instructions = {
        "ADD R1, R2, R3",
        "SUB R4, R1, R5",  // Data dependency on R1
        "MUL R6, R7, R8",
        "DIV R9, R6, R10"  // Data dependency on R6
    };
    
    std::vector<std::string> stages = {"IF", "ID", "EX", "MEM", "WB"};
    
    std::cout << "Instructions to execute:" << std::endl;
    for (size_t i = 0; i < instructions.size(); ++i) {
        std::cout << "  " << i << ": " << instructions[i] << std::endl;
    }
    
    std::cout << "\nPipeline execution (with stalls for dependencies):" << std::endl;
    std::cout << "Cycle:";
    for (int cycle = 1; cycle <= 10; ++cycle) {
        std::cout << std::setw(4) << cycle;
    }
    std::cout << std::endl;
    
    // Simulate pipeline with basic dependency detection
    for (size_t i = 0; i < instructions.size(); ++i) {
        std::cout << "Instr" << i << ":";
        
        // Simple dependency: instruction 1 depends on 0, instruction 3 depends on 2
        int start_cycle = static_cast<int>(i) + 1;
        if (i == 1) start_cycle += 2; // Stall for data dependency
        if (i == 3) start_cycle += 2; // Stall for data dependency
        
        for (int cycle = 1; cycle <= 10; ++cycle) {
            if (cycle >= start_cycle && cycle < start_cycle + 5) {
                int stage_index = cycle - start_cycle;
                std::cout << std::setw(4) << stages[stage_index];
            } else {
                std::cout << std::setw(4) << " ";
            }
        }
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
}

void demonstrate_memory_hierarchy() {
    std::cout << "=== MEMORY HIERARCHY ===" << std::endl;
    
    // Memory Hierarchy Visualization:
    // ┌─ MEMORY HIERARCHY PYRAMID ──────────────────────────────┐
    // │                                                         │
    // │                    ┌─ CPU REGISTERS ─┐                 │
    // │                    │ 32-64 registers  │                 │
    // │                    │ <1 cycle access  │                 │
    // │                    │ ~1KB capacity    │                 │
    // │                    └──────────────────┘                 │
    // │                 ┌─ L1 CACHE (Split) ──┐                │
    // │                 │ I-Cache │ D-Cache   │                │
    // │                 │ 1-2 cycle access    │                │
    // │                 │ 32-64KB capacity    │                │
    // │                 └─────────────────────┘                │
    // │              ┌──── L2 CACHE (Unified) ────┐            │
    // │              │ 3-10 cycle access          │            │
    // │              │ 256KB-1MB capacity         │            │
    // │              └────────────────────────────┘            │
    // │           ┌────── L3 CACHE (Shared) ──────┐           │
    // │           │ 10-50 cycle access             │           │
    // │           │ 8-32MB capacity                │           │
    // │           └────────────────────────────────┘           │
    // │        ┌────────── MAIN MEMORY (RAM) ─────────┐       │
    // │        │ 100-300 cycle access                 │       │
    // │        │ 4-128GB capacity                     │       │
    // │        │ DRAM technology                      │       │
    // │        └──────────────────────────────────────┘       │
    // │     ┌─────────── SECONDARY STORAGE ──────────────┐     │
    // │     │ 10,000-1,000,000 cycle access             │     │
    // │     │ 500GB-10TB capacity                       │     │
    // │     │ SSD/HDD technology                        │     │
    // │     └───────────────────────────────────────────┘     │
    // │                                                         │
    // │ ↑ FASTER ACCESS    ↑ SMALLER CAPACITY                  │
    // │ ↓ SLOWER ACCESS    ↓ LARGER CAPACITY                   │
    // └─────────────────────────────────────────────────────────┘
    
    // Cache Organization:
    // ┌─ CACHE ORGANIZATION ────────────────────────────────────┐
    // │                                                         │
    // │ DIRECT MAPPED CACHE:                                    │
    // │ ┌─ Memory Address (32-bit) ───────────────────────────┐ │
    // │ │ Tag (20 bits) │ Index (8 bits) │ Offset (4 bits) │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ ┌─ Cache Structure ───────────────────────────────────┐ │
    // │ │ Index │ Valid │ Tag      │ Data Block             │ │
    // │ │   0   │   1   │ 0x12345  │ [16 bytes of data]     │ │
    // │ │   1   │   1   │ 0xABCDE  │ [16 bytes of data]     │ │
    // │ │   2   │   0   │    --    │ [invalid]              │ │
    // │ │  ...  │  ...  │   ...    │ ...                    │ │
    // │ │  255  │   1   │ 0x67890  │ [16 bytes of data]     │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ Cache Lookup Process:                                   │
    // │ 1. Extract index from address                           │
    // │ 2. Check if valid bit is set                           │
    // │ 3. Compare stored tag with address tag                 │
    // │ 4. If match: HIT, use offset to get data               │
    // │ 5. If no match: MISS, fetch from next level            │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- Memory Hierarchy Levels ---" << std::endl;
    std::cout << "1. CPU Registers:" << std::endl;
    std::cout << "   • Access time: <1 cycle" << std::endl;
    std::cout << "   • Capacity: ~1KB (32-64 registers)" << std::endl;
    std::cout << "   • Directly accessible by instructions" << std::endl;
    
    std::cout << "\n2. L1 Cache (Level 1):" << std::endl;
    std::cout << "   • Access time: 1-2 cycles" << std::endl;
    std::cout << "   • Capacity: 32-64KB" << std::endl;
    std::cout << "   • Split: Instruction cache (I-cache) + Data cache (D-cache)" << std::endl;
    
    std::cout << "\n3. L2 Cache (Level 2):" << std::endl;
    std::cout << "   • Access time: 3-10 cycles" << std::endl;
    std::cout << "   • Capacity: 256KB-1MB" << std::endl;
    std::cout << "   • Unified (instructions + data)" << std::endl;
    
    std::cout << "\n4. L3 Cache (Level 3):" << std::endl;
    std::cout << "   • Access time: 10-50 cycles" << std::endl;
    std::cout << "   • Capacity: 8-32MB" << std::endl;
    std::cout << "   • Shared among cores" << std::endl;
    
    std::cout << "\n5. Main Memory (RAM):" << std::endl;
    std::cout << "   • Access time: 100-300 cycles" << std::endl;
    std::cout << "   • Capacity: 4-128GB" << std::endl;
    std::cout << "   • DRAM technology" << std::endl;
    
    std::cout << "\n6. Secondary Storage:" << std::endl;
    std::cout << "   • Access time: 10,000-1,000,000 cycles" << std::endl;
    std::cout << "   • Capacity: 500GB-10TB" << std::endl;
    std::cout << "   • SSD/HDD technology" << std::endl;
    
    // Demonstrate cache performance impact
    std::cout << "\n--- Cache Performance Demonstration ---" << std::endl;
    
    const size_t array_size = 1024 * 1024; // 1MB
    std::vector<int> data(array_size);
    
    // Initialize data
    for (size_t i = 0; i < array_size; ++i) {
        data[i] = static_cast<int>(i);
    }
    
    // Sequential access (cache-friendly)
    auto start_time = std::chrono::high_resolution_clock::now();
    long long sum1 = 0;
    for (size_t i = 0; i < array_size; ++i) {
        sum1 += data[i];
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto sequential_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    
    // Random access (cache-unfriendly)
    start_time = std::chrono::high_resolution_clock::now();
    long long sum2 = 0;
    for (size_t i = 0; i < array_size; ++i) {
        size_t index = (i * 1023) % array_size; // Pseudo-random pattern
        sum2 += data[index];
    }
    end_time = std::chrono::high_resolution_clock::now();
    auto random_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    
    std::cout << "Sequential access time: " << sequential_time.count() << " μs" << std::endl;
    std::cout << "Random access time: " << random_time.count() << " μs" << std::endl;
    std::cout << "Performance ratio: " << 
                 static_cast<double>(random_time.count()) / sequential_time.count() 
                 << "x slower for random access" << std::endl;
    std::cout << "(Results show cache locality importance)" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_assembly_basics() {
    std::cout << "=== ASSEMBLY LANGUAGE BASICS ===" << std::endl;
    
    // Assembly Language Structure:
    // ┌─ ASSEMBLY INSTRUCTION FORMAT ───────────────────────────┐
    // │                                                         │
    // │ [LABEL:] MNEMONIC [OPERAND1], [OPERAND2], [OPERAND3]   │
    // │                                                         │
    // │ Examples:                                               │
    // │ ┌─ x86-64 Assembly ───────────────────────────────────┐ │
    // │ │ mov    rax, rbx        ; Copy RBX to RAX            │ │
    // │ │ add    rax, 42         ; Add immediate 42 to RAX    │ │
    // │ │ push   rbp             ; Push RBP onto stack        │ │
    // │ │ pop    rbp             ; Pop from stack to RBP      │ │
    // │ │ jmp    loop_start      ; Unconditional jump         │ │
    // │ │ cmp    rax, rbx        ; Compare RAX and RBX        │ │
    // │ │ je     equal_label     ; Jump if equal              │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ ┌─ ARM Assembly ──────────────────────────────────────┐ │
    // │ │ mov    r0, r1          ; Copy R1 to R0              │ │
    // │ │ add    r0, r1, #42     ; R0 = R1 + 42               │ │
    // │ │ ldr    r0, [r1]        ; Load from memory at R1     │ │
    // │ │ str    r0, [r1]        ; Store R0 to memory at R1   │ │
    // │ │ b      loop_start      ; Branch (unconditional)     │ │
    // │ │ cmp    r0, r1          ; Compare R0 and R1          │ │
    // │ │ beq    equal_label     ; Branch if equal            │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- Assembly Language Components ---" << std::endl;
    std::cout << "1. Instructions (Mnemonics):" << std::endl;
    std::cout << "   • Data movement: MOV, LOAD, STORE" << std::endl;
    std::cout << "   • Arithmetic: ADD, SUB, MUL, DIV" << std::endl;
    std::cout << "   • Logic: AND, OR, XOR, NOT" << std::endl;
    std::cout << "   • Control flow: JMP, CALL, RET, conditional branches" << std::endl;
    
    std::cout << "\n2. Operands:" << std::endl;
    std::cout << "   • Registers: RAX, RBX, R0, R1, etc." << std::endl;
    std::cout << "   • Immediate values: #42, $100, 0xFF" << std::endl;
    std::cout << "   • Memory addresses: [RBP+8], [R1, #4]" << std::endl;
    
    std::cout << "\n3. Addressing Modes:" << std::endl;
    std::cout << "   • Register: mov rax, rbx" << std::endl;
    std::cout << "   • Immediate: mov rax, 42" << std::endl;
    std::cout << "   • Direct: mov rax, [address]" << std::endl;
    std::cout << "   • Indirect: mov rax, [rbx]" << std::endl;
    std::cout << "   • Indexed: mov rax, [rbx + rcx*4 + 8]" << std::endl;
    
    // Demonstrate C to Assembly mapping
    std::cout << "\n--- C to Assembly Translation Example ---" << std::endl;
    std::cout << "C function:" << std::endl;
    std::cout << "  int add_numbers(int a, int b) {" << std::endl;
    std::cout << "      int result = a + b;" << std::endl;
    std::cout << "      return result;" << std::endl;
    std::cout << "  }" << std::endl;
    
    std::cout << "\nCorresponding x86-64 assembly (simplified):" << std::endl;
    std::cout << "  add_numbers:" << std::endl;
    std::cout << "      push   rbp           ; Save old frame pointer" << std::endl;
    std::cout << "      mov    rbp, rsp      ; Set up new frame pointer" << std::endl;
    std::cout << "      mov    eax, edi      ; a (first parameter) to EAX" << std::endl;
    std::cout << "      add    eax, esi      ; Add b (second parameter)" << std::endl;
    std::cout << "      pop    rbp           ; Restore frame pointer" << std::endl;
    std::cout << "      ret                  ; Return (result in EAX)" << std::endl;
    
    std::cout << "\nCorresponding ARM assembly (simplified):" << std::endl;
    std::cout << "  add_numbers:" << std::endl;
    std::cout << "      add    r0, r0, r1    ; R0 = R0 + R1 (a + b)" << std::endl;
    std::cout << "      bx     lr            ; Return (result in R0)" << std::endl;
    
    // Show register usage conventions
    std::cout << "\n--- Register Conventions ---" << std::endl;
    std::cout << "x86-64 System V ABI:" << std::endl;
    std::cout << "  • RAX: Return value" << std::endl;
    std::cout << "  • RDI, RSI, RDX, RCX, R8, R9: Function arguments" << std::endl;
    std::cout << "  • RBP: Frame pointer" << std::endl;
    std::cout << "  • RSP: Stack pointer" << std::endl;
    std::cout << "  • RBX, R12-R15: Callee-saved registers" << std::endl;
    
    std::cout << "\nARM AAPCS (32-bit):" << std::endl;
    std::cout << "  • R0-R3: Function arguments and return values" << std::endl;
    std::cout << "  • R4-R11: General purpose (callee-saved)" << std::endl;
    std::cout << "  • R12: Intra-procedure call register" << std::endl;
    std::cout << "  • R13 (SP): Stack pointer" << std::endl;
    std::cout << "  • R14 (LR): Link register (return address)" << std::endl;
    std::cout << "  • R15 (PC): Program counter" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_performance_characteristics() {
    std::cout << "=== PERFORMANCE CHARACTERISTICS ===" << std::endl;
    
    // Performance Comparison Chart:
    // ┌─ ARCHITECTURE PERFORMANCE COMPARISON ───────────────────┐
    // │                                                         │
    // │ Metric          │ x86-64    │ ARM      │ RISC-V        │
    // │ ──────────────────────────────────────────────────────  │
    // │ Instruction     │ Variable  │ Fixed    │ Fixed         │
    // │ Length          │ (1-15B)   │ (4B)     │ (4B)          │
    // │                 │           │          │               │
    // │ Decoding        │ Complex   │ Simple   │ Simple        │
    // │ Complexity      │ Multi-step│ 1-2 step│ 1-2 step      │
    // │                 │           │          │               │
    // │ Pipeline        │ Deep      │ Moderate │ Simple        │
    // │ Depth           │ (14-20)   │ (8-13)   │ (5-7)         │
    // │                 │           │          │               │
    // │ Branch          │ Advanced  │ Good     │ Basic         │
    // │ Prediction      │ 95%+      │ 90%+     │ 85%+          │
    // │                 │           │          │               │
    // │ Register        │ Limited   │ Large    │ Large         │
    // │ File            │ (16 GP)   │ (31 GP)  │ (31 GP)       │
    // │                 │           │          │               │
    // │ Code Density    │ High      │ Good     │ Good          │
    // │ (instructions/  │ (Variable │ (Thumb   │ (Compressed   │
    // │ byte)           │ length)   │ mode)    │ extension)    │
    // │                 │           │          │               │
    // │ Power           │ High      │ Low      │ Very Low      │
    // │ Consumption     │ (Desktop) │ (Mobile) │ (Embedded)    │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- Architecture Performance Trade-offs ---" << std::endl;
    
    std::cout << "x86-64 (Intel/AMD):" << std::endl;
    std::cout << "  Strengths:" << std::endl;
    std::cout << "  • High single-thread performance" << std::endl;
    std::cout << "  • Rich instruction set" << std::endl;
    std::cout << "  • Excellent software ecosystem" << std::endl;
    std::cout << "  • Advanced branch prediction" << std::endl;
    std::cout << "  • Out-of-order execution" << std::endl;
    std::cout << "  Weaknesses:" << std::endl;
    std::cout << "  • High power consumption" << std::endl;
    std::cout << "  • Complex decoding logic" << std::endl;
    std::cout << "  • Legacy instruction baggage" << std::endl;
    
    std::cout << "\nARM:" << std::endl;
    std::cout << "  Strengths:" << std::endl;
    std::cout << "  • Power efficient" << std::endl;
    std::cout << "  • Good performance per watt" << std::endl;
    std::cout << "  • Simple, regular instruction format" << std::endl;
    std::cout << "  • Large register file" << std::endl;
    std::cout << "  • Conditional execution" << std::endl;
    std::cout << "  Weaknesses:" << std::endl;
    std::cout << "  • Load/Store architecture overhead" << std::endl;
    std::cout << "  • More instructions for complex operations" << std::endl;
    
    std::cout << "\nRISC-V:" << std::endl;
    std::cout << "  Strengths:" << std::endl;
    std::cout << "  • Clean, simple design" << std::endl;
    std::cout << "  • Open standard (no licensing)" << std::endl;
    std::cout << "  • Modular and extensible" << std::endl;
    std::cout << "  • Easy to implement" << std::endl;
    std::cout << "  Weaknesses:" << std::endl;
    std::cout << "  • Limited software ecosystem" << std::endl;
    std::cout << "  • Fewer optimizations in compilers" << std::endl;
    std::cout << "  • Still emerging in market" << std::endl;
    
    // Demonstrate simple performance measurement
    std::cout << "\n--- Simple Performance Measurement ---" << std::endl;
    
    const int iterations = 1000000;
    
    // Integer arithmetic benchmark
    auto start_time = std::chrono::high_resolution_clock::now();
    volatile int result = 0;
    for (int i = 0; i < iterations; ++i) {
        result += i * 2 + 1;
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto int_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    
    // Floating point benchmark
    start_time = std::chrono::high_resolution_clock::now();
    volatile double fresult = 0.0;
    for (int i = 0; i < iterations; ++i) {
        fresult += static_cast<double>(i) * 2.5 + 1.0;
    }
    end_time = std::chrono::high_resolution_clock::now();
    auto float_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    
    std::cout << "Integer arithmetic (" << iterations << " ops): " 
              << int_time.count() << " μs" << std::endl;
    std::cout << "Floating point (" << iterations << " ops): " 
              << float_time.count() << " μs" << std::endl;
    std::cout << "Performance ratio: " << 
                 static_cast<double>(float_time.count()) / int_time.count() 
                 << "x (float vs int)" << std::endl;
    
    std::cout << std::endl;
}

int main() {
    std::cout << "CPU ARCHITECTURE AND COMPUTER SYSTEMS TUTORIAL" << std::endl;
    std::cout << "=============================================" << std::endl << std::endl;
    
    demonstrate_cpu_architecture_overview();
    demonstrate_instruction_sets();
    demonstrate_endianness();
    demonstrate_cpu_pipeline();
    demonstrate_memory_hierarchy();
    demonstrate_assembly_basics();
    demonstrate_performance_characteristics();
    
    std::cout << "CPU Architecture tutorial completed successfully!" << std::endl;
    std::cout << "\nKey Takeaways:" << std::endl;
    std::cout << "━━━━━━━━━━━━━━" << std::endl;
    
    std::cout << "\nARCHITECTURE CHOICES:" << std::endl;
    std::cout << "• CISC (x86): Rich instructions, complex decoding, high performance" << std::endl;
    std::cout << "• RISC (ARM): Simple instructions, power efficient, easy pipelining" << std::endl;
    std::cout << "• Open (RISC-V): Clean design, no licensing, emerging ecosystem" << std::endl;
    
    std::cout << "\nENDIANNESS:" << std::endl;
    std::cout << "• Little Endian: LSB at lowest address (x86, most ARM)" << std::endl;
    std::cout << "• Big Endian: MSB at lowest address (network protocols)" << std::endl;
    std::cout << "• Always consider when working with binary data" << std::endl;
    
    std::cout << "\nPERFORMANCE FACTORS:" << std::endl;
    std::cout << "• Pipeline depth and hazards" << std::endl;
    std::cout << "• Memory hierarchy and cache locality" << std::endl;
    std::cout << "• Instruction-level parallelism" << std::endl;
    std::cout << "• Branch prediction accuracy" << std::endl;
    
    std::cout << "\nPRACTICAL IMPLICATIONS:" << std::endl;
    std::cout << "• Write cache-friendly code (sequential access)" << std::endl;
    std::cout << "• Minimize branches in tight loops" << std::endl;
    std::cout << "• Consider data alignment and padding" << std::endl;
    std::cout << "• Understand your target architecture's strengths" << std::endl;
    
    return 0;
}