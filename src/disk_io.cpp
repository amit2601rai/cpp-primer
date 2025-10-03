/**
 * @file disk_io.cpp
 * @author GitHub Copilot
 * @brief Tutorial code for disk I/O operations, OS involvement, and storage systems.
 */

// This program provides a comprehensive overview of disk I/O including:
// - File system operations and OS involvement
// - Buffer management and caching
// - Synchronous vs Asynchronous I/O
// - Direct I/O vs Buffered I/O
// - Storage hierarchy and performance
// - Error handling and recovery

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <cstring>

void demonstrate_file_system_overview() {
  std::cout << "=== DISK I/O AND FILE SYSTEM OVERVIEW ===" << std::endl;
  
  // Complete I/O Stack Overview:
  // ┌─────────────────────────────────────────────────────────┐
  // │                    APPLICATION LAYER                    │
  // │  ┌─────────────────────────────────────────────────┐   │
  // │  │ C++ Program: fstream, iostream, file operations│   │
  // │  │ std::ifstream, std::ofstream, std::fstream      │   │
  // │  └─────────────────────────────────────────────────┘   │
  // ├─────────────────────────────────────────────────────────┤
  // │                   C STANDARD LIBRARY                   │
  // │  ┌─────────────────────────────────────────────────┐   │
  // │  │ FILE*, fopen(), fread(), fwrite(), fclose()     │   │
  // │  │ User-space buffering (stdio buffers)           │   │
  // │  └─────────────────────────────────────────────────┘   │
  // ├─────────────────────────────────────────────────────────┤
  // │                    SYSTEM CALLS                        │
  // │  ┌─────────────────────────────────────────────────┐   │
  // │  │ open(), read(), write(), close(), lseek()       │   │
  // │  │ User → Kernel transition                        │   │
  // │  └─────────────────────────────────────────────────┘   │
  // ├─────────────────────────────────────────────────────────┤
  // │                   OPERATING SYSTEM                     │
  // │  ┌─────────────────────────────────────────────────┐   │
  // │  │ Virtual File System (VFS)                       │   │
  // │  │ File Descriptor Table                           │   │
  // │  │ Page Cache / Buffer Cache                       │   │
  // │  │ I/O Scheduler                                   │   │
  // │  └─────────────────────────────────────────────────┘   │
  // ├─────────────────────────────────────────────────────────┤
  // │                   DEVICE DRIVERS                       │
  // │  ┌─────────────────────────────────────────────────┐   │
  // │  │ Block Device Driver (e.g., SATA, NVMe)         │   │
  // │  │ Hardware abstraction                            │   │
  // │  └─────────────────────────────────────────────────┘   │
  // ├─────────────────────────────────────────────────────────┤
  // │                      HARDWARE                           │
  // │  ┌─────────────────────────────────────────────────┐   │
  // │  │ Storage Controller (SATA, NVMe, etc.)          │   │
  // │  │ Physical Storage Device (HDD, SSD, NVMe)       │   │
  // │  └─────────────────────────────────────────────────┘   │
  // └─────────────────────────────────────────────────────────┘
  
  std::cout << "\n--- Storage Hierarchy Performance ---" << std::endl;
  // Storage Hierarchy (Latency & Capacity):
  // ┌─ Storage Hierarchy ─────────────────────────────────────┐
  // │                                                         │
  // │ CPU Registers:    < 1 ns      │ ~1 KB    │ Volatile    │
  // │ ├─ L1 Cache:      ~1 ns       │ ~64 KB   │ Volatile    │
  // │ ├─ L2 Cache:      ~3 ns       │ ~512 KB  │ Volatile    │
  // │ ├─ L3 Cache:      ~10 ns      │ ~8 MB    │ Volatile    │
  // │ ├─ Main Memory:   ~100 ns     │ ~16 GB   │ Volatile    │
  // │ ├─ SSD:           ~100 μs     │ ~1 TB    │ Persistent  │
  // │ ├─ HDD:           ~10 ms      │ ~4 TB    │ Persistent  │
  // │ └─ Network/Tape:  ~100 ms     │ ~∞       │ Persistent  │
  // │                                                         │
  // │ Speed:  ████████████████ → █ (10^8 times difference)   │
  // │ Cost:   █ → ████████████████ (per byte)                │
  // └─────────────────────────────────────────────────────────┘
  
  std::cout << "Storage devices provide persistent storage with much higher" << std::endl;
  std::cout << "latency than memory. OS optimizes access through caching," << std::endl;
  std::cout << "buffering, and intelligent scheduling." << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_basic_file_operations() {
  std::cout << "=== BASIC FILE OPERATIONS ===" << std::endl;
  
  // File Operation Flow:
  // ┌─ File Write Operation Flow ─────────────────────────────┐
  // │                                                         │
  // │ 1. Application calls std::ofstream::write()             │
  // │    ┌─────────────────────────────────────────────┐     │
  // │    │ std::ofstream buffer (user space)          │     │
  // │    │ [Data accumulates until buffer full]       │     │
  // │    └─────────────────────────────────────────────┘     │
  // │                       │                               │
  // │ 2. Buffer flush triggers system call                   │
  // │                       ↓                               │
  // │    ┌─────────────────────────────────────────────┐     │
  // │    │ write() system call                        │     │
  // │    │ User space → Kernel space transition       │     │
  // │    └─────────────────────────────────────────────┘     │
  // │                       │                               │
  // │ 3. Kernel processes the request                        │
  // │                       ↓                               │
  // │    ┌─────────────────────────────────────────────┐     │
  // │    │ Kernel Page Cache                          │     │
  // │    │ [Data may be cached for performance]       │     │
  // │    └─────────────────────────────────────────────┘     │
  // │                       │                               │
  // │ 4. Eventually written to storage                       │
  // │                       ↓                               │
  // │    ┌─────────────────────────────────────────────┐     │
  // │    │ Storage Device (SSD/HDD)                   │     │
  // │    │ [Persistent storage]                       │     │
  // │    └─────────────────────────────────────────────┘     │
  // └─────────────────────────────────────────────────────────┘
  
  const std::string filename = "test_file.txt";
  const std::string content = "Hello, Disk I/O World!\nThis is a test file.\n";
  
  std::cout << "\n--- Writing to File ---" << std::endl;
  {
    std::ofstream file(filename);
    if (file.is_open()) {
      auto start = std::chrono::high_resolution_clock::now();
      file << content;
      file.flush(); // Force buffer flush
      auto end = std::chrono::high_resolution_clock::now();
      
      auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
      std::cout << "Write operation took: " << duration.count() << " microseconds" << std::endl;
      std::cout << "Data written: " << content.size() << " bytes" << std::endl;
    } else {
      std::cout << "Error opening file for writing!" << std::endl;
    }
  } // RAII: file automatically closed here
  
  std::cout << "\n--- Reading from File ---" << std::endl;
  {
    std::ifstream file(filename);
    if (file.is_open()) {
      auto start = std::chrono::high_resolution_clock::now();
      
      std::string line;
      std::string file_content;
      while (std::getline(file, line)) {
        file_content += line + "\n";
      }
      
      auto end = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
      
      std::cout << "Read operation took: " << duration.count() << " microseconds" << std::endl;
      std::cout << "Data read: " << file_content.size() << " bytes" << std::endl;
      std::cout << "Content: " << file_content;
    } else {
      std::cout << "Error opening file for reading!" << std::endl;
    }
  }
  
  // Cleanup
  std::remove(filename.c_str());
  
  std::cout << std::endl;
}

void demonstrate_buffering_mechanisms() {
  std::cout << "=== BUFFERING MECHANISMS ===" << std::endl;
  
  // Multi-Level Buffering:
  // ┌─ Multi-Level Buffering Architecture ───────────────────┐
  // │                                                         │
  // │ Application:                                            │
  // │ ┌─────────────────────────────────────────────────┐     │
  // │ │ std::ofstream internal buffer                   │     │
  // │ │ Size: ~8KB (implementation dependent)          │     │
  // │ │ ┌─┬─┬─┬─┬─┬─┬─┬─┐ ← write() calls fill buffer │     │
  // │ │ │D│A│T│A│ │ │ │ │                             │     │
  // │ │ └─┴─┴─┴─┴─┴─┴─┴─┘                             │     │
  // │ └─────────────────────────────────────────────────┘     │
  // │                       │ flush() or buffer full         │
  // │                       ↓                               │
  // │ C Library (libc):                                      │
  // │ ┌─────────────────────────────────────────────────┐     │
  // │ │ FILE* buffer (stdio)                           │     │
  // │ │ Size: ~4KB (BUFSIZ)                            │     │
  // │ │ ┌─┬─┬─┬─┬─┬─┬─┬─┐ ← buffered I/O               │     │
  // │ │ │D│A│T│A│ │ │ │ │                             │     │
  // │ │ └─┴─┴─┴─┴─┴─┴─┴─┘                             │     │
  // │ └─────────────────────────────────────────────────┘     │
  // │                       │ system call boundary           │
  // │                       ↓                               │
  // │ Kernel:                                                │
  // │ ┌─────────────────────────────────────────────────┐     │
  // │ │ Page Cache / Buffer Cache                      │     │
  // │ │ Size: Variable (system memory dependent)       │     │
  // │ │ ┌───────┬───────┬───────┬───────┐             │     │
  // │ │ │ Page  │ Page  │ Page  │ Page  │ ← 4KB pages │     │
  // │ │ │ [DATA]│ [DATA]│ [FREE]│ [FREE]│             │     │
  // │ │ └───────┴───────┴───────┴───────┘             │     │
  // │ └─────────────────────────────────────────────────┘     │
  // │                       │ write-back / write-through     │
  // │                       ↓                               │
  // │ Storage:                                               │
  // │ ┌─────────────────────────────────────────────────┐     │
  // │ │ Physical Storage (SSD/HDD)                     │     │
  // │ │ ┌─────────────────────────────────────────┐     │     │
  // │ │ │ Blocks/Sectors (512B - 4KB each)       │     │     │
  // │ │ └─────────────────────────────────────────┘     │     │
  // │ └─────────────────────────────────────────────────┘     │
  // └─────────────────────────────────────────────────────────┘
  
  const std::string filename = "buffer_test.txt";
  
  std::cout << "\n--- Demonstrating Different Buffer Behaviors ---" << std::endl;
  
  // Test 1: Unbuffered output
  std::cout << "1. Unbuffered I/O (each write goes to kernel immediately):" << std::endl;
  {
    std::ofstream file(filename);
    file.rdbuf()->pubsetbuf(nullptr, 0); // Disable buffering
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
      file << "Line " << i << "\n";
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "   Unbuffered writes took: " << duration.count() << " microseconds" << std::endl;
  }
  
  // Test 2: Buffered output (default)
  std::cout << "2. Buffered I/O (writes accumulate in buffer):" << std::endl;
  {
    std::ofstream file(filename);
    // Default buffering
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
      file << "Line " << i << "\n";
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "   Buffered writes took: " << duration.count() << " microseconds" << std::endl;
  }
  
  // Test 3: Manual buffer control
  std::cout << "3. Large buffer (reduced system calls):" << std::endl;
  {
    std::ofstream file(filename);
    char large_buffer[64 * 1024]; // 64KB buffer
    file.rdbuf()->pubsetbuf(large_buffer, sizeof(large_buffer));
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
      file << "Line " << i << "\n";
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "   Large buffer writes took: " << duration.count() << " microseconds" << std::endl;
  }
  
  // Cleanup
  std::remove(filename.c_str());
  
  std::cout << std::endl;
}

void demonstrate_synchronous_vs_asynchronous() {
  std::cout << "=== SYNCHRONOUS vs ASYNCHRONOUS I/O ===" << std::endl;
  
  // Synchronous I/O Flow:
  // ┌─ Synchronous I/O ───────────────────────────────────────┐
  // │                                                         │
  // │ Thread Timeline:                                        │
  // │ ┌─────┬─────────────────────────┬─────┬─────┬─────┐     │
  // │ │CPU  │    BLOCKED WAITING      │CPU  │CPU  │CPU  │     │
  // │ │Work │    FOR DISK I/O         │Work │Work │Work │     │
  // │ └─────┴─────────────────────────┴─────┴─────┴─────┘     │
  // │   ↑           ↑               ↑                         │
  // │ write()   Disk Access    I/O Complete                   │
  // │ called                                                  │
  // │                                                         │
  // │ Process State:                                          │
  // │ RUNNING → BLOCKED → RUNNING                             │
  // └─────────────────────────────────────────────────────────┘
  
  // Asynchronous I/O Flow:
  // ┌─ Asynchronous I/O ──────────────────────────────────────┐
  // │                                                         │
  // │ Thread Timeline:                                        │
  // │ ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐       │
  // │ │CPU  │CPU  │CPU  │CPU  │CPU  │CPU  │CPU  │CPU  │       │
  // │ │Work │Work │Work │Work │Work │Work │Work │Work │       │
  // │ └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘       │
  // │   ↑                               ↑                     │
  // │ async_write()                 Callback/                 │
  // │ returns immediately           Completion                │
  // │                                                         │
  // │ Disk I/O happens in background:                         │
  // │ ┌─────────────────────────┐                             │
  // │ │     DISK OPERATION      │                             │
  // │ │   (Parallel to CPU)     │                             │
  // │ └─────────────────────────┘                             │
  // │                                                         │
  // │ Process State: RUNNING (never blocked)                  │
  // └─────────────────────────────────────────────────────────┘
  
  const std::string filename = "sync_async_test.txt";
  const size_t data_size = 1024 * 1024; // 1MB
  std::string large_data(data_size, 'A');
  
  std::cout << "\n--- Synchronous I/O Example ---" << std::endl;
  {
    auto start = std::chrono::high_resolution_clock::now();
    
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
      std::cout << "Thread blocks here until I/O completes..." << std::endl;
      file.write(large_data.c_str(), large_data.size());
      file.flush(); // Force immediate write
      std::cout << "I/O completed, thread can continue." << std::endl;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Synchronous write took: " << duration.count() << " ms" << std::endl;
  }
  
  std::cout << "\n--- Simulated Asynchronous I/O Pattern ---" << std::endl;
  {
    auto start = std::chrono::high_resolution_clock::now();
    
    // Simulate async I/O with threads (simplified example)
    std::cout << "Initiating async I/O..." << std::endl;
    std::cout << "Thread continues with other work immediately!" << std::endl;
    
    // Simulate other work while I/O happens
    std::cout << "Doing other CPU work..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    // In real async I/O, you'd use callbacks, futures, or polling
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
      file.write(large_data.c_str(), large_data.size());
    }
    
    std::cout << "I/O completion notification received." << std::endl;
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Total time (including parallel work): " << duration.count() << " ms" << std::endl;
  }
  
  // Cleanup
  std::remove(filename.c_str());
  
  std::cout << std::endl;
}

