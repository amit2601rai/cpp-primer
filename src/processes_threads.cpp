/**
 * @file processes_threads.cpp
 * @author GitHub Copilot
 * @brief Tutorial code for processes vs threads, memory layouts, and concurrency concepts.
 */

// This program provides a comprehensive overview of processes and threads including:
// - Process vs Thread architecture and memory layout
// - Process creation and communication
// - Thread creation and synchronization
// - Memory sharing and isolation
// - Performance characteristics
// - Practical examples and demonstrations

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <chrono>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <cstring>

// Global variables for demonstration
std::atomic<int> shared_counter{0};
std::mutex console_mutex;

void demonstrate_process_vs_thread_overview() {
  std::cout << "=== PROCESSES vs THREADS OVERVIEW ===" << std::endl;
  
  // Complete Process vs Thread Architecture:
  // ┌─ PROCESS ARCHITECTURE ──────────────────────────────────┐
  // │                                                         │
  // │ Process 1:               Process 2:                     │
  // │ ┌─────────────────────┐  ┌─────────────────────────┐   │
  // │ │ Virtual Address     │  │ Virtual Address         │   │
  // │ │ Space (Isolated)    │  │ Space (Isolated)        │   │
  // │ │                     │  │                         │   │
  // │ │ ┌─ Stack ─────────┐ │  │ ┌─ Stack ─────────────┐ │   │
  // │ │ │ Thread 1 Stack  │ │  │ │ Thread 1 Stack      │ │   │
  // │ │ └─────────────────┘ │  │ └─────────────────────┘ │   │
  // │ │ ┌─ Heap ──────────┐ │  │ ┌─ Heap ──────────────┐ │   │
  // │ │ │ Dynamic Memory  │ │  │ │ Dynamic Memory      │ │   │
  // │ │ └─────────────────┘ │  │ └─────────────────────┘ │   │
  // │ │ ┌─ Data/BSS ──────┐ │  │ ┌─ Data/BSS ──────────┐ │   │
  // │ │ │ Global Variables│ │  │ │ Global Variables    │ │   │
  // │ │ └─────────────────┘ │  │ └─────────────────────┘ │   │
  // │ │ ┌─ Code/Text ─────┐ │  │ ┌─ Code/Text ─────────┐ │   │
  // │ │ │ Program Code    │ │  │ │ Program Code        │ │   │
  // │ │ └─────────────────┘ │  │ └─────────────────────┘ │   │
  // │ └─────────────────────┘  └─────────────────────────┘   │
  // │                                                         │
  // │ Communication:           Communication:                 │
  // │ • Pipes, Sockets        • Shared Memory                │
  // │ • Message Queues        • Files                        │
  // │ • Shared Memory         • Network                      │
  // │                                                         │
  // │ Isolation: COMPLETE     Isolation: COMPLETE            │
  // │ Performance: SLOWER     Performance: SLOWER            │
  // │ Creation Cost: HIGH     Creation Cost: HIGH            │
  // └─────────────────────────────────────────────────────────┘
  
  // ┌─ THREAD ARCHITECTURE ───────────────────────────────────┐
  // │                                                         │
  // │ Single Process with Multiple Threads:                  │
  // │ ┌─────────────────────────────────────────────────────┐ │
  // │ │ Shared Virtual Address Space                        │ │
  // │ │                                                     │ │
  // │ │ ┌─ Thread Stacks ──────────────────────────────────┐ │ │
  // │ │ │ Thread 1 Stack │ Thread 2 Stack │ Thread 3 Stack│ │ │
  // │ │ │ ┌─────────────┐│ ┌─────────────┐│ ┌─────────────┐│ │ │
  // │ │ │ │Local Vars   ││ │Local Vars   ││ │Local Vars   ││ │ │
  // │ │ │ │Function     ││ │Function     ││ │Function     ││ │ │
  // │ │ │ │Call Stack   ││ │Call Stack   ││ │Call Stack   ││ │ │
  // │ │ │ └─────────────┘│ └─────────────┘│ └─────────────┘│ │ │
  // │ │ └────────────────────────────────────────────────────┘ │ │
  // │ │                                                     │ │
  // │ │ ┌─ SHARED HEAP ────────────────────────────────────┐ │ │
  // │ │ │ Dynamic Memory (Accessible by ALL threads)      │ │ │
  // │ │ │ • new/malloc allocations                        │ │ │
  // │ │ │ • STL containers                                │ │ │
  // │ │ │ • Smart pointers                                │ │ │
  // │ │ └─────────────────────────────────────────────────┘ │ │
  // │ │                                                     │ │
  // │ │ ┌─ SHARED DATA/BSS ────────────────────────────────┐ │ │
  // │ │ │ Global Variables (Accessible by ALL threads)    │ │ │
  // │ │ │ • Static variables                              │ │ │
  // │ │ │ • Global objects                                │ │ │
  // │ │ └─────────────────────────────────────────────────┘ │ │
  // │ │                                                     │ │
  // │ │ ┌─ SHARED CODE/TEXT ────────────────────────────────┐ │ │
  // │ │ │ Program Code (Shared by ALL threads)            │ │ │
  // │ │ │ • Function definitions                          │ │ │
  // │ │ │ • Class methods                                 │ │ │
  // │ │ └─────────────────────────────────────────────────┘ │ │
  // │ └─────────────────────────────────────────────────────┘ │
  // │                                                         │
  // │ Communication: DIRECT MEMORY ACCESS                    │
  // │ Synchronization: Mutexes, Atomics, Condition Variables │
  // │ Isolation: NONE (Shared address space)                 │
  // │ Performance: FASTER                                     │
  // │ Creation Cost: LOW                                      │
  // └─────────────────────────────────────────────────────────┘
  
  std::cout << "\n--- Key Differences Summary ---" << std::endl;
  std::cout << "PROCESSES:" << std::endl;
  std::cout << "• Complete memory isolation" << std::endl;
  std::cout << "• Higher creation/switching overhead" << std::endl;
  std::cout << "• IPC required for communication" << std::endl;
  std::cout << "• Fault isolation (one crash doesn't affect others)" << std::endl;
  std::cout << "• Each has own PID" << std::endl;
  
  std::cout << "\nTHREADS:" << std::endl;
  std::cout << "• Shared memory space (except stacks)" << std::endl;
  std::cout << "• Lower creation/switching overhead" << std::endl;
  std::cout << "• Direct memory sharing" << std::endl;
  std::cout << "• No fault isolation (crash affects entire process)" << std::endl;
  std::cout << "• Share same PID, have unique TIDs" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_process_creation() {
  std::cout << "=== PROCESS CREATION AND COMMUNICATION ===" << std::endl;
  
  // Process Creation Flow:
  // ┌─ Process Creation with fork() ──────────────────────────┐
  // │                                                         │
  // │ BEFORE fork():                                          │
  // │ ┌─────────────────────────────────────────────────┐     │
  // │ │ Parent Process                                  │     │
  // │ │ PID: 1234                                      │     │
  // │ │ ┌─ Memory Layout ─────────────────────────────┐ │     │
  // │ │ │ Stack:    [local variables]              │ │     │
  // │ │ │ Heap:     [dynamic allocations]          │ │     │
  // │ │ │ Data:     [global variables]             │ │     │
  // │ │ │ Code:     [program instructions]         │ │     │
  // │ │ └─────────────────────────────────────────────┘ │     │
  // │ └─────────────────────────────────────────────────┘     │
  // │                         │                               │
  // │                    fork() call                          │
  // │                         │                               │
  // │                         ↓                               │
  // │ AFTER fork():                                           │
  // │ ┌─────────────────────┐  ┌─────────────────────────┐   │
  // │ │ Parent Process      │  │ Child Process           │   │
  // │ │ PID: 1234          │  │ PID: 1235 (new)        │   │
  // │ │ fork() returns:    │  │ fork() returns: 0       │   │
  // │ │ child PID (1235)   │  │                         │   │
  // │ │                    │  │                         │   │
  // │ │ ┌─ Memory Layout ─┐ │  │ ┌─ Memory Layout ─────┐ │   │
  // │ │ │ Stack: [orig]  │ │  │ │ Stack: [COPY]       │ │   │
  // │ │ │ Heap:  [orig]  │ │  │ │ Heap:  [COPY]       │ │   │
  // │ │ │ Data:  [orig]  │ │  │ │ Data:  [COPY]       │ │   │
  // │ │ │ Code:  [shared]│ │  │ │ Code:  [shared]     │ │   │
  // │ │ └────────────────┘ │  │ └─────────────────────┘ │   │
  // │ └─────────────────────┘  └─────────────────────────┘   │
  // │                                                         │
  // │ Note: Copy-on-Write (COW) optimization used             │
  // │ Physical pages shared until modification                │
  // └─────────────────────────────────────────────────────────┘
  
  std::cout << "\n--- Fork Example ---" << std::endl;
  
  pid_t pid = fork();
  
  if (pid == -1) {
    std::cout << "Fork failed!" << std::endl;
    return;
  } else if (pid == 0) {
    // Child process
    std::cout << "Child Process:" << std::endl;
    std::cout << "  PID: " << getpid() << std::endl;
    std::cout << "  Parent PID: " << getppid() << std::endl;
    std::cout << "  Child is doing independent work..." << std::endl;
    
    // Child does some work
    for (int i = 0; i < 3; ++i) {
      std::cout << "  Child working... " << i << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    exit(0); // Child exits
  } else {
    // Parent process
    std::cout << "Parent Process:" << std::endl;
    std::cout << "  PID: " << getpid() << std::endl;
    std::cout << "  Created child with PID: " << pid << std::endl;
    std::cout << "  Parent continues its own work..." << std::endl;
    
    // Parent does some work
    for (int i = 0; i < 3; ++i) {
      std::cout << "  Parent working... " << i << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
    
    // Wait for child to complete
    int status;
    wait(&status);
    std::cout << "  Child process completed with status: " << status << std::endl;
  }
  
  // Process Communication Example using pipe
  std::cout << "\n--- Process Communication with Pipe ---" << std::endl;
  
  // Process Communication via Pipe:
  // ┌─ Inter-Process Communication (IPC) ─────────────────────┐
  // │                                                         │
  // │ Parent Process          │ PIPE │          Child Process │
  // │ ┌─────────────────────┐ │      │ ┌─────────────────────┐ │
  // │ │ Write End (fd[1])   │─┼─────→│ │ Read End (fd[0])    │ │
  // │ │                     │ │      │ │                     │ │
  // │ │ write(fd[1], data,  │ │      │ │ read(fd[0], buffer, │ │
  // │ │       sizeof(data)) │ │      │ │      sizeof(buffer))│ │
  // │ │                     │ │      │ │                     │ │
  // │ │ Memory Space A      │ │      │ │ Memory Space B      │ │
  // │ │ (Isolated)          │ │      │ │ (Isolated)          │ │
  // │ └─────────────────────┘ │      │ └─────────────────────┘ │
  // │                         │      │                         │
  // │ Kernel Buffer:          │      │                         │
  // │ ┌─────────────────────────────────────────────────────┐ │
  // │ │ [data] → [data] → [data] → FIFO Queue             │ │
  // │ └─────────────────────────────────────────────────────┘ │
  // └─────────────────────────────────────────────────────────┘
  
  int pipefd[2];
  if (pipe(pipefd) == -1) {
    std::cout << "Pipe creation failed!" << std::endl;
    return;
  }
  
  pid_t pipe_pid = fork();
  
  if (pipe_pid == 0) {
    // Child process - reader
    close(pipefd[1]); // Close write end
    
    char buffer[256];
    ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
      buffer[bytes_read] = '\0';
      std::cout << "Child received: " << buffer << std::endl;
    }
    
    close(pipefd[0]);
    exit(0);
  } else if (pipe_pid > 0) {
    // Parent process - writer
    close(pipefd[0]); // Close read end
    
    const char* message = "Hello from parent process!";
    write(pipefd[1], message, strlen(message));
    std::cout << "Parent sent: " << message << std::endl;
    
    close(pipefd[1]);
    
    int status;
    wait(&status);
  }
  
  std::cout << std::endl;
}

void demonstrate_thread_creation() {
  std::cout << "=== THREAD CREATION AND SYNCHRONIZATION ===" << std::endl;
  
  // Thread Creation and Memory Sharing:
  // ┌─ Thread Creation in Same Process ───────────────────────┐
  // │                                                         │
  // │ Main Thread creates Worker Threads:                    │
  // │                                                         │
  // │ ┌─ Process Address Space ─────────────────────────────┐ │
  // │ │                                                     │ │
  // │ │ ┌─ Stack Area ─────────────────────────────────────┐ │ │
  // │ │ │ Main Thread Stack     │ Worker Thread 1 Stack   │ │ │
  // │ │ │ ┌─────────────────┐   │ ┌─────────────────────┐ │ │ │
  // │ │ │ │ Local variables │   │ │ Local variables     │ │ │ │
  // │ │ │ │ Function calls  │   │ │ Function calls      │ │ │ │
  // │ │ │ │ Return addresses│   │ │ Return addresses    │ │ │ │
  // │ │ │ └─────────────────┘   │ └─────────────────────┘ │ │ │
  // │ │ │                       │                         │ │ │
  // │ │ │ Worker Thread 2 Stack │ Worker Thread 3 Stack   │ │ │
  // │ │ │ ┌─────────────────────┐│ ┌─────────────────────┐ │ │ │
  // │ │ │ │ Local variables     ││ │ Local variables     │ │ │ │
  // │ │ │ │ Function calls      ││ │ Function calls      │ │ │ │
  // │ │ │ │ Return addresses    ││ │ Return addresses    │ │ │ │
  // │ │ │ └─────────────────────┘│ └─────────────────────┘ │ │ │
  // │ │ └─────────────────────────────────────────────────┘ │ │
  // │ │                                                     │ │
  // │ │ ┌─ SHARED HEAP ────────────────────────────────────┐ │ │
  // │ │ │ All threads can access:                         │ │ │
  // │ │ │ • Global objects                                │ │ │
  // │ │ │ • Dynamically allocated memory                  │ │ │
  // │ │ │ • Static variables                              │ │ │
  // │ │ │ • File descriptors                              │ │ │
  // │ │ └─────────────────────────────────────────────────┘ │ │
  // │ │                                                     │ │
  // │ │ ┌─ SHARED DATA/CODE ───────────────────────────────┐ │ │
  // │ │ │ • Global variables                              │ │ │
  // │ │ │ • Function code                                 │ │ │
  // │ │ │ • Class definitions                             │ │ │
  // │ │ └─────────────────────────────────────────────────┘ │ │
  // │ └─────────────────────────────────────────────────────┘ │
  // │                                                         │
  // │ Synchronization Required:                               │
  // │ • Mutexes for exclusive access                          │
  // │ • Atomic operations for lock-free programming          │
  // │ • Condition variables for coordination                  │
  // └─────────────────────────────────────────────────────────┘
  
  std::cout << "\n--- Basic Thread Creation ---" << std::endl;
  
  // Worker function for threads
  auto worker_function = [](int thread_id) {
    {
      std::lock_guard<std::mutex> lock(console_mutex);
      std::cout << "Thread " << thread_id << " started (ID: " 
                << std::this_thread::get_id() << ")" << std::endl;
    }
    
    // Each thread increments shared counter
    for (int i = 0; i < 1000; ++i) {
      shared_counter.fetch_add(1);
    }
    
    {
      std::lock_guard<std::mutex> lock(console_mutex);
      std::cout << "Thread " << thread_id << " completed work" << std::endl;
    }
  };
  
  const int num_threads = 4;
  std::vector<std::thread> threads;
  
  auto start_time = std::chrono::high_resolution_clock::now();
  
  // Create and start threads
  for (int i = 0; i < num_threads; ++i) {
    threads.emplace_back(worker_function, i);
  }
  
  // Wait for all threads to complete
  for (auto& thread : threads) {
    thread.join();
  }
  
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
  
  std::cout << "All threads completed!" << std::endl;
  std::cout << "Final counter value: " << shared_counter.load() << std::endl;
  std::cout << "Expected: " << (num_threads * 1000) << std::endl;
  std::cout << "Time taken: " << duration.count() << " ms" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_thread_creation_deep_dive() {
  std::cout << "=== THREAD CREATION DEEP DIVE: What Exactly Creates Threads? ===" << std::endl;
  
  // Thread Creation Process Under the Hood:
  // ┌─ WHAT EXACTLY CREATES THREADS? ─────────────────────────┐
  // │                                                         │
  // │ std::thread Constructor Process:                        │
  // │ ┌─────────────────────────────────────────────────┐     │
  // │ │ 1. std::thread t(function, args...)            │     │
  // │ │    ↓                                            │     │
  // │ │ 2. Calls OS thread creation API:                │     │
  // │ │    • Linux/Mac: pthread_create()               │     │
  // │ │    • Windows: CreateThread()                   │     │
  // │ │    ↓                                            │     │
  // │ │ 3. OS allocates new thread stack (1-8MB)       │     │
  // │ │    ↓                                            │     │
  // │ │ 4. Creates Thread Control Block (TCB)          │     │
  // │ │    • Thread ID                                  │     │
  // │ │    • CPU register state                        │     │
  // │ │    • Stack pointer                             │     │
  // │ │    • Priority & scheduling info                │     │
  // │ │    ↓                                            │     │
  // │ │ 5. Registers with OS scheduler                  │     │
  // │ │    ↓                                            │     │
  // │ │ 6. Starts executing function in parallel       │     │
  // │ │    ↓                                            │     │
  // │ │ 7. Constructor returns IMMEDIATELY              │     │
  // │ │    (doesn't wait for completion)                │     │
  // │ └─────────────────────────────────────────────────┘     │
  // └─────────────────────────────────────────────────────────┘
  
  // Process Memory After Thread Creation:
  // ┌─ PROCESS MEMORY WITH MULTIPLE THREADS ──────────────────┐
  // │                                                         │
  // │ Virtual Address Space (Shared by all threads):         │
  // │ ┌─────────────────────────────────────────────────┐     │
  // │ │ 0xFFFFFFFF ┌─ Kernel Space ─────────────────┐   │     │
  // │ │            │ (OS code, drivers, syscalls)    │   │     │
  // │ │            └─────────────────────────────────┘   │     │
  // │ │ 0xC0000000 ┌─ Main Thread Stack ────────────┐   │     │
  // │ │            │ • main() local variables        │   │     │
  // │ │            │ • Function call stack           │   │     │
  // │ │            │ • Return addresses              │   │     │
  // │ │            │ (grows downward ↓)              │   │     │
  // │ │            └─────────────────────────────────┘   │     │
  // │ │ 0xB0000000 ┌─ Thread 1 Stack ───────────────┐   │     │
  // │ │            │ • Thread-specific locals        │   │     │
  // │ │            │ • Function parameters           │   │     │
  // │ │            │ • CPU register backup           │   │     │
  // │ │            └─────────────────────────────────┘   │     │
  // │ │ 0xA0000000 ┌─ Thread 2 Stack ───────────────┐   │     │
  // │ │            │ • Independent stack space       │   │     │
  // │ │            │ • Local variables isolated      │   │     │
  // │ │            └─────────────────────────────────┘   │     │
  // │ │ 0x80000000 ┌─ Heap (SHARED) ────────────────┐   │     │
  // │ │            │ • malloc/new allocations        │   │     │
  // │ │            │ • ALL THREADS CAN ACCESS        │   │     │
  // │ │            │ • Requires synchronization      │   │     │
  // │ │            └─────────────────────────────────┘   │     │
  // │ │ 0x40000000 ┌─ Data/BSS (SHARED) ────────────┐   │     │
  // │ │            │ • Global variables              │   │     │
  // │ │            │ • Static variables              │   │     │
  // │ │            │ • ALL THREADS CAN ACCESS        │   │     │
  // │ │            └─────────────────────────────────┘   │     │
  // │ │ 0x08000000 ┌─ Code Segment (SHARED) ────────┐   │     │
  // │ │            │ • Program instructions          │   │     │
  // │ │            │ • Function definitions          │   │     │
  // │ │            │ • ALL THREADS EXECUTE SAME CODE│   │     │
  // │ │            └─────────────────────────────────┘   │     │
  // │ │ 0x00000000                                      │     │
  // │ └─────────────────────────────────────────────────┘     │
  // └─────────────────────────────────────────────────────────┘
  
  std::cout << "\n--- Thread Creation Methods Demonstration ---" << std::endl;
  
  std::cout << "Main thread ID: " << std::this_thread::get_id() << std::endl;
  
  // Method 1: Function Pointer
  std::cout << "\n1. Function Pointer Method:" << std::endl;
  
  auto simple_function = []() {
    std::lock_guard<std::mutex> lock(console_mutex);
    std::cout << "   Function thread ID: " << std::this_thread::get_id() << std::endl;
    std::cout << "   This creates a NEW OS thread!" << std::endl;
  };
  
  std::thread t1(simple_function);  // ← THIS LINE CREATES THE THREAD
  t1.join();
  
  // Method 2: Lambda with Arguments
  std::cout << "2. Function with Arguments:" << std::endl;
  
  auto worker_with_args = [](int worker_id, const std::string& task) {
    std::lock_guard<std::mutex> lock(console_mutex);
    std::cout << "   Worker " << worker_id << " on thread " << std::this_thread::get_id() 
              << " executing: " << task << std::endl;
  };
  
  std::thread t2(worker_with_args, 42, "Processing data");  // ← Creates thread with args
  t2.join();
  
  // Method 3: Member Function
  std::cout << "3. Member Function Method:" << std::endl;
  
  class ThreadWorker {
  public:
    void do_work(int task_id) {
      std::lock_guard<std::mutex> lock(console_mutex);
      std::cout << "   Member function on thread " << std::this_thread::get_id() 
                << " handling task " << task_id << std::endl;
    }
  };
  
  ThreadWorker worker_obj;
  std::thread t3(&ThreadWorker::do_work, &worker_obj, 999);  // ← Creates thread for member function
  t3.join();
  
  // Demonstrate Thread Lifecycle
  std::cout << "\n--- Thread Lifecycle Demonstration ---" << std::endl;
  
  // Thread States Visualization:
  // ┌─ THREAD LIFECYCLE STATES ───────────────────────────────┐
  // │                                                         │
  // │ 1. NOT_STARTED                                          │
  // │    std::thread t; (default constructor)                │
  // │    ↓                                                    │
  // │ 2. CREATED → RUNNING                                    │
  // │    std::thread t(function); ← THREAD CREATED HERE      │
  // │    ↓                                                    │
  // │ 3. RUNNING ←→ BLOCKED                                   │
  // │    (function executing, may wait for I/O, mutexes)     │
  // │    ↓                                                    │
  // │ 4. FINISHED                                             │
  // │    (function completed, thread still joinable)         │
  // │    ↓                                                    │
  // │ 5. JOINED/DETACHED                                      │
  // │    t.join() or t.detach() ← CLEANUP HAPPENS HERE       │
  // │    ↓                                                    │
  // │ 6. DESTROYED                                            │
  // │    (thread object destroyed, resources freed)          │
  // │                                                         │
  // │ ⚠️  WARNING: Missing join()/detach() = std::terminate() │
  // └─────────────────────────────────────────────────────────┘
  
  {
    std::cout << "Creating thread..." << std::endl;
    
    std::thread lifecycle_thread([]{
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      std::lock_guard<std::mutex> lock(console_mutex);
      std::cout << "   Thread executing on ID: " << std::this_thread::get_id() << std::endl;
    });
    
    std::cout << "Thread created and running in parallel..." << std::endl;
    std::cout << "Thread is joinable: " << std::boolalpha << lifecycle_thread.joinable() << std::endl;
    
    lifecycle_thread.join();  // Wait for completion and cleanup
    std::cout << "Thread joined and cleaned up" << std::endl;
    std::cout << "Thread is joinable: " << std::boolalpha << lifecycle_thread.joinable() << std::endl;
  }
  
  // Performance Analysis
  std::cout << "\n--- Thread Creation Performance Analysis ---" << std::endl;
  
  const int num_test_threads = 10;
  auto start_time = std::chrono::high_resolution_clock::now();
  
  std::vector<std::thread> perf_threads;
  perf_threads.reserve(num_test_threads);
  
  // Measure thread creation time
  for (int i = 0; i < num_test_threads; ++i) {
    perf_threads.emplace_back([i]() {
      // Minimal work to avoid compiler optimizations
      volatile int x = i * 42;
      (void)x;
    });
  }
  
  auto creation_time = std::chrono::high_resolution_clock::now();
  
  // Join all threads
  for (auto& t : perf_threads) {
    t.join();
  }
  
  auto end_time = std::chrono::high_resolution_clock::now();
  
  auto creation_us = std::chrono::duration_cast<std::chrono::microseconds>(creation_time - start_time);
  auto total_us = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
  
  std::cout << "Created " << num_test_threads << " threads in " << creation_us.count() << " μs" << std::endl;
  std::cout << "Average creation time: " << (creation_us.count() / num_test_threads) << " μs per thread" << std::endl;
  std::cout << "Total lifecycle time: " << total_us.count() << " μs" << std::endl;
  
  // Thread vs Direct Function Call Overhead
  std::cout << "\n--- Thread Overhead vs Direct Function Calls ---" << std::endl;
  
  auto test_function = []() {
    volatile int result = 0;
    for (int i = 0; i < 1000; ++i) {
      result += i;
    }
    return result;
  };
  
  // Time direct function calls
  start_time = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < 50; ++i) {
    test_function();
  }
  auto function_time = std::chrono::high_resolution_clock::now() - start_time;
  
  // Time threaded function calls
  start_time = std::chrono::high_resolution_clock::now();
  std::vector<std::thread> overhead_threads;
  for (int i = 0; i < 50; ++i) {
    overhead_threads.emplace_back(test_function);
  }
  for (auto& t : overhead_threads) {
    t.join();
  }
  auto thread_time = std::chrono::high_resolution_clock::now() - start_time;
  
  auto function_us = std::chrono::duration_cast<std::chrono::microseconds>(function_time);
  auto thread_us = std::chrono::duration_cast<std::chrono::microseconds>(thread_time);
  
  std::cout << "50 direct function calls: " << function_us.count() << " μs" << std::endl;
  std::cout << "50 threaded calls: " << thread_us.count() << " μs" << std::endl;
  std::cout << "Thread overhead: " << (static_cast<double>(thread_us.count()) / function_us.count()) << "x slower" << std::endl;
  
  std::cout << "\n--- Key Thread Creation Insights ---" << std::endl;
  std::cout << "🧵 Thread creation involves expensive OS kernel calls" << std::endl;
  std::cout << "📚 Each thread gets its own stack (typically 1-8MB)" << std::endl;
  std::cout << "🔗 Threads share code, global data, and heap memory" << std::endl;
  std::cout << "⚡ std::thread constructor returns immediately (non-blocking)" << std::endl;
  std::cout << "⚠️  MUST call join() or detach() to avoid std::terminate()" << std::endl;
  std::cout << "🏊 Thread pools reuse threads to avoid creation overhead" << std::endl;
  std::cout << "🚫 Too many threads hurt performance due to context switching" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_thread_synchronization() {
  std::cout << "=== THREAD SYNCHRONIZATION MECHANISMS ===" << std::endl;
  
  // Thread Synchronization Problems:
  // ┌─ Race Condition Example ────────────────────────────────┐
  // │                                                         │
  // │ Shared Variable: int counter = 0;                      │
  // │                                                         │
  // │ Thread 1:              │ Thread 2:                     │
  // │ ┌────────────────────┐ │ ┌────────────────────────────┐ │
  // │ │ 1. Read counter    │ │ │ 1. Read counter            │ │
  // │ │    (gets 0)        │ │ │    (gets 0)                │ │
  // │ │ 2. Increment       │ │ │ 2. Increment               │ │
  // │ │    (0 + 1 = 1)     │ │ │    (0 + 1 = 1)             │ │
  // │ │ 3. Write back      │ │ │ 3. Write back              │ │
  // │ │    (counter = 1)   │ │ │    (counter = 1)           │ │
  // │ └────────────────────┘ │ └────────────────────────────┘ │
  // │                        │                               │
  // │ PROBLEM: Both threads read 0, both write 1             │
  // │ Expected: 2, Actual: 1 (Lost Update!)                  │
  // └─────────────────────────────────────────────────────────┘
  
  // ┌─ Mutex Solution ────────────────────────────────────────┐
  // │                                                         │
  // │ std::mutex counter_mutex;                               │
  // │                                                         │
  // │ Thread 1:              │ Thread 2:                     │
  // │ ┌────────────────────┐ │ ┌────────────────────────────┐ │
  // │ │ 1. Lock mutex      │ │ │ 1. Try lock mutex          │ │
  // │ │ 2. Read counter    │ │ │    (BLOCKED - waits)       │ │
  // │ │ 3. Increment       │ │ │                            │ │
  // │ │ 4. Write counter   │ │ │ 2. Lock acquired           │ │
  // │ │ 5. Unlock mutex    │ │ │ 3. Read counter            │ │
  // │ └────────────────────┘ │ │ 4. Increment               │ │
  // │                        │ │ 5. Write counter           │ │
  // │                        │ │ 6. Unlock mutex           │ │
  // │                        │ └────────────────────────────┘ │
  // │                                                         │
  // │ SOLUTION: Mutual exclusion ensures atomic operations    │
  // │ Result: counter = 2 (Correct!)                          │
  // └─────────────────────────────────────────────────────────┘
  
  std::cout << "\n--- Mutex Synchronization Example ---" << std::endl;
  
  int unsafe_counter = 0;
  int safe_counter = 0;
  std::mutex safe_mutex;
  
  // Unsafe increment function (race condition)
  auto unsafe_increment = [&unsafe_counter]() {
    for (int i = 0; i < 10000; ++i) {
      unsafe_counter++; // NOT THREAD-SAFE
    }
  };
  
  // Safe increment function (mutex protected)
  auto safe_increment = [&safe_counter, &safe_mutex]() {
    for (int i = 0; i < 10000; ++i) {
      std::lock_guard<std::mutex> lock(safe_mutex);
      safe_counter++; // THREAD-SAFE
    }
  };
  
  // Test unsafe version
  std::cout << "Testing unsafe increment..." << std::endl;
  std::vector<std::thread> unsafe_threads;
  
  for (int i = 0; i < 4; ++i) {
    unsafe_threads.emplace_back(unsafe_increment);
  }
  
  for (auto& thread : unsafe_threads) {
    thread.join();
  }
  
  std::cout << "Unsafe counter result: " << unsafe_counter << " (expected: 40000)" << std::endl;
  
  // Test safe version
  std::cout << "Testing safe increment..." << std::endl;
  std::vector<std::thread> safe_threads;
  
  for (int i = 0; i < 4; ++i) {
    safe_threads.emplace_back(safe_increment);
  }
  
  for (auto& thread : safe_threads) {
    thread.join();
  }
  
  std::cout << "Safe counter result: " << safe_counter << " (expected: 40000)" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_atomic_operations() {
  std::cout << "=== ATOMIC OPERATIONS ===" << std::endl;
  
  // Atomic Operations vs Mutex:
  // ┌─ Atomic Operations ─────────────────────────────────────┐
  // │                                                         │
  // │ Hardware-level atomic operations:                       │
  // │                                                         │
  // │ CPU Level:                                              │
  // │ ┌─────────────────────────────────────────────────┐     │
  // │ │ atomic_fetch_add():                             │     │
  // │ │ ┌─────────────────────────────────────────┐     │     │
  // │ │ │ 1. Lock memory bus                     │     │     │
  // │ │ │ 2. Read current value                  │     │     │
  // │ │ │ 3. Add increment                       │     │     │
  // │ │ │ 4. Write new value                     │     │     │
  // │ │ │ 5. Unlock memory bus                   │     │     │
  // │ │ └─────────────────────────────────────────┘     │     │
  // │ │ ALL IN SINGLE UNINTERRUPTIBLE INSTRUCTION      │     │
  // │ └─────────────────────────────────────────────────┘     │
  // │                                                         │
  // │ Advantages:                                             │
  // │ • No context switching overhead                         │
  // │ • Lock-free programming                                 │
  // │ • Better performance for simple operations              │
  // │ • No deadlock possibility                               │
  // │                                                         │
  // │ Limitations:                                            │
  // │ • Only for simple operations                            │
  // │ • Limited data types                                    │
  // │ • Complex for multi-step operations                     │
  // └─────────────────────────────────────────────────────────┘
  
  std::cout << "\n--- Atomic vs Mutex Performance Comparison ---" << std::endl;
  
  const int num_iterations = 1000000;
  const int num_threads = 4;
  
  // Atomic counter test
  std::atomic<int> atomic_counter{0};
  
  auto atomic_worker = [&atomic_counter, num_iterations]() {
    for (int i = 0; i < num_iterations; ++i) {
      atomic_counter.fetch_add(1);
    }
  };
  
  auto start_time = std::chrono::high_resolution_clock::now();
  
  std::vector<std::thread> atomic_threads;
  for (int i = 0; i < num_threads; ++i) {
    atomic_threads.emplace_back(atomic_worker);
  }
  
  for (auto& thread : atomic_threads) {
    thread.join();
  }
  
  auto end_time = std::chrono::high_resolution_clock::now();
  auto atomic_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
  
  // Mutex counter test
  int mutex_counter = 0;
  std::mutex counter_mutex;
  
  auto mutex_worker = [&mutex_counter, &counter_mutex, num_iterations]() {
    for (int i = 0; i < num_iterations; ++i) {
      std::lock_guard<std::mutex> lock(counter_mutex);
      mutex_counter++;
    }
  };
  
  start_time = std::chrono::high_resolution_clock::now();
  
  std::vector<std::thread> mutex_threads;
  for (int i = 0; i < num_threads; ++i) {
    mutex_threads.emplace_back(mutex_worker);
  }
  
  for (auto& thread : mutex_threads) {
    thread.join();
  }
  
  end_time = std::chrono::high_resolution_clock::now();
  auto mutex_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
  
  std::cout << "Atomic operations time: " << atomic_duration.count() << " ms" << std::endl;
  std::cout << "Mutex operations time: " << mutex_duration.count() << " ms" << std::endl;
  std::cout << "Atomic counter final value: " << atomic_counter.load() << std::endl;
  std::cout << "Mutex counter final value: " << mutex_counter << std::endl;
  std::cout << "Performance improvement: " << 
               (static_cast<double>(mutex_duration.count()) / atomic_duration.count()) 
               << "x faster" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_memory_sharing() {
  std::cout << "=== MEMORY SHARING: PROCESSES vs THREADS ===" << std::endl;
  
  // Memory Sharing Comparison:
  // ┌─ PROCESS MEMORY ISOLATION ──────────────────────────────┐
  // │                                                         │
  // │ Process A:                Process B:                    │
  // │ ┌─────────────────────┐   ┌─────────────────────────┐   │
  // │ │ Virtual Address 0x1000│ │ Virtual Address 0x1000  │   │
  // │ │ ┌─────────────────┐ │   │ ┌─────────────────────┐ │   │
  // │ │ │ int var = 42;   │ │   │ │ int var = 99;       │ │   │
  // │ │ └─────────────────┘ │   │ └─────────────────────┘ │   │
  // │ └─────────────────────┘   └─────────────────────────┘   │
  // │          │                          │                  │
  // │          ↓                          ↓                  │
  // │ ┌─────────────────────┐   ┌─────────────────────────┐   │
  // │ │ Physical Address    │   │ Physical Address        │   │
  // │ │ 0x8000 → [42]      │   │ 0x9000 → [99]          │   │
  // │ └─────────────────────┘   └─────────────────────────┘   │
  // │                                                         │
  // │ RESULT: Same virtual address maps to different         │
  // │         physical locations. Complete isolation!        │
  // └─────────────────────────────────────────────────────────┘
  
  // ┌─ THREAD MEMORY SHARING ─────────────────────────────────┐
  // │                                                         │
  // │ Thread 1:              Thread 2:                       │
  // │ ┌─────────────────────┐ ┌─────────────────────────────┐ │
  // │ │ Access global_var   │ │ Access global_var           │ │
  // │ │ Virtual Addr 0x2000 │ │ Virtual Addr 0x2000         │ │
  // │ └─────────────────────┘ └─────────────────────────────┘ │
  // │          │                          │                  │
  // │          └──────────┬───────────────┘                  │
  // │                     ↓                                  │
  // │          ┌─────────────────────────┐                   │
  // │          │ Physical Address        │                   │
  // │          │ 0xA000 → [shared_value] │                   │
  // │          └─────────────────────────┘                   │
  // │                                                         │
  // │ RESULT: Same virtual address maps to same physical     │
  // │         location. Direct sharing!                      │
  // └─────────────────────────────────────────────────────────┘
  
  std::cout << "\n--- Thread Memory Sharing Example ---" << std::endl;
  
  // Shared data structure
  struct SharedData {
    std::atomic<int> counter{0};
    std::vector<int> shared_vector;
    std::mutex vector_mutex;
  };
  
  SharedData shared_data;
  
  auto thread_worker = [&shared_data](int thread_id) {
    // Each thread can directly access shared data
    for (int i = 0; i < 100; ++i) {
      // Atomic increment
      int value = shared_data.counter.fetch_add(1);
      
      // Protected vector access
      {
        std::lock_guard<std::mutex> lock(shared_data.vector_mutex);
        shared_data.shared_vector.push_back(thread_id * 1000 + value);
      }
    }
    
    std::lock_guard<std::mutex> lock(console_mutex);
    std::cout << "Thread " << thread_id << " completed. Shared counter: " 
              << shared_data.counter.load() << std::endl;
  };
  
  // Create threads that share memory directly
  std::vector<std::thread> threads;
  for (int i = 0; i < 3; ++i) {
    threads.emplace_back(thread_worker, i);
  }
  
  // Wait for completion
  for (auto& thread : threads) {
    thread.join();
  }
  
  std::cout << "Final shared counter: " << shared_data.counter.load() << std::endl;
  std::cout << "Shared vector size: " << shared_data.shared_vector.size() << std::endl;
  
  // Process memory sharing (using shared memory)
  std::cout << "\n--- Process Shared Memory Example ---" << std::endl;
  
  // Process Shared Memory via mmap:
  // ┌─ Shared Memory Segment ─────────────────────────────────┐
  // │                                                         │
  // │ Process A:              │ Kernel │ Process B:            │
  // │ ┌─────────────────────┐ │ Memory │ ┌───────────────────┐ │
  // │ │ mmap() → 0x3000     │ │ Manager│ │ mmap() → 0x4000   │ │
  // │ │ ┌─────────────────┐ │ │        │ │ ┌───────────────┐ │ │
  // │ │ │ Mapped Memory   │ │ │        │ │ │ Mapped Memory │ │ │
  // │ │ │ [shared data]   │ │ │        │ │ │ [shared data] │ │ │
  // │ │ └─────────────────┘ │ │        │ │ └───────────────┘ │ │
  // │ └─────────────────────┘ │        │ └───────────────────┘ │
  // │          │              │        │           │           │
  // │          └──────────────┼────────┼───────────┘           │
  // │                         ↓        ↓                       │
  // │                ┌─────────────────────┐                   │
  // │                │ Physical Memory     │                   │
  // │                │ [actual shared data]│                   │
  // │                └─────────────────────┘                   │
  // └─────────────────────────────────────────────────────────┘
  
  // Create shared memory segment
  const char* shm_name = "/demo_shared_memory";
  const size_t shm_size = sizeof(int);
  
  // Create shared memory
  int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
  if (shm_fd != -1) {
    ftruncate(shm_fd, shm_size);
    
    // Map shared memory
    int* shared_int = static_cast<int*>(mmap(nullptr, shm_size, 
                                           PROT_READ | PROT_WRITE, 
                                           MAP_SHARED, shm_fd, 0));
    
    if (shared_int != MAP_FAILED) {
      *shared_int = 12345; // Write to shared memory
      
      std::cout << "Created shared memory segment with value: " << *shared_int << std::endl;
      std::cout << "This memory can be accessed by other processes using the same name" << std::endl;
      
      // Cleanup
      munmap(shared_int, shm_size);
      close(shm_fd);
      shm_unlink(shm_name);
    }
  }
  
  std::cout << std::endl;
}

void demonstrate_performance_comparison() {
  std::cout << "=== PERFORMANCE COMPARISON: PROCESSES vs THREADS ===" << std::endl;
  
  // Performance Characteristics:
  // ┌─ Creation Overhead Comparison ──────────────────────────┐
  // │                                                         │
  // │ PROCESS CREATION (fork()):                             │
  // │ ┌─────────────────────────────────────────────────┐     │
  // │ │ 1. Allocate new process control block (PCB)    │     │
  // │ │ 2. Create new virtual address space            │     │
  // │ │ 3. Copy page table entries (Copy-on-Write)     │     │
  // │ │ 4. Allocate new process ID (PID)               │     │
  // │ │ 5. Copy file descriptor table                  │     │
  // │ │ 6. Set up signal handlers                      │     │
  // │ │ 7. Initialize scheduling structures            │     │
  // │ └─────────────────────────────────────────────────┘     │
  // │ Time: ~1000-10000 microseconds                         │
  // │                                                         │
  // │ THREAD CREATION (std::thread):                         │
  // │ ┌─────────────────────────────────────────────────┐     │
  // │ │ 1. Allocate thread control block (TCB)         │     │
  // │ │ 2. Allocate stack space (typically 2MB)        │     │
  // │ │ 3. Set up thread-local storage                 │     │
  // │ │ 4. Register with scheduler                      │     │
  // │ └─────────────────────────────────────────────────┘     │
  // │ Time: ~10-100 microseconds                             │
  // │                                                         │
  // │ PERFORMANCE RATIO: Threads ~100x faster to create     │
  // └─────────────────────────────────────────────────────────┘
  
  // ┌─ Context Switching Overhead ────────────────────────────┐
  // │                                                         │
  // │ PROCESS CONTEXT SWITCH:                                 │
  // │ ┌─────────────────────────────────────────────────┐     │
  // │ │ 1. Save CPU registers                          │     │
  // │ │ 2. Save process state                          │     │
  // │ │ 3. Update process control block                │     │
  // │ │ 4. Switch virtual memory context (TLB flush)   │     │
  // │ │ 5. Load new process state                      │     │
  // │ │ 6. Restore CPU registers                       │     │
  // │ │ 7. Jump to new process                         │     │
  // │ └─────────────────────────────────────────────────┘     │
  // │ Time: ~1-10 microseconds                               │
  // │                                                         │
  // │ THREAD CONTEXT SWITCH (same process):                  │
  // │ ┌─────────────────────────────────────────────────┐     │
  // │ │ 1. Save CPU registers                          │     │
  // │ │ 2. Save thread state                           │     │
  // │ │ 3. Load new thread state                       │     │
  // │ │ 4. Restore CPU registers                       │     │
  // │ │ 5. Jump to new thread                          │     │
  // │ └─────────────────────────────────────────────────┘     │
  // │ Time: ~0.1-1 microseconds                              │
  // │                                                         │
  // │ PERFORMANCE RATIO: Thread switching ~10x faster       │
  // └─────────────────────────────────────────────────────────┘
  
  std::cout << "\n--- Creation Time Comparison ---" << std::endl;
  
  const int num_iterations = 100;
  
  // Measure thread creation time
  auto start_time = std::chrono::high_resolution_clock::now();
  
  for (int i = 0; i < num_iterations; ++i) {
    std::thread t([]() {
      // Minimal work
      volatile int x = 42;
      (void)x;
    });
    t.join();
  }
  
  auto end_time = std::chrono::high_resolution_clock::now();
  auto thread_duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
  
  std::cout << "Thread creation+join (" << num_iterations << " iterations): " 
            << thread_duration.count() << " microseconds" << std::endl;
  std::cout << "Average per thread: " << (thread_duration.count() / num_iterations) 
            << " microseconds" << std::endl;
  
  // Measure process creation time (smaller sample due to overhead)
  start_time = std::chrono::high_resolution_clock::now();
  
  const int process_iterations = 10; // Fewer iterations due to higher cost
  for (int i = 0; i < process_iterations; ++i) {
    pid_t pid = fork();
    if (pid == 0) {
      // Child process - minimal work
      exit(0);
    } else if (pid > 0) {
      // Parent process - wait for child
      int status;
      wait(&status);
    }
  }
  
  end_time = std::chrono::high_resolution_clock::now();
  auto process_duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
  
  std::cout << "Process creation+wait (" << process_iterations << " iterations): " 
            << process_duration.count() << " microseconds" << std::endl;
  std::cout << "Average per process: " << (process_duration.count() / process_iterations) 
            << " microseconds" << std::endl;
  
  double speedup = static_cast<double>(process_duration.count()) / process_iterations /
                   (static_cast<double>(thread_duration.count()) / num_iterations);
  
  std::cout << "Threads are approximately " << speedup << "x faster to create" << std::endl;
  
  std::cout << std::endl;
}

int main() {
  std::cout << "PROCESSES vs THREADS TUTORIAL" << std::endl;
  std::cout << "=============================" << std::endl << std::endl;
  
  demonstrate_process_vs_thread_overview();
  demonstrate_process_creation();
  demonstrate_thread_creation();
  demonstrate_thread_creation_deep_dive();
  demonstrate_thread_synchronization();
  demonstrate_atomic_operations();
  demonstrate_memory_sharing();
  demonstrate_performance_comparison();
  
  std::cout << "Processes vs Threads tutorial completed successfully!" << std::endl;
  std::cout << "\nKey Takeaways:" << std::endl;
  std::cout << "━━━━━━━━━━━━━━" << std::endl;
  std::cout << "PROCESSES:" << std::endl;
  std::cout << "✓ Strong isolation and fault tolerance" << std::endl;
  std::cout << "✓ Security through memory separation" << std::endl;
  std::cout << "✗ Higher overhead for creation and communication" << std::endl;
  std::cout << "✗ Complex inter-process communication" << std::endl;
  
  std::cout << "\nTHREADS:" << std::endl;
  std::cout << "✓ Fast creation and context switching" << std::endl;
  std::cout << "✓ Direct memory sharing and communication" << std::endl;
  std::cout << "✗ No fault isolation (shared crash risk)" << std::endl;
  std::cout << "✗ Requires careful synchronization" << std::endl;
  
  std::cout << "\nChoose based on your requirements:" << std::endl;
  std::cout << "• High performance + shared data → Threads" << std::endl;
  std::cout << "• Fault tolerance + security → Processes" << std::endl;
  std::cout << "• CPU-intensive parallel tasks → Threads" << std::endl;
  std::cout << "• Independent services → Processes" << std::endl;
  
  return 0;
}