/**
 * @file locking_mechanisms_comparison.cpp
 * @author GitHub Copilot
 * @brief Demonstrates differences between lock_guard/notify vs manual lock/unlock
 * 
 * This file provides comprehensive comparisons between different locking mechanisms:
 * 
 * RAII vs Manual Locking:
 * ┌─ RESOURCE ACQUISITION IS INITIALIZATION (RAII) ───────┐
 * │                                                        │
 * │ Concept: Tie resource lifetime to object lifetime      │
 * │                                                        │
 * │ Object Lifecycle:                                      │
 * │ ┌────────────────────────────────────────────────┐     │
 * │ │ 1. Object constructed  → Resource acquired     │     │
 * │ │ 2. Object in scope     → Resource in use       │     │
 * │ │ 3. Object destroyed    → Resource released     │     │
 * │ │    (automatically!)                            │     │
 * │ └────────────────────────────────────────────────┘     │
 * │                                                        │
 * │ Benefits:                                              │
 * │ • ✅ Exception safe (destructor always called)        │
 * │ • ✅ No resource leaks                                 │
 * │ • ✅ Clear scope-based semantics                       │
 * │ • ✅ Compiler enforced cleanup                         │
 * │                                                        │
 * │ Example: std::lock_guard follows RAII pattern          │
 * └────────────────────────────────────────────────────────┘
 * 
 * Manual Resource Management:
 * ┌─ MANUAL RESOURCE MANAGEMENT ───────────────────────────┐
 * │                                                        │
 * │ Concept: Programmer explicitly manages resources       │
 * │                                                        │
 * │ Resource Lifecycle:                                    │
 * │ ┌────────────────────────────────────────────────┐     │
 * │ │ 1. acquire_resource()  → Must remember to call │     │
 * │ │ 2. use_resource()      → Hope nothing goes wrong│     │
 * │ │ 3. release_resource()  → Must remember to call │     │
 * │ │    (manually!)                                 │     │
 * │ └────────────────────────────────────────────────┘     │
 * │                                                        │
 * │ Problems:                                              │
 * │ • ❌ Forget to release → Resource leak                 │
 * │ • ❌ Exception thrown → Resource never released        │
 * │ • ❌ Early return → Resource never released            │
 * │ • ❌ Complex control flow → Easy to miss release       │
 * │                                                        │
 * │ Example: mutex.lock() / mutex.unlock() pattern         │
 * └────────────────────────────────────────────────────────┘
 */

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <exception>
#include <thread>
#include <atomic>
#include <memory>

// ANSI Color codes
namespace Colors {
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string BOLD = "\033[1m";
}

// =============================================================================
// DEMONSTRATION 1: Manual lock/unlock vs lock_guard
// =============================================================================

// Manual lock/unlock Flow (DANGEROUS):
// ┌─ MANUAL LOCK/UNLOCK FLOW ───────────────────────────────┐
// │                                                         │
// │ Thread Execution:                                       │
// │ ┌─────────────────────────────────────────────────┐     │
// │ │ 1. mutex.lock()     ← Acquire lock manually    │     │
// │ │ 2. Critical section code                       │     │
// │ │ 3. if (error) return; ← EXCEPTION/EARLY EXIT   │     │
// │ │    ↓                                            │     │
// │ │    💥 DEADLOCK: mutex.unlock() never called!   │     │
// │ │ 4. mutex.unlock()   ← This line never reached  │     │
// │ └─────────────────────────────────────────────────┘     │
// │                                                         │
// │ Memory State:                                           │
// │ ┌─────────────────────────────────────────────────┐     │
// │ │ Mutex: LOCKED                                   │     │
// │ │ Owner: Thread that exited early                 │     │
// │ │ Other threads: BLOCKED FOREVER                  │     │
// │ │ Result: 💀 DEADLOCK                             │     │
// │ └─────────────────────────────────────────────────┘     │
// └─────────────────────────────────────────────────────────┘

// RAII lock_guard Flow (SAFE):
// ┌─ RAII LOCK_GUARD FLOW ──────────────────────────────────┐
// │                                                         │
// │ Thread Execution:                                       │
// │ ┌─────────────────────────────────────────────────┐     │
// │ │ {                                               │     │
// │ │   std::lock_guard<std::mutex> lock(mutex);     │     │
// │ │   ↓                                             │     │
// │ │   Constructor automatically calls mutex.lock() │     │
// │ │                                                 │     │
// │ │   Critical section code                        │     │
// │ │                                                 │     │
// │ │   if (error) return; ← EXCEPTION/EARLY EXIT    │     │
// │ │   ↓                                             │     │
// │ │   ✅ Destructor automatically calls unlock()   │     │
// │ │ }                                               │     │
// │ └─────────────────────────────────────────────────┘     │
// │                                                         │
// │ Stack Frame Destruction:                                │
// │ ┌─────────────────────────────────────────────────┐     │
// │ │ 1. Exception thrown or early return             │     │
// │ │ 2. Stack unwinding begins                       │     │
// │ │ 3. lock_guard destructor called automatically   │     │
// │ │ 4. mutex.unlock() called in destructor          │     │
// │ │ 5. ✅ Mutex released, other threads can proceed │     │
// │ └─────────────────────────────────────────────────┘     │
// └─────────────────────────────────────────────────────────┘

