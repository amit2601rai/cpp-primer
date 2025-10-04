/**
 * @file promises_futures.cpp
 * @author GitHub Copilot
 * @brief Tutorial code for C++ STL promises and futures.
 */

// This program demonstrates the use of std::promise and std::future.
// std::promise allows one thread to set a value or exception that can be
// retrieved by another thread using std::future. This provides a way to
// communicate between threads asynchronously.

#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <vector>

// Example 1: Basic promise/future usage
void basic_example() {
    std::cout << "\n=== Basic Promise/Future Example ===" << std::endl;
    
    // Create a promise that will hold an integer
    std::promise<int> promise;
    
    // Get the future associated with the promise
    std::future<int> future = promise.get_future();
    
    // Launch a thread that will set the promise value
    std::thread producer([&promise]() {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Producer: Setting value to 42" << std::endl;
        promise.set_value(42);
    });
    
    std::cout << "Main thread: Waiting for result..." << std::endl;
    
    // Get the value from the future (this will block until the value is set)
    int result = future.get();
    std::cout << "Main thread: Received result: " << result << std::endl;
    
    producer.join();
}

// Example 2: Promise with exception handling
void exception_example() {
    std::cout << "\n=== Promise/Future Exception Example ===" << std::endl;
    
    std::promise<double> promise;
    std::future<double> future = promise.get_future();
    
    std::thread worker([&promise]() {
        try {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            // Simulate an error condition
            throw std::runtime_error("Something went wrong in worker thread!");
        } catch (...) {
            // Set the exception in the promise
            promise.set_exception(std::current_exception());
        }
    });
    
    try {
        std::cout << "Main thread: Waiting for result..." << std::endl;
        double result = future.get();
        std::cout << "Received: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    
    worker.join();
}

// Example 3: Multiple futures with std::async
void async_example() {
    std::cout << "\n=== std::async Example ===" << std::endl;
    
    // std::async automatically creates promise/future pairs
    auto future1 = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return 10;
    });
    
    auto future2 = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return 20;
    });
    
    auto future3 = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return 30;
    });
    
    std::cout << "Main thread: Started 3 async tasks..." << std::endl;
    
    // Get results (order doesn't matter, each will block until ready)
    int result1 = future1.get();
    int result2 = future2.get();
    int result3 = future3.get();
    
    std::cout << "Results: " << result1 << ", " << result2 << ", " << result3 << std::endl;
    std::cout << "Sum: " << (result1 + result2 + result3) << std::endl;
}

// Example 4: Checking future status without blocking
void non_blocking_example() {
    std::cout << "\n=== Non-blocking Future Check Example ===" << std::endl;
    
    auto future = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return 100;
    });
    
    std::cout << "Main thread: Task started, checking status..." << std::endl;
    
    // Check status without blocking
    while (future.wait_for(std::chrono::milliseconds(500)) != std::future_status::ready) {
        std::cout << "Still waiting..." << std::endl;
    }
    
    std::cout << "Task completed! Result: " << future.get() << std::endl;
}

// Example 5: Promise/Future with custom data type
struct TaskResult {
    int id;
    std::string message;
    double value;
};

void custom_type_example() {
    std::cout << "\n=== Custom Type Promise/Future Example ===" << std::endl;
    
    std::promise<TaskResult> promise;
    std::future<TaskResult> future = promise.get_future();
    
    std::thread worker([&promise]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        TaskResult result{42, "Task completed successfully", 3.14159};
        promise.set_value(result);
    });
    
    std::cout << "Main thread: Waiting for custom result..." << std::endl;
    TaskResult result = future.get();
    
    std::cout << "Received TaskResult:" << std::endl;
    std::cout << "  ID: " << result.id << std::endl;
    std::cout << "  Message: " << result.message << std::endl;
    std::cout << "  Value: " << result.value << std::endl;
    
    worker.join();
}

// Example 6: Using std::packaged_task
void packaged_task_example() {
    std::cout << "\n=== std::packaged_task Example ===" << std::endl;
    
    // Create a packaged_task that wraps a callable
    std::packaged_task<int(int, int)> task([](int a, int b) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return a + b;
    });
    
    // Get the future from the packaged_task
    std::future<int> future = task.get_future();
    
    // Run the task in a separate thread
    std::thread worker(std::move(task), 15, 25);
    
    std::cout << "Main thread: Waiting for packaged_task result..." << std::endl;
    int result = future.get();
    std::cout << "Packaged task result: " << result << std::endl;
    
    worker.join();
}

// Example 7: Multiple futures with different completion times
void multiple_futures_example() {
    std::cout << "\n=== Multiple Futures Example ===" << std::endl;
    
    std::vector<std::future<int>> futures;
    
    // Create multiple async tasks with different durations
    for (int i = 1; i <= 3; ++i) {
        futures.push_back(std::async(std::launch::async, [i]() {
            std::this_thread::sleep_for(std::chrono::seconds(i));
            std::cout << "Task " << i << " completed" << std::endl;
            return i * 10;
        }));
    }
    
    std::cout << "All tasks started, collecting results..." << std::endl;
    
    // Collect results as they become available
    for (size_t i = 0; i < futures.size(); ++i) {
        int result = futures[i].get();
        std::cout << "Collected result " << i + 1 << ": " << result << std::endl;
    }
}

int main() {
    std::cout << "C++ Promises and Futures Tutorial" << std::endl;
    std::cout << "==================================" << std::endl;
    
    basic_example();
    exception_example();
    async_example();
    non_blocking_example();
    custom_type_example();
    packaged_task_example();
    multiple_futures_example();
    
    std::cout << "\nAll examples completed!" << std::endl;
    return 0;
}