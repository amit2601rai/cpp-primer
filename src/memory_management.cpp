/**
 * @file memory_management.cpp
 * @author GitHub Copilot
 * @brief Tutorial code for C++ memory model, memory areas, and management best practices.
 */

// This program provides a comprehensive overview of C++ memory management including:
// - Memory model and layout (VIRTUAL MEMORY LAYOUT OF A PROCESS)
// - Stack vs Heap memory
// - Dynamic memory allocation
// - Memory leaks and best practices
// - Smart pointers and RAII
// - Memory alignment and optimization
//
// NOTE: This represents the VIRTUAL MEMORY LAYOUT of a process, NOT the PCB!
//
// PCB (Process Control Block) vs Virtual Memory Layout:
// ┌─ PCB (Kernel Data Structure) ───────────────────┐
// │                                                │
// │ ┌─ Process Control Block ──────────────────┐   │
// │ │ • Process ID (PID)                      │   │
// │ │ • Process State (running/waiting/ready) │   │
// │ │ • CPU Registers (saved context)         │   │
// │ │ • Memory Management Info:               │   │
// │ │   - Page table pointer                  │   │
// │ │   - Segment table pointer               │   │
// │ │   - Memory limits/permissions           │   │
// │ │ • File Descriptor Table                 │   │
// │ │ • Scheduling Info (priority, time)      │   │
// │ │ • Parent/Child Process Pointers         │   │
// │ │ • Signal Handlers                       │   │
// │ │ • Working Directory                     │   │
// │ │ • User/Group IDs                        │   │
// │ └─────────────────────────────────────────┘   │
// └────────────────────────────────────────────────┘
//
// ┌─ Virtual Memory Layout (What we show below) ────┐
// │                                                │
// │ ┌─ Process Virtual Address Space ──────────┐   │
// │ │ │ Stack (high addresses)                │   │
// │ │ │ ↓ grows downward                      │   │
// │ │ │                                       │   │
// │ │ │ ↕ unused space                        │   │
// │ │ │                                       │   │
// │ │ │ ↑ grows upward                        │   │
// │ │ │ Heap (dynamic allocation)             │   │
// │ │ │ BSS (uninitialized globals)           │   │
// │ │ │ Data (initialized globals)            │   │
// │ │ │ Text/Code (program instructions)      │   │
// │ │ └─────────────────────────────────────┘   │
// └────────────────────────────────────────────────┘
//
// Key Differences:
// - PCB: OS kernel data structure for process management
// - Virtual Memory Layout: How memory appears to the running process
// - PCB contains METADATA about the process
// - Virtual Memory Layout contains the ACTUAL PROGRAM DATA AND CODE
//
// MEMORY MANAGEMENT MECHANISMS: Page Table vs Segment Table
// ═══════════════════════════════════════════════════════════
//
// ┌─ PAGING (Page Table) ────────────────────────────────────┐
// │                                                         │
// │ Concept: Divide memory into fixed-size blocks (pages)   │
// │                                                         │
// │ Virtual Address Space:                                  │
// │ ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐       │
// │ │Page0│Page1│Page2│Page3│Page4│Page5│Page6│Page7│       │
// │ │4KB  │4KB  │4KB  │4KB  │4KB  │4KB  │4KB  │4KB  │       │
// │ └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘       │
// │                                                         │
// │ Physical Memory (Frames):                               │
// │ ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐       │
// │ │Frame│Frame│Frame│Frame│Frame│Frame│Frame│Frame│       │
// │ │  0  │  1  │  2  │  3  │  4  │  5  │  6  │  7  │       │
// │ └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘       │
// │                                                         │
// │ Page Table Maps Pages → Frames:                         │
// │ ┌─ Page Table ──────────────────────────────────────┐   │
// │ │ Page 0 → Frame 3  │ Valid: 1 │ R/W: 1 │ Present │   │
// │ │ Page 1 → Frame 1  │ Valid: 1 │ R/W: 1 │ Present │   │
// │ │ Page 2 → Frame 7  │ Valid: 1 │ R/W: 0 │ Present │   │
// │ │ Page 3 → Frame 2  │ Valid: 0 │ R/W: 0 │ Swapped │   │
// │ │ Page 4 → Frame 5  │ Valid: 1 │ R/W: 1 │ Present │   │
// │ │ ...               │          │        │         │   │
// │ └───────────────────────────────────────────────────┘   │
// │                                                         │
// │ Characteristics:                                        │
// │ • Fixed page size (typically 4KB)                      │
// │ • No external fragmentation                             │
// │ • Internal fragmentation possible                       │
// │ • Transparent to programmer                             │
// │ • Hardware MMU support required                         │
// └─────────────────────────────────────────────────────────┘
//
// ┌─ SEGMENTATION (Segment Table) ───────────────────────────┐
// │                                                         │
// │ Concept: Divide memory into logical segments            │
// │                                                         │
// │ Virtual Address Space:                                  │
// │ ┌───────────────────────────────────────────────────┐   │
// │ │ Segment 0: Code Segment (Instructions)           │   │
// │ │ Base: 0x1000, Limit: 8KB, R/X permissions       │   │
// │ ├───────────────────────────────────────────────────┤   │
// │ │ Segment 1: Data Segment (Global Variables)       │   │
// │ │ Base: 0x3000, Limit: 4KB, R/W permissions       │   │
// │ ├───────────────────────────────────────────────────┤   │
// │ │ Segment 2: Stack Segment (Local Variables)       │   │
// │ │ Base: 0x7000, Limit: 16KB, R/W permissions      │   │
// │ ├───────────────────────────────────────────────────┤   │
// │ │ Segment 3: Heap Segment (Dynamic Allocation)     │   │
// │ │ Base: 0x5000, Limit: 32KB, R/W permissions      │   │
// │ └───────────────────────────────────────────────────┘   │
// │                                                         │
// │ Segment Table:                                          │
// │ ┌─ Segment Table ───────────────────────────────────┐   │
// │ │ Seg 0 │ Base: 0x1000 │ Limit: 8KB  │ R/X │ Valid │   │
// │ │ Seg 1 │ Base: 0x3000 │ Limit: 4KB  │ R/W │ Valid │   │
// │ │ Seg 2 │ Base: 0x7000 │ Limit: 16KB │ R/W │ Valid │   │
// │ │ Seg 3 │ Base: 0x5000 │ Limit: 32KB │ R/W │ Valid │   │
// │ └───────────────────────────────────────────────────┘   │
// │                                                         │
// │ Address Translation:                                    │
// │ Virtual Address = (Segment ID, Offset)                 │
// │ Physical Address = Segment_Base + Offset               │
// │                                                         │
// │ Characteristics:                                        │
// │ • Variable segment sizes                                │
// │ • Logical organization (matches program structure)     │
// │ • External fragmentation possible                       │
// │ • No internal fragmentation                             │
// │ • Protection at segment level                           │
// └─────────────────────────────────────────────────────────┘
//
// COMPARISON SUMMARY:
// ┌─────────────────┬─────────────────┬─────────────────────┐
// │    Aspect       │     Paging      │    Segmentation     │
// ├─────────────────┼─────────────────┼─────────────────────┤
// │ Unit Size       │ Fixed (4KB)     │ Variable            │
// │ Fragmentation   │ Internal only   │ External only       │
// │ Logical View    │ Linear/Flat     │ Multiple segments   │
// │ Protection      │ Page-level      │ Segment-level       │
// │ Sharing         │ Page-based      │ Segment-based       │
// │ Growth          │ Page-by-page    │ Segment expansion   │
// │ Programmer      │ Transparent     │ May be visible      │
// │ Hardware        │ MMU required    │ Segment registers   │
// └─────────────────┴─────────────────┴─────────────────────┘
//
// MODERN SYSTEMS often use BOTH:
// • Segmented Paging: Segments divided into pages
// • x86-64: Flat memory model with paging (segments mostly legacy)
// • Each approach solves different aspects of memory management

