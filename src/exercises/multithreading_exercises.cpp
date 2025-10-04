/**
 * @file multithreading_exercises.cpp
 * @author GitHub Copilot
 * @brief Guided learning and practice activities for C++ multithreading concepts
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>
#include <vector>
#include <queue>
#include <chrono>
#include <random>
#include <functional>

// ANSI Color codes for better output
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
// EXERCISE 1: Basic Thread Creation and Joining
// =============================================================================

namespace Exercise1 {
    void print_header() {
        std::cout << Colors::BOLD << Colors::BLUE << "\n🧵 EXERCISE 1: Basic Thread Creation" << Colors::RESET << std::endl;
        std::cout << Colors::YELLOW << "Task: Create 3 threads that print their ID and a message" << Colors::RESET << std::endl;
        std::cout << std::string(60, '-') << std::endl;
    }
    
    // TODO: Implement this function
    void worker_function(int thread_id, const std::string& message) {
        // YOUR CODE HERE:
        // 1. Print the thread ID (use std::this_thread::get_id())
        // 2. Print the custom message
        // 3. Sleep for a random time between 1-3 seconds
        
        std::cout << Colors::GREEN << "Thread " << thread_id 
                  << " (ID: " << std::this_thread::get_id() << "): " 
                  << message << Colors::RESET << std::endl;
        
        // Random sleep 1-3 seconds
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 3);
        std::this_thread::sleep_for(std::chrono::seconds(dis(gen)));
        
        std::cout << Colors::CYAN << "Thread " << thread_id << " finished!" << Colors::RESET << std::endl;
    }
    
    void run_exercise() {
        print_header();
        
        // YOUR CODE HERE:
        // 1. Create a vector to store thread objects
        // 2. Create 3 threads using different messages
        // 3. Join all threads
        
        std::vector<std::thread> threads;
        
        // Create threads
        threads.emplace_back(worker_function, 1, "Processing data...");
        threads.emplace_back(worker_function, 2, "Calculating results...");
        threads.emplace_back(worker_function, 3, "Saving to database...");
        
        std::cout << Colors::YELLOW << "All threads started!" << Colors::RESET << std::endl;
        
        // Join all threads
        for (auto& t : threads) {
            t.join();
        }
        
        std::cout << Colors::GREEN << "✅ Exercise 1 completed!" << Colors::RESET << std::endl;
    }
}

// =============================================================================
// EXERCISE 2: Mutex and Race Conditions
// =============================================================================

namespace Exercise2 {
    int shared_counter = 0;
    std::mutex counter_mutex;
    
    void print_header() {
        std::cout << Colors::BOLD << Colors::BLUE << "\n🔒 EXERCISE 2: Mutex and Race Conditions" << Colors::RESET << std::endl;
        std::cout << Colors::YELLOW << "Task: Fix race condition in shared counter increment" << Colors::RESET << std::endl;
        std::cout << std::string(60, '-') << std::endl;
    }
    
    // UNSAFE version - demonstrates race condition
    void unsafe_increment(int thread_id, int iterations) {
        for (int i = 0; i < iterations; ++i) {
            // RACE CONDITION: Multiple threads accessing shared_counter
            int temp = shared_counter;
            std::this_thread::sleep_for(std::chrono::microseconds(1)); // Simulate work
            shared_counter = temp + 1;
        }
        std::cout << Colors::RED << "Thread " << thread_id << " (unsafe) finished" << Colors::RESET << std::endl;
    }
    
    // TODO: Implement safe version
    void safe_increment(int thread_id, int iterations) {
        // YOUR CODE HERE:
        // 1. Use std::lock_guard to protect the critical section
        // 2. Make sure shared_counter is accessed atomically
        
        for (int i = 0; i < iterations; ++i) {
            std::lock_guard<std::mutex> lock(counter_mutex);
            shared_counter++;
        }
        std::cout << Colors::GREEN << "Thread " << thread_id << " (safe) finished" << Colors::RESET << std::endl;
    }
    
    void run_exercise() {
        print_header();
        
        // Part A: Demonstrate race condition
        std::cout << Colors::CYAN << "Part A: Unsafe version (race condition)" << Colors::RESET << std::endl;
        shared_counter = 0;
        
        std::vector<std::thread> unsafe_threads;
        for (int i = 0; i < 5; ++i) {
            unsafe_threads.emplace_back(unsafe_increment, i, 100);
        }
        
        for (auto& t : unsafe_threads) {
            t.join();
        }
        
        std::cout << Colors::RED << "Unsafe result: " << shared_counter 
                  << " (expected: 500)" << Colors::RESET << std::endl;
        
        // Part B: Safe version with mutex
        std::cout << Colors::CYAN << "\nPart B: Safe version (with mutex)" << Colors::RESET << std::endl;
        shared_counter = 0;
        
        std::vector<std::thread> safe_threads;
        for (int i = 0; i < 5; ++i) {
            safe_threads.emplace_back(safe_increment, i, 100);
        }
        
        for (auto& t : safe_threads) {
            t.join();
        }
        
        std::cout << Colors::GREEN << "Safe result: " << shared_counter 
                  << " (expected: 500)" << Colors::RESET << std::endl;
        std::cout << Colors::GREEN << "✅ Exercise 2 completed!" << Colors::RESET << std::endl;
    }
}

// =============================================================================
// EXERCISE 3: Producer-Consumer with Condition Variables
// =============================================================================

namespace Exercise3 {
    std::queue<int> data_queue;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool finished = false;
    
    void print_header() {
        std::cout << Colors::BOLD << Colors::BLUE << "\n📦 EXERCISE 3: Producer-Consumer Pattern" << Colors::RESET << std::endl;
        std::cout << Colors::YELLOW << "Task: Implement producer-consumer using condition variables" << Colors::RESET << std::endl;
        std::cout << std::string(60, '-') << std::endl;
    }
    
    // TODO: Implement producer function
    void producer(int producer_id, int items_to_produce) {
        // YOUR CODE HERE:
        // 1. Produce items and add them to the queue
        // 2. Use proper locking when accessing the queue
        // 3. Notify consumers when items are available
        
        for (int i = 0; i < items_to_produce; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work
            
            {
                std::lock_guard<std::mutex> lock(queue_mutex);
                int item = producer_id * 1000 + i;
                data_queue.push(item);
                std::cout << Colors::GREEN << "Producer " << producer_id 
                          << " produced item: " << item << Colors::RESET << std::endl;
            }
            condition.notify_one();
        }
        
        std::cout << Colors::CYAN << "Producer " << producer_id << " finished!" << Colors::RESET << std::endl;
    }
    
    // TODO: Implement consumer function
    void consumer(int consumer_id) {
        // YOUR CODE HERE:
        // 1. Wait for items in the queue using condition variable
        // 2. Process items when available
        // 3. Handle the case when production is finished
        
        while (true) {
            std::unique_lock<std::mutex> lock(queue_mutex);
            
            // Wait for items or until finished
            condition.wait(lock, [] { return !data_queue.empty() || finished; });
            
            if (data_queue.empty() && finished) {
                break; // No more items to process
            }
            
            if (!data_queue.empty()) {
                int item = data_queue.front();
                data_queue.pop();
                lock.unlock(); // Release lock while processing
                
                std::cout << Colors::MAGENTA << "Consumer " << consumer_id 
                          << " consumed item: " << item << Colors::RESET << std::endl;
                
                std::this_thread::sleep_for(std::chrono::milliseconds(150)); // Simulate processing
            }
        }
        
        std::cout << Colors::YELLOW << "Consumer " << consumer_id << " finished!" << Colors::RESET << std::endl;
    }
    
    void run_exercise() {
        print_header();
        
        // Reset state
        while (!data_queue.empty()) data_queue.pop();
        finished = false;
        
        // Create producers and consumers
        std::vector<std::thread> threads;
        
        // Start consumers first
        threads.emplace_back(consumer, 1);
        threads.emplace_back(consumer, 2);
        
        // Start producers
        threads.emplace_back(producer, 1, 5);
        threads.emplace_back(producer, 2, 3);
        
        // Wait for producers to finish
        threads[2].join();
        threads[3].join();
        
        // Signal that production is finished
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            finished = true;
        }
        condition.notify_all();
        
        // Wait for consumers to finish
        threads[0].join();
        threads[1].join();
        
        std::cout << Colors::GREEN << "✅ Exercise 3 completed!" << Colors::RESET << std::endl;
    }
}

// =============================================================================
// EXERCISE 4: Atomic Operations
// =============================================================================

namespace Exercise4 {
    std::atomic<int> atomic_counter(0);
    int regular_counter = 0;
    std::mutex regular_mutex;
    
    void print_header() {
        std::cout << Colors::BOLD << Colors::BLUE << "\n⚛️  EXERCISE 4: Atomic Operations" << Colors::RESET << std::endl;
        std::cout << Colors::YELLOW << "Task: Compare atomic vs mutex performance" << Colors::RESET << std::endl;
        std::cout << std::string(60, '-') << std::endl;
    }
    
    // TODO: Implement atomic increment
    void atomic_increment_worker(int iterations) {
        // YOUR CODE HERE:
        // 1. Use atomic operations to increment the counter
        // 2. Try different atomic operations (++, fetch_add, etc.)
        
        for (int i = 0; i < iterations; ++i) {
            atomic_counter.fetch_add(1); // or atomic_counter++
        }
    }
    
    // TODO: Implement mutex-protected increment
    void mutex_increment_worker(int iterations) {
        // YOUR CODE HERE:
        // 1. Use mutex to protect regular counter increment
        
        for (int i = 0; i < iterations; ++i) {
            std::lock_guard<std::mutex> lock(regular_mutex);
            regular_counter++;
        }
    }
    
    void run_exercise() {
        print_header();
        
        const int num_threads = 4;
        const int iterations_per_thread = 100000;
        
        // Test atomic operations
        std::cout << Colors::CYAN << "Testing atomic operations..." << Colors::RESET << std::endl;
        atomic_counter = 0;
        
        auto start_time = std::chrono::high_resolution_clock::now();
        
        std::vector<std::thread> atomic_threads;
        for (int i = 0; i < num_threads; ++i) {
            atomic_threads.emplace_back(atomic_increment_worker, iterations_per_thread);
        }
        
        for (auto& t : atomic_threads) {
            t.join();
        }
        
        auto atomic_time = std::chrono::high_resolution_clock::now() - start_time;
        
        // Test mutex operations
        std::cout << Colors::CYAN << "Testing mutex operations..." << Colors::RESET << std::endl;
        regular_counter = 0;
        
        start_time = std::chrono::high_resolution_clock::now();
        
        std::vector<std::thread> mutex_threads;
        for (int i = 0; i < num_threads; ++i) {
            mutex_threads.emplace_back(mutex_increment_worker, iterations_per_thread);
        }
        
        for (auto& t : mutex_threads) {
            t.join();
        }
        
        auto mutex_time = std::chrono::high_resolution_clock::now() - start_time;
        
        // Results
        std::cout << Colors::GREEN << "Results:" << Colors::RESET << std::endl;
        std::cout << "Atomic counter: " << atomic_counter << " (expected: " << num_threads * iterations_per_thread << ")" << std::endl;
        std::cout << "Mutex counter: " << regular_counter << " (expected: " << num_threads * iterations_per_thread << ")" << std::endl;
        
        auto atomic_ms = std::chrono::duration_cast<std::chrono::milliseconds>(atomic_time).count();
        auto mutex_ms = std::chrono::duration_cast<std::chrono::milliseconds>(mutex_time).count();
        
        std::cout << Colors::YELLOW << "Atomic time: " << atomic_ms << "ms" << Colors::RESET << std::endl;
        std::cout << Colors::YELLOW << "Mutex time: " << mutex_ms << "ms" << Colors::RESET << std::endl;
        std::cout << Colors::MAGENTA << "Performance ratio: " << (double)mutex_ms / atomic_ms << "x" << Colors::RESET << std::endl;
        
        std::cout << Colors::GREEN << "✅ Exercise 4 completed!" << Colors::RESET << std::endl;
    }
}

// =============================================================================
// EXERCISE 5: Futures and Promises
// =============================================================================

namespace Exercise5 {
    void print_header() {
        std::cout << Colors::BOLD << Colors::BLUE << "\n🔮 EXERCISE 5: Futures and Promises" << Colors::RESET << std::endl;
        std::cout << Colors::YELLOW << "Task: Use futures/promises for async computation" << Colors::RESET << std::endl;
        std::cout << std::string(60, '-') << std::endl;
    }
    
    // TODO: Implement async computation function
    int expensive_computation(int n) {
        // YOUR CODE HERE:
        // 1. Simulate expensive computation (e.g., factorial, fibonacci)
        // 2. Add some delay to simulate real work
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        // Calculate factorial
        int result = 1;
        for (int i = 1; i <= n; ++i) {
            result *= i;
        }
        
        std::cout << Colors::GREEN << "Computed factorial(" << n << ") = " << result << Colors::RESET << std::endl;
        return result;
    }
    
    // TODO: Implement promise-based computation
    void computation_with_promise(std::promise<int> promise, int n) {
        // YOUR CODE HERE:
        // 1. Perform computation
        // 2. Set the result in the promise
        // 3. Handle potential exceptions
        
        try {
            int result = expensive_computation(n);
            promise.set_value(result);
        } catch (...) {
            promise.set_exception(std::current_exception());
        }
    }
    
    void run_exercise() {
        print_header();
        
        // Part A: Using std::async
        std::cout << Colors::CYAN << "Part A: Using std::async" << Colors::RESET << std::endl;
        
        auto future1 = std::async(std::launch::async, expensive_computation, 5);
        auto future2 = std::async(std::launch::async, expensive_computation, 6);
        auto future3 = std::async(std::launch::async, expensive_computation, 7);
        
        std::cout << Colors::YELLOW << "Started async computations..." << Colors::RESET << std::endl;
        
        // Get results
        int result1 = future1.get();
        int result2 = future2.get();
        int result3 = future3.get();
        
        std::cout << Colors::GREEN << "Async results: " << result1 << ", " << result2 << ", " << result3 << Colors::RESET << std::endl;
        
        // Part B: Using promise/future manually
        std::cout << Colors::CYAN << "\nPart B: Using promise/future manually" << Colors::RESET << std::endl;
        
        std::promise<int> promise1, promise2;
        auto manual_future1 = promise1.get_future();
        auto manual_future2 = promise2.get_future();
        
        std::thread t1(computation_with_promise, std::move(promise1), 8);
        std::thread t2(computation_with_promise, std::move(promise2), 9);
        
        std::cout << Colors::YELLOW << "Started manual promise/future computations..." << Colors::RESET << std::endl;
        
        int manual_result1 = manual_future1.get();
        int manual_result2 = manual_future2.get();
        
        t1.join();
        t2.join();
        
        std::cout << Colors::GREEN << "Manual results: " << manual_result1 << ", " << manual_result2 << Colors::RESET << std::endl;
        std::cout << Colors::GREEN << "✅ Exercise 5 completed!" << Colors::RESET << std::endl;
    }
}

// =============================================================================
// EXERCISE 6: Thread Pool Implementation
// =============================================================================

namespace Exercise6 {
    void print_header() {
        std::cout << Colors::BOLD << Colors::BLUE << "\n🏊‍♂️ EXERCISE 6: Thread Pool Implementation" << Colors::RESET << std::endl;
        std::cout << Colors::YELLOW << "Task: Implement a simple thread pool" << Colors::RESET << std::endl;
        std::cout << std::string(60, '-') << std::endl;
    }
    
    // TODO: Implement ThreadPool class
    class ThreadPool {
    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queue_mutex;
        std::condition_variable condition;
        bool stop;
        
    public:
        ThreadPool(size_t num_threads) : stop(false) {
            // YOUR CODE HERE:
            // 1. Create worker threads
            // 2. Each worker should wait for tasks and execute them
            
            for (size_t i = 0; i < num_threads; ++i) {
                workers.emplace_back([this] {
                    while (true) {
                        std::function<void()> task;
                        
                        {
                            std::unique_lock<std::mutex> lock(queue_mutex);
                            condition.wait(lock, [this] { return stop || !tasks.empty(); });
                            
                            if (stop && tasks.empty()) {
                                return;
                            }
                            
                            task = std::move(tasks.front());
                            tasks.pop();
                        }
                        
                        task();
                    }
                });
            }
        }
        
        template<class F>
        void enqueue(F&& f) {
            // YOUR CODE HERE:
            // 1. Add task to queue
            // 2. Notify workers
            
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                if (stop) {
                    throw std::runtime_error("enqueue on stopped ThreadPool");
                }
                tasks.emplace(std::forward<F>(f));
            }
            condition.notify_one();
        }
        
        ~ThreadPool() {
            // YOUR CODE HERE:
            // 1. Stop accepting new tasks
            // 2. Wait for all workers to finish
            
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                stop = true;
            }
            condition.notify_all();
            
            for (std::thread& worker : workers) {
                worker.join();
            }
        }
    };
    
    void sample_task(int task_id) {
        std::cout << Colors::GREEN << "Executing task " << task_id 
                  << " on thread " << std::this_thread::get_id() << Colors::RESET << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << Colors::CYAN << "Task " << task_id << " completed!" << Colors::RESET << std::endl;
    }
    
    void run_exercise() {
        print_header();
        
        // Create thread pool with 3 workers
        ThreadPool pool(3);
        
        std::cout << Colors::YELLOW << "Created thread pool with 3 workers" << Colors::RESET << std::endl;
        
        // Submit tasks to the pool
        for (int i = 1; i <= 8; ++i) {
            pool.enqueue([i] { sample_task(i); });
        }
        
        std::cout << Colors::YELLOW << "Submitted 8 tasks to the pool" << Colors::RESET << std::endl;
        
        // Wait a bit to see tasks being executed
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        std::cout << Colors::GREEN << "✅ Exercise 6 completed!" << Colors::RESET << std::endl;
    }
}

// =============================================================================
// MAIN MENU AND EXECUTION
// =============================================================================

void print_main_menu() {
    std::cout << Colors::BOLD << Colors::CYAN << "\n🎓 C++ Multithreading Learning Exercises" << Colors::RESET << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << Colors::GREEN << "1. Basic Thread Creation and Joining" << Colors::RESET << std::endl;
    std::cout << Colors::GREEN << "2. Mutex and Race Conditions" << Colors::RESET << std::endl;
    std::cout << Colors::GREEN << "3. Producer-Consumer with Condition Variables" << Colors::RESET << std::endl;
    std::cout << Colors::GREEN << "4. Atomic Operations" << Colors::RESET << std::endl;
    std::cout << Colors::GREEN << "5. Futures and Promises" << Colors::RESET << std::endl;
    std::cout << Colors::GREEN << "6. Thread Pool Implementation" << Colors::RESET << std::endl;
    std::cout << Colors::GREEN << "7. Run All Exercises" << Colors::RESET << std::endl;
    std::cout << Colors::GREEN << "0. Exit" << Colors::RESET << std::endl;
    std::cout << std::string(60, '=') << std::endl;
}

void run_all_exercises() {
    std::cout << Colors::BOLD << Colors::MAGENTA << "🚀 Running All Exercises" << Colors::RESET << std::endl;
    
    Exercise1::run_exercise();
    Exercise2::run_exercise();
    Exercise3::run_exercise();
    Exercise4::run_exercise();
    Exercise5::run_exercise();
    Exercise6::run_exercise();
    
    std::cout << Colors::BOLD << Colors::GREEN << "\n🎉 All exercises completed successfully!" << Colors::RESET << std::endl;
}

int main() {
    std::cout << Colors::BOLD << Colors::CYAN << "Welcome to C++ Multithreading Practice!" << Colors::RESET << std::endl;
    
    while (true) {
        print_main_menu();
        
        std::cout << Colors::YELLOW << "Enter your choice (0-7): " << Colors::RESET;
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                Exercise1::run_exercise();
                break;
            case 2:
                Exercise2::run_exercise();
                break;
            case 3:
                Exercise3::run_exercise();
                break;
            case 4:
                Exercise4::run_exercise();
                break;
            case 5:
                Exercise5::run_exercise();
                break;
            case 6:
                Exercise6::run_exercise();
                break;
            case 7:
                run_all_exercises();
                break;
            case 0:
                std::cout << Colors::GREEN << "👋 Happy coding!" << Colors::RESET << std::endl;
                return 0;
            default:
                std::cout << Colors::RED << "Invalid choice! Please try again." << Colors::RESET << std::endl;
                break;
        }
        
        std::cout << Colors::YELLOW << "\nPress Enter to continue..." << Colors::RESET;
        std::cin.ignore();
        std::cin.get();
    }
    
    return 0;
}