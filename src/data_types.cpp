/**
 * @file data_types.cpp
 * @author GitHub Copilot
 * @brief Tutorial code for C++ primitive types, structs, unions, and data structures.
 */

// This program provides a comprehensive overview of C++ data types including:
// - Primitive/fundamental types
// - Structs and classes
// - Unions and enums
// - Type modifiers and qualifiers
// - Memory layout and alignment

#include <iostream>
#include <string>
#include <climits>    // For type limits
#include <cfloat>     // For floating point limits
#include <cstdint>    // For fixed-width integer types
#include <memory>     // For smart pointers
#include <vector>     // For std::vector
#include <cmath>      // For sqrt function
#include <cstddef>    // For offsetof

void demonstrate_primitive_types() {
  std::cout << "=== PRIMITIVE/FUNDAMENTAL TYPES ===" << std::endl;
  
  // Integer types
  std::cout << "--- Integer Types ---" << std::endl;
  
  // Basic integer types
  char c = 'A';
  short s = 32000;
  int i = 2147483647;
  long l = 9223372036854775807L;
  long long ll = 9223372036854775807LL;
  
  std::cout << "char: " << c << " (size: " << sizeof(char) << " byte)" << std::endl;
  std::cout << "short: " << s << " (size: " << sizeof(short) << " bytes)" << std::endl;
  std::cout << "int: " << i << " (size: " << sizeof(int) << " bytes)" << std::endl;
  std::cout << "long: " << l << " (size: " << sizeof(long) << " bytes)" << std::endl;
  std::cout << "long long: " << ll << " (size: " << sizeof(long long) << " bytes)" << std::endl;
  
  // Unsigned variants
  std::cout << "\n--- Unsigned Integer Types ---" << std::endl;
  unsigned char uc = 255;
  unsigned short us = 65535;
  unsigned int ui = 4294967295U;
  unsigned long ul = 18446744073709551615UL;
  unsigned long long ull = 18446744073709551615ULL;
  
  std::cout << "unsigned char: " << static_cast<int>(uc) << " (size: " << sizeof(unsigned char) << " byte)" << std::endl;
  std::cout << "unsigned short: " << us << " (size: " << sizeof(unsigned short) << " bytes)" << std::endl;
  std::cout << "unsigned int: " << ui << " (size: " << sizeof(unsigned int) << " bytes)" << std::endl;
  std::cout << "unsigned long: " << ul << " (size: " << sizeof(unsigned long) << " bytes)" << std::endl;
  std::cout << "unsigned long long: " << ull << " (size: " << sizeof(unsigned long long) << " bytes)" << std::endl;
  
  // Fixed-width integer types (C++11)
  std::cout << "\n--- Fixed-Width Integer Types (C++11) ---" << std::endl;
  int8_t i8 = 127;
  int16_t i16 = 32767;
  int32_t i32 = 2147483647;
  int64_t i64 = 9223372036854775807LL;
  
  uint8_t ui8 = 255;
  uint16_t ui16 = 65535;
  uint32_t ui32 = 4294967295U;
  uint64_t ui64 = 18446744073709551615ULL;
  
  std::cout << "int8_t: " << static_cast<int>(i8) << " (size: " << sizeof(int8_t) << " byte)" << std::endl;
  std::cout << "int16_t: " << i16 << " (size: " << sizeof(int16_t) << " bytes)" << std::endl;
  std::cout << "int32_t: " << i32 << " (size: " << sizeof(int32_t) << " bytes)" << std::endl;
  std::cout << "int64_t: " << i64 << " (size: " << sizeof(int64_t) << " bytes)" << std::endl;
  
  std::cout << "uint8_t: " << static_cast<int>(ui8) << " (size: " << sizeof(uint8_t) << " byte)" << std::endl;
  std::cout << "uint16_t: " << ui16 << " (size: " << sizeof(uint16_t) << " bytes)" << std::endl;
  std::cout << "uint32_t: " << ui32 << " (size: " << sizeof(uint32_t) << " bytes)" << std::endl;
  std::cout << "uint64_t: " << ui64 << " (size: " << sizeof(uint64_t) << " bytes)" << std::endl;
  
  // Floating point types
  std::cout << "\n--- Floating Point Types ---" << std::endl;
  float f = 3.14159f;
  double d = 3.141592653589793;
  long double ld = 3.141592653589793238L;
  
  std::cout << "float: " << f << " (size: " << sizeof(float) << " bytes)" << std::endl;
  std::cout << "double: " << d << " (size: " << sizeof(double) << " bytes)" << std::endl;
  std::cout << "long double: " << ld << " (size: " << sizeof(long double) << " bytes)" << std::endl;
  
  // Boolean type
  std::cout << "\n--- Boolean Type ---" << std::endl;
  bool b1 = true;
  bool b2 = false;
  
  std::cout << "bool (true): " << b1 << " (size: " << sizeof(bool) << " byte)" << std::endl;
  std::cout << "bool (false): " << b2 << " (size: " << sizeof(bool) << " byte)" << std::endl;
  
  // Character types
  std::cout << "\n--- Character Types ---" << std::endl;
  char ch = 'A';
  wchar_t wch = L'A';
  char16_t ch16 = u'A';
  char32_t ch32 = U'A';
  
  std::cout << "char: " << ch << " (size: " << sizeof(char) << " byte)" << std::endl;
  std::cout << "wchar_t: " << static_cast<int>(wch) << " (size: " << sizeof(wchar_t) << " bytes)" << std::endl;
  std::cout << "char16_t: " << static_cast<int>(ch16) << " (size: " << sizeof(char16_t) << " bytes)" << std::endl;
  std::cout << "char32_t: " << static_cast<int>(ch32) << " (size: " << sizeof(char32_t) << " bytes)" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_type_limits() {
  std::cout << "=== TYPE LIMITS AND RANGES ===" << std::endl;
  
  std::cout << "--- Integer Type Limits ---" << std::endl;
  std::cout << "char: " << static_cast<int>(CHAR_MIN) << " to " << static_cast<int>(CHAR_MAX) << std::endl;
  std::cout << "short: " << SHRT_MIN << " to " << SHRT_MAX << std::endl;
  std::cout << "int: " << INT_MIN << " to " << INT_MAX << std::endl;
  std::cout << "long: " << LONG_MIN << " to " << LONG_MAX << std::endl;
  std::cout << "long long: " << LLONG_MIN << " to " << LLONG_MAX << std::endl;
  
  std::cout << "\n--- Unsigned Integer Type Limits ---" << std::endl;
  std::cout << "unsigned char: 0 to " << static_cast<int>(UCHAR_MAX) << std::endl;
  std::cout << "unsigned short: 0 to " << USHRT_MAX << std::endl;
  std::cout << "unsigned int: 0 to " << UINT_MAX << std::endl;
  std::cout << "unsigned long: 0 to " << ULONG_MAX << std::endl;
  std::cout << "unsigned long long: 0 to " << ULLONG_MAX << std::endl;
  
  std::cout << "\n--- Floating Point Type Limits ---" << std::endl;
  std::cout << "float: " << FLT_MIN << " to " << FLT_MAX << " (precision: " << FLT_DIG << " digits)" << std::endl;
  std::cout << "double: " << DBL_MIN << " to " << DBL_MAX << " (precision: " << DBL_DIG << " digits)" << std::endl;
  std::cout << "long double: " << LDBL_MIN << " to " << LDBL_MAX << " (precision: " << LDBL_DIG << " digits)" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_type_modifiers() {
  std::cout << "=== TYPE MODIFIERS AND QUALIFIERS ===" << std::endl;
  
  // Storage class specifiers
  std::cout << "--- Storage Class Specifiers ---" << std::endl;
  static int static_var = 42;        // Static storage duration
  extern int extern_var;             // External linkage (declared elsewhere)
  
  std::cout << "static int: " << static_var << " (persistent across function calls)" << std::endl;
  std::cout << "extern int: declared but defined elsewhere" << std::endl;
  
  // Type qualifiers
  std::cout << "\n--- Type Qualifiers ---" << std::endl;
  const int const_var = 100;         // Cannot be modified
  volatile int volatile_var = 200;   // Can be changed by external factors
  
  std::cout << "const int: " << const_var << " (cannot be modified)" << std::endl;
  std::cout << "volatile int: " << volatile_var << " (can change unexpectedly)" << std::endl;
  
  // Pointer and reference types
  std::cout << "\n--- Pointer and Reference Types ---" << std::endl;
  int value = 42;
  int* ptr = &value;                  // Pointer to int
  int& ref = value;                   // Reference to int
  const int* const_ptr = &value;     // Pointer to const int
  int* const ptr_const = &value;     // Const pointer to int
  
  std::cout << "int value: " << value << std::endl;
  std::cout << "int* ptr: " << *ptr << " (points to value)" << std::endl;
  std::cout << "int& ref: " << ref << " (reference to value)" << std::endl;
  std::cout << "const int* const_ptr: " << *const_ptr << " (pointer to const int)" << std::endl;
  std::cout << "int* const ptr_const: " << *ptr_const << " (const pointer to int)" << std::endl;
  
  // Array types
  std::cout << "\n--- Array Types ---" << std::endl;
  int arr[5] = {1, 2, 3, 4, 5};
  int matrix[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  
  std::cout << "int arr[5]: ";
  for (int i = 0; i < 5; ++i) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;
  
  std::cout << "int matrix[3][3]: " << std::endl;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << std::endl;
  }
  
  std::cout << std::endl;
}

// Struct definition
struct Point {
  int x;
  int y;
  
  // Constructor
  Point(int x_val = 0, int y_val = 0) : x(x_val), y(y_val) {}
  
  // Member function
  double distance_from_origin() const {
    return sqrt(x * x + y * y);
  }
};

// Struct with different data types
struct Person {
  std::string name;
  int age;
  double height;
  bool is_student;
  
  // Constructor
  Person(const std::string& n, int a, double h, bool student) 
    : name(n), age(a), height(h), is_student(student) {}
  
  // Member function
  void display() const {
    std::cout << "Name: " << name << ", Age: " << age 
              << ", Height: " << height << "m, Student: " << (is_student ? "Yes" : "No") << std::endl;
  }
};

void demonstrate_structs() {
  std::cout << "=== STRUCTS ===" << std::endl;
  
  // Basic struct usage
  std::cout << "--- Basic Struct Usage ---" << std::endl;
  Point p1;                    // Default constructor
  Point p2(3, 4);             // Parameterized constructor
  
  std::cout << "Point p1: (" << p1.x << ", " << p1.y << ")" << std::endl;
  std::cout << "Point p2: (" << p2.x << ", " << p2.y << ")" << std::endl;
  std::cout << "Distance from origin: " << p2.distance_from_origin() << std::endl;
  
  // Struct with mixed data types
  std::cout << "\n--- Struct with Mixed Data Types ---" << std::endl;
  Person person1("Alice", 25, 1.65, true);
  Person person2("Bob", 30, 1.80, false);
  
  person1.display();
  person2.display();
  
  // Struct size and alignment
  std::cout << "\n--- Struct Size and Alignment ---" << std::endl;
  std::cout << "Size of Point: " << sizeof(Point) << " bytes" << std::endl;
  std::cout << "Size of Person: " << sizeof(Person) << " bytes" << std::endl;
  
  // Array of structs
  std::cout << "\n--- Array of Structs ---" << std::endl;
  Point points[3] = {{1, 2}, {3, 4}, {5, 6}};
  
  for (int i = 0; i < 3; ++i) {
    std::cout << "Point " << i << ": (" << points[i].x << ", " << points[i].y << ")" << std::endl;
  }
  
  std::cout << std::endl;
}

// Union definition
union Data {
  int i;
  float f;
  char c[4];
  
  // Constructor
  Data(int value) : i(value) {}
  Data(float value) : f(value) {}
};

// Union with struct
union FlexibleData {
  struct {
    short low;
    short high;
  } parts;
  int whole;
};

void demonstrate_unions() {
  std::cout << "=== UNIONS ===" << std::endl;
  
  // Basic union usage
  std::cout << "--- Basic Union Usage ---" << std::endl;
  Data data1(42);
  std::cout << "Union with int 42: " << data1.i << std::endl;
  std::cout << "Same memory as float: " << data1.f << std::endl;
  std::cout << "Same memory as char array: ";
  for (int i = 0; i < 4; ++i) {
    std::cout << static_cast<int>(data1.c[i]) << " ";
  }
  std::cout << std::endl;
  
  Data data2(3.14f);
  std::cout << "\nUnion with float 3.14: " << data2.f << std::endl;
  std::cout << "Same memory as int: " << data2.i << std::endl;
  
  // Union size
  std::cout << "\n--- Union Size ---" << std::endl;
  std::cout << "Size of Data union: " << sizeof(Data) << " bytes" << std::endl;
  std::cout << "Size of int: " << sizeof(int) << " bytes" << std::endl;
  std::cout << "Size of float: " << sizeof(float) << " bytes" << std::endl;
  std::cout << "Size of char[4]: " << sizeof(char[4]) << " bytes" << std::endl;
  
  // Union with struct
  std::cout << "\n--- Union with Struct ---" << std::endl;
  FlexibleData flex;
  flex.whole = 0x12345678;
  std::cout << "Whole value: 0x" << std::hex << flex.whole << std::dec << std::endl;
  std::cout << "Low part: 0x" << std::hex << flex.parts.low << std::dec << std::endl;
  std::cout << "High part: 0x" << std::hex << flex.parts.high << std::dec << std::endl;
  
  std::cout << std::endl;
}

// Enum definitions
enum Color {
  RED,
  GREEN,
  BLUE
};

enum class Status {
  PENDING,
  PROCESSING,
  COMPLETED,
  FAILED
};

// Enum with explicit values
enum Priority : int {
  LOW = 1,
  MEDIUM = 5,
  HIGH = 10,
  CRITICAL = 20
};

void demonstrate_enums() {
  std::cout << "=== ENUMS ===" << std::endl;
  
  // Basic enum
  std::cout << "--- Basic Enum ---" << std::endl;
  Color c1 = RED;
  Color c2 = GREEN;
  Color c3 = BLUE;
  
  std::cout << "RED: " << c1 << std::endl;
  std::cout << "GREEN: " << c2 << std::endl;
  std::cout << "BLUE: " << c3 << std::endl;
  
  // Scoped enum (enum class)
  std::cout << "\n--- Scoped Enum (enum class) ---" << std::endl;
  Status s1 = Status::PENDING;
  Status s2 = Status::COMPLETED;
  
  std::cout << "PENDING: " << static_cast<int>(s1) << std::endl;
  std::cout << "COMPLETED: " << static_cast<int>(s2) << std::endl;
  
  // Enum with explicit values
  std::cout << "\n--- Enum with Explicit Values ---" << std::endl;
  Priority p1 = LOW;
  Priority p2 = HIGH;
  
  std::cout << "LOW: " << p1 << std::endl;
  std::cout << "HIGH: " << p2 << std::endl;
  
  // Enum size
  std::cout << "\n--- Enum Sizes ---" << std::endl;
  std::cout << "Size of Color: " << sizeof(Color) << " bytes" << std::endl;
  std::cout << "Size of Status: " << sizeof(Status) << " bytes" << std::endl;
  std::cout << "Size of Priority: " << sizeof(Priority) << " bytes" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_typedef_and_using() {
  std::cout << "=== TYPEDEF AND USING DECLARATIONS ===" << std::endl;
  
  // typedef (C-style)
  typedef int Integer;
  typedef unsigned long long ULL;
  typedef Point* PointPtr;
  
  std::cout << "--- typedef (C-style) ---" << std::endl;
  Integer num = 42;
  ULL big_num = 18446744073709551615ULL;
  Point p(1, 2);
  PointPtr ptr = &p;
  
  std::cout << "Integer (typedef int): " << num << std::endl;
  std::cout << "ULL (typedef unsigned long long): " << big_num << std::endl;
  std::cout << "PointPtr (typedef Point*): (" << ptr->x << ", " << ptr->y << ")" << std::endl;
  
  // using (C++11 style)
  using Real = double;
  using StringVector = std::vector<std::string>;
  using PersonPtr = std::unique_ptr<Person>;
  
  std::cout << "\n--- using (C++11 style) ---" << std::endl;
  Real pi = 3.14159;
  StringVector names = {"Alice", "Bob", "Charlie"};
  PersonPtr person_ptr = std::make_unique<Person>("David", 28, 1.75, false);
  
  std::cout << "Real (using double): " << pi << std::endl;
  std::cout << "StringVector: ";
  for (const auto& name : names) {
    std::cout << name << " ";
  }
  std::cout << std::endl;
  std::cout << "PersonPtr: ";
  person_ptr->display();
  
  std::cout << std::endl;
}

void demonstrate_memory_layout() {
  std::cout << "=== MEMORY LAYOUT AND ALIGNMENT ===" << std::endl;
  
  // Memory addresses
  std::cout << "--- Memory Addresses ---" << std::endl;
  int a = 10;
  int b = 20;
  int c = 30;
  
  std::cout << "Variable a (value " << a << ") at address: " << &a << std::endl;
  std::cout << "Variable b (value " << b << ") at address: " << &b << std::endl;
  std::cout << "Variable c (value " << c << ") at address: " << &c << std::endl;
  
  // Struct padding
  std::cout << "\n--- Struct Padding ---" << std::endl;
  struct PaddedStruct {
    char c;      // 1 byte
    int i;       // 4 bytes (aligned)
    char c2;     // 1 byte
    double d;    // 8 bytes (aligned)
  };
  
  PaddedStruct ps;
  std::cout << "PaddedStruct size: " << sizeof(PaddedStruct) << " bytes" << std::endl;
  std::cout << "char c at offset: " << offsetof(PaddedStruct, c) << std::endl;
  std::cout << "int i at offset: " << offsetof(PaddedStruct, i) << std::endl;
  std::cout << "char c2 at offset: " << offsetof(PaddedStruct, c2) << std::endl;
  std::cout << "double d at offset: " << offsetof(PaddedStruct, d) << std::endl;
  
  // Packed struct (compiler-specific)
  std::cout << "\n--- Packed Struct (using pragma pack) ---" << std::endl;
  #pragma pack(push, 1)
  struct PackedStruct {
    char c;      // 1 byte
    int i;       // 4 bytes (no padding)
    char c2;     // 1 byte
    double d;    // 8 bytes (no padding)
  };
  #pragma pack(pop)
  
  std::cout << "PackedStruct size: " << sizeof(PackedStruct) << " bytes" << std::endl;
  
  std::cout << std::endl;
}

int main() {
  std::cout << "C++ DATA TYPES AND STRUCTURES TUTORIAL" << std::endl;
  std::cout << "=======================================" << std::endl << std::endl;
  
  demonstrate_primitive_types();
  demonstrate_type_limits();
  demonstrate_type_modifiers();
  demonstrate_structs();
  demonstrate_unions();
  demonstrate_enums();
  demonstrate_typedef_and_using();
  demonstrate_memory_layout();
  
  std::cout << "Tutorial completed successfully!" << std::endl;
  return 0;
}