#include <iostream>
#include <string>
#include <memory>     // For smart pointers
#include <vector>     // For containers
#include <new>        // For placement new and bad_alloc
#include <cstdlib>    // For malloc/free
#include <chrono>     // For timing

// Forward declarations for demonstration
class Resource;

void demonstrate_memory_model() {
  std::cout << "=== C++ MEMORY MODEL AND LAYOUT ===" << std::endl;
  
  // Complete Memory Layout Diagram:
  // ┌─────────────────────────────────────────────────┐
  // │                 HIGH ADDRESSES                  │
  // ├─────────────────────────────────────────────────┤
  // │  STACK (grows downward ↓)                      │
  // │  ┌─────────────────────────────────────────┐    │
  // │  │ Function parameters                     │    │
  // │  │ Return addresses                        │    │
  // │  │ Local variables                         │    │
  // │  │ Auto objects (RAII)                     │    │
  // │  └─────────────────────────────────────────┘    │
  // ├─────────────────────────────────────────────────┤
  // │                    ↕                            │
  // │              UNUSED SPACE                       │
  // │                    ↕                            │
  // ├─────────────────────────────────────────────────┤
  // │  HEAP (grows upward ↑)                         │
  // │  ┌─────────────────────────────────────────┐    │
  // │  │ new/malloc allocated objects            │    │
  // │  │ Dynamic arrays                          │    │
  // │  │ Smart pointer managed objects           │    │
  // │  │ STL container internal storage          │    │
  // │  └─────────────────────────────────────────┘    │
  // ├─────────────────────────────────────────────────┤
  // │  BSS SEGMENT (uninitialized data)              │
  // │  ┌─────────────────────────────────────────┐    │
  // │  │ static int uninitialized_global;       │    │
  // │  │ Global arrays without initializers     │    │
  // │  └─────────────────────────────────────────┘    │
  // ├─────────────────────────────────────────────────┤
  // │  DATA SEGMENT (initialized data)               │
  // │  ┌─────────────────────────────────────────┐    │
  // │  │ static int initialized_global = 42;    │    │
  // │  │ const char* string_literal = "hello"; │    │
  // │  └─────────────────────────────────────────┘    │
  // ├─────────────────────────────────────────────────┤
  // │  CODE/TEXT SEGMENT (read-only)                 │
  // │  ┌─────────────────────────────────────────┐    │
  // │  │ Machine code instructions               │    │
  // │  │ Function definitions                    │    │
  // │  │ String literals                         │    │
  // │  └─────────────────────────────────────────┘    │
  // ├─────────────────────────────────────────────────┤
  // │                 LOW ADDRESSES                   │
  // └─────────────────────────────────────────────────┘
  
  std::cout << "\n--- Memory Segments ---" << std::endl;
  std::cout << "1. Code/Text Segment: Contains executable instructions" << std::endl;
  std::cout << "2. Data Segment: Global and static variables (initialized)" << std::endl;
  std::cout << "3. BSS Segment: Uninitialized global and static variables" << std::endl;
  std::cout << "4. Heap: Dynamic memory allocation (grows upward)" << std::endl;
  std::cout << "5. Stack: Local variables and function calls (grows downward)" << std::endl;
  
  // Global variables (in data segment)
  static int global_initialized = 42;
  static int global_uninitialized;
  
  // Local variables (on stack)
  int local_var = 100;
  char local_array[1024];
  
  // Dynamic allocation (on heap)
  int* heap_var = new int(200);
  
  std::cout << "\n--- Memory Addresses ---" << std::endl;
  std::cout << "Global initialized variable: " << &global_initialized << std::endl;
  std::cout << "Global uninitialized variable: " << &global_uninitialized << std::endl;
  std::cout << "Local variable: " << &local_var << std::endl;
  std::cout << "Local array: " << static_cast<void*>(local_array) << std::endl;
  std::cout << "Heap variable: " << heap_var << std::endl;
  std::cout << "Function address: " << reinterpret_cast<void*>(&demonstrate_memory_model) << std::endl;
  
  // Address Space Visualization:
  // ┌─ Stack (High Addresses) ─────────────────────┐
  // │ Local variable:    0x7fff5fbff6bc            │
  // │ Local array:       0x7fff5fbff2bc            │
  // ├──────────────────────────────────────────────┤
  // │                   Gap                        │
  // ├─ Heap (Dynamic) ─────────────────────────────┤
  // │ Heap variable:     0x7f8b1bc05a70            │
  // ├─ Data Segment ───────────────────────────────┤
  // │ Global init:       0x100008030               │
  // │ Global uninit:     0x100008034               │
  // ├─ Code Segment ───────────────────────────────┤
  // │ Function addr:     0x100001e80               │
  // └─ (Low Addresses) ───────────────────────────┘
  
  // Clean up
  delete heap_var;
  
  std::cout << std::endl;
}

