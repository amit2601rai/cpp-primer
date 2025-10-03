/**
 * @file memory_addressing.cpp
 * @author GitHub Copilot
 * @brief Tutorial code for physical memory addressing, virtual memory management, and memory system architecture.
 */

// This program provides a comprehensive overview of memory addressing including:
// - Physical vs Virtual memory addressing
// - Memory Management Unit (MMU) operation
// - Page tables and address translation
// - Physical memory discovery and management
// - DRAM organization and mapping
// - Memory controllers and hardware details
// - Operating system memory management
// - NUMA and multi-channel memory systems

#include <iostream>
#include <vector>
#include <map>
#include <cstdint>
#include <iomanip>
#include <chrono>
#include <unistd.h>
#include <sys/mman.h>

// Simulated structures for demonstration
struct PhysicalPageFrame {
    uint64_t physical_address;
    bool is_free;
    int reference_count;
    int numa_node;
};

struct PageTableEntry {
    uint64_t physical_page : 40;  // Physical page number
    uint64_t present : 1;         // Page is in physical memory
    uint64_t writable : 1;        // Page is writable
    uint64_t user : 1;           // User mode accessible
    uint64_t accessed : 1;       // Page has been accessed
    uint64_t dirty : 1;          // Page has been modified
    uint64_t reserved : 19;      // Reserved bits
};