void demonstrate_error_handling() {
  std::cout << "=== ERROR HANDLING IN DISK I/O ===" << std::endl;
  
  // Error Handling Flow:
  // ┌─ Error Handling Layers ─────────────────────────────────┐
  // │                                                         │
  // │ Application Layer:                                      │
  // │ ┌─────────────────────────────────────────────────┐     │
  // │ │ try/catch blocks                                │     │
  // │ │ std::ios::failbit, std::ios::badbit checking   │     │
  // │ │ errno checking for C-style I/O                 │     │
  // │ └─────────────────────────────────────────────────┘     │
  // │                       │                               │
  // │                       ↓                               │
  // │ C Library Layer:                                       │
  // │ ┌─────────────────────────────────────────────────┐     │
  // │ │ errno codes (ENOENT, EACCES, ENOSPC, etc.)     │     │
  // │ │ Return value checking (-1 indicates error)     │     │
  // │ └─────────────────────────────────────────────────┘     │
  // │                       │                               │
  // │                       ↓                               │
  // │ Kernel Layer:                                          │
  // │ ┌─────────────────────────────────────────────────┐     │
  // │ │ File system errors                             │     │
  // │ │ Permission checks                              │     │
  // │ │ Disk space validation                          │     │
  // │ │ Hardware error detection                       │     │
  // │ └─────────────────────────────────────────────────┘     │
  // │                       │                               │
  // │                       ↓                               │
  // │ Hardware Layer:                                        │
  // │ ┌─────────────────────────────────────────────────┐     │
  // │ │ ECC error correction                           │     │
  // │ │ Bad sector remapping                           │     │
  // │ │ Device failure detection                       │     │
  // │ └─────────────────────────────────────────────────┘     │
  // └─────────────────────────────────────────────────────────┘
  
  std::cout << "\n--- Common Error Scenarios ---" << std::endl;
  
  // Test 1: File not found
  std::cout << "1. Attempting to read non-existent file:" << std::endl;
  {
    std::ifstream file("non_existent_file.txt");
    if (!file.is_open()) {
      std::cout << "   Error: Could not open file (file not found)" << std::endl;
      std::cout << "   C++ stream state: " << (file.fail() ? "fail" : "ok") << std::endl;
    }
  }
  
  // Test 2: Permission denied (simulated)
  std::cout << "2. Attempting to write to read-only location:" << std::endl;
  {
    std::ofstream file("/root/test_file.txt"); // Likely to fail with permission denied
    if (!file.is_open()) {
      std::cout << "   Error: Permission denied or path not accessible" << std::endl;
    } else {
      std::cout << "   Unexpected: File opened successfully" << std::endl;
      file.close();
    }
  }
  
  // Test 3: Disk space issues (simulated by writing large file)
  std::cout << "3. Handling potential disk space issues:" << std::endl;
  {
    const std::string filename = "space_test.txt";
    std::ofstream file(filename);
    
    if (file.is_open()) {
      try {
        // Write large amount of data
        std::string large_block(1024 * 1024, 'X'); // 1MB block
        
        for (int i = 0; i < 10; ++i) {
          file << large_block;
          if (file.fail()) {
            std::cout << "   Error during write operation (possibly disk full)" << std::endl;
            break;
          }
        }
        
        file.flush();
        if (file.fail()) {
          std::cout << "   Error during flush (possibly disk full)" << std::endl;
        } else {
          std::cout << "   Large file write completed successfully" << std::endl;
        }
        
      } catch (const std::exception& e) {
        std::cout << "   Exception caught: " << e.what() << std::endl;
      }
      
      file.close();
    }
    
    // Cleanup
    std::remove(filename.c_str());
  }
  
  // Test 4: Low-level error handling with C-style I/O
  std::cout << "4. Low-level error handling example:" << std::endl;
  {
    int fd = open("test_lowlevel.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
      std::cout << "   Error opening file: " << std::strerror(errno) << std::endl;
    } else {
      const char* data = "Test data\n";
      ssize_t bytes_written = write(fd, data, strlen(data));
      
      if (bytes_written == -1) {
        std::cout << "   Error writing file: " << std::strerror(errno) << std::endl;
      } else {
        std::cout << "   Successfully wrote " << bytes_written << " bytes" << std::endl;
      }
      
      if (close(fd) == -1) {
        std::cout << "   Error closing file: " << std::strerror(errno) << std::endl;
      }
      
      // Cleanup
      unlink("test_lowlevel.txt");
    }
  }
  
  std::cout << std::endl;
}

