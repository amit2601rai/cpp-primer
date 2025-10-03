/**
 * @file heaps.cpp
 * @author GitHub Copilot
 * @brief Tutorial code for implementing min and max heaps using C++ STL.
 */

// This program provides a comprehensive overview of heap data structures in C++.
// It covers both min and max heaps using STL containers and algorithms,
// demonstrating various heap operations and use cases.

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
#include <random>
#include <chrono>
#include <set>

void demonstrate_priority_queue_basics() {
  std::cout << "=== PRIORITY QUEUE BASICS ===" << std::endl;
  
  // Default priority_queue is a MAX HEAP
  std::priority_queue<int> max_heap;
  
  // Insert elements
  std::vector<int> elements = {30, 10, 50, 20, 40};
  std::cout << "Inserting elements: ";
  for (int val : elements) {
    max_heap.push(val);
    std::cout << val << " ";
  }
  std::cout << std::endl;
  
  // Display heap contents (by popping - this destroys the heap)
  std::cout << "Max heap elements (largest first): ";
  std::priority_queue<int> temp_heap = max_heap;  // Make a copy
  while (!temp_heap.empty()) {
    std::cout << temp_heap.top() << " ";
    temp_heap.pop();
  }
  std::cout << std::endl;
  
  // Basic operations
  std::cout << "\nBasic Operations:" << std::endl;
  std::cout << "Top element (maximum): " << max_heap.top() << std::endl;
  std::cout << "Size: " << max_heap.size() << std::endl;
  std::cout << "Is empty: " << (max_heap.empty() ? "Yes" : "No") << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_max_heap() {
  std::cout << "=== MAX HEAP IMPLEMENTATION ===" << std::endl;
  
  // Method 1: Using default priority_queue (max heap)
  std::priority_queue<int> max_heap;
  
  std::cout << "Method 1 - Default priority_queue (max heap):" << std::endl;
  
  // Insert elements
  std::vector<int> data = {15, 25, 5, 35, 10, 30};
  for (int val : data) {
    max_heap.push(val);
    std::cout << "Pushed " << val << ", current max: " << max_heap.top() << std::endl;
  }
  
  std::cout << "Extracting all elements (largest first):" << std::endl;
  while (!max_heap.empty()) {
    std::cout << max_heap.top() << " ";
    max_heap.pop();
  }
  std::cout << std::endl;
  
  // Method 2: Using vector with make_heap/push_heap/pop_heap
  std::cout << "\nMethod 2 - Using vector with heap algorithms:" << std::endl;
  std::vector<int> heap_vector = {15, 25, 5, 35, 10, 30};
  
  // Convert vector to heap
  std::make_heap(heap_vector.begin(), heap_vector.end());
  std::cout << "After make_heap: ";
  for (int val : heap_vector) {
    std::cout << val << " ";
  }
  std::cout << std::endl;
  std::cout << "Max element: " << heap_vector.front() << std::endl;
  
  // Add new element
  heap_vector.push_back(40);
  std::push_heap(heap_vector.begin(), heap_vector.end());
  std::cout << "After pushing 40: max = " << heap_vector.front() << std::endl;
  
  // Remove max element
  std::pop_heap(heap_vector.begin(), heap_vector.end());
  int max_val = heap_vector.back();
  heap_vector.pop_back();
  std::cout << "Popped max element: " << max_val << std::endl;
  std::cout << "New max: " << heap_vector.front() << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_min_heap() {
  std::cout << "=== MIN HEAP IMPLEMENTATION ===" << std::endl;
  
  // Method 1: Using priority_queue with greater<int> comparator
  std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;
  
  std::cout << "Method 1 - priority_queue with greater<int> (min heap):" << std::endl;
  
  // Insert elements
  std::vector<int> data = {15, 25, 5, 35, 10, 30};
  for (int val : data) {
    min_heap.push(val);
    std::cout << "Pushed " << val << ", current min: " << min_heap.top() << std::endl;
  }
  
  std::cout << "Extracting all elements (smallest first):" << std::endl;
  while (!min_heap.empty()) {
    std::cout << min_heap.top() << " ";
    min_heap.pop();
  }
  std::cout << std::endl;
  
  // Method 2: Using vector with heap algorithms and greater<int>
  std::cout << "\nMethod 2 - Using vector with heap algorithms (min heap):" << std::endl;
  std::vector<int> min_heap_vector = {15, 25, 5, 35, 10, 30};
  
  // Convert vector to min heap
  std::make_heap(min_heap_vector.begin(), min_heap_vector.end(), std::greater<int>());
  std::cout << "After make_heap with greater<int>: ";
  for (int val : min_heap_vector) {
    std::cout << val << " ";
  }
  std::cout << std::endl;
  std::cout << "Min element: " << min_heap_vector.front() << std::endl;
  
  // Add new element
  min_heap_vector.push_back(3);
  std::push_heap(min_heap_vector.begin(), min_heap_vector.end(), std::greater<int>());
  std::cout << "After pushing 3: min = " << min_heap_vector.front() << std::endl;
  
  // Remove min element
  std::pop_heap(min_heap_vector.begin(), min_heap_vector.end(), std::greater<int>());
  int min_val = min_heap_vector.back();
  min_heap_vector.pop_back();
  std::cout << "Popped min element: " << min_val << std::endl;
  std::cout << "New min: " << min_heap_vector.front() << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_custom_comparators() {
  std::cout << "=== CUSTOM COMPARATORS ===" << std::endl;
  
  // Custom class for demonstration
  struct Person {
    std::string name;
    int age;
    
    Person(const std::string& n, int a) : name(n), age(a) {}
  };
  
  // Custom comparator for max heap based on age
  auto age_comparator = [](const Person& a, const Person& b) {
    return a.age < b.age;  // For max heap (older person has higher priority)
  };
  
  std::priority_queue<Person, std::vector<Person>, decltype(age_comparator)> age_max_heap(age_comparator);
  
  // Add people
  age_max_heap.push(Person("Alice", 25));
  age_max_heap.push(Person("Bob", 30));
  age_max_heap.push(Person("Charlie", 20));
  age_max_heap.push(Person("Diana", 35));
  
  std::cout << "Max heap by age (oldest first):" << std::endl;
  while (!age_max_heap.empty()) {
    const Person& p = age_max_heap.top();
    std::cout << p.name << " (age " << p.age << ")" << std::endl;
    age_max_heap.pop();
  }
  
  // Min heap based on name length
  auto name_length_comparator = [](const Person& a, const Person& b) {
    return a.name.length() > b.name.length();  // For min heap (shorter name has higher priority)
  };
  
  std::priority_queue<Person, std::vector<Person>, decltype(name_length_comparator)> name_min_heap(name_length_comparator);
  
  name_min_heap.push(Person("Alexander", 28));
  name_min_heap.push(Person("Jo", 22));
  name_min_heap.push(Person("Christina", 26));
  name_min_heap.push(Person("Sam", 24));
  
  std::cout << "\nMin heap by name length (shortest first):" << std::endl;
  while (!name_min_heap.empty()) {
    const Person& p = name_min_heap.top();
    std::cout << p.name << " (length " << p.name.length() << ")" << std::endl;
    name_min_heap.pop();
  }
  
  std::cout << std::endl;
}

void demonstrate_heap_algorithms() {
  std::cout << "=== HEAP ALGORITHMS IN DETAIL ===" << std::endl;
  
  std::vector<int> data = {4, 10, 3, 5, 1, 8, 9, 2, 7, 6};
  std::cout << "Original vector: ";
  for (int val : data) {
    std::cout << val << " ";
  }
  std::cout << std::endl;
  
  // make_heap: Convert range to heap
  std::make_heap(data.begin(), data.end());
  std::cout << "After make_heap (max heap): ";
  for (int val : data) {
    std::cout << val << " ";
  }
  std::cout << std::endl;
  std::cout << "Max element: " << data.front() << std::endl;
  
  // is_heap: Check if range is a heap
  bool is_max_heap = std::is_heap(data.begin(), data.end());
  std::cout << "Is max heap: " << (is_max_heap ? "Yes" : "No") << std::endl;
  
  // is_heap_until: Find first element that violates heap property
  auto heap_end = std::is_heap_until(data.begin(), data.end());
  std::cout << "Heap property maintained until index: " << std::distance(data.begin(), heap_end) << std::endl;
  
  // push_heap: Add element to heap
  data.push_back(15);
  std::push_heap(data.begin(), data.end());
  std::cout << "After pushing 15: max = " << data.front() << std::endl;
  
  // pop_heap: Remove max element
  std::pop_heap(data.begin(), data.end());
  int max_element = data.back();
  data.pop_back();
  std::cout << "Popped element: " << max_element << std::endl;
  std::cout << "New max: " << data.front() << std::endl;
  
  // sort_heap: Sort heap (destroys heap property)
  std::sort_heap(data.begin(), data.end());
  std::cout << "After sort_heap (ascending order): ";
  for (int val : data) {
    std::cout << val << " ";
  }
  std::cout << std::endl;
  
  std::cout << "Is still a heap: " << (std::is_heap(data.begin(), data.end()) ? "Yes" : "No") << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_heap_applications() {
  std::cout << "=== HEAP APPLICATIONS ===" << std::endl;
  
  // Application 1: Finding K largest elements
  std::cout << "Application 1 - Finding K largest elements:" << std::endl;
  std::vector<int> numbers = {23, 67, 45, 12, 89, 34, 56, 78, 90, 11};
  int k = 3;
  
  std::cout << "Array: ";
  for (int num : numbers) {
    std::cout << num << " ";
  }
  std::cout << std::endl;
  
  // Use min heap to find k largest
  std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap_k;
  
  for (int num : numbers) {
    if (min_heap_k.size() < k) {
      min_heap_k.push(num);
    } else if (num > min_heap_k.top()) {
      min_heap_k.pop();
      min_heap_k.push(num);
    }
  }
  
  std::cout << k << " largest elements: ";
  std::vector<int> largest;
  while (!min_heap_k.empty()) {
    largest.push_back(min_heap_k.top());
    min_heap_k.pop();
  }
  std::reverse(largest.begin(), largest.end());  // Reverse to get descending order
  for (int val : largest) {
    std::cout << val << " ";
  }
  std::cout << std::endl;
  
  // Application 2: Median of stream
  std::cout << "\nApplication 2 - Running median:" << std::endl;
  std::priority_queue<int> max_heap_lower;  // For lower half
  std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap_upper;  // For upper half
  
  std::vector<int> stream = {5, 15, 1, 3, 8, 7, 9, 2, 6, 10};
  
  for (int num : stream) {
    // Add to appropriate heap
    if (max_heap_lower.empty() || num <= max_heap_lower.top()) {
      max_heap_lower.push(num);
    } else {
      min_heap_upper.push(num);
    }
    
    // Balance heaps
    if (max_heap_lower.size() > min_heap_upper.size() + 1) {
      min_heap_upper.push(max_heap_lower.top());
      max_heap_lower.pop();
    } else if (min_heap_upper.size() > max_heap_lower.size() + 1) {
      max_heap_lower.push(min_heap_upper.top());
      min_heap_upper.pop();
    }
    
    // Calculate median
    double median;
    if (max_heap_lower.size() == min_heap_upper.size()) {
      median = (max_heap_lower.top() + min_heap_upper.top()) / 2.0;
    } else if (max_heap_lower.size() > min_heap_upper.size()) {
      median = max_heap_lower.top();
    } else {
      median = min_heap_upper.top();
    }
    
    std::cout << "Added " << num << ", median: " << median << std::endl;
  }
  
  // Application 3: Task scheduling with priorities
  std::cout << "\nApplication 3 - Task scheduling:" << std::endl;
  
  struct Task {
    std::string name;
    int priority;
    int duration;
    
    Task(const std::string& n, int p, int d) : name(n), priority(p), duration(d) {}
  };
  
  auto task_comparator = [](const Task& a, const Task& b) {
    if (a.priority != b.priority) {
      return a.priority < b.priority;  // Higher priority first
    }
    return a.duration > b.duration;  // Shorter duration first for same priority
  };
  
  std::priority_queue<Task, std::vector<Task>, decltype(task_comparator)> task_queue(task_comparator);
  
  // Add tasks
  task_queue.push(Task("Email", 2, 5));
  task_queue.push(Task("Meeting", 5, 60));
  task_queue.push(Task("Code Review", 4, 30));
  task_queue.push(Task("Documentation", 1, 120));
  task_queue.push(Task("Bug Fix", 5, 15));
  
  std::cout << "Task execution order:" << std::endl;
  while (!task_queue.empty()) {
    const Task& t = task_queue.top();
    std::cout << t.name << " (priority: " << t.priority << ", duration: " << t.duration << " min)" << std::endl;
    task_queue.pop();
  }
  
  std::cout << std::endl;
}

void demonstrate_performance_analysis() {
  std::cout << "=== PERFORMANCE ANALYSIS ===" << std::endl;
  
  const int data_size = 100000;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 1000000);
  
  // Generate random data
  std::vector<int> test_data;
  test_data.reserve(data_size);
  for (int i = 0; i < data_size; ++i) {
    test_data.push_back(dis(gen));
  }
  
  // Test 1: Heap construction
  auto start = std::chrono::high_resolution_clock::now();
  std::priority_queue<int> pq(test_data.begin(), test_data.end());
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "Heap construction (" << data_size << " elements): " << duration.count() << " microseconds" << std::endl;
  
  // Test 2: Sequential insertions
  start = std::chrono::high_resolution_clock::now();
  std::priority_queue<int> pq2;
  for (int val : test_data) {
    pq2.push(val);
  }
  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "Sequential insertions: " << duration.count() << " microseconds" << std::endl;
  
  // Test 3: Extract all elements
  start = std::chrono::high_resolution_clock::now();
  while (!pq.empty()) {
    pq.pop();
  }
  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "Extract all elements: " << duration.count() << " microseconds" << std::endl;
  
  // Test 4: Compare with sorted approach
  std::vector<int> sorted_data = test_data;
  start = std::chrono::high_resolution_clock::now();
  std::sort(sorted_data.begin(), sorted_data.end(), std::greater<int>());
  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "Full sort (for comparison): " << duration.count() << " microseconds" << std::endl;
  
  std::cout << "\nTime Complexity Summary:" << std::endl;
  std::cout << "  Insertion: O(log n)" << std::endl;
  std::cout << "  Deletion: O(log n)" << std::endl;
  std::cout << "  Peek (top): O(1)" << std::endl;
  std::cout << "  Build heap: O(n)" << std::endl;
  std::cout << "  Heapsort: O(n log n)" << std::endl;
  
  std::cout << "\nSpace Complexity: O(n)" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_heap_vs_alternatives() {
  std::cout << "=== HEAP VS ALTERNATIVES ===" << std::endl;
  
  const int n = 10000;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 100000);
  
  // Generate test data
  std::vector<int> test_data;
  for (int i = 0; i < n; ++i) {
    test_data.push_back(dis(gen));
  }
  
  // Test finding maximum with different approaches
  auto start = std::chrono::high_resolution_clock::now();
  
  // Approach 1: Priority Queue (Heap)
  std::priority_queue<int> pq(test_data.begin(), test_data.end());
  int max_heap = pq.top();
  auto end1 = std::chrono::high_resolution_clock::now();
  
  // Approach 2: Sorting
  std::vector<int> sorted_data = test_data;
  std::sort(sorted_data.begin(), sorted_data.end());
  int max_sort = sorted_data.back();
  auto end2 = std::chrono::high_resolution_clock::now();
  
  // Approach 3: Linear search
  int max_linear = *std::max_element(test_data.begin(), test_data.end());
  auto end3 = std::chrono::high_resolution_clock::now();
  
  // Approach 4: Set (balanced BST)
  std::set<int> ordered_set(test_data.begin(), test_data.end());
  int max_set = *ordered_set.rbegin();
  auto end4 = std::chrono::high_resolution_clock::now();
  
  std::cout << "Finding maximum in " << n << " elements:" << std::endl;
  std::cout << "  Heap construction + top(): " 
            << std::chrono::duration_cast<std::chrono::microseconds>(end1 - start).count() 
            << " µs (result: " << max_heap << ")" << std::endl;
  std::cout << "  Full sort + back(): " 
            << std::chrono::duration_cast<std::chrono::microseconds>(end2 - end1).count() 
            << " µs (result: " << max_sort << ")" << std::endl;
  std::cout << "  Linear search: " 
            << std::chrono::duration_cast<std::chrono::microseconds>(end3 - end2).count() 
            << " µs (result: " << max_linear << ")" << std::endl;
  std::cout << "  Set construction + rbegin(): " 
            << std::chrono::duration_cast<std::chrono::microseconds>(end4 - end3).count() 
            << " µs (result: " << max_set << ")" << std::endl;
  
  std::cout << "\nUse Cases:" << std::endl;
  std::cout << "  Heap: Dynamic insertions/deletions with priority" << std::endl;
  std::cout << "  Sort: When you need all elements in order" << std::endl;
  std::cout << "  Linear: Single query, no preprocessing needed" << std::endl;
  std::cout << "  Set: When you need ordered structure with no duplicates" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_heap_operations_summary() {
  std::cout << "=== HEAP OPERATIONS SUMMARY ===" << std::endl;
  
  std::cout << "MAX HEAP (Default priority_queue):" << std::endl;
  std::cout << "  Declaration: std::priority_queue<int> max_heap;" << std::endl;
  std::cout << "  Insert: max_heap.push(value);" << std::endl;
  std::cout << "  Get max: max_heap.top();" << std::endl;
  std::cout << "  Remove max: max_heap.pop();" << std::endl;
  std::cout << "  Size: max_heap.size();" << std::endl;
  std::cout << "  Empty: max_heap.empty();" << std::endl;
  
  std::cout << "\nMIN HEAP:" << std::endl;
  std::cout << "  Declaration: std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;" << std::endl;
  std::cout << "  Insert: min_heap.push(value);" << std::endl;
  std::cout << "  Get min: min_heap.top();" << std::endl;
  std::cout << "  Remove min: min_heap.pop();" << std::endl;
  
  std::cout << "\nVECTOR-BASED HEAP ALGORITHMS:" << std::endl;
  std::cout << "  Make heap: std::make_heap(vec.begin(), vec.end());" << std::endl;
  std::cout << "  Add element: vec.push_back(val); std::push_heap(vec.begin(), vec.end());" << std::endl;
  std::cout << "  Remove max: std::pop_heap(vec.begin(), vec.end()); vec.pop_back();" << std::endl;
  std::cout << "  Check heap: std::is_heap(vec.begin(), vec.end());" << std::endl;
  std::cout << "  Sort heap: std::sort_heap(vec.begin(), vec.end());" << std::endl;
  
  std::cout << "\nCUSTOM COMPARATORS:" << std::endl;
  std::cout << "  For objects: Use lambda or function object" << std::endl;
  std::cout << "  For min heap: Use std::greater<T>" << std::endl;
  std::cout << "  For max heap: Use std::less<T> (default)" << std::endl;
  
  std::cout << "\nCOMMON PATTERNS:" << std::endl;
  std::cout << "  K largest elements: Use min heap of size k" << std::endl;
  std::cout << "  K smallest elements: Use max heap of size k" << std::endl;
  std::cout << "  Running median: Use two heaps (max + min)" << std::endl;
  std::cout << "  Priority scheduling: Use max heap with custom comparator" << std::endl;
  
  std::cout << std::endl;
}

int main() {
  std::cout << "C++ HEAPS USING STL TUTORIAL" << std::endl;
  std::cout << "============================" << std::endl << std::endl;
  
  demonstrate_priority_queue_basics();
  demonstrate_max_heap();
  demonstrate_min_heap();
  demonstrate_custom_comparators();
  demonstrate_heap_algorithms();
  demonstrate_heap_applications();
  demonstrate_performance_analysis();
  demonstrate_heap_vs_alternatives();
  demonstrate_heap_operations_summary();
  
  std::cout << "Tutorial completed successfully!" << std::endl;
  return 0;
}