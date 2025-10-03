/**
 * @file pointers_references.cpp
 * @author GitHub Copilot
 * @brief Comprehensive tutorial on pointers, references, lvalue/rvalue, and function pointers in C++.
 */

// This program provides a complete overview of pointer and reference concepts in C++.
// It covers pointers, references, lvalue/rvalue semantics, move semantics,
// function pointers, and related constructs that are fundamental to C++ programming.

/*
THEORETICAL FOUNDATION: MEMORY MODEL AND INDIRECTION

1. MEMORY FUNDAMENTALS:
   - Every variable in C++ occupies memory with a unique address
   - Memory addresses are typically represented as hexadecimal numbers
   - The size of a memory address depends on the system architecture (32-bit or 64-bit)
   - Variables are stored in different memory regions: stack, heap, data segment, code segment

2. INDIRECTION CONCEPT:
   - Indirection means accessing a value through its address rather than directly
   - Enables dynamic memory allocation, polymorphism, and efficient data structures
   - Foundation for linked lists, trees, graphs, and other dynamic structures
   - Allows functions to modify variables from calling scope

3. ADDRESS SPACE LAYOUT:
   - Stack: Local variables, function parameters, return addresses (grows downward)
   - Heap: Dynamically allocated memory (grows upward)
   - Data Segment: Global/static variables
   - Code Segment: Executable instructions (read-only)

4. POINTER ARITHMETIC THEORY:
   - Pointers support arithmetic operations: +, -, ++, --
   - Arithmetic is scaled by the size of the pointed-to type
   - ptr + 1 advances by sizeof(T) bytes, not 1 byte
   - Enables efficient array traversal and memory management
*/

#include <iostream>
#include <memory>
#include <functional>
#include <vector>
#include <string>
#include <utility>
#include <type_traits>

// Forward declarations for function pointer examples
int add(int a, int b);
int multiply(int a, int b);
void process_array(int* arr, size_t size, int (*operation)(int));