void demonstrate_performance_optimization() {
  std::cout << "=== PERFORMANCE OPTIMIZATION TECHNIQUES ===" << std::endl;
  
  // I/O Performance Optimization Strategies:
  // ┌─ I/O Optimization Techniques ──────────────────────────┐
  // │                                                         │
  // │ 1. Buffering & Caching:                                │
  // │    ┌─────────────────────────────────────────────┐     │
  // │    │ Application: Large buffers reduce syscalls │     │
  // │    │ Kernel: Page cache reduces disk access     │     │
  // │    │ Hardware: Disk cache/SSD cache             │     │
  // │    └─────────────────────────────────────────────┘     │
  // │                                                         │
  // │ 2. Sequential vs Random Access:                        │
  // │    Sequential: ████████████████ (Fast)                 │
  // │    Random:     ████ (Slow - disk head movement)        │
  // │                                                         │
  // │ 3. Batch Operations:                                    │
  // │    ┌─────┬─────┬─────┬─────┐ Single large operation    │
  // │    │ Op1 │ Op2 │ Op3 │ Op4 │ vs many small ones        │
  // │    └─────┴─────┴─────┴─────┘                           │
  // │                                                         │
  // │ 4. Asynchronous I/O:                                   │
  // │    CPU: ████████████████ (Not blocked)                 │
  // │    I/O:   ████████████████ (Parallel)                  │
  // │                                                         │
  // │ 5. Memory-Mapped Files:                                │
  // │    Virtual Memory ↔ File (OS handles paging)           │
  // └─────────────────────────────────────────────────────────┘
  
  const std::string filename = "performance_test.txt";
  const size_t block_size = 64 * 1024; // 64KB blocks
  const size_t num_blocks = 100;
  const size_t total_size = block_size * num_blocks;
  
  std::cout << "\n--- Performance Test: Different I/O Strategies ---" << std::endl;
  
  // Strategy 1: Small writes (inefficient)
  std::cout << "1. Many small writes (inefficient):" << std::endl;
  {
    auto start = std::chrono::high_resolution_clock::now();
    
    std::ofstream file(filename, std::ios::binary);
    std::string small_data(1024, 'A'); // 1KB per write
    
    for (size_t i = 0; i < total_size / 1024; ++i) {
      file.write(small_data.c_str(), small_data.size());
    }
    file.flush();
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "   Small writes took: " << duration.count() << " ms" << std::endl;
  }
  
  // Strategy 2: Large buffered writes (efficient)
  std::cout << "2. Large buffered writes (efficient):" << std::endl;
  {
    auto start = std::chrono::high_resolution_clock::now();
    
    std::ofstream file(filename, std::ios::binary);
    std::string large_data(block_size, 'B'); // 64KB per write
    
    for (size_t i = 0; i < num_blocks; ++i) {
      file.write(large_data.c_str(), large_data.size());
    }
    file.flush();
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "   Large writes took: " << duration.count() << " ms" << std::endl;
  }
  
  // Strategy 3: Single massive write (most efficient)
  std::cout << "3. Single massive write (most efficient):" << std::endl;
  {
    auto start = std::chrono::high_resolution_clock::now();
    
    std::ofstream file(filename, std::ios::binary);
    std::string massive_data(total_size, 'C');
    
    file.write(massive_data.c_str(), massive_data.size());
    file.flush();
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "   Massive write took: " << duration.count() << " ms" << std::endl;
  }
  
  // Read performance comparison
  std::cout << "\n--- Read Performance: Sequential vs Random Access ---" << std::endl;
  
  // Sequential read
  std::cout << "1. Sequential read:" << std::endl;
  {
    auto start = std::chrono::high_resolution_clock::now();
    
    std::ifstream file(filename, std::ios::binary);
    std::string buffer(block_size, '\0');
    size_t total_read = 0;
    
    while (file.read(&buffer[0], buffer.size()) || file.gcount() > 0) {
      total_read += file.gcount();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "   Sequential read took: " << duration.count() << " ms" << std::endl;
    std::cout << "   Total bytes read: " << total_read << std::endl;
  }
  
  // Random access read (simulated)
  std::cout << "2. Random access read:" << std::endl;
  {
    auto start = std::chrono::high_resolution_clock::now();
    
    std::ifstream file(filename, std::ios::binary);
    std::string buffer(1024, '\0'); // Smaller reads
    size_t total_read = 0;
    
    // Simulate random access by seeking to different positions
    for (int i = 0; i < 100; ++i) {
      size_t random_pos = (i * 12347) % (total_size - 1024); // Pseudo-random
      file.seekg(random_pos);
      if (file.read(&buffer[0], buffer.size())) {
        total_read += buffer.size();
      }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "   Random access read took: " << duration.count() << " ms" << std::endl;
    std::cout << "   Total bytes read: " << total_read << std::endl;
  }
  
  // Cleanup
  std::remove(filename.c_str());
  
  std::cout << std::endl;
}

void demonstrate_os_involvement() {
  std::cout << "=== OPERATING SYSTEM INVOLVEMENT IN I/O ===" << std::endl;
  
  // Detailed OS I/O Processing:
  // ┌─ Complete I/O Request Processing Flow ─────────────────┐
  // │                                                         │
  // │ 1. Application Request:                                 │
  // │    ┌─────────────────────────────────────────────┐     │
  // │    │ app: file.write("data")                     │     │
  // │    └─────────────────────────────────────────────┘     │
  // │                       │                               │
  // │                       ↓                               │
  // │ 2. C++ Library Processing:                             │
  // │    ┌─────────────────────────────────────────────┐     │
  // │    │ std::ofstream buffer management             │     │
  // │    │ Format conversion, buffer accumulation      │     │
  // │    └─────────────────────────────────────────────┘     │
  // │                       │                               │
  // │                       ↓                               │
  // │ 3. System Call Interface:                              │
  // │    ┌─────────────────────────────────────────────┐     │
  // │    │ write(fd, buffer, size)                     │     │
  // │    │ User mode → Kernel mode transition          │     │
  // │    │ Context switch overhead                     │     │
  // │    └─────────────────────────────────────────────┘     │
  // │                       │                               │
  // │                       ↓                               │
  // │ 4. Virtual File System (VFS):                          │
  // │    ┌─────────────────────────────────────────────┐     │
  // │    │ File descriptor → inode mapping             │     │
  // │    │ Permission checking                         │     │
  // │    │ File system type resolution                 │     │
  // │    └─────────────────────────────────────────────┘     │
  // │                       │                               │
  // │                       ↓                               │
  // │ 5. File System Layer:                                  │
  // │    ┌─────────────────────────────────────────────┐     │
  // │    │ Block allocation/deallocation               │     │
  // │    │ Metadata updates (size, timestamps)        │     │
  // │    │ Journal/log updates (if journaling FS)     │     │
  // │    └─────────────────────────────────────────────┘     │
  // │                       │                               │
  // │                       ↓                               │
  // │ 6. Page Cache Management:                              │
  // │    ┌─────────────────────────────────────────────┐     │
  // │    │ Check if data already in cache              │     │
  // │    │ Allocate new pages if needed                │     │
  // │    │ Mark pages as dirty                         │     │
  // │    │ Schedule writeback (write-behind)           │     │
  // │    └─────────────────────────────────────────────┘     │
  // │                       │                               │
  // │                       ↓                               │
  // │ 7. I/O Scheduler:                                      │
  // │    ┌─────────────────────────────────────────────┐     │
  // │    │ Queue I/O requests                          │     │
  // │    │ Optimize disk head movement (elevator)      │     │
  // │    │ Merge adjacent requests                     │     │
  // │    │ Apply fairness policies                     │     │
  // │    └─────────────────────────────────────────────┘     │
  // │                       │                               │
  // │                       ↓                               │
  // │ 8. Block Device Driver:                                │
  // │    ┌─────────────────────────────────────────────┐     │
  // │    │ Convert logical → physical addresses        │     │
  // │    │ Device-specific command formatting          │     │
  // │    │ DMA setup and management                    │     │
  // │    │ Interrupt handling                          │     │
  // │    └─────────────────────────────────────────────┘     │
  // │                       │                               │
  // │                       ↓                               │
  // │ 9. Hardware Controller:                                │
  // │    ┌─────────────────────────────────────────────┐     │
  // │    │ SATA/NVMe protocol handling                 │     │
  // │    │ Error correction and retry logic            │     │
  // │    │ Physical media access                       │     │
  // │    └─────────────────────────────────────────────┘     │
  // └─────────────────────────────────────────────────────────┘
  
  std::cout << "\n--- OS Services in I/O Operations ---" << std::endl;
  
  std::cout << "1. File Descriptor Management:" << std::endl;
  std::cout << "   - OS maintains a table mapping FDs to file structures" << std::endl;
  std::cout << "   - Each process has its own FD table" << std::endl;
  std::cout << "   - Reference counting for shared files" << std::endl;
  
  std::cout << "\n2. Buffer/Cache Management:" << std::endl;
  std::cout << "   - Page cache reduces physical I/O operations" << std::endl;
  std::cout << "   - Write-behind: dirty pages written asynchronously" << std::endl;
  std::cout << "   - Read-ahead: anticipatory reading for sequential access" << std::endl;
  
  std::cout << "\n3. I/O Scheduling:" << std::endl;
  std::cout << "   - Elevator algorithm minimizes disk head movement" << std::endl;
  std::cout << "   - Request merging for adjacent blocks" << std::endl;
  std::cout << "   - Fairness policies prevent starvation" << std::endl;
  
  std::cout << "\n4. Error Handling and Recovery:" << std::endl;
  std::cout << "   - Automatic retry for transient errors" << std::endl;
  std::cout << "   - Bad block remapping" << std::endl;
  std::cout << "   - Filesystem consistency checks" << std::endl;
  
  std::cout << "\n5. Security and Permissions:" << std::endl;
  std::cout << "   - Access control (read/write/execute permissions)" << std::endl;
  std::cout << "   - User/group ownership verification" << std::endl;
  std::cout << "   - Mandatory access controls (SELinux, etc.)" << std::endl;
  
  // Demonstrate file descriptor information
  const std::string filename = "os_demo.txt";
  std::ofstream file(filename);
  
  if (file.is_open()) {
    std::cout << "\n--- File System Information ---" << std::endl;
    
    // Get file information using system calls
    struct stat file_stat;
    if (stat(filename.c_str(), &file_stat) == 0) {
      std::cout << "File size: " << file_stat.st_size << " bytes" << std::endl;
      std::cout << "Inode number: " << file_stat.st_ino << std::endl;
      std::cout << "Device ID: " << file_stat.st_dev << std::endl;
      std::cout << "Number of links: " << file_stat.st_nlink << std::endl;
      std::cout << "Block size: " << file_stat.st_blksize << " bytes" << std::endl;
      std::cout << "Blocks allocated: " << file_stat.st_blocks << std::endl;
    }
    
    file.close();
  }
  
  // Cleanup
  std::remove(filename.c_str());
  
  std::cout << std::endl;
}

int main() {
  std::cout << "DISK I/O AND OPERATING SYSTEM TUTORIAL" << std::endl;
  std::cout << "=======================================" << std::endl << std::endl;
  
  demonstrate_file_system_overview();
  demonstrate_basic_file_operations();
  demonstrate_buffering_mechanisms();
  demonstrate_synchronous_vs_asynchronous();
  demonstrate_error_handling();
  demonstrate_performance_optimization();
  demonstrate_os_involvement();
  
  std::cout << "Disk I/O tutorial completed successfully!" << std::endl;
  std::cout << "Key takeaways:" << std::endl;
  std::cout << "- OS provides multiple layers of abstraction and optimization" << std::endl;
  std::cout << "- Buffering and caching are crucial for performance" << std::endl;
  std::cout << "- Asynchronous I/O prevents blocking and improves concurrency" << std::endl;
  std::cout << "- Error handling should occur at multiple levels" << std::endl;
  std::cout << "- Understanding the I/O stack helps optimize application performance" << std::endl;
  
  return 0;
}