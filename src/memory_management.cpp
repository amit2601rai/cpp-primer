/**
 * @file memory_management.cpp
 * @author GitHub Copilot
 * @brief Tutorial code for C++ memory model, memory areas, and management best practices.
 */

// This program provides a comprehensive overview of C++ memory management including:
// - Memory model and layout
// - Stack vs Heap memory
// - Dynamic memory allocation
// - Memory leaks and best practices
// - Smart pointers and RAII
// - Memory alignment and optimization

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
  
  std::cout << "--- Memory Segments ---" << std::endl;
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
  
  // Clean up
  delete heap_var;
  
  std::cout << std::endl;
}

void demonstrate_stack_memory() {
  std::cout << "=== STACK MEMORY ===" << std::endl;
  
  std::cout << "--- Stack Characteristics ---" << std::endl;
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
  auto lambda = [&]() {
    int nested_var = 999;
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
  
  std::cout << "--- Heap Characteristics ---" << std::endl;
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
  
  std::cout << "--- What is a Memory Leak? ---" << std::endl;
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
  
  std::cout << "Smart pointers automatically manage memory and provide" << std::endl;
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
  
  std::cout << "--- Memory Alignment Basics ---" << std::endl;
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
  
  std::cout << "--- Memory Pool Concept ---" << std::endl;
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