namespace Demo1 {
    std::mutex console_mutex;
    int shared_counter = 0;
    std::mutex counter_mutex;
    
    void print_header() {
        std::cout << Colors::BOLD << Colors::BLUE << "\n🔒 DEMO 1: Manual lock/unlock vs lock_guard" << Colors::RESET << std::endl;
        std::cout << std::string(60, '=') << std::endl;
    }
    
    // PROBLEMATIC: Manual lock/unlock approach
    void manual_lock_unlock_problematic(int thread_id) {
        try {
            counter_mutex.lock(); // Manual lock
            
            // Simulate some work
            shared_counter++;
            
            // Simulate potential error condition
            if (shared_counter > 50 && thread_id == 2) {
                throw std::runtime_error("Simulated error in thread 2");
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
            counter_mutex.unlock(); // Manual unlock
            
            std::lock_guard<std::mutex> console_lock(console_mutex);
            std::cout << Colors::GREEN << "Thread " << thread_id << " completed successfully" << Colors::RESET << std::endl;
            
        } catch (const std::exception& e) {
            // 💥 BUG: Exception thrown, mutex never unlocked!
            // counter_mutex.unlock(); // This line never executes
            std::lock_guard<std::mutex> console_lock(console_mutex);
            std::cout << Colors::RED << "Thread " << thread_id << " threw exception: " << e.what() << Colors::RESET << std::endl;
            std::cout << Colors::RED << "💥 DEADLOCK: Mutex never unlocked!" << Colors::RESET << std::endl;
        }
    }
    
    // SAFE: lock_guard approach (RAII)
    void lock_guard_safe(int thread_id) {
        try {
            std::lock_guard<std::mutex> lock(counter_mutex); // RAII lock
            
            // Simulate some work
            shared_counter++;
            
            // Simulate potential error condition
            if (shared_counter > 100 && thread_id == 2) {
                throw std::runtime_error("Simulated error in thread 2");
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
            // ✅ Automatic unlock when lock goes out of scope
            
            std::lock_guard<std::mutex> console_lock(console_mutex);
            std::cout << Colors::GREEN << "Thread " << thread_id << " completed successfully" << Colors::RESET << std::endl;
            
        } catch (const std::exception& e) {
            // ✅ SAFE: lock_guard destructor automatically unlocks
            std::lock_guard<std::mutex> console_lock(console_mutex);
            std::cout << Colors::YELLOW << "Thread " << thread_id << " threw exception: " << e.what() << Colors::RESET << std::endl;
            std::cout << Colors::GREEN << "✅ SAFE: Mutex automatically unlocked by RAII" << Colors::RESET << std::endl;
        }
    }
    
    void demonstrate() {
        print_header();
        
        std::cout << Colors::CYAN << "\nPart A: Manual lock/unlock (DANGEROUS)" << Colors::RESET << std::endl;
        std::cout << "This will likely cause a deadlock..." << std::endl;
        
        shared_counter = 0;
        std::vector<std::thread> manual_threads;
        
        // NOTE: This might deadlock, so we'll use a timeout mechanism
        auto start_time = std::chrono::steady_clock::now();
        
        for (int i = 1; i <= 3; ++i) {
            manual_threads.emplace_back(manual_lock_unlock_problematic, i);
        }
        
        // Try to join with timeout
        bool all_joined = true;
        for (auto& t : manual_threads) {
            if (t.joinable()) {
                // In real code, you'd use a proper timeout mechanism
                t.join();
            }
        }
        
        std::cout << Colors::CYAN << "\nPart B: lock_guard (SAFE)" << Colors::RESET << std::endl;
        
        shared_counter = 50; // Start higher to trigger exception
        std::vector<std::thread> safe_threads;
        
        for (int i = 1; i <= 3; ++i) {
            safe_threads.emplace_back(lock_guard_safe, i);
        }
        
        for (auto& t : safe_threads) {
            t.join();
        }
        
        std::cout << Colors::GREEN << "All threads completed safely with RAII!" << Colors::RESET << std::endl;
    }
}

// =============================================================================
// DEMONSTRATION 2: Condition Variables vs Manual Polling
// =============================================================================

// Polling Approach (INEFFICIENT):
// ┌─ POLLING MECHANISM ─────────────────────────────────────┐
// │                                                         │
// │ Thread Timeline:                                        │
// │ T1: Producer │─wait─│─wait─│─wait─│─produce─│─notify─│   │
// │ T2: Consumer │─check│─check│─check│─wait────│─consume│   │
// │              └─ CPU ┘─ CPU ┘─ CPU ┘         └─ work ┘   │
// │                 ↑       ↑       ↑                       │
// │              Wasted   Wasted  Wasted                    │
// │              cycles   cycles  cycles                    │
// │                                                         │
// │ CPU Usage Pattern:                                      │
// │ ████▒▒▒▒████▒▒▒▒████▒▒▒▒████████                       │
// │ ↑busy  idle↑busy  idle↑busy  idle↑actual work           │
// │                                                         │
// │ Problems:                                               │
// │ • 🔥 High CPU usage from busy waiting                   │
// │ • ⏰ Poor responsiveness (check interval delay)         │
// │ • 🔋 Battery drain on mobile devices                    │
// │ • 🎯 Race conditions possible between check and wait    │
// └─────────────────────────────────────────────────────────┘

// Condition Variable Approach (EFFICIENT):
// ┌─ CONDITION VARIABLE MECHANISM ─────────────────────────┐
// │                                                        │
// │ Thread Timeline:                                       │
// │ T1: Producer │─────────────────│─produce─│─notify─│    │
// │ T2: Consumer │─wait(blocked)───│─wakeup──│─consume│    │
// │              └─ SLEEPING ──────┘         └─ work ┘    │
// │                     ↑                                  │
// │                 Zero CPU                               │
// │                 usage!                                 │
// │                                                        │
// │ CPU Usage Pattern:                                     │
// │ ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████████                          │
// │ ↑──── idle (sleeping) ────↑actual work                 │
// │                                                        │
// │ Memory State During wait():                            │
// │ ┌─────────────────────────────────────────┐            │
// │ │ Thread State: BLOCKED                   │            │
// │ │ Mutex: UNLOCKED (released during wait)  │            │
// │ │ Condition: Checked atomically on wakeup │            │
// │ │ CPU Scheduler: Thread not in run queue  │            │
// │ └─────────────────────────────────────────┘            │
// │                                                        │
// │ Benefits:                                              │
// │ • ⚡ Zero CPU usage while waiting                      │
// │ • 🚀 Instant notification (no polling delay)          │
// │ • 🔐 Atomic condition checking prevents race conditions│
// │ • 🔋 Power efficient for mobile/embedded systems      │
// └────────────────────────────────────────────────────────┘

namespace Demo2 {
    std::mutex console_mutex;
    std::mutex data_mutex;
    std::condition_variable data_cv;
    bool data_ready = false;
    int shared_data = 0;
    
    void print_header() {
        std::cout << Colors::BOLD << Colors::BLUE << "\n🔔 DEMO 2: Condition Variables vs Manual Polling" << Colors::RESET << std::endl;
        std::cout << std::string(60, '=') << std::endl;
    }
    
    // INEFFICIENT: Manual polling approach
    void manual_polling_consumer(int consumer_id) {
        auto start_time = std::chrono::steady_clock::now();
        
        while (true) {
            {
                std::lock_guard<std::mutex> lock(data_mutex);
                if (data_ready) {
                    auto end_time = std::chrono::steady_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
                    
                    std::lock_guard<std::mutex> console_lock(console_mutex);
                    std::cout << Colors::YELLOW << "Consumer " << consumer_id 
                              << " found data: " << shared_data 
                              << " (waited " << duration.count() << "ms, polling)" << Colors::RESET << std::endl;
                    return;
                }
            }
            
            // 💭 Problem: Wastes CPU cycles polling
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Busy waiting
        }
    }
    
    // EFFICIENT: Condition variable approach
    void cv_consumer(int consumer_id) {
        auto start_time = std::chrono::steady_clock::now();
        
        std::unique_lock<std::mutex> lock(data_mutex);
        data_cv.wait(lock, []{ return data_ready; }); // Efficient waiting
        
        auto end_time = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        
        {
            std::lock_guard<std::mutex> console_lock(console_mutex);
            std::cout << Colors::GREEN << "Consumer " << consumer_id 
                      << " found data: " << shared_data 
                      << " (waited " << duration.count() << "ms, condition variable)" << Colors::RESET << std::endl;
        }
    }
    
    void producer_manual() {
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
        
        {
            std::lock_guard<std::mutex> lock(data_mutex);
            shared_data = 42;
            data_ready = true;
        }
        
        {
            std::lock_guard<std::mutex> console_lock(console_mutex);
            std::cout << Colors::CYAN << "Producer: Data ready (manual polling test)" << Colors::RESET << std::endl;
        }
    }
    
    void producer_cv() {
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
        
        {
            std::lock_guard<std::mutex> lock(data_mutex);
            shared_data = 99;
            data_ready = true;
        }
        data_cv.notify_all(); // Efficient notification
        
        {
            std::lock_guard<std::mutex> console_lock(console_mutex);
            std::cout << Colors::CYAN << "Producer: Data ready + notified consumers" << Colors::RESET << std::endl;
        }
    }
    
    void demonstrate() {
        print_header();
        
        std::cout << Colors::CYAN << "\nPart A: Manual Polling (INEFFICIENT)" << Colors::RESET << std::endl;
        
        // Reset state
        data_ready = false;
        shared_data = 0;
        
        std::vector<std::thread> polling_threads;
        
        // Start consumers first
        for (int i = 1; i <= 2; ++i) {
            polling_threads.emplace_back(manual_polling_consumer, i);
        }
        
        // Start producer
        polling_threads.emplace_back(producer_manual);
        
        // Wait for completion
        for (auto& t : polling_threads) {
            t.join();
        }
        
        std::cout << Colors::CYAN << "\nPart B: Condition Variables (EFFICIENT)" << Colors::RESET << std::endl;
        
        // Reset state
        data_ready = false;
        shared_data = 0;
        
        std::vector<std::thread> cv_threads;
        
        // Start consumers first
        for (int i = 1; i <= 2; ++i) {
            cv_threads.emplace_back(cv_consumer, i);
        }
        
        // Start producer
        cv_threads.emplace_back(producer_cv);
        
        // Wait for completion
        for (auto& t : cv_threads) {
            t.join();
        }
    }
}

// =============================================================================
// DEMONSTRATION 3: Exception Safety
// =============================================================================

// Exception Safety Comparison:
// ┌─ MANUAL LOCKING WITH EXCEPTIONS ───────────────────────┐
// │                                                        │
// │ Stack Frame During Exception:                          │
// │ ┌────────────────────────────────────────────────┐     │
// │ │ function_scope() {                             │     │
// │ │   mutex.lock();           ← Lock acquired      │     │
// │ │   ┌─────────────────────────────────────┐      │     │
// │ │   │ risky_operation();    ← Exception!  │      │     │
// │ │   │ throw std::exception("error");      │      │     │
// │ │   └─────────────────────────────────────┘      │     │
// │ │   mutex.unlock();         ← NEVER REACHED!     │     │
// │ │ }                                              │     │
// │ └────────────────────────────────────────────────┘     │
// │                                                        │
// │ Exception Propagation:                                 │
// │ ┌────────────────────────────────────────────────┐     │
// │ │ Exception thrown  →  Stack unwinding begins    │     │
// │ │        ↓                        ↓              │     │
// │ │ Function exits    →  Local variables destroyed │     │
// │ │        ↓                        ↓              │     │
// │ │ 💀 DISASTER: Mutex still locked, never unlocked│     │
// │ │              Other threads deadlocked forever  │     │
// │ └────────────────────────────────────────────────┘     │
// └────────────────────────────────────────────────────────┘

// RAII Exception Safety:
// ┌─ RAII LOCKING WITH EXCEPTIONS ─────────────────────────┐
// │                                                        │
// │ Stack Frame During Exception:                          │
// │ ┌────────────────────────────────────────────────┐     │
// │ │ function_scope() {                             │     │
// │ │   std::lock_guard<std::mutex> guard(mutex);    │     │
// │ │   ↓                                            │     │
// │ │   Constructor: mutex.lock()   ← Lock acquired  │     │
// │ │   ┌─────────────────────────────────────┐      │     │
// │ │   │ risky_operation();    ← Exception!  │      │     │
// │ │   │ throw std::exception("error");      │      │     │
// │ │   └─────────────────────────────────────┘      │     │
// │ │ } ← Function scope ends                        │     │
// │ └────────────────────────────────────────────────┘     │
// │                                                        │
// │ Exception with RAII Cleanup:                           │
// │ ┌────────────────────────────────────────────────┐     │
// │ │ Exception thrown  →  Stack unwinding begins    │     │
// │ │        ↓                        ↓              │     │
// │ │ Function exits    →  Local variables destroyed │     │
// │ │        ↓                        ↓              │     │
// │ │ guard destructor  →  mutex.unlock() called     │     │
// │ │        ↓                        ↓              │     │
// │ │ ✅ SAFE: Mutex unlocked, other threads continue│     │
// │ └────────────────────────────────────────────────┘     │
// └────────────────────────────────────────────────────────┘

namespace Demo3 {
    std::mutex console_mutex;
    std::mutex resource_mutex;
    
    void print_header() {
        std::cout << Colors::BOLD << Colors::BLUE << "\n🔐 DEMO 3: Different Lock Types" << Colors::RESET << std::endl;
        std::cout << std::string(60, '=') << std::endl;
    }
    
    void demonstrate_lock_guard() {
        std::cout << Colors::YELLOW << "\n--- std::lock_guard Demo ---" << Colors::RESET << std::endl;
        
        {
            std::lock_guard<std::mutex> lock(resource_mutex);
            std::cout << "lock_guard: Automatic lock on construction" << std::endl;
            std::cout << "lock_guard: Cannot manually unlock" << std::endl;
            std::cout << "lock_guard: RAII - automatic cleanup" << std::endl;
            // Automatic unlock when 'lock' goes out of scope
        }
        std::cout << "lock_guard: Mutex unlocked automatically" << std::endl;
    }
    
    void demonstrate_unique_lock() {
        std::cout << Colors::YELLOW << "\n--- std::unique_lock Demo ---" << Colors::RESET << std::endl;
        
        std::unique_lock<std::mutex> lock(resource_mutex);
        std::cout << "unique_lock: Can lock/unlock manually" << std::endl;
        
        lock.unlock();
        std::cout << "unique_lock: Manually unlocked" << std::endl;
        
        lock.lock();
        std::cout << "unique_lock: Manually locked again" << std::endl;
        
        // Can transfer ownership
        std::unique_lock<std::mutex> another_lock = std::move(lock);
        std::cout << "unique_lock: Ownership transferred" << std::endl;
        
        // Automatically unlocked when another_lock goes out of scope
    }
    
    void demonstrate_manual_lock() {
        std::cout << Colors::YELLOW << "\n--- Manual lock/unlock Demo ---" << Colors::RESET << std::endl;
        
        resource_mutex.lock();
        std::cout << "Manual: Explicitly locked" << std::endl;
        std::cout << "Manual: Must remember to unlock" << std::endl;
        std::cout << "Manual: Risk of forgetting unlock" << std::endl;
        resource_mutex.unlock();
        std::cout << "Manual: Explicitly unlocked" << std::endl;
    }
    
    void demonstrate() {
        print_header();
        
        demonstrate_lock_guard();
        demonstrate_unique_lock();
        demonstrate_manual_lock();
    }
}

// =============================================================================
// PERFORMANCE COMPARISON
// =============================================================================

// Performance Characteristics Analysis:
// ┌─ LOCK ACQUISITION OVERHEAD ────────────────────────────┐
// │                                                        │
// │ Manual lock/unlock Assembly (simplified):              │
// │ ┌────────────────────────────────────────────────┐     │
// │ │ call    mutex.lock()      ; System call        │     │
// │ │ ; ... critical section code ...                │     │
// │ │ call    mutex.unlock()    ; System call        │     │
// │ │                                                │     │
// │ │ Instructions: ~10-50 (depending on contention) │     │
// │ │ Syscalls: 2 (lock + unlock)                   │     │
// │ └────────────────────────────────────────────────┘     │
// │                                                        │
// │ RAII lock_guard Assembly (simplified):                 │
// │ ┌────────────────────────────────────────────────┐     │
// │ │ ; Constructor inlined:                         │     │
// │ │ call    mutex.lock()      ; System call        │     │
// │ │ ; ... critical section code ...                │     │
// │ │ ; Destructor inlined:                          │     │
// │ │ call    mutex.unlock()    ; System call        │     │
// │ │                                                │     │
// │ │ Instructions: ~10-50 (identical!)             │     │
// │ │ Syscalls: 2 (lock + unlock)                   │     │
// │ │ Overhead: ZERO (constructor/destructor inlined)│     │
// │ └────────────────────────────────────────────────┘     │
// │                                                        │
// │ Compiler Optimization Result:                          │
// │ ┌────────────────────────────────────────────────┐     │
// │ │ Manual:     mov rdi, mutex_addr                │     │
// │ │             call _ZNSt5mutex4lockEv            │     │
// │ │             ; critical section                 │     │
// │ │             mov rdi, mutex_addr                │     │
// │ │             call _ZNSt5mutex6unlockEv          │     │
// │ │                                                │     │
// │ │ RAII:       mov rdi, mutex_addr                │     │
// │ │             call _ZNSt5mutex4lockEv            │     │
// │ │             ; critical section                 │     │
// │ │             mov rdi, mutex_addr                │     │
// │ │             call _ZNSt5mutex6unlockEv          │     │
// │ │                                                │     │
// │ │ Result: IDENTICAL ASSEMBLY CODE!               │     │
// │ └────────────────────────────────────────────────┘     │
// └────────────────────────────────────────────────────────┘

// Performance Timing Visualization:
// ┌─ OPERATION TIMELINE ───────────────────────────────────┐
// │                                                        │
// │ Manual Approach (10 million operations):               │
// │ ████████████████████████████████                      │
// │ ↑─── Lock/Unlock Time ───↑─ Work ─↑                   │
// │                                                        │
// │ RAII Approach (10 million operations):                 │
// │ ████████████████████████████████                      │
// │ ↑─── Lock/Unlock Time ───↑─ Work ─↑                   │
// │                                                        │
// │ Difference: None (both compile to same code)           │
// │                                                        │
// │ Cache Performance:                                     │
// │ ┌────────────────────────────────────────────────┐     │
// │ │ RAII objects are stack-allocated               │     │
// │ │ ↓                                              │     │
// │ │ Better cache locality                          │     │
// │ │ ↓                                              │     │
// │ │ Potential slight performance improvement       │     │
// │ └────────────────────────────────────────────────┘     │
// └────────────────────────────────────────────────────────┘

namespace PerformanceDemo {
    std::mutex test_mutex;
    volatile int counter = 0;
    
    void print_header() {
        std::cout << Colors::BOLD << Colors::BLUE << "\n⚡ PERFORMANCE COMPARISON" << Colors::RESET << std::endl;
        std::cout << std::string(60, '=') << std::endl;
    }
    
    void test_manual_lock(int iterations) {
        for (int i = 0; i < iterations; ++i) {
            test_mutex.lock();
            counter++;
            test_mutex.unlock();
        }
    }
    
    void test_lock_guard(int iterations) {
        for (int i = 0; i < iterations; ++i) {
            std::lock_guard<std::mutex> lock(test_mutex);
            counter++;
        }
    }
    
    void test_unique_lock(int iterations) {
        for (int i = 0; i < iterations; ++i) {
            std::unique_lock<std::mutex> lock(test_mutex);
            counter++;
        }
    }
    
    void demonstrate() {
        print_header();
        
        const int iterations = 100000;
        
        // Test manual lock/unlock
        counter = 0;
        auto start = std::chrono::high_resolution_clock::now();
        test_manual_lock(iterations);
        auto manual_time = std::chrono::high_resolution_clock::now() - start;
        
        // Test lock_guard
        counter = 0;
        start = std::chrono::high_resolution_clock::now();
        test_lock_guard(iterations);
        auto lock_guard_time = std::chrono::high_resolution_clock::now() - start;
        
        // Test unique_lock
        counter = 0;
        start = std::chrono::high_resolution_clock::now();
        test_unique_lock(iterations);
        auto unique_lock_time = std::chrono::high_resolution_clock::now() - start;
        
        auto manual_ms = std::chrono::duration_cast<std::chrono::microseconds>(manual_time);
        auto lock_guard_ms = std::chrono::duration_cast<std::chrono::microseconds>(lock_guard_time);
        auto unique_lock_ms = std::chrono::duration_cast<std::chrono::microseconds>(unique_lock_time);
        
        std::cout << Colors::GREEN << "Performance Results (" << iterations << " iterations):" << Colors::RESET << std::endl;
        std::cout << "Manual lock/unlock: " << manual_ms.count() << " μs" << std::endl;
        std::cout << "std::lock_guard:    " << lock_guard_ms.count() << " μs" << std::endl;
        std::cout << "std::unique_lock:   " << unique_lock_ms.count() << " μs" << std::endl;
        
        std::cout << Colors::YELLOW << "\nKey Insight: Performance is nearly identical!" << Colors::RESET << std::endl;
        std::cout << Colors::GREEN << "RAII has no performance penalty - always use it!" << Colors::RESET << std::endl;
    }
}

// =============================================================================
// MAIN FUNCTION
// =============================================================================

int main() {
    std::cout << Colors::BOLD << Colors::CYAN << "🔒 C++ Locking Mechanisms Comparison" << Colors::RESET << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    Demo1::demonstrate();
    Demo2::demonstrate();
    Demo3::demonstrate();
    PerformanceDemo::demonstrate();
    
    std::cout << Colors::BOLD << Colors::GREEN << "\n📋 Summary: When to Use What?" << Colors::RESET << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << Colors::CYAN << "🔒 std::lock_guard:" << Colors::RESET << std::endl;
    std::cout << "  ✅ Use for simple critical sections" << std::endl;
    std::cout << "  ✅ Automatic lock/unlock (RAII)" << std::endl;
    std::cout << "  ✅ Exception safe" << std::endl;
    std::cout << "  ✅ Cannot forget to unlock" << std::endl;
    std::cout << "  ❌ Cannot manually unlock before scope ends" << std::endl;
    
    std::cout << Colors::CYAN << "\n🔓 std::unique_lock:" << Colors::RESET << std::endl;
    std::cout << "  ✅ Can manually lock/unlock" << std::endl;
    std::cout << "  ✅ Works with condition variables" << std::endl;
    std::cout << "  ✅ Can transfer ownership" << std::endl;
    std::cout << "  ✅ More flexible than lock_guard" << std::endl;
    std::cout << "  ❌ Slightly more overhead" << std::endl;
    
    std::cout << Colors::CYAN << "\n🚫 Manual lock/unlock:" << Colors::RESET << std::endl;
    std::cout << "  ❌ Error prone" << std::endl;
    std::cout << "  ❌ Not exception safe" << std::endl;
    std::cout << "  ❌ Can cause deadlocks" << std::endl;
    std::cout << "  ❌ Easy to forget unlock" << std::endl;
    std::cout << "  ✅ No overhead (but not worth the risk)" << std::endl;
    
    std::cout << Colors::CYAN << "\n🔔 Condition Variables:" << Colors::RESET << std::endl;
    std::cout << "  ✅ Efficient thread coordination" << std::endl;
    std::cout << "  ✅ No CPU wasting (vs polling)" << std::endl;
    std::cout << "  ✅ notify_one() / notify_all()" << std::endl;
    std::cout << "  ✅ Use with std::unique_lock" << std::endl;
    
    std::cout << Colors::BOLD << Colors::GREEN << "\n🎯 Best Practice: Always use RAII!" << Colors::RESET << std::endl;
    std::cout << "• std::lock_guard for simple cases" << std::endl;
    std::cout << "• std::unique_lock for condition variables" << std::endl;
    std::cout << "• Never use manual lock/unlock" << std::endl;

    // ==========================================================================
    // COMPREHENSIVE CONCEPTS EXPLANATION WITH ASCII DIAGRAMS
    // ==========================================================================
    
    std::cout << Colors::BOLD << Colors::MAGENTA << "\n📚 COMPREHENSIVE CONCEPTS SUMMARY" << Colors::RESET << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    // Mutex Concepts
    std::cout << Colors::BOLD << Colors::BLUE << "\n🔒 MUTEX (Mutual Exclusion) CONCEPTS:" << Colors::RESET << std::endl;
    std::cout << "┌─ MUTEX STATES ─────────────────────────────────────────┐" << std::endl;
    std::cout << "│                                                        │" << std::endl;
    std::cout << "│ UNLOCKED STATE:                                        │" << std::endl;
    std::cout << "│ ┌────────────────────────────────────────────────┐     │" << std::endl;
    std::cout << "│ │ Mutex: Available                               │     │" << std::endl;
    std::cout << "│ │ Owner: None                                    │     │" << std::endl;
    std::cout << "│ │ Waiting: No threads blocked                   │     │" << std::endl;
    std::cout << "│ │ Next acquire: Will succeed immediately        │     │" << std::endl;
    std::cout << "│ └────────────────────────────────────────────────┘     │" << std::endl;
    std::cout << "│                           ↓ lock()                     │" << std::endl;
    std::cout << "│ LOCKED STATE:                                          │" << std::endl;
    std::cout << "│ ┌────────────────────────────────────────────────┐     │" << std::endl;
    std::cout << "│ │ Mutex: Owned                                   │     │" << std::endl;
    std::cout << "│ │ Owner: Thread ID (e.g., Thread-1)             │     │" << std::endl;
    std::cout << "│ │ Waiting: Other threads blocked in queue       │     │" << std::endl;
    std::cout << "│ │ Next acquire: Will block until unlock()       │     │" << std::endl;
    std::cout << "│ └────────────────────────────────────────────────┘     │" << std::endl;
    std::cout << "└────────────────────────────────────────────────────────┘" << std::endl;
    
    // RAII Concepts
    std::cout << Colors::BOLD << Colors::GREEN << "\n⚡ RAII (Resource Acquisition Is Initialization):" << Colors::RESET << std::endl;
    std::cout << "┌─ RAII PRINCIPLES ──────────────────────────────────────┐" << std::endl;
    std::cout << "│                                                        │" << std::endl;
    std::cout << "│ Stack-Based Resource Management:                       │" << std::endl;
    std::cout << "│ ┌────────────────────────────────────────────────┐     │" << std::endl;
    std::cout << "│ │ function() {                                   │     │" << std::endl;
    std::cout << "│ │   std::lock_guard<std::mutex> lock(mutex);     │ ←─┐ │" << std::endl;
    std::cout << "│ │   ↑                                            │   │ │" << std::endl;
    std::cout << "│ │   Constructor automatically acquires resource │   │ │" << std::endl;
    std::cout << "│ │                                                │   │ │" << std::endl;
    std::cout << "│ │   // ... work with resource ...               │   │ │" << std::endl;
    std::cout << "│ │                                                │   │ │" << std::endl;
    std::cout << "│ │ } ← Destructor automatically releases resource│ ←─┘ │" << std::endl;
    std::cout << "│ └────────────────────────────────────────────────┘     │" << std::endl;
    std::cout << "│                                                        │" << std::endl;
    std::cout << "│ Exception Safety Guarantee:                            │" << std::endl;
    std::cout << "│ ┌────────────────────────────────────────────────┐     │" << std::endl;
    std::cout << "│ │ ANY exit from scope (normal return, exception, │     │" << std::endl;
    std::cout << "│ │ early return, goto, etc.) will ALWAYS call    │     │" << std::endl;
    std::cout << "│ │ the destructor and release the resource!      │     │" << std::endl;
    std::cout << "│ └────────────────────────────────────────────────┘     │" << std::endl;
    std::cout << "└────────────────────────────────────────────────────────┘" << std::endl;
    
    // Condition Variable Concepts
    std::cout << Colors::BOLD << Colors::CYAN << "\n🔔 CONDITION VARIABLE CONCEPTS:" << Colors::RESET << std::endl;
    std::cout << "┌─ WAIT/NOTIFY MECHANISM ────────────────────────────────┐" << std::endl;
    std::cout << "│                                                        │" << std::endl;
    std::cout << "│ wait() Operation (atomic):                             │" << std::endl;
    std::cout << "│ ┌────────────────────────────────────────────────┐     │" << std::endl;
    std::cout << "│ │ 1. Check condition while holding lock          │     │" << std::endl;
    std::cout << "│ │ 2. If condition false:                         │     │" << std::endl;
    std::cout << "│ │    a) Release lock                             │     │" << std::endl;
    std::cout << "│ │    b) Block thread (go to sleep)              │     │" << std::endl;
    std::cout << "│ │    c) Wait for notify signal                   │     │" << std::endl;
    std::cout << "│ │ 3. When notified:                              │     │" << std::endl;
    std::cout << "│ │    a) Re-acquire lock                          │     │" << std::endl;
    std::cout << "│ │    b) Re-check condition                       │     │" << std::endl;
    std::cout << "│ │    c) If true: proceed, if false: wait again   │     │" << std::endl;
    std::cout << "│ └────────────────────────────────────────────────┘     │" << std::endl;
    std::cout << "│                                                        │" << std::endl;
    std::cout << "│ notify_one() / notify_all():                           │" << std::endl;
    std::cout << "│ ┌────────────────────────────────────────────────┐     │" << std::endl;
    std::cout << "│ │ • notify_one(): Wake up ONE waiting thread     │     │" << std::endl;
    std::cout << "│ │ • notify_all(): Wake up ALL waiting threads    │     │" << std::endl;
    std::cout << "│ │ • No guarantee which thread wakes up first     │     │" << std::endl;
    std::cout << "│ │ • Spurious wakeups possible (always recheck!)  │     │" << std::endl;
    std::cout << "│ └────────────────────────────────────────────────┘     │" << std::endl;
    std::cout << "└────────────────────────────────────────────────────────┘" << std::endl;
    
    // Performance Characteristics
    std::cout << Colors::BOLD << Colors::YELLOW << "\n⚡ PERFORMANCE CHARACTERISTICS:" << Colors::RESET << std::endl;
    std::cout << "┌─ OPERATION COSTS ──────────────────────────────────────┐" << std::endl;
    std::cout << "│                                                        │" << std::endl;
    std::cout << "│ mutex.lock() / mutex.unlock():                         │" << std::endl;
    std::cout << "│ • Uncontended: ~20-50 CPU cycles                      │" << std::endl;
    std::cout << "│ • Contended: Kernel syscall (~1000+ cycles)           │" << std::endl;
    std::cout << "│                                                        │" << std::endl;
    std::cout << "│ condition_variable.wait():                             │" << std::endl;
    std::cout << "│ • Always involves kernel syscall                      │" << std::endl;
    std::cout << "│ • Thread context switch overhead                      │" << std::endl;
    std::cout << "│ • BUT: Zero CPU usage while waiting!                  │" << std::endl;
    std::cout << "│                                                        │" << std::endl;
    std::cout << "│ Polling vs Condition Variables:                       │" << std::endl;
    std::cout << "│ • Polling: High CPU, poor battery life                │" << std::endl;
    std::cout << "│ • CV: Zero CPU while waiting, instant notification    │" << std::endl;
    std::cout << "└────────────────────────────────────────────────────────┘" << std::endl;
    
    return 0;
}