void demonstrate_basic_pointers() {
  std::cout << "=== BASIC POINTERS ===" << std::endl;
  
  /*
  POINTER THEORY:
  
  1. DEFINITION:
     - A pointer is a variable that stores the memory address of another variable
     - Declared using the dereference operator (*): int* ptr;
     - The * can be placed next to type (int*), variable (int *ptr), or both
  
  2. MEMORY REPRESENTATION:
     - On 64-bit systems, pointers are typically 8 bytes (64 bits)
     - On 32-bit systems, pointers are typically 4 bytes (32 bits)
     - The pointer itself has its own memory address
  
  3. OPERATIONS:
     - Address-of (&): Gets the memory address of a variable
     - Dereference (*): Accesses the value at the address stored in pointer
     - Assignment: Can change what the pointer points to
  
  4. NULL POINTERS:
     - A pointer that doesn't point to any valid memory location
     - Modern C++ uses nullptr (type-safe) instead of NULL or 0
     - Dereferencing a null pointer causes undefined behavior (often segfault)
  
  5. POINTER ARITHMETIC:
     - Pointers can be incremented/decremented to navigate arrays
     - Arithmetic operations are scaled by sizeof(pointed-to-type)
     - Only valid within the same array or allocated memory block
  */
  
  // Basic pointer declaration and initialization
  int value = 42;
  int* ptr = &value;  // ptr points to the address of value
  
  std::cout << "Basic Pointer Operations:" << std::endl;
  std::cout << "  value = " << value << std::endl;
  std::cout << "  Address of value (&value) = " << &value << std::endl;
  std::cout << "  ptr = " << ptr << std::endl;
  std::cout << "  *ptr (dereferencing) = " << *ptr << std::endl;
  std::cout << "  Address of ptr (&ptr) = " << &ptr << std::endl;
  
  // Modifying value through pointer
  *ptr = 100;
  std::cout << "\nAfter *ptr = 100:" << std::endl;
  std::cout << "  value = " << value << std::endl;
  std::cout << "  *ptr = " << *ptr << std::endl;
  
  // Null pointer
  int* null_ptr = nullptr;
  std::cout << "\nNull pointer:" << std::endl;
  std::cout << "  null_ptr = " << null_ptr << std::endl;
  std::cout << "  (null_ptr == nullptr) = " << (null_ptr == nullptr) << std::endl;
  
  // Pointer arithmetic
  int arr[] = {10, 20, 30, 40, 50};
  int* arr_ptr = arr;  // Points to first element
  
  std::cout << "\nPointer Arithmetic:" << std::endl;
  for (int i = 0; i < 5; ++i) {
    std::cout << "  arr_ptr[" << i << "] = " << arr_ptr[i] << ", *(arr_ptr + " << i << ") = " << *(arr_ptr + i) << std::endl;
  }
  
  // Pointer to pointer
  int** ptr_to_ptr = &ptr;
  std::cout << "\nPointer to Pointer:" << std::endl;
  std::cout << "  ptr = " << ptr << std::endl;
  std::cout << "  ptr_to_ptr = " << ptr_to_ptr << std::endl;
  std::cout << "  *ptr_to_ptr = " << *ptr_to_ptr << std::endl;
  std::cout << "  **ptr_to_ptr = " << **ptr_to_ptr << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_basic_references() {
  std::cout << "=== BASIC REFERENCES ===" << std::endl;
  
  /*
  REFERENCE THEORY:
  
  1. DEFINITION:
     - A reference is an alias (alternative name) for an existing variable
     - Declared using the reference operator (&): int& ref = variable;
     - Must be initialized at declaration (cannot be left uninitialized)
  
  2. IMPLEMENTATION:
     - Typically implemented as a constant pointer under the hood
     - The compiler handles dereferencing automatically
     - No additional memory overhead in optimized builds
     - Same memory address as the referenced variable
  
  3. CHARACTERISTICS:
     - Cannot be null (always refers to a valid object)
     - Cannot be reassigned to refer to another object
     - Automatic dereferencing (no * operator needed)
     - No pointer arithmetic allowed
  
  4. CONST REFERENCES:
     - Can bind to both lvalues and rvalues
     - Extends the lifetime of temporary objects
     - Common in function parameters to avoid unnecessary copies
     - Cannot modify the referenced value
  
  5. REFERENCE BINDING RULES:
     - Non-const references can only bind to lvalues
     - Const references can bind to both lvalues and rvalues
     - References must match the exact type (no implicit conversions for non-const)
  */
  
  // Basic reference declaration and initialization
  int value = 42;
  int& ref = value;  // ref is an alias for value
  
  std::cout << "Basic Reference Operations:" << std::endl;
  std::cout << "  value = " << value << std::endl;
  std::cout << "  ref = " << ref << std::endl;
  std::cout << "  Address of value (&value) = " << &value << std::endl;
  std::cout << "  Address of ref (&ref) = " << &ref << std::endl;
  std::cout << "  Same address? " << (&value == &ref ? "Yes" : "No") << std::endl;
  
  // Modifying value through reference
  ref = 100;
  std::cout << "\nAfter ref = 100:" << std::endl;
  std::cout << "  value = " << value << std::endl;
  std::cout << "  ref = " << ref << std::endl;
  
  // Reference must be initialized (unlike pointers)
  // int& uninitialized_ref;  // ERROR: References must be initialized
  
  // Reference cannot be reassigned (unlike pointers)
  int another_value = 200;
  ref = another_value;  // This assigns the VALUE, doesn't change what ref refers to
  std::cout << "\nAfter ref = another_value (assigns value, not reference):" << std::endl;
  std::cout << "  value = " << value << std::endl;
  std::cout << "  ref = " << ref << std::endl;
  std::cout << "  another_value = " << another_value << std::endl;
  std::cout << "  ref still refers to original value: " << (&ref == &value ? "Yes" : "No") << std::endl;
  
  // Const references
  const int const_value = 300;
  const int& const_ref = const_value;
  // const_ref = 400;  // ERROR: Cannot modify through const reference
  
  std::cout << "\nConst Reference:" << std::endl;
  std::cout << "  const_value = " << const_value << std::endl;
  std::cout << "  const_ref = " << const_ref << std::endl;
  
  // Const reference can bind to temporaries
  const int& temp_ref = 42;  // Binds to temporary
  std::cout << "  temp_ref (bound to temporary) = " << temp_ref << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_pointers_vs_references() {
  std::cout << "=== POINTERS VS REFERENCES ===" << std::endl;
  
  /*
  POINTERS VS REFERENCES - THEORETICAL COMPARISON:
  
  1. MEMORY MODEL:
     - Pointers: Have their own memory location and address
     - References: Share the same memory location as the referenced variable
  
  2. INITIALIZATION:
     - Pointers: Can be declared without initialization (dangerous but allowed)
     - References: Must be initialized at declaration time
  
  3. REASSIGNMENT:
     - Pointers: Can be reassigned to point to different objects
     - References: Cannot be reassigned once initialized
  
  4. NULL VALUES:
     - Pointers: Can be null (nullptr)
     - References: Cannot be null (always refer to valid objects)
  
  5. ARITHMETIC:
     - Pointers: Support arithmetic operations (+, -, ++, --)
     - References: No arithmetic operations allowed
  
  6. INDIRECTION LEVELS:
     - Pointers: Can have multiple levels (int**, int***, etc.)
     - References: Only one level of indirection
  
  7. FUNCTION OVERLOADING:
     - Pointers and references create different function signatures
     - void func(int* ptr) vs void func(int& ref) are different functions
  
  8. PERFORMANCE:
     - Pointers: May require explicit dereferencing
     - References: Compiler optimizes away indirection
  
  9. SYNTAX:
     - Pointers: Explicit dereferencing with * operator
     - References: Automatic dereferencing (transparent to user)
  */
  
  int a = 10, b = 20;
  
  // Pointers
  int* ptr = &a;
  std::cout << "Pointers:" << std::endl;
  std::cout << "  Initially pointing to a: *ptr = " << *ptr << std::endl;
  
  ptr = &b;  // Can reassign pointer
  std::cout << "  After reassignment to b: *ptr = " << *ptr << std::endl;
  
  *ptr = 50;  // Modifies b through pointer
  std::cout << "  After *ptr = 50: b = " << b << std::endl;
  
  // References
  int& ref = a;
  std::cout << "\nReferences:" << std::endl;
  std::cout << "  Initially referring to a: ref = " << ref << std::endl;
  
  ref = b;  // This assigns b's value to a, doesn't change reference
  std::cout << "  After ref = b (assigns value): a = " << a << ", ref = " << ref << std::endl;
  
  // Key differences summary
  std::cout << "\nKey Differences:" << std::endl;
  std::cout << "  1. Pointers can be null, references cannot" << std::endl;
  std::cout << "  2. Pointers can be reassigned, references cannot" << std::endl;
  std::cout << "  3. Pointers have their own memory address, references don't" << std::endl;
  std::cout << "  4. Pointers require explicit dereferencing (*), references are automatic" << std::endl;
  std::cout << "  5. Pointer arithmetic is allowed, reference arithmetic is not" << std::endl;
  std::cout << "  6. Pointers can have multiple levels of indirection, references only one" << std::endl;
  
  std::cout << std::endl;
}

// Helper functions to demonstrate lvalue/rvalue concepts
int get_value() { return 42; }
int& get_reference() { static int x = 100; return x; }

void demonstrate_lvalue_rvalue() {
  std::cout << "=== LVALUE AND RVALUE ===" << std::endl;
  
  /*
  LVALUE AND RVALUE THEORY:
  
  1. HISTORICAL CONTEXT:
     - Terms originated from assignment expressions: lvalue = rvalue
     - lvalue: "left-hand side value" (can appear on left of assignment)
     - rvalue: "right-hand side value" (can appear on right of assignment)
  
  2. MODERN DEFINITION:
     - lvalue: Expression that refers to a memory location and persists beyond expression
     - rvalue: Expression that represents a temporary value or literal
  
  3. LVALUE CHARACTERISTICS:
     - Has a definable memory address (&lvalue is valid)
     - Persists beyond the single expression
     - Can be assigned to (if not const)
     - Examples: variables, array elements, dereferenced pointers, function calls returning references
  
  4. RVALUE CHARACTERISTICS:
     - Temporary values that don't persist beyond the expression
     - Cannot take address (&rvalue is invalid)
     - Cannot be assigned to
     - Examples: literals, arithmetic expressions, function calls returning by value
  
  5. VALUE CATEGORIES (C++11 Extended):
     - lvalue: Locator value (has identity, can be moved from)
     - xvalue: eXpiring value (has identity, can be moved from) - like std::move(x)
     - prvalue: Pure rvalue (no identity, can be moved from) - like literals
     - glvalue: Generalized lvalue (lvalue + xvalue) - expressions with identity
     - rvalue: (xvalue + prvalue) - expressions that can be moved from
  
  6. REFERENCE BINDING RULES:
     - lvalue references (&) bind only to lvalues
     - const lvalue references (const&) bind to both lvalues and rvalues
     - rvalue references (&&) bind only to rvalues (C++11)
  
  7. TEMPORARY LIFETIME EXTENSION:
     - Binding rvalue to const reference extends temporary's lifetime
     - Temporary lives as long as the reference exists
     - Important for function parameters and return values
  */
  
  // Lvalues - expressions that refer to memory locations
  int x = 10;
  int y = 20;
  int* ptr = &x;
  
  std::cout << "Lvalue Examples:" << std::endl;
  std::cout << "  x = " << x << " (variable name)" << std::endl;
  std::cout << "  *ptr = " << *ptr << " (dereferenced pointer)" << std::endl;
  std::cout << "  get_reference() = " << get_reference() << " (function returning reference)" << std::endl;
  
  // These are lvalues - they can appear on left side of assignment
  x = 30;           // OK: x is lvalue
  *ptr = 40;        // OK: *ptr is lvalue
  get_reference() = 50;  // OK: function returns reference (lvalue)
  
  std::cout << "  After assignments: x = " << x << ", *ptr = " << *ptr << ", get_reference() = " << get_reference() << std::endl;
  
  // Rvalues - temporary values that don't persist beyond the expression
  std::cout << "\nRvalue Examples:" << std::endl;
  std::cout << "  42 (literal)" << std::endl;
  std::cout << "  x + y = " << (x + y) << " (result of expression)" << std::endl;
  std::cout << "  get_value() = " << get_value() << " (function returning by value)" << std::endl;
  
  // These are rvalues - they cannot appear on left side of assignment
  // 42 = x;        // ERROR: cannot assign to literal
  // (x + y) = 100; // ERROR: cannot assign to temporary
  // get_value() = 200; // ERROR: cannot assign to temporary
  
  // Lvalue references can bind to lvalues
  int& lref1 = x;        // OK: x is lvalue
  int& lref2 = *ptr;     // OK: *ptr is lvalue
  // int& lref3 = 42;    // ERROR: cannot bind lvalue reference to rvalue
  // int& lref4 = x + y; // ERROR: cannot bind lvalue reference to rvalue
  
  // Const lvalue references can bind to both lvalues and rvalues
  const int& const_lref1 = x;        // OK: x is lvalue
  const int& const_lref2 = 42;       // OK: const ref can bind to rvalue
  const int& const_lref3 = x + y;    // OK: const ref can bind to rvalue
  const int& const_lref4 = get_value(); // OK: const ref can bind to rvalue
  
  std::cout << "\nConst lvalue references binding to rvalues:" << std::endl;
  std::cout << "  const_lref2 (bound to 42) = " << const_lref2 << std::endl;
  std::cout << "  const_lref3 (bound to x + y) = " << const_lref3 << std::endl;
  std::cout << "  const_lref4 (bound to get_value()) = " << const_lref4 << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_rvalue_references() {
  std::cout << "=== RVALUE REFERENCES (C++11) ===" << std::endl;
  
  /*
  RVALUE REFERENCE THEORY:
  
  1. MOTIVATION:
     - Enable move semantics for performance optimization
     - Distinguish between copying and moving resources
     - Perfect forwarding in template functions
     - Avoid unnecessary deep copies of expensive objects
  
  2. SYNTAX AND SEMANTICS:
     - Declared with double ampersand (&&): int&& rref
     - Can bind to rvalues (temporaries, literals, moved objects)
     - Cannot bind to lvalues directly
  
  3. RVALUE REFERENCE PARADOX:
     - Rvalue references themselves are lvalues when used as expressions
     - They have names and addresses, so they're lvalues
     - This enables multiple uses of the same rvalue reference
  
  4. STD::MOVE SEMANTICS:
     - std::move() performs static_cast<T&&>(arg)
     - Converts lvalue to xvalue (expiring value)
     - Doesn't actually move anything - just enables move semantics
     - Original object becomes "moved-from" (valid but unspecified state)
  
  5. MOVE SEMANTICS BENEFITS:
     - Transfer ownership instead of copying
     - O(1) operation instead of O(n) for containers
     - Reduces memory allocations and deallocations
     - Enables efficient return by value
  
  6. REFERENCE COLLAPSING RULES:
     - T& & → T&    (lvalue ref + lvalue ref = lvalue ref)
     - T& && → T&   (lvalue ref + rvalue ref = lvalue ref)
     - T&& & → T&   (rvalue ref + lvalue ref = lvalue ref)
     - T&& && → T&& (rvalue ref + rvalue ref = rvalue ref)
  
  7. PERFECT FORWARDING:
     - Preserve value category (lvalue/rvalue) in template functions
     - Use std::forward<T>() to conditionally cast
     - Essential for wrapper functions and constructors
  */
  
  // Rvalue references (&&) can bind to rvalues
  int&& rref1 = 42;           // OK: 42 is rvalue
  int&& rref2 = get_value();  // OK: get_value() returns rvalue
  
  int x = 10, y = 20;
  int&& rref3 = x + y;        // OK: x + y is rvalue
  
  std::cout << "Rvalue references:" << std::endl;
  std::cout << "  rref1 (bound to 42) = " << rref1 << std::endl;
  std::cout << "  rref2 (bound to get_value()) = " << rref2 << std::endl;
  std::cout << "  rref3 (bound to x + y) = " << rref3 << std::endl;
  
  // Rvalue references themselves are lvalues when used as expressions
  std::cout << "\nRvalue references are lvalues when used:" << std::endl;
  std::cout << "  Address of rref1: " << &rref1 << std::endl;
  rref1 = 100;  // Can assign to rvalue reference (it's an lvalue expression)
  std::cout << "  After rref1 = 100: rref1 = " << rref1 << std::endl;
  
  // std::move converts lvalue to rvalue
  int z = 300;
  // int&& rref4 = z;  // ERROR: z is lvalue
  int&& rref4 = std::move(z);  // OK: std::move converts z to rvalue
  std::cout << "\nUsing std::move:" << std::endl;
  std::cout << "  z = " << z << std::endl;
  std::cout << "  rref4 (bound to std::move(z)) = " << rref4 << std::endl;
  
  rref4 = 400;
  std::cout << "  After rref4 = 400: z = " << z << ", rref4 = " << rref4 << std::endl;
  
  std::cout << std::endl;
}

// Helper class for move semantics demonstration
class MoveExample {
private:
  int* data;
  size_t size;
  
public:
  // Constructor
  MoveExample(size_t s) : size(s) {
    data = new int[size];
    for (size_t i = 0; i < size; ++i) {
      data[i] = static_cast<int>(i);
    }
    std::cout << "    Constructor: allocated " << size << " integers" << std::endl;
  }
  
  // Copy constructor
  MoveExample(const MoveExample& other) : size(other.size) {
    data = new int[size];
    for (size_t i = 0; i < size; ++i) {
      data[i] = other.data[i];
    }
    std::cout << "    Copy constructor: copied " << size << " integers" << std::endl;
  }
  
  // Move constructor
  MoveExample(MoveExample&& other) noexcept : data(other.data), size(other.size) {
    other.data = nullptr;
    other.size = 0;
    std::cout << "    Move constructor: moved " << size << " integers" << std::endl;
  }
  
  // Copy assignment
  MoveExample& operator=(const MoveExample& other) {
    if (this != &other) {
      delete[] data;
      size = other.size;
      data = new int[size];
      for (size_t i = 0; i < size; ++i) {
        data[i] = other.data[i];
      }
      std::cout << "    Copy assignment: copied " << size << " integers" << std::endl;
    }
    return *this;
  }
  
  // Move assignment
  MoveExample& operator=(MoveExample&& other) noexcept {
    if (this != &other) {
      delete[] data;
      data = other.data;
      size = other.size;
      other.data = nullptr;
      other.size = 0;
      std::cout << "    Move assignment: moved " << size << " integers" << std::endl;
    }
    return *this;
  }
  
  // Destructor
  ~MoveExample() {
    if (data) {
      std::cout << "    Destructor: deallocated " << size << " integers" << std::endl;
    } else {
      std::cout << "    Destructor: moved-from object" << std::endl;
    }
    delete[] data;
  }
  
  size_t get_size() const { return size; }
  
  void print() const {
    if (data && size > 0) {
      std::cout << "    Data: ";
      for (size_t i = 0; i < std::min(size, size_t(5)); ++i) {
        std::cout << data[i] << " ";
      }
      if (size > 5) std::cout << "... (" << size << " total)";
      std::cout << std::endl;
    } else {
      std::cout << "    Data: empty (moved-from)" << std::endl;
    }
  }
};

MoveExample create_move_example(size_t size) {
  return MoveExample(size);  // Returns by value (potential move)
}

void demonstrate_move_semantics() {
  std::cout << "=== MOVE SEMANTICS ===" << std::endl;
  
  /*
  MOVE SEMANTICS THEORY:
  
  1. PROBLEM STATEMENT:
     - Traditional C++ copying can be expensive for resource-owning objects
     - Deep copying involves allocating new memory and copying data
     - Temporary objects are copied unnecessarily before destruction
     - Return by value was discouraged due to performance costs
  
  2. MOVE SEMANTICS SOLUTION:
     - Transfer ownership of resources instead of copying
     - Source object enters "moved-from" state (valid but unspecified)
     - Destination object takes ownership of resources
     - Source object's destructor should handle moved-from state safely
  
  3. MOVE CONSTRUCTOR:
     - Signature: T(T&& other) noexcept
     - Transfers resources from other to this
     - Sets other to safe-to-destroy state
     - Should be noexcept to enable optimizations
  
  4. MOVE ASSIGNMENT OPERATOR:
     - Signature: T& operator=(T&& other) noexcept
     - Releases current resources
     - Transfers resources from other
     - Handles self-assignment safely
  
  5. RULE OF FIVE:
     - If you define any of: destructor, copy constructor, copy assignment, move constructor, move assignment
     - You probably need to define all five
     - Modern alternative: Rule of Zero (use smart pointers/RAII)
  
  6. MOVED-FROM STATE:
     - Object must be in valid but unspecified state
     - Can be safely destroyed
     - Can be assigned new values
     - Should not be used for other operations
  
  7. AUTOMATIC MOVE OPPORTUNITIES:
     - Return by value from functions
     - Temporary objects as function arguments
     - Throwing and catching exceptions by value
     - Compiler optimizations (RVO, NRVO)
  
  8. PERFORMANCE IMPLICATIONS:
     - Move operations should be O(1) vs O(n) for copy
     - Reduces memory allocations
     - Enables efficient use of containers
     - Makes return by value viable again
  */
  
  std::cout << "1. Creating object:" << std::endl;
  MoveExample obj1(5);
  obj1.print();
  
  std::cout << "\n2. Copy construction:" << std::endl;
  MoveExample obj2 = obj1;  // Copy constructor
  obj2.print();
  
  std::cout << "\n3. Move construction:" << std::endl;
  MoveExample obj3 = std::move(obj1);  // Move constructor
  std::cout << "  obj1 after move:" << std::endl;
  obj1.print();
  std::cout << "  obj3 after move:" << std::endl;
  obj3.print();
  
  std::cout << "\n4. Copy assignment:" << std::endl;
  MoveExample obj4(2);
  obj4 = obj2;  // Copy assignment
  obj4.print();
  
  std::cout << "\n5. Move assignment:" << std::endl;
  obj4 = std::move(obj3);  // Move assignment
  std::cout << "  obj3 after move:" << std::endl;
  obj3.print();
  std::cout << "  obj4 after move:" << std::endl;
  obj4.print();
  
  std::cout << "\n6. Return value optimization (RVO):" << std::endl;
  MoveExample obj5 = create_move_example(3);  // May use RVO or move
  obj5.print();
  
  std::cout << "\n7. Function parameter (perfect forwarding context):" << std::endl;
  auto process_by_value = [](MoveExample obj) {
    std::cout << "    Inside function:" << std::endl;
    obj.print();
  };
  
  MoveExample temp_obj(2);
  std::cout << "  Calling with lvalue:" << std::endl;
  process_by_value(temp_obj);  // Copy
  std::cout << "  Calling with rvalue:" << std::endl;
  process_by_value(MoveExample(2));  // Move or RVO
  
  std::cout << std::endl;
}

// Function pointer examples
int add(int a, int b) {
  return a + b;
}

int multiply(int a, int b) {
  return a * b;
}

int subtract(int a, int b) {
  return a - b;
}

void process_array(int* arr, size_t size, int (*operation)(int)) {
  std::cout << "  Processing array with operation: ";
  for (size_t i = 0; i < size; ++i) {
    arr[i] = operation(arr[i]);
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;
}

void demonstrate_function_pointers() {
  std::cout << "=== FUNCTION POINTERS ===" << std::endl;
  
  /*
  FUNCTION POINTER THEORY:
  
  1. CONCEPTUAL FOUNDATION:
     - Functions are stored in memory like variables
     - Function name is a pointer to the function's memory location
     - Function pointers enable runtime polymorphism and callbacks
     - Foundation for function objects and lambda expressions
  
  2. SYNTAX AND DECLARATION:
     - return_type (*pointer_name)(parameter_types)
     - Parentheses around *pointer_name are crucial for precedence
     - Without parentheses: return_type* function_name(params) is different
  
  3. FUNCTION POINTER MECHANICS:
     - Function name implicitly converts to function pointer
     - Address-of operator (&) is optional but explicit
     - Dereferencing (*) is optional when calling
     - Function pointers can be stored in variables, arrays, structures
  
  4. APPLICATIONS:
     - Callback mechanisms
     - Event handling systems
     - Strategy pattern implementation
     - Plugin architectures
     - Polymorphism without inheritance
  
  5. TYPE SAFETY:
     - Function pointers are strongly typed
     - Must match exact signature (return type and parameters)
     - Compile-time checking prevents most errors
     - No implicit conversions between different function pointer types
  
  6. LIMITATIONS:
     - Cannot capture variables (unlike lambdas)
     - No state beyond function parameters
     - Syntax can be complex for complicated signatures
     - No overload resolution at assignment time
  
  7. ALTERNATIVE APPROACHES:
     - C++ std::function (type-erased callable wrapper)
     - Lambda expressions with captures
     - Function objects (functors)
     - Member function pointers
  */
  
  // Basic function pointer declaration and usage
  int (*operation)(int, int);  // Pointer to function taking two ints and returning int
  
  std::cout << "Basic function pointers:" << std::endl;
  operation = add;
  std::cout << "  add(5, 3) = " << operation(5, 3) << std::endl;
  
  operation = multiply;
  std::cout << "  multiply(5, 3) = " << operation(5, 3) << std::endl;
  
  operation = &subtract;  // & is optional but explicit
  std::cout << "  subtract(5, 3) = " << operation(5, 3) << std::endl;
  
  // Array of function pointers
  std::cout << "\nArray of function pointers:" << std::endl;
  int (*operations[])(int, int) = {add, multiply, subtract};
  const char* names[] = {"add", "multiply", "subtract"};
  
  for (int i = 0; i < 3; ++i) {
    std::cout << "  " << names[i] << "(10, 4) = " << operations[i](10, 4) << std::endl;
  }
  
  // Function pointer as parameter
  std::cout << "\nFunction pointer as parameter:" << std::endl;
  int arr[] = {1, 2, 3, 4, 5};
  
  auto double_value = [](int x) { return x * 2; };
  auto square_value = [](int x) { return x * x; };
  
  // Note: We need to convert lambdas to function pointers
  int (*double_ptr)(int) = double_value;
  int (*square_ptr)(int) = square_value;
  
  std::cout << "  Original: 1 2 3 4 5" << std::endl;
  process_array(arr, 5, double_ptr);
  
  int arr2[] = {1, 2, 3, 4, 5};
  process_array(arr2, 5, square_ptr);
  
  std::cout << std::endl;
}

// Class for member function pointer examples
class Calculator {
public:
  int value;
  
  Calculator(int v) : value(v) {}
  
  int add(int x) { return value + x; }
  int multiply(int x) { return value * x; }
  static int static_add(int a, int b) { return a + b; }
  
  void print() const { std::cout << "Calculator value: " << value << std::endl; }
};

void demonstrate_member_function_pointers() {
  std::cout << "=== MEMBER FUNCTION POINTERS ===" << std::endl;
  
  /*
  MEMBER FUNCTION POINTER THEORY:
  
  1. CONCEPTUAL DIFFERENCE:
     - Member functions require an object instance to be called
     - Member function pointers store offset within class vtable
     - Cannot be called without an object context
     - Different syntax from regular function pointers
  
  2. SYNTAX COMPLEXITY:
     - return_type (ClassName::*pointer_name)(parameters)
     - Requires class scope qualification
     - Special operators: .* and ->* for invocation
     - More complex than regular function pointers
  
  3. MEMBER FUNCTION MECHANICS:
     - Member functions have implicit 'this' parameter
     - Compiler adds 'this' as hidden first parameter
     - Virtual functions use vtable lookup
     - Non-virtual functions use direct addressing
  
  4. POINTER-TO-MEMBER OPERATORS:
     - .* operator: object.*member_function_ptr(args)
     - ->* operator: object_ptr->*member_function_ptr(args)
     - Different from . and -> for direct member access
  
  5. MEMBER DATA POINTERS:
     - Similar syntax: type ClassName::*pointer_name
     - Points to offset within object layout
     - Access via .* and ->* operators
     - Useful for generic programming and reflection
  
  6. STATIC MEMBER FUNCTIONS:
     - Behave like regular function pointers
     - No implicit 'this' parameter
     - Can be assigned to regular function pointers
     - Same syntax as non-member functions
  
  7. VIRTUAL FUNCTION CONSIDERATIONS:
     - Virtual function calls through member function pointers
     - Maintains polymorphic behavior
     - Runtime dispatch through vtable
     - Performance overhead similar to virtual calls
  
  8. PRACTICAL APPLICATIONS:
     - Generic algorithms on class members
     - Event systems and callbacks
     - Reflection and introspection
     - Command pattern implementation
  */
  
  Calculator calc(10);
  
  // Pointer to member function
  int (Calculator::*member_func)(int) = &Calculator::add;
  
  std::cout << "Member function pointers:" << std::endl;
  std::cout << "  calc.add(5) via pointer = " << (calc.*member_func)(5) << std::endl;
  
  member_func = &Calculator::multiply;
  std::cout << "  calc.multiply(5) via pointer = " << (calc.*member_func)(5) << std::endl;
  
  // Using with pointer to object
  Calculator* calc_ptr = &calc;
  std::cout << "  calc_ptr->multiply(3) via pointer = " << (calc_ptr->*member_func)(3) << std::endl;
  
  // Pointer to member data
  int Calculator::*member_data = &Calculator::value;
  std::cout << "\nMember data pointer:" << std::endl;
  std::cout << "  calc.value via pointer = " << calc.*member_data << std::endl;
  calc.*member_data = 20;
  std::cout << "  After setting to 20: " << calc.*member_data << std::endl;
  
  // Static member function pointer (behaves like regular function pointer)
  int (*static_func)(int, int) = &Calculator::static_add;
  std::cout << "\nStatic member function pointer:" << std::endl;
  std::cout << "  Calculator::static_add(7, 8) = " << static_func(7, 8) << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_std_function() {
  std::cout << "=== STD::FUNCTION (MODERN C++) ===" << std::endl;
  
  /*
  STD::FUNCTION THEORY:
  
  1. TYPE ERASURE CONCEPT:
     - std::function erases the specific type of callable
     - Provides uniform interface for different callable types
     - Enables storage of lambdas, functors, and function pointers together
     - Runtime polymorphism for callable objects
  
  2. CALLABLE REQUIREMENTS:
     - Must be callable with specified signature
     - Can be function pointers, lambdas, functors, bound functions
     - Supports member functions via std::bind
     - Type safety maintained through template signature
  
  3. PERFORMANCE CONSIDERATIONS:
     - Small function optimization for simple callables
     - May use heap allocation for large captures
     - Virtual call overhead for type-erased invocation
     - Generally slower than direct function calls
  
  4. ADVANTAGES OVER FUNCTION POINTERS:
     - Can store stateful objects (lambdas with captures)
     - Unified syntax for different callable types
     - Exception safety guarantees
     - Modern C++ idioms and standard library integration
  
  5. LAMBDA INTEGRATION:
     - Seamless storage of lambda expressions
     - Capture variables and state
     - Closures enable more flexible programming patterns
     - Replaces many use cases of function pointers
  
  6. STD::BIND FUNCTIONALITY:
     - Bind member functions to specific objects
     - Partial application of function arguments
     - Placeholder arguments (_1, _2, etc.)
     - Creates callable objects from non-callable expressions
  
  7. CONTAINER STORAGE:
     - Can store std::function objects in containers
     - Enables collections of heterogeneous callables
     - Dynamic dispatch at runtime
     - Useful for event systems and command patterns
  
  8. MODERN ALTERNATIVES:
     - Generic lambdas (C++14)
     - std::invoke (C++17)
     - Concepts for constrained templates (C++20)
     - Direct template parameters often preferred
  */
  
  // std::function can hold various callable objects
  std::cout << "std::function examples:" << std::endl;
  
  // Regular function
  std::function<int(int, int)> func1 = add;
  std::cout << "  Regular function: " << func1(3, 4) << std::endl;
  
  // Lambda
  std::function<int(int, int)> func2 = [](int a, int b) { return a - b; };
  std::cout << "  Lambda: " << func2(10, 3) << std::endl;
  
  // Function object (functor)
  struct Multiplier {
    int operator()(int a, int b) const { return a * b; }
  };
  
  std::function<int(int, int)> func3 = Multiplier{};
  std::cout << "  Function object: " << func3(4, 5) << std::endl;
  
  // Member function with std::bind
  Calculator calc(100);
  std::function<int(int)> func4 = std::bind(&Calculator::add, &calc, std::placeholders::_1);
  std::cout << "  Bound member function: " << func4(25) << std::endl;
  
  // Capturing lambda
  int multiplier = 3;
  std::function<int(int)> func5 = [multiplier](int x) { return x * multiplier; };
  std::cout << "  Capturing lambda: " << func5(7) << std::endl;
  
  // Using in containers
  std::cout << "\nstd::function in containers:" << std::endl;
  std::vector<std::function<int(int, int)>> operations = {
    add,
    [](int a, int b) { return a - b; },
    [](int a, int b) { return a * b; },
    [](int a, int b) { return a == 0 ? 0 : a / b; }
  };
  
  const char* op_names[] = {"add", "subtract", "multiply", "divide"};
  for (size_t i = 0; i < operations.size(); ++i) {
    std::cout << "  " << op_names[i] << "(12, 3) = " << operations[i](12, 3) << std::endl;
  }
  
  std::cout << std::endl;
}

// Template function to demonstrate perfect forwarding
template<typename T>
void universal_reference_demo(T&& param) {
  std::cout << "  Parameter type: ";
  if (std::is_lvalue_reference<T>::value) {
    std::cout << "lvalue reference" << std::endl;
  } else {
    std::cout << "rvalue reference" << std::endl;
  }
  
  // Perfect forwarding
  // forward_to_another(std::forward<T>(param));
}

void demonstrate_universal_references() {
  std::cout << "=== UNIVERSAL REFERENCES (FORWARDING REFERENCES) ===" << std::endl;
  
  /*
  UNIVERSAL REFERENCE THEORY:
  
  1. TEMPLATE ARGUMENT DEDUCTION:
     - T&& in template context has special deduction rules
     - Not always an rvalue reference despite && syntax
     - Deduction depends on the argument's value category
     - Enables perfect forwarding of arguments
  
  2. DEDUCTION RULES:
     - If argument is lvalue of type U, T deduced as U&
     - If argument is rvalue of type U, T deduced as U
     - Reference collapsing rules then apply
     - Results in either lvalue or rvalue reference
  
  3. REFERENCE COLLAPSING MECHANICS:
     - Multiple references collapse to single reference
     - Any lvalue reference in chain results in lvalue reference
     - Only rvalue + rvalue = rvalue reference
     - Compiler applies rules automatically
  
  4. PERFECT FORWARDING PURPOSE:
     - Preserve value category through function calls
     - Avoid unnecessary copies in wrapper functions
     - Enable efficient generic programming
     - Foundation for many standard library functions
  
  5. STD::FORWARD MECHANISM:
     - Conditionally casts argument to rvalue reference
     - If T is lvalue reference, returns lvalue reference
     - If T is not reference, returns rvalue reference
     - Preserves original value category
  
  6. FORWARDING REFERENCE CONDITIONS:
     - Must be in template parameter context
     - Must be exactly T&& (not const T&& or vector<T>&&)
     - Template parameter must be deduced
     - Auto&& also creates forwarding reference
  
  7. COMMON PATTERNS:
     - Factory functions and constructors
     - Wrapper functions that forward to other functions
     - Generic algorithms that preserve value categories
     - Standard library implementation (make_shared, etc.)
  
  8. PITFALLS AND GOTCHAS:
     - Easy to confuse with rvalue references
     - Requires understanding of template deduction
     - Can lead to unexpected template instantiations
     - Debugging templates can be challenging
  */
  
  std::cout << "Universal references in templates:" << std::endl;
  
  int x = 42;
  const int cx = 100;
  
  std::cout << "Calling with lvalue (x):" << std::endl;
  universal_reference_demo(x);  // T deduced as int&
  
  std::cout << "Calling with const lvalue (cx):" << std::endl;
  universal_reference_demo(cx);  // T deduced as const int&
  
  std::cout << "Calling with rvalue (42):" << std::endl;
  universal_reference_demo(42);  // T deduced as int
  
  std::cout << "Calling with rvalue (std::move(x)):" << std::endl;
  universal_reference_demo(std::move(x));  // T deduced as int
  
  std::cout << "\nReference collapsing rules:" << std::endl;
  std::cout << "  T& + & = T&   (lvalue ref + lvalue ref = lvalue ref)" << std::endl;
  std::cout << "  T& + && = T&  (lvalue ref + rvalue ref = lvalue ref)" << std::endl;
  std::cout << "  T&& + & = T&  (rvalue ref + lvalue ref = lvalue ref)" << std::endl;
  std::cout << "  T&& + && = T&&(rvalue ref + rvalue ref = rvalue ref)" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_type_deduction() {
  std::cout << "=== TYPE DEDUCTION EXAMPLES ===" << std::endl;
  
  auto a = 42;           // int
  auto b = 42.0;         // double
  auto c = "hello";      // const char*
  auto d = std::string("hello");  // std::string
  
  int x = 10;
  auto e = x;            // int (copy)
  auto& f = x;           // int& (reference)
  auto&& g = x;          // int& (universal reference with lvalue)
  auto&& h = 42;         // int&& (universal reference with rvalue)
  
  std::cout << "Type deduction with auto:" << std::endl;
  std::cout << "  auto a = 42 -> int" << std::endl;
  std::cout << "  auto b = 42.0 -> double" << std::endl;
  std::cout << "  auto c = \"hello\" -> const char*" << std::endl;
  std::cout << "  auto d = std::string(\"hello\") -> std::string" << std::endl;
  std::cout << "  auto e = x -> int (copy)" << std::endl;
  std::cout << "  auto& f = x -> int& (reference)" << std::endl;
  std::cout << "  auto&& g = x -> int& (universal ref with lvalue)" << std::endl;
  std::cout << "  auto&& h = 42 -> int&& (universal ref with rvalue)" << std::endl;
  
  // Using type traits to verify
  std::cout << "\nType verification:" << std::endl;
  std::cout << "  g is lvalue reference: " << std::is_lvalue_reference<decltype(g)>::value << std::endl;
  std::cout << "  h is rvalue reference: " << std::is_rvalue_reference<decltype(h)>::value << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_smart_pointers() {
  std::cout << "=== SMART POINTERS ===" << std::endl;
  
  /*
  SMART POINTER THEORY:
  
  1. RAII PRINCIPLE:
     - Resource Acquisition Is Initialization
     - Constructor acquires resource, destructor releases it
     - Exception safety through automatic cleanup
     - Eliminates manual memory management
  
  2. UNIQUE_PTR DESIGN:
     - Exclusive ownership semantics
     - Non-copyable but movable
     - Zero runtime overhead compared to raw pointers
     - Custom deleters for specialized cleanup
  
  3. SHARED_PTR IMPLEMENTATION:
     - Reference counting for shared ownership
     - Thread-safe reference counting
     - Control block stores count and deleter
     - Weak reference support to break cycles
  
  4. WEAK_PTR PURPOSE:
     - Non-owning observer of shared_ptr
     - Breaks circular reference cycles
     - Can detect if referenced object still exists
     - Must convert to shared_ptr for access
  
  5. MEMORY OVERHEAD:
     - unique_ptr: typically same size as raw pointer
     - shared_ptr: pointer + control block pointer
     - Control block: reference counts + deleter + allocator
     - make_shared optimizes by combining allocations
  
  6. THREAD SAFETY:
     - shared_ptr reference counting is thread-safe
     - Object access is not automatically thread-safe
     - Multiple threads can safely copy shared_ptr
     - Still need synchronization for object modification
  
  7. CUSTOM DELETERS:
     - Function objects that define cleanup behavior
     - Enable RAII for non-memory resources
     - File handles, network connections, etc.
     - Type-erased in shared_ptr, part of type in unique_ptr
  
  8. BEST PRACTICES:
     - Prefer make_unique and make_shared
     - Use unique_ptr by default
     - shared_ptr only when sharing is necessary
     - weak_ptr to break cycles and for caching
  */
  
  // unique_ptr
  std::cout << "unique_ptr:" << std::endl;
  std::unique_ptr<int> unique_ptr1 = std::make_unique<int>(42);
  std::cout << "  *unique_ptr1 = " << *unique_ptr1 << std::endl;
  
  // Transfer ownership
  std::unique_ptr<int> unique_ptr2 = std::move(unique_ptr1);
  std::cout << "  After move: unique_ptr1 is " << (unique_ptr1 ? "valid" : "null") << std::endl;
  std::cout << "  *unique_ptr2 = " << *unique_ptr2 << std::endl;
  
  // shared_ptr
  std::cout << "\nshared_ptr:" << std::endl;
  std::shared_ptr<int> shared_ptr1 = std::make_shared<int>(100);
  std::cout << "  Reference count: " << shared_ptr1.use_count() << std::endl;
  
  {
    std::shared_ptr<int> shared_ptr2 = shared_ptr1;  // Share ownership
    std::cout << "  Reference count after copy: " << shared_ptr1.use_count() << std::endl;
    std::cout << "  *shared_ptr2 = " << *shared_ptr2 << std::endl;
  }  // shared_ptr2 goes out of scope
  
  std::cout << "  Reference count after scope: " << shared_ptr1.use_count() << std::endl;
  
  // weak_ptr
  std::cout << "\nweak_ptr:" << std::endl;
  std::weak_ptr<int> weak_ptr1 = shared_ptr1;
  std::cout << "  weak_ptr expired: " << weak_ptr1.expired() << std::endl;
  
  if (auto locked = weak_ptr1.lock()) {
    std::cout << "  *locked from weak_ptr = " << *locked << std::endl;
  }
  
  shared_ptr1.reset();  // Release shared_ptr
  std::cout << "  After shared_ptr reset, weak_ptr expired: " << weak_ptr1.expired() << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_const_correctness() {
  std::cout << "=== CONST CORRECTNESS ===" << std::endl;
  
  /*
  CONST CORRECTNESS THEORY:
  
  1. CONST PHILOSOPHY:
     - Express immutability in the type system
     - Compiler enforcement of design intent
     - Self-documenting code through const annotations
     - Enables optimizations and prevents bugs
  
  2. TOP-LEVEL vs LOW-LEVEL CONST:
     - Top-level: const applies to the object itself
     - Low-level: const applies to pointed-to or referenced object
     - const int* ptr: low-level const (can't modify *ptr)
     - int* const ptr: top-level const (can't modify ptr)
  
  3. CONST PROPAGATION:
     - const objects can only call const member functions
     - const member functions cannot modify object state
     - mutable keyword bypasses const restrictions
     - const_cast can remove const (dangerous)
  
  4. CONST AND FUNCTION OVERLOADING:
     - const and non-const versions can coexist
     - Compiler selects based on object's constness
     - Common pattern for accessor methods
     - const version often returns const reference
  
  5. CONST CORRECTNESS BENEFITS:
     - Compile-time error prevention
     - Interface documentation through types
     - Optimization opportunities for compiler
     - Thread safety implications
  
  6. CONST REFERENCES:
     - Prevent copying of large objects
     - Can bind to both lvalues and rvalues
     - Extend lifetime of temporary objects
     - Standard pattern for function parameters
  
  7. CONST AND TEMPLATES:
     - Template argument deduction preserves const
     - const& is forwarded as const&
     - Perfect forwarding maintains const qualification
     - Template specialization for const types
  
  8. LOGICAL vs BITWISE CONST:
     - Bitwise const: no modification of any bit
     - Logical const: conceptual immutability
     - mutable enables logical const implementation
     - Cache invalidation and lazy evaluation patterns
  */
  
  int value = 42;
  const int const_value = 100;
  
  // Different types of const with pointers
  std::cout << "Const with pointers:" << std::endl;
  
  // Pointer to const int
  const int* ptr1 = &value;
  // *ptr1 = 50;  // ERROR: cannot modify through pointer to const
  ptr1 = &const_value;  // OK: can change what pointer points to
  std::cout << "  const int* ptr1 = " << *ptr1 << std::endl;
  
  // Const pointer to int
  int* const ptr2 = &value;
  *ptr2 = 50;  // OK: can modify value
  // ptr2 = &const_value;  // ERROR: cannot change const pointer
  std::cout << "  int* const ptr2 = " << *ptr2 << std::endl;
  
  // Const pointer to const int
  const int* const ptr3 = &const_value;
  // *ptr3 = 60;  // ERROR: cannot modify value
  // ptr3 = &value;  // ERROR: cannot change pointer
  std::cout << "  const int* const ptr3 = " << *ptr3 << std::endl;
  
  // Const with references
  std::cout << "\nConst with references:" << std::endl;
  const int& const_ref = value;
  // const_ref = 70;  // ERROR: cannot modify through const reference
  std::cout << "  const int& const_ref = " << const_ref << std::endl;
  
  // Top-level const vs low-level const
  std::cout << "\nTop-level vs low-level const:" << std::endl;
  std::cout << "  Top-level const: affects the object itself" << std::endl;
  std::cout << "  Low-level const: affects the pointed-to/referenced object" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_memory_layout() {
  std::cout << "=== MEMORY LAYOUT AND ADDRESSES ===" << std::endl;
  
  /*
  MEMORY LAYOUT THEORY:
  
  1. VIRTUAL MEMORY MODEL:
     - Each process has its own virtual address space
     - Operating system maps virtual to physical addresses
     - Memory protection and isolation between processes
     - Address space layout randomization (ASLR) for security
  
  2. MEMORY SEGMENTS:
     - Code Segment: Executable instructions (read-only)
     - Data Segment: Initialized global/static variables
     - BSS Segment: Uninitialized global/static variables
     - Heap: Dynamic allocation (malloc, new)
     - Stack: Local variables, function calls
  
  3. STACK CHARACTERISTICS:
     - Grows downward (higher to lower addresses)
     - Automatic storage duration
     - Function call frames and local variables
     - Limited size (typically 1-8 MB)
     - Fast allocation and deallocation
  
  4. HEAP CHARACTERISTICS:
     - Grows upward (lower to higher addresses)
     - Dynamic storage duration
     - Managed by allocator (malloc/free, new/delete)
     - Virtually unlimited size (system memory)
     - Slower allocation, potential fragmentation
  
  5. POINTER ARITHMETIC MECHANICS:
     - Addition/subtraction scaled by pointed-to type size
     - Only valid within same array or memory block
     - Undefined behavior beyond object boundaries
     - Comparison operators work within same array
  
  6. ALIGNMENT CONSIDERATIONS:
     - Hardware requires certain data alignment
     - Compiler adds padding for alignment
     - sizeof() includes padding
     - alignof() operator returns alignment requirement
  
  7. ENDIANNESS:
     - Big-endian: most significant byte first
     - Little-endian: least significant byte first
     - Affects multi-byte data interpretation
     - Network byte order vs. host byte order
  
  8. CACHE LOCALITY:
     - Spatial locality: nearby data accessed together
     - Temporal locality: same data accessed repeatedly
     - Array traversal benefits from cache line optimization
     - Structure layout affects cache performance
  */
  
  int a = 10, b = 20, c = 30;
  int* ptr_a = &a;
  int* ptr_b = &b;
  
  std::cout << "Stack variables:" << std::endl;
  std::cout << "  a = " << a << " at address " << &a << std::endl;
  std::cout << "  b = " << b << " at address " << &b << std::endl;
  std::cout << "  c = " << c << " at address " << &c << std::endl;
  
  std::cout << "\nPointers:" << std::endl;
  std::cout << "  ptr_a = " << ptr_a << " (points to a)" << std::endl;
  std::cout << "  ptr_b = " << ptr_b << " (points to b)" << std::endl;
  std::cout << "  Address of ptr_a = " << &ptr_a << std::endl;
  std::cout << "  Address of ptr_b = " << &ptr_b << std::endl;
  
  // Heap allocation
  int* heap_ptr = new int(100);
  std::cout << "\nHeap allocation:" << std::endl;
  std::cout << "  *heap_ptr = " << *heap_ptr << " at address " << heap_ptr << std::endl;
  std::cout << "  Address of heap_ptr = " << &heap_ptr << std::endl;
  
  delete heap_ptr;  // Don't forget to deallocate
  
  // Array memory layout
  int arr[] = {1, 2, 3, 4, 5};
  std::cout << "\nArray memory layout:" << std::endl;
  for (int i = 0; i < 5; ++i) {
    std::cout << "  arr[" << i << "] = " << arr[i] << " at address " << &arr[i] << std::endl;
  }
  
  std::cout << "  Address difference between consecutive elements: " 
            << (reinterpret_cast<char*>(&arr[1]) - reinterpret_cast<char*>(&arr[0])) 
            << " bytes" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_theoretical_summary() {
  std::cout << "=== THEORETICAL SUMMARY AND BEST PRACTICES ===" << std::endl;
  
  /*
  COMPREHENSIVE THEORY SYNTHESIS:
  
  1. FUNDAMENTAL CONCEPTS HIERARCHY:
     - Memory addresses are the foundation of all indirection
     - Pointers provide explicit indirection with full control
     - References provide implicit indirection with safety guarantees
     - Value categories (lvalue/rvalue) determine usage patterns
     - Move semantics optimize resource transfer
  
  2. EVOLUTION OF C++ INDIRECTION:
     - C-style pointers: Manual memory management, full control
     - C++ references: Safer alternative with aliasing semantics
     - C++11 rvalue references: Enable move semantics and perfect forwarding
     - C++11 smart pointers: RAII-based automatic memory management
     - C++14/17/20: Further refinements and utilities
  
  3. DESIGN PATTERNS AND IDIOMS:
     - RAII: Constructor acquires, destructor releases
     - Rule of Zero: Prefer smart pointers over manual management
     - Rule of Five: If you manage resources, define all five operations
     - Perfect forwarding: Preserve value categories in templates
     - Type erasure: std::function for runtime polymorphism
  
  4. PERFORMANCE CONSIDERATIONS:
     - Stack allocation: Fast, automatic cleanup, limited size
     - Heap allocation: Flexible size, manual management, potential fragmentation
     - Move semantics: O(1) transfer vs O(n) copy operations
     - Cache locality: Sequential access patterns optimize performance
     - Compiler optimizations: RVO, inlining, dead code elimination
  
  5. SAFETY AND CORRECTNESS:
     - const correctness: Express immutability in type system
     - Smart pointers: Prevent leaks and dangling pointers
     - References: Cannot be null, cannot be reassigned
     - Template argument deduction: Type safety with generics
     - RAII: Exception safety through automatic cleanup
  
  6. MODERN C++ RECOMMENDATIONS:
     - Prefer smart pointers over raw pointers for ownership
     - Use references for non-owning access
     - Apply const correctness throughout codebase
     - Leverage move semantics for performance
     - Use std::function for type-erased callables
     - Apply perfect forwarding in generic code
  */
  
  std::cout << "Key Takeaways:" << std::endl;
  std::cout << "1. Pointers provide explicit control over memory and indirection" << std::endl;
  std::cout << "2. References offer safer alternatives for aliasing and parameter passing" << std::endl;
  std::cout << "3. Value categories (lvalue/rvalue) enable move semantics optimization" << std::endl;
  std::cout << "4. Smart pointers combine RAII with automatic memory management" << std::endl;
  std::cout << "5. Function pointers enable callbacks and runtime polymorphism" << std::endl;
  std::cout << "6. Universal references and perfect forwarding preserve value categories" << std::endl;
  std::cout << "7. Const correctness expresses design intent and enables optimizations" << std::endl;
  std::cout << "8. Memory layout understanding helps with performance optimization" << std::endl;
  
  std::cout << "\nBest Practices Summary:" << std::endl;
  std::cout << "- Use unique_ptr for exclusive ownership" << std::endl;
  std::cout << "- Use shared_ptr only when sharing is necessary" << std::endl;
  std::cout << "- Prefer references over pointers for non-owning access" << std::endl;
  std::cout << "- Apply const wherever possible" << std::endl;
  std::cout << "- Use std::move for transferring ownership" << std::endl;
  std::cout << "- Leverage perfect forwarding in generic functions" << std::endl;
  std::cout << "- Follow RAII principles for resource management" << std::endl;
  std::cout << "- Understand value categories for optimal performance" << std::endl;
  
  std::cout << std::endl;
}

int main() {
  std::cout << "C++ POINTERS, REFERENCES, AND RELATED CONSTRUCTS TUTORIAL" << std::endl;
  std::cout << "==========================================================" << std::endl << std::endl;
  
  demonstrate_basic_pointers();
  demonstrate_basic_references();
  demonstrate_pointers_vs_references();
  demonstrate_lvalue_rvalue();
  demonstrate_rvalue_references();
  demonstrate_move_semantics();
  demonstrate_function_pointers();
  demonstrate_member_function_pointers();
  demonstrate_std_function();
  demonstrate_universal_references();
  demonstrate_type_deduction();
  demonstrate_smart_pointers();
  demonstrate_const_correctness();
  demonstrate_memory_layout();
  demonstrate_theoretical_summary();
  
  std::cout << "Tutorial completed successfully!" << std::endl;
  return 0;
}