void demonstrate_stack_memory() {
  std::cout << "=== STACK MEMORY ===" << std::endl;
  
  // Stack Memory Layout:
  // ┌─────────────────────────────────────────────────┐
  // │                HIGH ADDRESSES                   │
  // ├─────────────────────────────────────────────────┤
  // │  MAIN FUNCTION FRAME                           │
  // │  ┌─────────────────────────────────────────┐    │
  // │  │ main() local variables                  │    │
  // │  │ argc, argv                              │    │
  // │  └─────────────────────────────────────────┘    │
  // ├─────────────────────────────────────────────────┤
  // │  CURRENT FUNCTION FRAME                        │
  // │  ┌─────────────────────────────────────────┐    │
  // │  │ Return address to main()                │    │
  // │  │ Function parameters                     │    │
  // │  │ Local variables (this function)         │    │
  // │  │ ┌─ stack_int (4 bytes)                 │    │
  // │  │ ├─ stack_double (8 bytes)              │    │
  // │  │ ├─ stack_array (256 bytes)             │    │
  // │  │ ├─ stack_string (object)               │    │
  // │  │ └─ stack_vector (object)               │    │
  // │  └─────────────────────────────────────────┘    │
  // ├─────────────────────────────────────────────────┤
  // │  LAMBDA/NESTED FUNCTION FRAME                  │
  // │  ┌─────────────────────────────────────────┐    │
  // │  │ Return address                          │    │
  // │  │ Captured variables (references)        │    │
  // │  │ nested_var                              │    │
  // │  └─────────────────────────────────────────┘    │
  // ├─────────────────────────────────────────────────┤
  // │  STACK POINTER (SP) →                          │
  // │                LOW ADDRESSES                    │
  // └─────────────────────────────────────────────────┘
  //          ↑ Stack grows downward ↑
  
  std::cout << "\n--- Stack Characteristics ---" << std::endl;
  std::cout << "- Fast allocation/deallocation (just move stack pointer)" << std::endl;
  std::cout << "- Automatic cleanup (RAII - Resource Acquisition Is Initialization)" << std::endl;
  std::cout << "- Limited size (typically 1-8 MB)" << std::endl;
  std::cout << "- LIFO (Last In, First Out) order" << std::endl;
  std::cout << "- Thread-local (each thread has its own stack)" << std::endl;
  
  // Stack allocation examples
  std::cout << "\n--- Stack Allocation Examples ---" << std::endl;
  
  // Primitive types on stack
  int stack_int = 42;
  double stack_double = 3.14159;
  char stack_array[256] = "Hello, Stack!";
  
  // Objects on stack
  std::string stack_string = "Stack-allocated string";
  std::vector<int> stack_vector = {1, 2, 3, 4, 5};
  
  std::cout << "stack_int: " << stack_int << " at " << &stack_int << std::endl;
  std::cout << "stack_double: " << stack_double << " at " << &stack_double << std::endl;
  std::cout << "stack_array: " << stack_array << " at " << static_cast<void*>(stack_array) << std::endl;
  std::cout << "stack_string: " << stack_string << " at " << &stack_string << std::endl;
  std::cout << "stack_vector size: " << stack_vector.size() << " at " << &stack_vector << std::endl;
  
  // Demonstrate stack frame
  std::cout << "\n--- Stack Frame Demonstration ---" << std::endl;
  
  // Before lambda call:
  // ┌─ Current Frame ─────────────────────────┐
  // │ stack_int:    0x7fff5fbff6bc           │
  // │ stack_double: 0x7fff5fbff6b0           │
  // │ stack_vector: 0x7fff5fbff680           │
  // └─────────────────────────────────────────┘
  
  auto lambda = [&]() {
    int nested_var = 999;
    
    // During lambda execution:
    // ┌─ Lambda Frame ──────────────────────────┐
    // │ nested_var:   0x7fff5fbff67c           │
    // │ [&] captures: references to outer vars │
    // └─────────────────────────────────────────┘
    // ┌─ Outer Frame ───────────────────────────┐
    // │ stack_int:    0x7fff5fbff6bc           │
    // │ (still accessible via reference)       │
    // └─────────────────────────────────────────┘
    
    std::cout << "Nested variable: " << nested_var << " at " << &nested_var << std::endl;
    std::cout << "Distance from outer variable: " << 
                 abs(reinterpret_cast<intptr_t>(&nested_var) - reinterpret_cast<intptr_t>(&stack_int)) 
                 << " bytes" << std::endl;
  };
  lambda();
  
  // Stack overflow warning
  std::cout << "\n--- Stack Limitations ---" << std::endl;
  std::cout << "WARNING: Large arrays on stack can cause stack overflow!" << std::endl;
  std::cout << "Example: int huge_array[1000000]; // May cause stack overflow" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_heap_memory() {
  std::cout << "=== HEAP MEMORY ===" << std::endl;
  
  // Heap Memory Structure:
  // ┌─────────────────────────────────────────────────┐
  // │                 HEAP MEMORY                     │
  // ├─────────────────────────────────────────────────┤
  // │  FREE BLOCKS (available for allocation)        │
  // │  ┌─────────┐ ┌───────────┐ ┌─────────────┐     │
  // │  │ Free    │ │ Free      │ │ Free        │     │
  // │  │ 64 bytes│ │ 128 bytes │ │ 256 bytes   │     │
  // │  └─────────┘ └───────────┘ └─────────────┘     │
  // ├─────────────────────────────────────────────────┤
  // │  ALLOCATED BLOCKS (in use)                     │
  // │  ┌─────────┐ ┌─────────┐ ┌─────────────────┐   │
  // │  │ Object1 │ │ Array   │ │ Large Object    │   │
  // │  │ 4 bytes │ │ 40 bytes│ │ 1000 bytes      │   │
  // │  └─────────┘ └─────────┘ └─────────────────┘   │
  // ├─────────────────────────────────────────────────┤
  // │  FRAGMENTED SPACE (unusable small blocks)      │
  // │  ┌─┐ ┌───┐ ┌─┐ ┌──┐                            │
  // │  │ │ │   │ │ │ │  │ (too small for requests)   │
  // │  └─┘ └───┘ └─┘ └──┘                            │
  // ├─────────────────────────────────────────────────┤
  // │                  METADATA                       │
  // │  ┌─────────────────────────────────────────┐    │
  // │  │ Block headers with size info            │    │
  // │  │ Free list pointers                      │    │
  // │  │ Allocation bookkeeping                  │    │
  // │  └─────────────────────────────────────────┘    │
  // └─────────────────────────────────────────────────┘
  //               ↑ Heap grows upward ↑
  
  std::cout << "\n--- Heap Characteristics ---" << std::endl;
  std::cout << "- Slower allocation/deallocation (requires memory management)" << std::endl;
  std::cout << "- Manual cleanup required (or use smart pointers)" << std::endl;
  std::cout << "- Large size (limited by available RAM)" << std::endl;
  std::cout << "- Flexible allocation order" << std::endl;
  std::cout << "- Shared across threads (with synchronization)" << std::endl;
  
  // Basic heap allocation
  std::cout << "\n--- Basic Heap Allocation ---" << std::endl;
  
  // Single object allocation
  int* heap_int = new int(42);
  double* heap_double = new double(3.14159);
  std::string* heap_string = new std::string("Heap-allocated string");
  
  std::cout << "heap_int: " << *heap_int << " at " << heap_int << std::endl;
  std::cout << "heap_double: " << *heap_double << " at " << heap_double << std::endl;
  std::cout << "heap_string: " << *heap_string << " at " << heap_string << std::endl;
  
  // Array allocation
  int* heap_array = new int[1000];
  for (int i = 0; i < 10; ++i) {
    heap_array[i] = i * i;
  }
  
  std::cout << "heap_array[0-9]: ";
  for (int i = 0; i < 10; ++i) {
    std::cout << heap_array[i] << " ";
  }
  std::cout << std::endl;
  std::cout << "heap_array starts at: " << heap_array << std::endl;
  
  // Memory Layout of Allocated Objects:
  // ┌─ Heap Allocation Pattern ──────────────────────┐
  // │                                                │
  // │ heap_int:    [0x7f8b1bc05a70] → 4 bytes       │
  // │               ┌─────┐                          │
  // │               │  42 │                          │
  // │               └─────┘                          │
  // │                                                │
  // │ heap_double: [0x7f8b1bc05a80] → 8 bytes       │
  // │               ┌─────────────┐                  │
  // │               │   3.14159   │                  │
  // │               └─────────────┘                  │
  // │                                                │
  // │ heap_string: [0x7f8b1bc05a90] → object        │
  // │               ┌─────────────────────────────┐  │
  // │               │ std::string object          │  │
  // │               │ ├─ size: 21                 │  │
  // │               │ ├─ capacity: >21            │  │
  // │               │ └─ data*: → "Heap-alloc..." │  │
  // │               └─────────────────────────────┘  │
  // │                                                │
  // │ heap_array:  [0x7f8b1bc05ab0] → 4000 bytes    │
  // │               ┌─────┬─────┬─────┬─────┬───┐    │
  // │               │  0  │  1  │  4  │  9  │...│    │
  // │               └─────┴─────┴─────┴─────┴───┘    │
  // │               [0]   [1]   [2]   [3]  ...[999] │
  // └────────────────────────────────────────────────┘
  
  // Large allocation demonstration
  std::cout << "\n--- Large Allocation ---" << std::endl;
  size_t large_size = 10000000; // 10 million integers
  int* large_array = nullptr;
  
  try {
    large_array = new int[large_size];
    std::cout << "Successfully allocated " << large_size * sizeof(int) << " bytes" << std::endl;
    std::cout << "Large array starts at: " << large_array << std::endl;
    
    // Initialize some values
    large_array[0] = 1;
    large_array[large_size - 1] = 999999;
    
  } catch (const std::bad_alloc& e) {
    std::cout << "Allocation failed: " << e.what() << std::endl;
  }
  
  // Cleanup (very important!)
  delete heap_int;
  delete heap_double;
  delete heap_string;
  delete[] heap_array;
  delete[] large_array;
  
  std::cout << std::endl;
}

void demonstrate_memory_leaks() {
  std::cout << "=== MEMORY LEAKS AND COMMON PITFALLS ===" << std::endl;
  
  // Memory Leak Visualization:
  // ┌─ Normal Memory Usage ───────────────────────────┐
  // │                                                │
  // │ STACK:     │ int* ptr │ ──────┐                │
  // │            └──────────┘       │                │
  // │                               │                │
  // │ HEAP:      ┌──────────┐      │                │
  // │            │   42     │ ←────┘                │
  // │            └──────────┘                       │
  // │            ↑ Reachable via ptr                │
  // └────────────────────────────────────────────────┘
  //              delete ptr; // ✓ Memory freed
  
  // ┌─ Memory Leak Scenario ──────────────────────────┐
  // │                                                │
  // │ STACK:     │   ????   │                       │
  // │            └──────────┘                       │
  // │            ↑ ptr went out of scope            │
  // │                                               │
  // │ HEAP:      ┌──────────┐                      │
  // │            │   42     │ ← LEAKED!             │
  // │            └──────────┘   No way to access    │
  // │            ↑ Unreachable memory               │
  // └───────────────────────────────────────────────┘
  //              // Forgot delete ptr; ✗ LEAK!
  
  std::cout << "\n--- What is a Memory Leak? ---" << std::endl;
  std::cout << "A memory leak occurs when dynamically allocated memory" << std::endl;
  std::cout << "is not properly deallocated, making it inaccessible but" << std::endl;
  std::cout << "still consuming system resources." << std::endl;
  
  std::cout << "\n--- Common Leak Scenarios ---" << std::endl;
  
  // Scenario 1: Forgot to delete
  std::cout << "1. Forgetting to delete:" << std::endl;
  std::cout << "   int* ptr = new int(42);" << std::endl;
  std::cout << "   // Forgot: delete ptr;" << std::endl;
  
  // Scenario 2: Exception before delete
  std::cout << "\n2. Exception thrown before delete:" << std::endl;
  std::cout << "   int* ptr = new int(42);" << std::endl;
  std::cout << "   risky_function(); // might throw exception" << std::endl;
  std::cout << "   delete ptr; // Never reached if exception thrown" << std::endl;
  
  // Scenario 3: Double delete
  std::cout << "\n3. Double delete (undefined behavior):" << std::endl;
  std::cout << "   int* ptr = new int(42);" << std::endl;
  std::cout << "   delete ptr;" << std::endl;
  std::cout << "   delete ptr; // ERROR: Double delete!" << std::endl;
  
  // Scenario 4: Array/single object mismatch
  std::cout << "\n4. Array/single object mismatch:" << std::endl;
  std::cout << "   int* arr = new int[10];" << std::endl;
  std::cout << "   delete arr; // ERROR: Should be delete[] arr;" << std::endl;
  
  // Scenario 5: Memory leak in loops
  std::cout << "\n5. Memory leak in loops:" << std::endl;
  std::cout << "   for (int i = 0; i < 1000; ++i) {" << std::endl;
  std::cout << "     int* ptr = new int(i);" << std::endl;
  std::cout << "     // Forgot to delete ptr in each iteration" << std::endl;
  std::cout << "   }" << std::endl;
  
  // Demonstration of proper cleanup with RAII
  std::cout << "\n--- RAII (Resource Acquisition Is Initialization) ---" << std::endl;
  std::cout << "RAII automatically manages resources through object lifetime:" << std::endl;
  
  {
    std::vector<int> auto_managed_vector(1000, 42);
    std::string auto_managed_string = "RAII automatically cleans this up";
    std::cout << "Vector size: " << auto_managed_vector.size() << std::endl;
    std::cout << "String: " << auto_managed_string << std::endl;
    // Automatic cleanup when going out of scope
  }
  std::cout << "Vector and string automatically destroyed!" << std::endl;
  
  std::cout << std::endl;
}

// Resource class for smart pointer demonstration
class Resource {
private:
  std::string name_;
  int* data_;
  
public:
  Resource(const std::string& name) : name_(name), data_(new int[100]) {
    std::cout << "Resource '" << name_ << "' constructed" << std::endl;
    for (int i = 0; i < 100; ++i) {
      data_[i] = i;
    }
  }
  
  ~Resource() {
    std::cout << "Resource '" << name_ << "' destroyed" << std::endl;
    delete[] data_;
  }
  
  void use() {
    std::cout << "Using resource '" << name_ << "'" << std::endl;
  }
  
  // Prevent copying for simplicity
  Resource(const Resource&) = delete;
  Resource& operator=(const Resource&) = delete;
};

void demonstrate_smart_pointers() {
  std::cout << "=== SMART POINTERS (C++11 and later) ===" << std::endl;
  
  // Smart Pointer Memory Management:
  // ┌─ Raw Pointer (Manual Management) ───────────────┐
  // │                                                │
  // │ Resource* ptr = new Resource();                │
  // │                                                │
  // │ STACK:    │ ptr* │ ────────┐                   │
  // │           └──────┘         │                   │
  // │                           │                   │
  // │ HEAP:     ┌─────────────┐ │                   │
  // │           │ Resource    │←┘                   │
  // │           │ object      │                     │
  // │           └─────────────┘                     │
  // │                                               │
  // │ delete ptr; // ← MUST REMEMBER!              │
  // └───────────────────────────────────────────────┘
  
  // ┌─ Smart Pointer (Automatic Management) ──────────┐
  // │                                                │
  // │ std::unique_ptr<Resource> ptr =                │
  // │     std::make_unique<Resource>();              │
  // │                                                │
  // │ STACK:    ┌─────────────────┐                 │
  // │           │ unique_ptr      │ ──────┐         │
  // │           │ ├─ ptr*         │       │         │
  // │           │ └─ deleter      │       │         │
  // │           └─────────────────┘       │         │
  // │                                     │         │
  // │ HEAP:     ┌─────────────┐           │         │
  // │           │ Resource    │←──────────┘         │
  // │           │ object      │                     │
  // │           └─────────────┘                     │
  // │                                               │
  // │ } // ← AUTOMATIC CLEANUP! No delete needed   │
  // └───────────────────────────────────────────────┘
  
  std::cout << "\nSmart pointers automatically manage memory and provide" << std::endl;
  std::cout << "exception safety through RAII principles." << std::endl;
  
  // unique_ptr demonstration
  std::cout << "\n--- std::unique_ptr ---" << std::endl;
  std::cout << "Exclusive ownership, cannot be copied, can be moved" << std::endl;
  
  {
    std::unique_ptr<Resource> unique_res = std::make_unique<Resource>("UniqueResource");
    unique_res->use();
    
    // Transfer ownership
    std::unique_ptr<Resource> another_unique = std::move(unique_res);
    if (!unique_res) {
      std::cout << "Original unique_ptr is now null after move" << std::endl;
    }
    another_unique->use();
    
    // Automatic cleanup when going out of scope
  }
  std::cout << "unique_ptr automatically cleaned up!" << std::endl;
  
  // shared_ptr demonstration
  std::cout << "\n--- std::shared_ptr ---" << std::endl;
  std::cout << "Shared ownership with reference counting" << std::endl;
  
  // shared_ptr Reference Counting:
  // ┌─ shared_ptr Reference Counting ─────────────────┐
  // │                                                │
  // │ Initial state (count = 1):                    │
  // │                                               │
  // │ STACK:  ┌─────────────┐                      │
  // │         │ shared_ptr1 │ ──┐                  │
  // │         └─────────────┘   │                  │
  // │                           │                  │
  // │ HEAP:   ┌─────────────┐   │ ┌─────────────┐  │
  // │         │ Resource    │←──┘ │ Control     │  │
  // │         │ object      │     │ Block       │  │
  // │         └─────────────┘     │ count: 1    │  │
  // │                             └─────────────┘  │
  // │                                              │
  // │ After copy (count = 2):                     │
  // │                                             │
  // │ STACK:  ┌─────────────┐ ┌─────────────┐     │
  // │         │ shared_ptr1 │ │ shared_ptr2 │     │
  // │         └─────┬───────┘ └─────┬───────┘     │
  // │               └───────┬───────┘             │
  // │                       │                     │
  // │ HEAP:   ┌─────────────┐ │ ┌─────────────┐   │
  // │         │ Resource    │←┘ │ Control     │   │
  // │         │ object      │   │ Block       │   │
  // │         └─────────────┘   │ count: 2    │   │
  // │                           └─────────────┘   │
  // └─────────────────────────────────────────────┘
  
  {
    std::shared_ptr<Resource> shared_res = std::make_shared<Resource>("SharedResource");
    std::cout << "Reference count: " << shared_res.use_count() << std::endl;
    
    {
      std::shared_ptr<Resource> another_shared = shared_res;
      std::cout << "Reference count after copy: " << shared_res.use_count() << std::endl;
      another_shared->use();
    }
    
    std::cout << "Reference count after inner scope: " << shared_res.use_count() << std::endl;
    shared_res->use();
  }
  std::cout << "shared_ptr automatically cleaned up when count reached 0!" << std::endl;
  
  // weak_ptr demonstration
  std::cout << "\n--- std::weak_ptr ---" << std::endl;
  std::cout << "Non-owning observer, breaks circular dependencies" << std::endl;
  
  std::weak_ptr<Resource> weak_res;
  {
    std::shared_ptr<Resource> shared_res = std::make_shared<Resource>("WeakObserved");
    weak_res = shared_res;
    
    if (auto locked = weak_res.lock()) {
      std::cout << "weak_ptr successfully locked shared_ptr" << std::endl;
      locked->use();
    }
  }
  
  if (weak_res.expired()) {
    std::cout << "weak_ptr detected that resource was destroyed" << std::endl;
  }
  
  std::cout << std::endl;
}

void demonstrate_memory_alignment() {
  std::cout << "=== MEMORY ALIGNMENT AND OPTIMIZATION ===" << std::endl;
  
  // Memory Alignment Visualization:
  // ┌─ Memory Address Layout (byte-by-byte) ──────────┐
  // │                                                │
  // │ Address:  0x00  0x01  0x02  0x03  0x04  0x05  │
  // │           ┌────┬────┬────┬────┬────┬────┬────┐ │
  // │ Aligned:  │char│    │    │    │int │int │int │ │
  // │           │ A  │ ❌ │ ❌ │ ❌ │ B1 │ B2 │ B3 │ │
  // │           └────┴────┴────┴────┴────┴────┴────┘ │
  // │           ↑    ↑         padding        ↑      │
  // │           1B   3B wasted space         4B      │
  // │                                               │
  // │ Address:  0x00  0x01  0x02  0x03  0x04  0x05  │
  // │           ┌────┬────┬────┬────┬────┬────┬────┐ │
  // │ Packed:   │int │int │int │int │char│    │    │ │
  // │           │ B1 │ B2 │ B3 │ B4 │ A  │ ❌ │ ❌ │ │
  // │           └────┴────┴────┴────┴────┴────┴────┘ │
  // │           ↑         4B         ↑   ↑         │
  // │           Better layout!       1B  2B pad    │
  // └───────────────────────────────────────────────┘
  
  std::cout << "\n--- Memory Alignment Basics ---" << std::endl;
  std::cout << "Modern CPUs access memory more efficiently when data" << std::endl;
  std::cout << "is aligned to specific byte boundaries." << std::endl;
  
  // Alignment requirements
  std::cout << "\n--- Alignment Requirements ---" << std::endl;
  std::cout << "char alignment: " << alignof(char) << " bytes" << std::endl;
  std::cout << "short alignment: " << alignof(short) << " bytes" << std::endl;
  std::cout << "int alignment: " << alignof(int) << " bytes" << std::endl;
  std::cout << "long alignment: " << alignof(long) << " bytes" << std::endl;
  std::cout << "double alignment: " << alignof(double) << " bytes" << std::endl;
  std::cout << "pointer alignment: " << alignof(void*) << " bytes" << std::endl;
  
  // Struct padding demonstration
  std::cout << "\n--- Struct Padding ---" << std::endl;
  struct Unpadded {
    char c;
    int i;
    char c2;
  };
  
  struct Optimized {
    int i;
    char c;
    char c2;
  };
  
  std::cout << "Unpadded struct size: " << sizeof(Unpadded) << " bytes" << std::endl;
  std::cout << "Optimized struct size: " << sizeof(Optimized) << " bytes" << std::endl;
  
  // Struct Padding Detailed Visualization:
  // Unpadded struct layout:
  // ┌─ struct Unpadded ───────────────────────────────┐
  // │                                                │
  // │ Offset: 0    1    2    3    4    5    6    7   │
  // │         ┌────┬────┬────┬────┬────┬────┬────┬───┐│
  // │         │char│ ❌ │ ❌ │ ❌ │int │int │int │int││
  // │         │ c  │pad│pad│pad│ i1 │ i2 │ i3 │ i4 ││
  // │         └────┴────┴────┴────┴────┴────┴────┴───┘│
  // │         └─1B─┘└─3B padding─┘└────4B int────┘   │
  // │                                                │
  // │ Offset: 8    9    10   11   12                │
  // │         ┌────┬────┬────┬────┐                 │
  // │         │char│ ❌ │ ❌ │ ❌ │                 │
  // │         │ c2 │pad│pad│pad│                 │
  // │         └────┴────┴────┴────┘                 │
  // │         └─1B─┘└─3B padding─┘                  │
  // │                                               │
  // │ Total size: 12 bytes (8 bytes wasted!)       │
  // └───────────────────────────────────────────────┘
  
  // Optimized struct layout:
  // ┌─ struct Optimized ──────────────────────────────┐
  // │                                                │
  // │ Offset: 0    1    2    3    4    5    6    7   │
  // │         ┌────┬────┬────┬────┬────┬────┬────┬───┐│
  // │         │int │int │int │int │char│char│ ❌ │ ❌ ││
  // │         │ i1 │ i2 │ i3 │ i4 │ c1 │ c2 │pad│pad││
  // │         └────┴────┴────┴────┴────┴────┴────┴───┘│
  // │         └────4B int────┘└─1B─┘└1B┘└─2B pad─┘   │
  // │                                                │
  // │ Total size: 8 bytes (4 bytes saved!)          │
  // └────────────────────────────────────────────────┘
  
  // Cache line considerations
  std::cout << "\n--- Cache Line Considerations ---" << std::endl;
  std::cout << "Modern CPUs typically have 64-byte cache lines." << std::endl;
  std::cout << "Accessing data within the same cache line is faster." << std::endl;
  
  // Aligned allocation (C++17)
  std::cout << "\n--- Aligned Allocation ---" << std::endl;
  constexpr size_t alignment = 64; // Cache line size
  constexpr size_t size = 1024;
  
  // Note: This requires C++17 aligned new
  // void* aligned_ptr = operator new(size, std::align_val_t{alignment});
  // For compatibility, we'll demonstrate the concept
  std::cout << "Aligned allocation ensures data starts at specific boundaries" << std::endl;
  std::cout << "Example: 64-byte aligned allocation for cache optimization" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_memory_pools() {
  std::cout << "=== MEMORY POOLS AND CUSTOM ALLOCATORS ===" << std::endl;
  
  // Memory Pool Architecture:
  // ┌─ Traditional Heap Allocation ──────────────────┐
  // │                                                │
  // │ Each new/delete goes to system allocator:     │
  // │                                               │
  // │ ┌──────────────── HEAP ─────────────────────┐ │
  // │ │ ┌───┐ ┌─────┐ ┌──┐   ┌────┐ ┌──┐ ┌──────┐│ │
  // │ │ │ A │ │ FREE│ │B │   │FREE│ │C │ │ FREE ││ │
  // │ │ └───┘ └─────┘ └──┘   └────┘ └──┘ └──────┘│ │
  // │ └──────────────────────────────────────────┘ │
  // │          ↑ Fragmented, slow allocation       │
  // └───────────────────────────────────────────────┘
  
  // ┌─ Memory Pool Allocation ────────────────────────┐
  // │                                                │
  // │ Pre-allocate large block, divide into chunks: │
  // │                                               │
  // │ ┌──────────── MEMORY POOL ──────────────────┐ │
  // │ │ ┌────┐ ┌────┐ ┌────┐ ┌────┐ ┌────┐ ┌────┐│ │
  // │ │ │ 0  │ │ 1  │ │ 2  │ │ 3  │ │ 4  │ │ 5  ││ │
  // │ │ │USED│ │FREE│ │USED│ │FREE│ │USED│ │FREE││ │
  // │ │ └────┘ └────┘ └────┘ └────┘ └────┘ └────┘│ │
  // │ └──────────────────────────────────────────┘ │
  // │                                              │
  // │ ┌─ Free List ──────────────────────────────┐ │
  // │ │ Head → Chunk1 → Chunk3 → Chunk5 → NULL  │ │
  // │ └──────────────────────────────────────────┘ │
  // │          ↑ Fast allocation, no fragmentation │
  // └───────────────────────────────────────────────┘
  
  std::cout << "\n--- Memory Pool Concept ---" << std::endl;
  std::cout << "Memory pools pre-allocate large blocks of memory" << std::endl;
  std::cout << "and distribute smaller chunks as needed." << std::endl;
  std::cout << "Benefits:" << std::endl;
  std::cout << "- Reduced fragmentation" << std::endl;
  std::cout << "- Faster allocation/deallocation" << std::endl;
  std::cout << "- Better cache locality" << std::endl;
  std::cout << "- Predictable memory usage" << std::endl;
  
  // Simple memory pool simulation
  std::cout << "\n--- Simple Memory Pool Simulation ---" << std::endl;
  constexpr size_t pool_size = 4096; // 4KB pool
  constexpr size_t chunk_size = 64;  // 64-byte chunks
  constexpr size_t num_chunks = pool_size / chunk_size;
  
  std::cout << "Pool size: " << pool_size << " bytes" << std::endl;
  std::cout << "Chunk size: " << chunk_size << " bytes" << std::endl;
  std::cout << "Number of chunks: " << num_chunks << std::endl;
  
  // Allocate pool
  char* memory_pool = new char[pool_size];
  bool chunk_used[num_chunks] = {false};
  
  std::cout << "Memory pool allocated at: " << static_cast<void*>(memory_pool) << std::endl;
  
  // Simulate allocations
  std::vector<void*> allocated_chunks;
  for (size_t i = 0; i < 10 && i < num_chunks; ++i) {
    chunk_used[i] = true;
    void* chunk_ptr = memory_pool + (i * chunk_size);
    allocated_chunks.push_back(chunk_ptr);
    std::cout << "Allocated chunk " << i << " at: " << chunk_ptr << std::endl;
  }
  
  // Pool State After Allocations:
  // ┌─ Memory Pool Layout ────────────────────────────┐
  // │                                                │
  // │ Pool Base: 0x7f8b1bc05000                      │
  // │                                                │
  // │ ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬───┐│
  // │ │ 0   │ 1   │ 2   │ 3   │ 4   │ 5   │ 6   │...││
  // │ │USED │USED │USED │USED │USED │USED │USED │...││
  // │ └─────┴─────┴─────┴─────┴─────┴─────┴─────┴───┘│
  // │  64B   64B   64B   64B   64B   64B   64B       │
  // │                                               │
  // │ ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬───┐│
  // │ │ 7   │ 8   │ 9   │ 10  │ 11  │ 12  │ 13  │...││
  // │ │USED │USED │USED │FREE │FREE │FREE │FREE │...││
  // │ └─────┴─────┴─────┴─────┴─────┴─────┴─────┴───┘│
  // │  64B   64B   64B   64B   64B   64B   64B       │
  // │                                               │
  // │ Allocation Pattern: O(1) time complexity      │
  // │ ✓ 10 chunks allocated                         │
  // │ ✓ 54 chunks remain free                       │
  // └───────────────────────────────────────────────┘
  
  // Cleanup
  delete[] memory_pool;
  
  std::cout << std::endl;
}

void demonstrate_memory_best_practices() {
  std::cout << "=== MEMORY MANAGEMENT BEST PRACTICES ===" << std::endl;
  
  std::cout << "--- 1. Prefer Stack Allocation ---" << std::endl;
  std::cout << "✓ Use stack allocation when possible (faster, automatic cleanup)" << std::endl;
  std::cout << "✓ Prefer std::vector over raw arrays" << std::endl;
  std::cout << "✓ Use std::string instead of char*" << std::endl;
  
  std::cout << "\n--- 2. Use Smart Pointers ---" << std::endl;
  std::cout << "✓ std::unique_ptr for exclusive ownership" << std::endl;
  std::cout << "✓ std::shared_ptr for shared ownership" << std::endl;
  std::cout << "✓ std::weak_ptr to break circular references" << std::endl;
  std::cout << "✗ Avoid raw pointers for ownership" << std::endl;
  
  std::cout << "\n--- 3. Follow RAII Principles ---" << std::endl;
  std::cout << "✓ Acquire resources in constructors" << std::endl;
  std::cout << "✓ Release resources in destructors" << std::endl;
  std::cout << "✓ Use containers and smart pointers" << std::endl;
  
  std::cout << "\n--- 4. Exception Safety ---" << std::endl;
  std::cout << "✓ Use RAII to ensure cleanup on exceptions" << std::endl;
  std::cout << "✓ Prefer std::make_unique and std::make_shared" << std::endl;
  std::cout << "✓ Avoid naked new/delete" << std::endl;
  
  std::cout << "\n--- 5. Performance Considerations ---" << std::endl;
  std::cout << "✓ Minimize dynamic allocations in hot paths" << std::endl;
  std::cout << "✓ Consider memory pools for frequent allocations" << std::endl;
  std::cout << "✓ Be aware of cache locality" << std::endl;
  std::cout << "✓ Profile memory usage in production code" << std::endl;
  
  std::cout << "\n--- 6. Debugging and Tools ---" << std::endl;
  std::cout << "✓ Use memory debugging tools (Valgrind, AddressSanitizer)" << std::endl;
  std::cout << "✓ Enable compiler warnings (-Wall -Wextra)" << std::endl;
  std::cout << "✓ Use static analysis tools" << std::endl;
  std::cout << "✓ Test with different memory allocators" << std::endl;
  
  // Code examples of best practices
  std::cout << "\n--- Code Examples ---" << std::endl;
  
  // Good: Using containers
  std::cout << "Good: Using containers" << std::endl;
  std::vector<int> good_vector = {1, 2, 3, 4, 5};
  std::string good_string = "Automatic memory management";
  
  // Good: Using smart pointers
  std::cout << "Good: Using smart pointers" << std::endl;
  auto good_resource = std::make_unique<Resource>("BestPractice");
  good_resource->use();
  
  // Good: RAII with custom classes
  std::cout << "Good: RAII ensures automatic cleanup" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_performance_comparison() {
  std::cout << "=== PERFORMANCE COMPARISON ===" << std::endl;
  
  // Stack vs Heap Performance Overview:
  // ┌─ STACK ALLOCATION ──────────────────────────────┐
  // │                                                │
  // │ Algorithm: Move stack pointer                  │
  // │                                               │
  // │   BEFORE:  SP ────────────┐                   │
  // │            ┌─────────────┐│                   │
  // │            │             ││                   │
  // │            │   FREE      ││                   │
  // │            │   SPACE     ││                   │
  // │            │             ││                   │
  // │            └─────────────┘│                   │
  // │                          │                   │
  // │   AFTER:   ┌─────────────┐│                   │
  // │            │   NEW       ││                   │
  // │            │  VARIABLE   ││                   │
  // │            └─────────────┘│                   │
  // │            SP ────────────┘                   │
  // │                                               │
  // │ Time: ~1 CPU cycle                            │
  // └───────────────────────────────────────────────┘
  
  // ┌─ HEAP ALLOCATION ───────────────────────────────┐
  // │                                                │
  // │ Algorithm: Search free list, split blocks     │
  // │                                               │
  // │ 1. Search free list for suitable block        │
  // │    ┌─────┐    ┌──────┐    ┌────────┐          │
  // │    │ 32B │ -> │ 64B  │ -> │ 128B ✓│          │
  // │    └─────┘    └──────┘    └────────┘          │
  // │                                               │
  // │ 2. Split block if too large                   │
  // │    ┌────────┐  =>  ┌─────┐ ┌─────┐             │
  // │    │ 128B   │      │ 64B │ │ 64B │             │
  // │    └────────┘      └─────┘ └─────┘             │
  // │                    ALLOC   FREE               │
  // │                                               │
  // │ 3. Update metadata and bookkeeping            │
  // │                                               │
  // │ Time: ~100-1000 CPU cycles                    │
  // └───────────────────────────────────────────────┘
  
  const size_t num_operations = 1000000;
  
  // Stack allocation performance
  auto start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < num_operations; ++i) {
    int stack_var = static_cast<int>(i);
    volatile int dummy = stack_var; // Prevent optimization
    (void)dummy;
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto stack_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  
  // Heap allocation performance
  start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < num_operations; ++i) {
    int* heap_var = new int(static_cast<int>(i));
    volatile int dummy = *heap_var; // Prevent optimization
    (void)dummy;
    delete heap_var;
  }
  end = std::chrono::high_resolution_clock::now();
  auto heap_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  
  std::cout << "Stack allocation (" << num_operations << " ops): " 
            << stack_duration.count() << " microseconds" << std::endl;
  std::cout << "Heap allocation (" << num_operations << " ops): " 
            << heap_duration.count() << " microseconds" << std::endl;
  std::cout << "Heap is ~" << (heap_duration.count() / static_cast<double>(stack_duration.count())) 
            << "x slower than stack" << std::endl;
  
  std::cout << std::endl;
}

int main() {
  std::cout << "C++ MEMORY MANAGEMENT TUTORIAL" << std::endl;
  std::cout << "===============================" << std::endl << std::endl;
  
  demonstrate_memory_model();
  demonstrate_stack_memory();
  demonstrate_heap_memory();
  demonstrate_memory_leaks();
  demonstrate_smart_pointers();
  demonstrate_memory_alignment();
  demonstrate_memory_pools();
  demonstrate_memory_best_practices();
  demonstrate_performance_comparison();
  
  std::cout << "Memory management tutorial completed successfully!" << std::endl;
  std::cout << "Remember: Prefer RAII and smart pointers for safe, efficient code!" << std::endl;
  
  return 0;
}