void demonstrate_physical_memory_overview() {
    std::cout << "=== PHYSICAL MEMORY ADDRESSING OVERVIEW ===" << std::endl;
    
    // Physical Memory System Architecture:
    // ┌─ COMPLETE MEMORY SYSTEM ARCHITECTURE ───────────────────┐
    // │                                                         │
    // │ ┌─ CPU Core ──────────────────────────────────────────┐ │
    // │ │ Virtual Address: 0x7FFF12345000                     │ │
    // │ │ ┌─ MMU (Memory Management Unit) ─────────────────┐  │ │
    // │ │ │ • Address Translation                          │  │ │
    // │ │ │ • Page Table Walker                            │  │ │
    // │ │ │ • TLB (Translation Lookaside Buffer)           │  │ │
    // │ │ └────────────────────────────────────────────────┘  │ │
    // │ │ Physical Address: 0x12345000                        │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                         │                               │
    // │                         ↓                               │
    // │ ┌─ Memory Controller ─────────────────────────────────┐ │
    // │ │ • Address Decoding                                  │ │
    // │ │ • Channel/Bank Selection                            │ │
    // │ │ • Row/Column Address Generation                     │ │
    // │ │ • Memory Interleaving                               │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                         │                               │
    // │                         ↓                               │
    // │ ┌─ Physical DRAM Layout ──────────────────────────────┐ │
    // │ │ Channel 0 │ Channel 1 │ Channel 2 │ Channel 3      │ │
    // │ │ ┌───────┐ │ ┌───────┐ │ ┌───────┐ │ ┌───────┐      │ │
    // │ │ │DIMM 0 │ │ │DIMM 1 │ │ │DIMM 2 │ │ │DIMM 3 │      │ │
    // │ │ │Rank 0 │ │ │Rank 0 │ │ │Rank 0 │ │ │Rank 0 │      │ │
    // │ │ │Bank 0-7│ │ │Bank 0-7│ │ │Bank 0-7│ │ │Bank 0-7│      │ │
    // │ │ └───────┘ │ └───────┘ │ └───────┘ │ └───────┘      │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ Key Point: Physical addresses are MANAGED, not fixed!  │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- Memory System Layers ---" << std::endl;
    std::cout << "1. Application Layer:" << std::endl;
    std::cout << "   • Uses virtual addresses (0x7FFF...)" << std::endl;
    std::cout << "   • Never sees physical addresses directly" << std::endl;
    std::cout << "   • Memory appears as contiguous address space" << std::endl;
    
    std::cout << "\n2. Operating System Layer:" << std::endl;
    std::cout << "   • Manages physical memory allocation" << std::endl;
    std::cout << "   • Maintains page tables for address translation" << std::endl;
    std::cout << "   • Handles memory protection and sharing" << std::endl;
    
    std::cout << "\n3. Hardware Layer (MMU):" << std::endl;
    std::cout << "   • Translates virtual to physical addresses" << std::endl;
    std::cout << "   • Caches translations in TLB" << std::endl;
    std::cout << "   • Enforces memory protection" << std::endl;
    
    std::cout << "\n4. Memory Controller:" << std::endl;
    std::cout << "   • Maps physical addresses to DRAM locations" << std::endl;
    std::cout << "   • Handles channel/bank interleaving" << std::endl;
    std::cout << "   • Controls DRAM timing and refresh" << std::endl;
    
    std::cout << "\n5. Physical DRAM:" << std::endl;
    std::cout << "   • Actual storage cells organized in arrays" << std::endl;
    std::cout << "   • Addressed by channel/rank/bank/row/column" << std::endl;
    std::cout << "   • Layout determined by hardware design" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_physical_memory_discovery() {
    std::cout << "=== PHYSICAL MEMORY DISCOVERY PROCESS ===" << std::endl;
    
    // Memory Discovery Process:
    // ┌─ SYSTEM BOOT MEMORY DISCOVERY ──────────────────────────┐
    // │                                                         │
    // │ Step 1: BIOS/UEFI POST (Power-On Self Test)           │
    // │ ┌─ Hardware Detection ────────────────────────────────┐ │
    // │ │ • SPD (Serial Presence Detect) readout             │ │
    // │ │ • Memory module identification                      │ │
    // │ │ • Speed/timing/capacity detection                   │ │
    // │ │ • Memory testing and validation                     │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                         │                               │
    // │                         ↓                               │
    // │ Step 2: Memory Map Creation (E820 on x86)              │
    // │ ┌─ Physical Memory Layout ────────────────────────────┐ │
    // │ │ 0x00000000-0x0009FFFF │ Usable RAM (640KB)         │ │
    // │ │ 0x000A0000-0x000FFFFF │ Reserved (Video/BIOS)      │ │
    // │ │ 0x00100000-0x7FFEFFFF │ Usable RAM (Main Memory)   │ │
    // │ │ 0x7FFF0000-0x7FFFFFFF │ ACPI Data                  │ │
    // │ │ 0x80000000-0xFEFFFFFF │ PCI/Device Memory          │ │
    // │ │ 0xFF000000-0xFFFFFFFF │ BIOS ROM                   │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                         │                               │
    // │                         ↓                               │
    // │ Step 3: Bootloader Processing                          │
    // │ ┌─ Memory Handoff ────────────────────────────────────┐ │
    // │ │ • Parse BIOS memory map                             │ │
    // │ │ • Identify usable vs reserved regions              │ │
    // │ │ • Set up initial page tables                       │ │
    // │ │ • Pass memory info to kernel                       │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                         │                               │
    // │                         ↓                               │
    // │ Step 4: OS Memory Manager Initialization              │
    // │ ┌─ Kernel Memory Setup ───────────────────────────────┐ │
    // │ │ • Parse memory map from bootloader                 │ │
    // │ │ • Initialize page frame database                   │ │
    // │ │ • Set up physical memory allocator                 │ │
    // │ │ • Create virtual memory subsystem                  │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- Boot Process Memory Discovery ---" << std::endl;
    std::cout << "1. BIOS/UEFI POST Phase:" << std::endl;
    std::cout << "   • Detects memory modules via SPD (Serial Presence Detect)" << std::endl;
    std::cout << "   • Determines memory size, speed, and timings" << std::endl;
    std::cout << "   • Performs basic memory testing" << std::endl;
    std::cout << "   • Maps memory-mapped I/O regions" << std::endl;
    
    std::cout << "\n2. Memory Map Creation:" << std::endl;
    std::cout << "   • Creates E820 memory map (x86) or similar" << std::endl;
    std::cout << "   • Identifies usable vs reserved memory regions" << std::endl;
    std::cout << "   • Accounts for ACPI tables, device memory, etc." << std::endl;
    
    std::cout << "\n3. Operating System Initialization:" << std::endl;
    std::cout << "   • Parses memory map from firmware" << std::endl;
    std::cout << "   • Initializes physical page frame database" << std::endl;
    std::cout << "   • Sets up memory allocators and virtual memory" << std::endl;
    
    // Simulate memory map discovery
    std::cout << "\n--- Simulated System Memory Map ---" << std::endl;
    
    struct MemoryRegion {
        uint64_t start;
        uint64_t end;
        std::string type;
        std::string description;
    };
    
    std::vector<MemoryRegion> memory_map = {
        {0x00000000, 0x0009FFFF, "Usable", "Low Memory (640KB)"},
        {0x000A0000, 0x000FFFFF, "Reserved", "Video Memory / BIOS"},
        {0x00100000, 0x7FFEFFFF, "Usable", "Main System RAM"},
        {0x7FFF0000, 0x7FFFFFFF, "ACPI", "ACPI Tables"},
        {0x80000000, 0xFEFFFFFF, "MMIO", "PCI/Device Memory"},
        {0xFF000000, 0xFFFFFFFF, "Reserved", "BIOS/UEFI ROM"}
    };
    
    std::cout << "Physical Memory Regions:" << std::endl;
    std::cout << "Start       End         Type      Description" << std::endl;
    std::cout << "────────────────────────────────────────────────────" << std::endl;
    
    for (const auto& region : memory_map) {
        std::cout << "0x" << std::hex << std::setfill('0') << std::setw(8) << region.start
                  << "  0x" << std::setw(8) << region.end << std::dec
                  << "  " << std::setw(8) << std::left << region.type
                  << "  " << region.description << std::endl;
    }
    
    std::cout << std::endl;
}

void demonstrate_virtual_to_physical_translation() {
    std::cout << "=== VIRTUAL TO PHYSICAL ADDRESS TRANSLATION ===" << std::endl;
    
    // Address Translation Process:
    // ┌─ VIRTUAL TO PHYSICAL ADDRESS TRANSLATION ───────────────┐
    // │                                                         │
    // │ Virtual Address (64-bit x86-64):                       │
    // │ ┌─ Bit Layout ────────────────────────────────────────┐ │
    // │ │63    48│47    39│38    30│29    21│20    12│11    0││ │
    // │ │ Sign   │ PML4   │ PDPT   │ PD     │ PT     │Offset ││ │
    // │ │ Extend │ Index  │ Index  │ Index  │ Index  │       ││ │
    // │ │        │(9 bits)│(9 bits)│(9 bits)│(9 bits)│(12b) ││ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ 4-Level Page Table Walk:                               │
    // │ ┌─ Step 1: PML4 (Page Map Level 4) ──────────────────┐ │
    // │ │ CR3 Register → PML4 Base Address                   │ │
    // │ │ PML4[PML4_index] → PDPT Physical Address           │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                         │                               │
    // │                         ↓                               │
    // │ ┌─ Step 2: PDPT (Page Directory Pointer Table) ──────┐ │
    // │ │ PDPT Base + PDPT_index → PD Physical Address       │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                         │                               │
    // │                         ↓                               │
    // │ ┌─ Step 3: PD (Page Directory) ───────────────────────┐ │
    // │ │ PD Base + PD_index → PT Physical Address           │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                         │                               │
    // │                         ↓                               │
    // │ ┌─ Step 4: PT (Page Table) ────────────────────────────┐ │
    // │ │ PT Base + PT_index → Physical Page Address         │ │
    // │ │ Physical Page + Offset → Final Physical Address    │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ TLB (Translation Lookaside Buffer) Optimization:       │
    // │ ┌─ TLB Cache ─────────────────────────────────────────┐ │
    // │ │ Virtual Page → Physical Page (cached translation)  │ │
    // │ │ Avoids 4-step page table walk for hot pages       │ │
    // │ │ Typical size: 64-1024 entries per level           │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- Address Translation Mechanism ---" << std::endl;
    std::cout << "Virtual Address Structure (x86-64):" << std::endl;
    std::cout << "• Bits 63-48: Sign extension (not used)" << std::endl;
    std::cout << "• Bits 47-39: PML4 index (9 bits, 512 entries)" << std::endl;
    std::cout << "• Bits 38-30: PDPT index (9 bits, 512 entries)" << std::endl;
    std::cout << "• Bits 29-21: PD index (9 bits, 512 entries)" << std::endl;
    std::cout << "• Bits 20-12: PT index (9 bits, 512 entries)" << std::endl;
    std::cout << "• Bits 11-0:  Page offset (12 bits, 4KB pages)" << std::endl;
    
    std::cout << "\n--- Page Table Walk Example ---" << std::endl;
    
    // Simulate a virtual address translation
    uint64_t virtual_addr = 0x7FFF12345678;
    
    // Extract page table indices
    uint64_t pml4_index = (virtual_addr >> 39) & 0x1FF;
    uint64_t pdpt_index = (virtual_addr >> 30) & 0x1FF;
    uint64_t pd_index = (virtual_addr >> 21) & 0x1FF;
    uint64_t pt_index = (virtual_addr >> 12) & 0x1FF;
    uint64_t page_offset = virtual_addr & 0xFFF;
    
    std::cout << "Virtual Address: 0x" << std::hex << virtual_addr << std::dec << std::endl;
    std::cout << "Page Table Indices:" << std::endl;
    std::cout << "  PML4 Index: " << pml4_index << " (0x" << std::hex << pml4_index << std::dec << ")" << std::endl;
    std::cout << "  PDPT Index: " << pdpt_index << " (0x" << std::hex << pdpt_index << std::dec << ")" << std::endl;
    std::cout << "  PD Index:   " << pd_index << " (0x" << std::hex << pd_index << std::dec << ")" << std::endl;
    std::cout << "  PT Index:   " << pt_index << " (0x" << std::hex << pt_index << std::dec << ")" << std::endl;
    std::cout << "  Page Offset: " << page_offset << " (0x" << std::hex << page_offset << std::dec << ")" << std::endl;
    
    // Simulate translation result
    uint64_t physical_page = 0x12345;  // Simulated physical page number
    uint64_t physical_addr = (physical_page << 12) | page_offset;
    
    std::cout << "Translation Result:" << std::endl;
    std::cout << "  Physical Page: 0x" << std::hex << physical_page << std::dec << std::endl;
    std::cout << "  Physical Address: 0x" << std::hex << physical_addr << std::dec << std::endl;
    
    std::cout << "\n--- TLB (Translation Lookaside Buffer) ---" << std::endl;
    std::cout << "• Hardware cache for page translations" << std::endl;
    std::cout << "• Avoids expensive page table walks" << std::endl;
    std::cout << "• Typical sizes: 64-1024 entries per level" << std::endl;
    std::cout << "• Separate TLBs for instructions and data" << std::endl;
    std::cout << "• Must be flushed on context switches" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_dram_organization() {
    std::cout << "=== DRAM ORGANIZATION AND ADDRESSING ===" << std::endl;
    
    // DRAM Hierarchical Organization:
    // ┌─ DRAM MEMORY HIERARCHY ─────────────────────────────────┐
    // │                                                         │
    // │ Memory Controller View of Physical Address:             │
    // │ ┌─ 32-bit Physical Address Breakdown ─────────────────┐ │
    // │ │31 30│29  27│26  24│23     14│13      6│5       0│ │
    // │ │ CH │ DIMM │ RANK │   ROW    │ COLUMN  │ BURST   │ │
    // │ │(2b)│ (3b) │ (3b) │  (10b)   │  (8b)   │  (6b)   │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ Physical DRAM Layout:                                   │
    // │ ┌─ Channel 0 ────────────┐ ┌─ Channel 1 ──────────────┐ │
    // │ │ ┌─ DIMM 0 ───────────┐ │ │ ┌─ DIMM 0 ─────────────┐ │ │
    // │ │ │ ┌─ Rank 0 ───────┐ │ │ │ │ ┌─ Rank 0 ─────────┐ │ │ │
    // │ │ │ │ ┌─ Bank 0 ───┐ │ │ │ │ │ │ ┌─ Bank 0 ─────┐ │ │ │ │
    // │ │ │ │ │ Row Array  │ │ │ │ │ │ │ │ Row Array    │ │ │ │ │
    // │ │ │ │ │ [8K rows]  │ │ │ │ │ │ │ │ [8K rows]    │ │ │ │ │
    // │ │ │ │ │ [1K cols]  │ │ │ │ │ │ │ │ [1K cols]    │ │ │ │ │
    // │ │ │ │ └────────────┘ │ │ │ │ │ │ └──────────────┘ │ │ │ │
    // │ │ │ │ ... Bank 7     │ │ │ │ │ │ ... Bank 7       │ │ │ │
    // │ │ │ └────────────────┘ │ │ │ │ └──────────────────┘ │ │ │
    // │ │ │ ┌─ Rank 1 ───────┐ │ │ │ │ ┌─ Rank 1 ─────────┐ │ │ │
    // │ │ │ │ Banks 0-7      │ │ │ │ │ │ Banks 0-7        │ │ │ │
    // │ │ │ └────────────────┘ │ │ │ │ └──────────────────┘ │ │ │
    // │ │ └────────────────────┘ │ │ └────────────────────────┘ │ │
    // │ │ ┌─ DIMM 1 ───────────┐ │ │ ┌─ DIMM 1 ─────────────┐ │ │
    // │ │ │ Similar structure  │ │ │ │ Similar structure    │ │ │
    // │ │ └────────────────────┘ │ │ └────────────────────────┘ │ │
    // │ └────────────────────────┘ └──────────────────────────────┘ │
    // │                                                         │
    // │ Key Insight: Same logical address can map to different │
    // │ physical DRAM locations based on interleaving!         │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- DRAM Hierarchical Structure ---" << std::endl;
    std::cout << "1. Channel Level:" << std::endl;
    std::cout << "   • Independent memory buses (64-bit wide)" << std::endl;
    std::cout << "   • Parallel access for bandwidth increase" << std::endl;
    std::cout << "   • Typical: 1-4 channels in consumer systems" << std::endl;
    
    std::cout << "\n2. DIMM Level:" << std::endl;
    std::cout << "   • Physical memory modules" << std::endl;
    std::cout << "   • Multiple DIMMs per channel for capacity" << std::endl;
    std::cout << "   • Each DIMM has chip select signals" << std::endl;
    
    std::cout << "\n3. Rank Level:" << std::endl;
    std::cout << "   • Set of DRAM chips accessed simultaneously" << std::endl;
    std::cout << "   • Single/Dual rank DIMMs common" << std::endl;
    std::cout << "   • Different ranks share address/data bus" << std::endl;
    
    std::cout << "\n4. Bank Level:" << std::endl;
    std::cout << "   • Independent arrays within each chip" << std::endl;
    std::cout << "   • Allow parallel operations (bank interleaving)" << std::endl;
    std::cout << "   • Typical: 4-16 banks per chip" << std::endl;
    
    std::cout << "\n5. Row/Column Level:" << std::endl;
    std::cout << "   • 2D array of memory cells" << std::endl;
    std::cout << "   • Row activation required before column access" << std::endl;
    std::cout << "   • Row buffer caches active row data" << std::endl;
    
    // Demonstrate address interleaving
    std::cout << "\n--- Memory Interleaving Example ---" << std::endl;
    
    auto decode_physical_address = [](uint64_t phys_addr) {
        struct DecodedAddress {
            int channel;
            int dimm;
            int rank;
            int bank;
            int row;
            int column;
        };
        
        DecodedAddress decoded;
        // Simplified decoding (actual implementation varies by controller)
        decoded.channel = (phys_addr >> 6) & 0x3;   // Bits 7:6
        decoded.dimm = (phys_addr >> 8) & 0x1;      // Bit 8
        decoded.rank = (phys_addr >> 9) & 0x1;      // Bit 9
        decoded.bank = (phys_addr >> 10) & 0x7;     // Bits 12:10
        decoded.row = (phys_addr >> 13) & 0x3FFF;   // Bits 26:13
        decoded.column = (phys_addr >> 3) & 0x7;    // Bits 5:3
        
        return decoded;
    };
    
    std::vector<uint64_t> test_addresses = {
        0x12345000, 0x12345040, 0x12345080, 0x123450C0
    };
    
    std::cout << "Physical Address Decoding:" << std::endl;
    std::cout << "Address     Channel DIMM Rank Bank Row    Column" << std::endl;
    std::cout << "──────────────────────────────────────────────────" << std::endl;
    
    for (uint64_t addr : test_addresses) {
        auto decoded = decode_physical_address(addr);
        std::cout << "0x" << std::hex << std::setfill('0') << std::setw(8) << addr
                  << std::dec << "     " << decoded.channel
                  << "     " << decoded.dimm
                  << "    " << decoded.rank
                  << "    " << decoded.bank
                  << "  " << std::setw(5) << decoded.row
                  << "   " << decoded.column << std::endl;
    }
    
    std::cout << "\nInterleaving Benefits:" << std::endl;
    std::cout << "• Distributes consecutive addresses across channels/banks" << std::endl;
    std::cout << "• Increases effective memory bandwidth" << std::endl;
    std::cout << "• Reduces bank conflicts for sequential access" << std::endl;
    std::cout << "• Allows parallel operations on different banks" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_memory_controller_operation() {
    std::cout << "=== MEMORY CONTROLLER OPERATION ===" << std::endl;
    
    // Memory Controller State Machine:
    // ┌─ MEMORY CONTROLLER OPERATION ───────────────────────────┐
    // │                                                         │
    // │ Request Processing Pipeline:                            │
    // │ ┌─ Step 1: Address Decode ────────────────────────────┐ │
    // │ │ Physical Address → Channel/DIMM/Rank/Bank/Row/Col   │ │
    // │ │ • Extract address components                        │ │
    // │ │ • Apply interleaving algorithms                     │ │
    // │ │ • Check for address conflicts                       │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                         │                               │
    // │                         ↓                               │
    // │ ┌─ Step 2: Bank State Check ──────────────────────────┐ │
    // │ │ • Is target bank idle/active/precharging?           │ │
    // │ │ • Is target row already open in row buffer?        │ │
    // │ │ • Queue request or issue immediately?               │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                         │                               │
    // │                         ↓                               │
    // │ ┌─ Step 3: Command Scheduling ────────────────────────┐ │
    // │ │ CASE 1: Row Hit (target row open)                  │ │
    // │ │   → Issue READ/WRITE command directly              │ │
    // │ │                                                     │ │
    // │ │ CASE 2: Row Miss (different row open)              │ │
    // │ │   → Issue PRECHARGE → ACTIVATE → READ/WRITE        │ │
    // │ │                                                     │ │
    // │ │ CASE 3: Row Empty (no row open)                    │ │
    // │ │   → Issue ACTIVATE → READ/WRITE                    │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                         │                               │
    // │                         ↓                               │
    // │ ┌─ Step 4: DRAM Command Execution ────────────────────┐ │
    // │ │ • Send commands over DRAM bus                       │ │
    // │ │ • Respect DRAM timing constraints                   │ │
    // │ │ • Handle data transfer and completion               │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- Memory Controller Functions ---" << std::endl;
    std::cout << "1. Address Translation:" << std::endl;
    std::cout << "   • Converts physical addresses to DRAM coordinates" << std::endl;
    std::cout << "   • Applies interleaving for performance" << std::endl;
    std::cout << "   • Maps to specific channel/DIMM/rank/bank/row/column" << std::endl;
    
    std::cout << "\n2. Command Scheduling:" << std::endl;
    std::cout << "   • Optimizes DRAM command ordering" << std::endl;
    std::cout << "   • Handles row buffer management" << std::endl;
    std::cout << "   • Respects DRAM timing constraints" << std::endl;
    
    std::cout << "\n3. Bank State Management:" << std::endl;
    std::cout << "   • Tracks active rows in each bank" << std::endl;
    std::cout << "   • Manages precharge/activate cycles" << std::endl;
    std::cout << "   • Handles refresh operations" << std::endl;
    
    std::cout << "\n--- DRAM Access Patterns ---" << std::endl;
    std::cout << "Row Hit (Best Case):" << std::endl;
    std::cout << "  • Target row already open in row buffer" << std::endl;
    std::cout << "  • Direct column access (CAS latency ~15ns)" << std::endl;
    std::cout << "  • Highest bandwidth and lowest latency" << std::endl;
    
    std::cout << "\nRow Miss (Worst Case):" << std::endl;
    std::cout << "  • Different row open, must close and open new row" << std::endl;
    std::cout << "  • PRECHARGE → ACTIVATE → READ/WRITE sequence" << std::endl;
    std::cout << "  • Total latency ~50ns (RAS + CAS + precharge)" << std::endl;
    
    std::cout << "\nRow Empty (Common Case):" << std::endl;
    std::cout << "  • No row open in target bank" << std::endl;
    std::cout << "  • ACTIVATE → READ/WRITE sequence" << std::endl;
    std::cout << "  • Latency ~35ns (RAS + CAS)" << std::endl;
    
    // Simulate memory access timing
    std::cout << "\n--- Memory Access Timing Simulation ---" << std::endl;
    
    struct MemoryAccess {
        uint64_t address;
        std::string type;
        int latency_ns;
        std::string reason;
    };
    
    std::vector<MemoryAccess> accesses = {
        {0x12345000, "Row Hit", 15, "Same row as previous access"},
        {0x12345040, "Row Hit", 15, "Sequential access in same row"},
        {0x12346000, "Row Miss", 50, "Different row in same bank"},
        {0x22345000, "Row Empty", 35, "Different bank, no row open"},
        {0x12345080, "Row Miss", 50, "Back to first row (was closed)"}
    };
    
    std::cout << "Memory Access Pattern Analysis:" << std::endl;
    std::cout << "Address     Type      Latency  Reason" << std::endl;
    std::cout << "────────────────────────────────────────────────────────" << std::endl;
    
    for (const auto& access : accesses) {
        std::cout << "0x" << std::hex << std::setfill('0') << std::setw(8) << access.address
                  << std::dec << "  " << std::setw(9) << std::left << access.type
                  << " " << std::setw(3) << access.latency_ns << "ns   "
                  << access.reason << std::endl;
    }
    
    std::cout << "\nOptimization Strategies:" << std::endl;
    std::cout << "• Row Buffer Locality: Keep accessing same row" << std::endl;
    std::cout << "• Bank Interleaving: Spread accesses across banks" << std::endl;
    std::cout << "• Prefetching: Predict and pre-activate rows" << std::endl;
    std::cout << "• Request Reordering: Optimize command scheduling" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_numa_architecture() {
    std::cout << "=== NUMA (Non-Uniform Memory Access) SYSTEMS ===" << std::endl;
    
    // NUMA System Architecture:
    // ┌─ NUMA SYSTEM LAYOUT ────────────────────────────────────┐
    // │                                                         │
    // │ ┌─ NUMA Node 0 ─────────────┐ ┌─ NUMA Node 1 ─────────┐ │
    // │ │ ┌─ CPU Socket 0 ─────────┐ │ │ ┌─ CPU Socket 1 ─────┐ │ │
    // │ │ │ Core 0  Core 1         │ │ │ │ Core 2  Core 3     │ │ │
    // │ │ │ L1/L2   L1/L2          │ │ │ │ L1/L2   L1/L2      │ │ │
    // │ │ │ └─── Shared L3 ────────┘ │ │ │ └─── Shared L3 ────┘ │ │ │
    // │ │ │ Memory Controller       │ │ │ │ Memory Controller   │ │ │
    // │ │ └─────────────────────────┘ │ │ └─────────────────────┘ │ │
    // │ │ ┌─ Local Memory ──────────┐ │ │ ┌─ Local Memory ──────┐ │ │
    // │ │ │ 16GB DDR4               │ │ │ │ 16GB DDR4           │ │ │
    // │ │ │ Low latency access      │ │ │ │ Low latency access  │ │ │
    // │ │ │ High bandwidth          │ │ │ │ High bandwidth      │ │ │
    // │ │ └─────────────────────────┘ │ │ └─────────────────────┘ │ │
    // │ └─────────────────────────────┘ └─────────────────────────┘ │
    // │                   │                         │               │
    // │                   └── QPI/UPI Interconnect ─┘               │
    // │                                                             │
    // │ Memory Access Latencies:                                   │
    // │ • Local Memory:  ~100ns (1x latency)                      │
    // │ • Remote Memory: ~150ns (1.5x latency)                    │
    // │ • Cache-to-cache: ~50ns (coherency traffic)               │
    // └─────────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- NUMA Architecture Characteristics ---" << std::endl;
    std::cout << "NUMA Nodes:" << std::endl;
    std::cout << "• Each node contains CPU cores + local memory" << std::endl;
    std::cout << "• Local memory access is faster than remote" << std::endl;
    std::cout << "• Nodes connected via high-speed interconnect" << std::endl;
    
    std::cout << "\nMemory Access Patterns:" << std::endl;
    std::cout << "• Local Access: CPU accesses memory on same node" << std::endl;
    std::cout << "• Remote Access: CPU accesses memory on different node" << std::endl;
    std::cout << "• Cross-node traffic uses QPI/UPI/Infinity Fabric" << std::endl;
    
    std::cout << "\nPerformance Implications:" << std::endl;
    std::cout << "• Local memory: ~100ns latency, full bandwidth" << std::endl;
    std::cout << "• Remote memory: ~150ns latency, reduced bandwidth" << std::endl;
    std::cout << "• Cache coherency overhead for shared data" << std::endl;
    
    // Simulate NUMA memory access
    std::cout << "\n--- NUMA Memory Access Simulation ---" << std::endl;
    
    // Get NUMA info (simplified simulation)
    std::cout << "NUMA Node Information:" << std::endl;
    std::cout << "Node 0: CPU cores 0-7, Memory 0x00000000-0x3FFFFFFFF" << std::endl;
    std::cout << "Node 1: CPU cores 8-15, Memory 0x400000000-0x7FFFFFFFF" << std::endl;
    
    // Demonstrate memory access latency differences
    const size_t array_size = 1024 * 1024;  // 1MB
    std::vector<int> local_data(array_size);
    
    // Initialize data
    for (size_t i = 0; i < array_size; ++i) {
        local_data[i] = static_cast<int>(i);
    }
    
    // Sequential access (likely local)
    auto start_time = std::chrono::high_resolution_clock::now();
    long long sum = 0;
    for (size_t i = 0; i < array_size; ++i) {
        sum += local_data[i];
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto access_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    
    std::cout << "\nMemory Access Performance:" << std::endl;
    std::cout << "Sequential access time: " << access_time.count() << " μs" << std::endl;
    std::cout << "Effective bandwidth: " << 
                 (array_size * sizeof(int) / 1024.0 / 1024.0) / (access_time.count() / 1000000.0)
                 << " MB/s" << std::endl;
    
    std::cout << "\nNUMA Optimization Strategies:" << std::endl;
    std::cout << "• Memory Affinity: Allocate memory on same node as CPU" << std::endl;
    std::cout << "• Thread Affinity: Keep threads on same NUMA node" << std::endl;
    std::cout << "• Data Locality: Minimize cross-node data sharing" << std::endl;
    std::cout << "• NUMA-aware Algorithms: Partition data by node" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_physical_memory_management() {
    std::cout << "=== PHYSICAL MEMORY MANAGEMENT ===" << std::endl;
    
    // Physical Memory Manager Structure:
    // ┌─ PHYSICAL MEMORY MANAGER ───────────────────────────────┐
    // │                                                         │
    // │ ┌─ Page Frame Database ──────────────────────────────┐  │
    // │ │ struct page {                                      │  │
    // │ │   unsigned long flags;      // Page status flags   │  │
    // │ │   atomic_t count;          // Reference count     │  │
    // │ │   struct list_head list;   // Free list linkage   │  │
    // │ │   unsigned long private;   // Private data        │  │
    // │ │ };                                                │  │
    // │ │                                                    │  │
    // │ │ Array covers ALL physical memory:                  │  │
    // │ │ page_frames[0]     → Physical page 0x00000000     │  │
    // │ │ page_frames[1]     → Physical page 0x00001000     │  │
    // │ │ page_frames[2]     → Physical page 0x00002000     │  │
    // │ │ ...                                               │  │
    // │ │ page_frames[N-1]   → Physical page 0xFFFFF000     │  │
    // │ └────────────────────────────────────────────────────┘  │
    // │                                                         │
    // │ ┌─ Free Page Lists ───────────────────────────────────┐  │
    // │ │ Order 0: [page][page][page]... (4KB pages)         │  │
    // │ │ Order 1: [page pair]...        (8KB blocks)        │  │
    // │ │ Order 2: [4-page block]...     (16KB blocks)       │  │
    // │ │ ...                                                │  │
    // │ │ Order 10: [1024-page block]... (4MB blocks)        │  │
    // │ └────────────────────────────────────────────────────┘  │
    // │                                                         │
    // │ ┌─ Allocation Algorithm (Buddy System) ───────────────┐  │
    // │ │ Request: Allocate 2 pages (8KB)                    │  │
    // │ │ 1. Check Order 1 free list                         │  │
    // │ │ 2. If empty, split Order 2 block into 2 Order 1   │  │
    // │ │ 3. If Order 2 empty, split Order 3, etc.          │  │
    // │ │ 4. Return allocated block                           │  │
    // │ └────────────────────────────────────────────────────┘  │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- Physical Page Frame Management ---" << std::endl;
    std::cout << "Page Frame Database:" << std::endl;
    std::cout << "• One entry per physical page (typically 4KB)" << std::endl;
    std::cout << "• Tracks page status: free, allocated, reserved" << std::endl;
    std::cout << "• Reference counting for shared pages" << std::endl;
    std::cout << "• Links for free list management" << std::endl;
    
    std::cout << "\nBuddy System Allocator:" << std::endl;
    std::cout << "• Maintains free lists of power-of-2 sized blocks" << std::endl;
    std::cout << "• Efficient allocation and coalescing" << std::endl;
    std::cout << "• Reduces external fragmentation" << std::endl;
    std::cout << "• Orders from 0 (4KB) to 10 (4MB) typically" << std::endl;
    
    // Simulate physical memory management
    std::cout << "\n--- Simulated Physical Memory Manager ---" << std::endl;
    
    class SimplePhysicalMemoryManager {
    private:
        std::vector<PhysicalPageFrame> page_frames;
        std::map<int, std::vector<size_t>> free_lists;  // order -> list of page indices
        size_t total_pages;
        
    public:
        SimplePhysicalMemoryManager(size_t total_memory_mb) {
            total_pages = (total_memory_mb * 1024 * 1024) / 4096;  // 4KB pages
            page_frames.resize(total_pages);
            
            // Initialize all pages as free
            for (size_t i = 0; i < total_pages; ++i) {
                page_frames[i].physical_address = i * 4096;
                page_frames[i].is_free = true;
                page_frames[i].reference_count = 0;
                page_frames[i].numa_node = i < (total_pages / 2) ? 0 : 1;  // Simple NUMA simulation
            }
            
            // Add all pages to maximum order free list (simplified)
            free_lists[0] = std::vector<size_t>();
            for (size_t i = 0; i < total_pages; ++i) {
                free_lists[0].push_back(i);
            }
        }
        
        uint64_t allocate_page() {
            if (free_lists[0].empty()) {
                return 0;  // Out of memory
            }
            
            size_t page_index = free_lists[0].back();
            free_lists[0].pop_back();
            
            page_frames[page_index].is_free = false;
            page_frames[page_index].reference_count = 1;
            
            return page_frames[page_index].physical_address;
        }
        
        void free_page(uint64_t physical_addr) {
            size_t page_index = physical_addr / 4096;
            if (page_index < total_pages) {
                page_frames[page_index].is_free = true;
                page_frames[page_index].reference_count = 0;
                free_lists[0].push_back(page_index);
            }
        }
        
        void print_stats() {
            size_t free_pages = free_lists[0].size();
            size_t used_pages = total_pages - free_pages;
            
            std::cout << "Memory Statistics:" << std::endl;
            std::cout << "  Total pages: " << total_pages << " (4KB each)" << std::endl;
            std::cout << "  Free pages: " << free_pages << std::endl;
            std::cout << "  Used pages: " << used_pages << std::endl;
            std::cout << "  Total memory: " << (total_pages * 4096) / (1024 * 1024) << " MB" << std::endl;
            std::cout << "  Free memory: " << (free_pages * 4096) / (1024 * 1024) << " MB" << std::endl;
        }
    };
    
    // Demonstrate memory manager
    SimplePhysicalMemoryManager mem_mgr(1024);  // 1GB system
    
    std::cout << "Initial state:" << std::endl;
    mem_mgr.print_stats();
    
    // Allocate some pages
    std::vector<uint64_t> allocated_pages;
    for (int i = 0; i < 100; ++i) {
        uint64_t page = mem_mgr.allocate_page();
        if (page != 0) {
            allocated_pages.push_back(page);
        }
    }
    
    std::cout << "\nAfter allocating 100 pages:" << std::endl;
    mem_mgr.print_stats();
    
    // Free half the pages
    for (size_t i = 0; i < allocated_pages.size() / 2; ++i) {
        mem_mgr.free_page(allocated_pages[i]);
    }
    
    std::cout << "\nAfter freeing 50 pages:" << std::endl;
    mem_mgr.print_stats();
    
    std::cout << "\nPhysical Memory Management Features:" << std::endl;
    std::cout << "• Page-based allocation (typically 4KB pages)" << std::endl;
    std::cout << "• Reference counting for shared pages" << std::endl;
    std::cout << "• NUMA-aware allocation policies" << std::endl;
    std::cout << "• Memory reclamation and compaction" << std::endl;
    std::cout << "• Hot-plug memory support (servers)" << std::endl;
    
    std::cout << std::endl;
}

int main() {
    std::cout << "PHYSICAL MEMORY ADDRESSING AND MANAGEMENT TUTORIAL" << std::endl;
    std::cout << "=================================================" << std::endl << std::endl;
    
    demonstrate_physical_memory_overview();
    demonstrate_physical_memory_discovery();
    demonstrate_virtual_to_physical_translation();
    demonstrate_dram_organization();
    demonstrate_memory_controller_operation();
    demonstrate_numa_architecture();
    demonstrate_physical_memory_management();
    
    std::cout << "Physical Memory tutorial completed successfully!" << std::endl;
    std::cout << "\nKey Takeaways:" << std::endl;
    std::cout << "━━━━━━━━━━━━━━" << std::endl;
    
    std::cout << "\nPHYSICAL vs VIRTUAL MEMORY:" << std::endl;
    std::cout << "• Applications use virtual addresses (managed by OS)" << std::endl;
    std::cout << "• Physical addresses determined by hardware design" << std::endl;
    std::cout << "• MMU translates virtual to physical addresses" << std::endl;
    std::cout << "• Physical memory layout managed by OS and firmware" << std::endl;
    
    std::cout << "\nMEMORY DISCOVERY PROCESS:" << std::endl;
    std::cout << "• BIOS/UEFI detects memory during POST" << std::endl;
    std::cout << "• Creates memory map with usable/reserved regions" << std::endl;
    std::cout << "• OS initializes physical memory manager" << std::endl;
    std::cout << "• Dynamic allocation through page frame database" << std::endl;
    
    std::cout << "\nDRAM ORGANIZATION:" << std::endl;
    std::cout << "• Hierarchical: Channel → DIMM → Rank → Bank → Row/Column" << std::endl;
    std::cout << "• Address interleaving improves performance" << std::endl;
    std::cout << "• Row buffer locality critical for performance" << std::endl;
    std::cout << "• Memory controller optimizes access patterns" << std::endl;
    
    std::cout << "\nNUMA CONSIDERATIONS:" << std::endl;
    std::cout << "• Local memory access faster than remote" << std::endl;
    std::cout << "• Memory affinity policies important" << std::endl;
    std::cout << "• Cross-node data sharing has overhead" << std::endl;
    std::cout << "• NUMA-aware algorithms improve performance" << std::endl;
    
    std::cout << "\nPRACTICAL IMPLICATIONS:" << std::endl;
    std::cout << "• Physical addresses are NOT fixed slots" << std::endl;
    std::cout << "• Memory layout varies by system configuration" << std::endl;
    std::cout << "• Virtual memory abstracts physical complexity" << std::endl;
    std::cout << "• Understanding memory hierarchy improves performance" << std::endl;
    
    return 0;
}