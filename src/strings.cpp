/**
 * @file strings.cpp
 * @author GitHub Copilot
 * @brief Tutorial code for C++ strings and character arrays.
 */

// This program provides a comprehensive overview of C++ strings and character arrays.
// It covers C-style strings (character arrays), std::string class, and various
// string manipulation operations that are commonly used in C++ programming.

#include <iostream>
#include <string>
#include <cstring>  // For C-style string functions
#include <algorithm>
#include <sstream>
#include <vector>
#include <typeinfo>  // For type information
#include <cctype>    // For character classification functions
#include <stack>     // For stack-based reversal
#include <functional> // For std::function

void demonstrate_character_types() {
  std::cout << "=== CHARACTER TYPES AND PROPERTIES ===" << std::endl;
  
  std::string str = "Hello";
  char cstr[] = "World";
  
  // Get single characters
  char c1 = str[0];        // From std::string
  char c2 = cstr[0];       // From C-style string
  auto c3 = str[1];        // Using auto (will be char)
  
  std::cout << "Working with string: '" << str << "'" << std::endl;
  std::cout << "Working with C-string: '" << cstr << "'" << std::endl << std::endl;
  
  // Type information
  std::cout << "--- Type Information ---" << std::endl;
  std::cout << "Type of std::string element: char" << std::endl;
  std::cout << "Type of C-string element: char" << std::endl;
  std::cout << "Type when using 'auto': char (auto-deduced)" << std::endl;
  
  // Size information
  std::cout << "\n--- Size Information ---" << std::endl;
  std::cout << "Size of char: " << sizeof(char) << " byte(s)" << std::endl;
  std::cout << "Size of str[0]: " << sizeof(str[0]) << " byte(s)" << std::endl;
  std::cout << "Size of cstr[0]: " << sizeof(cstr[0]) << " byte(s)" << std::endl;
  
  // Value demonstration
  std::cout << "\n--- Character Values and ASCII ---" << std::endl;
  std::cout << "str[0] = '" << c1 << "' has ASCII value: " << static_cast<int>(c1) << std::endl;
  std::cout << "cstr[0] = '" << c2 << "' has ASCII value: " << static_cast<int>(c2) << std::endl;
  std::cout << "str[1] = '" << c3 << "' has ASCII value: " << static_cast<int>(c3) << std::endl;
  
  // ASCII conversion methods comparison
  std::cout << "\n--- ASCII Conversion Methods ---" << std::endl;
  char test_char = 'Z';
  std::cout << "Converting character '" << test_char << "' to ASCII:" << std::endl;
  
  // Method 1: static_cast (RECOMMENDED)
  int ascii1 = static_cast<int>(test_char);
  std::cout << "static_cast<int>('" << test_char << "') = " << ascii1 << " (recommended)" << std::endl;
  
  // Method 2: C-style cast
  int ascii2 = (int)test_char;
  std::cout << "(int)('" << test_char << "') = " << ascii2 << " (works but less preferred)" << std::endl;
  
  // Method 3: Implicit conversion
  int ascii3 = test_char;
  std::cout << "int ascii = '" << test_char << "' = " << ascii3 << " (implicit, less clear)" << std::endl;
  
  // Method 4: Using + operator trick
  int ascii4 = test_char + 0;
  std::cout << "'" << test_char << "' + 0 = " << ascii4 << " (arithmetic trick)" << std::endl;
  
  // Demonstrating with string elements directly
  std::cout << "\nDirect from string elements:" << std::endl;
  std::cout << "str[0] ASCII: " << static_cast<int>(str[0]) << std::endl;
  std::cout << "str[1] ASCII: " << static_cast<int>(str[1]) << std::endl;
  
  // Showing negative values for extended ASCII (if applicable)
  char extended_char = static_cast<char>(200);  // Extended ASCII
  std::cout << "\nExtended ASCII character (value 200):" << std::endl;
  std::cout << "Character: '" << extended_char << "' ASCII: " << static_cast<int>(extended_char) << std::endl;
  std::cout << "Unsigned value: " << static_cast<unsigned char>(extended_char) << std::endl;
  
  // Character range demonstration
  std::cout << "\n--- Character Range ---" << std::endl;
  std::cout << "char can hold values from " << static_cast<int>(CHAR_MIN) 
            << " to " << static_cast<int>(CHAR_MAX) << std::endl;
  
  // ASCII character count and ranges
  std::cout << "\n--- ASCII Character Information ---" << std::endl;
  std::cout << "Total ASCII characters: 128 (values 0-127)" << std::endl;
  std::cout << "ASCII ranges:" << std::endl;
  std::cout << "  Control characters: 0-31 (32 characters)" << std::endl;
  std::cout << "  Printable characters: 32-126 (95 characters)" << std::endl;
  std::cout << "  DEL character: 127 (1 character)" << std::endl;
  std::cout << "  Space character: 32 (ASCII for ' ')" << std::endl;
  std::cout << "  Digits '0'-'9': 48-57 (10 characters)" << std::endl;
  std::cout << "  Uppercase 'A'-'Z': 65-90 (26 characters)" << std::endl;
  std::cout << "  Lowercase 'a'-'z': 97-122 (26 characters)" << std::endl;
  
  // Demonstrate some ASCII ranges
  std::cout << "\nASCII examples:" << std::endl;
  std::cout << "  '0' = " << static_cast<int>('0') << std::endl;
  std::cout << "  '9' = " << static_cast<int>('9') << std::endl;
  std::cout << "  'A' = " << static_cast<int>('A') << std::endl;
  std::cout << "  'Z' = " << static_cast<int>('Z') << std::endl;
  std::cout << "  'a' = " << static_cast<int>('a') << std::endl;
  std::cout << "  'z' = " << static_cast<int>('z') << std::endl;
  std::cout << "  ' ' = " << static_cast<int>(' ') << " (space)" << std::endl;
  
  // Extended ASCII note
  std::cout << "\nExtended ASCII:" << std::endl;
  std::cout << "  Values 128-255 (128 additional characters)" << std::endl;
  std::cout << "  Total with extended: 256 characters" << std::endl;
  std::cout << "  Note: Extended ASCII varies by character set" << std::endl;
  
  // Different ways to declare and initialize char
  std::cout << "\n--- Character Declaration Methods ---" << std::endl;
  char ch1 = 'A';              // Character literal
  char ch2 = 65;               // ASCII value
  char ch3 = str[0];           // From string
  char ch4 = '\n';             // Escape sequence
  
  std::cout << "char ch1 = 'A': '" << ch1 << "' (ASCII " << static_cast<int>(ch1) << ")" << std::endl;
  std::cout << "char ch2 = 65: '" << ch2 << "' (ASCII " << static_cast<int>(ch2) << ")" << std::endl;
  std::cout << "char ch3 = str[0]: '" << ch3 << "' (ASCII " << static_cast<int>(ch3) << ")" << std::endl;
  std::cout << "char ch4 = '\\n': newline (ASCII " << static_cast<int>(ch4) << ")" << std::endl;
  
  // Character operations
  std::cout << "\n--- Character Operations ---" << std::endl;
  char letter = 'a';
  std::cout << "Original: '" << letter << "'" << std::endl;
  letter = letter - 'a' + 'A';  // Convert to uppercase
  std::cout << "After manual uppercase: '" << letter << "'" << std::endl;
  
  letter = 'b';
  letter = std::toupper(letter);  // Using library function
  std::cout << "Using std::toupper('b'): '" << letter << "'" << std::endl;
  
  // Character classification
  std::cout << "\n--- Character Classification ---" << std::endl;
  std::string test_chars = "A5@_";
  for (char c : test_chars) {
    std::cout << "Character '" << c << "': ";
    if (std::isalpha(c)) std::cout << "letter ";
    if (std::isdigit(c)) std::cout << "digit ";
    if (std::isalnum(c)) std::cout << "alphanumeric ";
    if (std::ispunct(c)) std::cout << "punctuation ";
    if (std::isspace(c)) std::cout << "whitespace ";
    std::cout << std::endl;
  }
  
  // Special characters in strings
  std::cout << "\n--- Special Characters ---" << std::endl;
  std::string special = "Hello\nWorld\tTab";
  std::cout << "String with special chars: " << std::endl;
  for (size_t i = 0; i < special.length(); ++i) {
    char c = special[i];
    if (c == '\n') {
      std::cout << "[newline at position " << i << "]" << std::endl;
    } else if (c == '\t') {
      std::cout << "[tab at position " << i << "]";
    } else {
      std::cout << c;
    }
  }
  std::cout << std::endl;
  
  // Unicode considerations (basic)
  std::cout << "\n--- Unicode Notes ---" << std::endl;
  std::cout << "- 'char' is typically 1 byte and handles ASCII (0-127)" << std::endl;
  std::cout << "- For Unicode, consider 'wchar_t', 'char16_t', 'char32_t'" << std::endl;
  std::cout << "- std::string uses char, std::wstring uses wchar_t" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_character_arrays() {
  std::cout << "=== CHARACTER ARRAYS (C-style strings) ===" << std::endl;
  
  // Declaration and initialization of character arrays
  char str1[20] = "Hello";           // Null-terminated automatically
  char str2[] = "World";             // Size determined automatically
  char str3[10];                     // Uninitialized array
  
  // Manual initialization
  strcpy(str3, "C++");               // Copy string using strcpy
  
  std::cout << "str1: " << str1 << std::endl;
  std::cout << "str2: " << str2 << std::endl;
  std::cout << "str3: " << str3 << std::endl;
  
  // Character array operations using cstring functions
  std::cout << "\n--- C-string Operations ---" << std::endl;
  
  // Length calculation
  std::cout << "Length of str1: " << strlen(str1) << std::endl;
  
  // String concatenation (be careful with buffer size!)
  char result[50];
  strcpy(result, str1);              // Copy first string
  strcat(result, " ");               // Concatenate space
  strcat(result, str2);              // Concatenate second string
  std::cout << "Concatenated: " << result << std::endl;
  
  // String comparison
  if (strcmp(str1, str2) == 0) {
    std::cout << "str1 and str2 are equal" << std::endl;
  } else {
    std::cout << "str1 and str2 are different" << std::endl;
  }
  
  // Character access and modification
  std::cout << "\n--- Character Access ---" << std::endl;
  std::cout << "First character of str1: " << str1[0] << std::endl;
  str1[0] = 'h';  // Modify character (Hello -> hello)
  std::cout << "Modified str1: " << str1 << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_cpp_strings() {
  std::cout << "=== C++ std::string CLASS ===" << std::endl;
  
  // Declaration and initialization
  std::string s1 = "Hello";          // Direct initialization
  std::string s2("World");           // Constructor initialization
  std::string s3(5, 'A');            // Initialize with 5 'A's: "AAAAA"
  std::string s4;                    // Empty string
  
  std::cout << "s1: " << s1 << std::endl;
  std::cout << "s2: " << s2 << std::endl;
  std::cout << "s3: " << s3 << std::endl;
  std::cout << "s4 (empty): '" << s4 << "'" << std::endl;
  
  // Basic operations
  std::cout << "\n--- Basic Operations ---" << std::endl;
  
  // Length/size
  std::cout << "Length of s1: " << s1.length() << std::endl;
  std::cout << "Size of s1: " << s1.size() << std::endl;  // Same as length()
  
  // Concatenation (multiple ways)
  std::string greeting1 = s1 + " " + s2;           // Using + operator
  std::string greeting2 = s1;
  greeting2 += " ";                                // Using += operator
  greeting2 += s2;
  
  std::cout << "Greeting1: " << greeting1 << std::endl;
  std::cout << "Greeting2: " << greeting2 << std::endl;
  
  // Character access
  std::cout << "\n--- Character Access ---" << std::endl;
  std::cout << "First character: " << s1[0] << std::endl;
  std::cout << "Last character: " << s1[s1.length() - 1] << std::endl;
  std::cout << "Character at index 1: " << s1.at(1) << std::endl;  // Safer access
  
  // Modification
  s1[0] = 'h';  // hello
  std::cout << "Modified s1: " << s1 << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_string_methods() {
  std::cout << "=== STRING METHODS AND OPERATIONS ===" << std::endl;
  
  std::string text = "  Hello, C++ Programming World!  ";
  std::cout << "Original: '" << text << "'" << std::endl;
  
  // Substring extraction
  std::string sub1 = text.substr(2, 5);     // Starting at index 2, length 5
  std::string sub2 = text.substr(9);        // From index 9 to end
  std::cout << "Substring(2, 5): '" << sub1 << "'" << std::endl;
  std::cout << "Substring(9): '" << sub2 << "'" << std::endl;
  
  // Finding substrings
  size_t pos = text.find("C++");
  if (pos != std::string::npos) {
    std::cout << "Found 'C++' at position: " << pos << std::endl;
  }
  
  // Replace operations
  std::string text_copy = text;
  text_copy.replace(pos, 3, "Python");      // Replace "C++" with "Python"
  std::cout << "After replace: '" << text_copy << "'" << std::endl;
  
  // Insert and erase
  std::string demo = "Hello World";
  demo.insert(5, " Beautiful");             // Insert at position 5
  std::cout << "After insert: '" << demo << "'" << std::endl;
  
  demo.erase(5, 10);                        // Erase 10 characters starting at pos 5
  std::cout << "After erase: '" << demo << "'" << std::endl;
  
  // Case conversion (using algorithm)
  std::string upper_text = text;
  std::transform(upper_text.begin(), upper_text.end(), upper_text.begin(), ::toupper);
  std::cout << "Uppercase: '" << upper_text << "'" << std::endl;
  
  std::string lower_text = text;
  std::transform(lower_text.begin(), lower_text.end(), lower_text.begin(), ::tolower);
  std::cout << "Lowercase: '" << lower_text << "'" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_string_comparison() {
  std::cout << "=== STRING COMPARISON ===" << std::endl;
  
  std::string s1 = "apple";
  std::string s2 = "banana";
  std::string s3 = "apple";
  
  // Equality comparison
  std::cout << "s1 == s3: " << (s1 == s3) << std::endl;
  std::cout << "s1 == s2: " << (s1 == s2) << std::endl;
  
  // Lexicographic comparison
  std::cout << "s1 < s2: " << (s1 < s2) << std::endl;   // apple < banana
  std::cout << "s2 > s1: " << (s2 > s1) << std::endl;   // banana > apple
  
  // Using compare() method
  int result = s1.compare(s2);
  if (result < 0) {
    std::cout << "s1 comes before s2 lexicographically" << std::endl;
  } else if (result > 0) {
    std::cout << "s1 comes after s2 lexicographically" << std::endl;
  } else {
    std::cout << "s1 and s2 are equal" << std::endl;
  }
  
  std::cout << std::endl;
}

void demonstrate_string_conversion() {
  std::cout << "=== STRING CONVERSION ===" << std::endl;
  
  // String to number conversions
  std::string num_str = "12345";
  std::string float_str = "3.14159";
  std::string bool_str = "1";
  
  int num = std::stoi(num_str);
  double pi = std::stod(float_str);
  bool flag = std::stoi(bool_str);  // Non-zero = true
  
  std::cout << "String to int: " << num << std::endl;
  std::cout << "String to double: " << pi << std::endl;
  std::cout << "String to bool: " << flag << std::endl;
  
  // Number to string conversions
  int value = 42;
  double decimal = 2.71828;
  
  std::string str_from_int = std::to_string(value);
  std::string str_from_double = std::to_string(decimal);
  
  std::cout << "Int to string: '" << str_from_int << "'" << std::endl;
  std::cout << "Double to string: '" << str_from_double << "'" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_string_streams() {
  std::cout << "=== STRING STREAMS ===" << std::endl;
  
  // Using stringstream for complex string building
  std::stringstream ss;
  ss << "Name: " << "John" << ", Age: " << 25 << ", GPA: " << 3.75;
  std::string formatted = ss.str();
  std::cout << "Formatted string: " << formatted << std::endl;
  
  // Parsing with stringstream
  std::string data = "apple,banana,cherry,date";
  std::stringstream parser(data);
  std::string item;
  std::vector<std::string> fruits;
  
  while (std::getline(parser, item, ',')) {  // Split by comma
    fruits.push_back(item);
  }
  
  std::cout << "Parsed fruits: ";
  for (const auto& fruit : fruits) {
    std::cout << fruit << " ";
  }
  std::cout << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_string_iterators() {
  std::cout << "=== STRING CHARACTER ITERATION ===" << std::endl;
  
  std::string text = "Hello";
  char cstr[] = "World!";
  
  std::cout << "Working with string: '" << text << "'" << std::endl;
  std::cout << "Working with C-string: '" << cstr << "'" << std::endl << std::endl;
  
  // Method 1: Traditional for loop with indexing (std::string)
  std::cout << "Method 1 - Index-based iteration (std::string):" << std::endl;
  std::cout << "Characters: ";
  for (size_t i = 0; i < text.length(); ++i) {
    std::cout << "'" << text[i] << "'[" << i << "] ";
  }
  std::cout << std::endl;
  
  // Method 2: Traditional for loop with indexing (C-string)
  std::cout << "\nMethod 2 - Index-based iteration (C-string):" << std::endl;
  std::cout << "Characters: ";
  for (size_t i = 0; i < strlen(cstr); ++i) {
    std::cout << "'" << cstr[i] << "'[" << i << "] ";
  }
  std::cout << std::endl;
  
  // Method 3: Range-based for loop (C++11) - Read-only
  std::cout << "\nMethod 3 - Range-based for loop (read-only):" << std::endl;
  std::cout << "Characters: ";
  for (char c : text) {
    std::cout << "'" << c << "' ";
  }
  std::cout << std::endl;
  
  // Method 4: Range-based for loop with reference - Can modify
  std::cout << "\nMethod 4 - Range-based for loop (modifiable):" << std::endl;
  std::string modifiable = text;
  std::cout << "Before: " << modifiable << std::endl;
  for (char &c : modifiable) {
    if (c >= 'a' && c <= 'z') {
      c = c - 'a' + 'A';  // Convert to uppercase
    }
  }
  std::cout << "After:  " << modifiable << std::endl;
  
  // Method 5: Iterator-based iteration (forward)
  std::cout << "\nMethod 5 - Forward iterator:" << std::endl;
  std::cout << "Characters: ";
  for (auto it = text.begin(); it != text.end(); ++it) {
    std::cout << "'" << *it << "' ";
  }
  std::cout << std::endl;
  
  // Method 6: Iterator-based iteration (reverse)
  std::cout << "\nMethod 6 - Reverse iterator:" << std::endl;
  std::cout << "Characters: ";
  for (auto it = text.rbegin(); it != text.rend(); ++it) {
    std::cout << "'" << *it << "' ";
  }
  std::cout << std::endl;
  
  // Method 7: Const iterator (cannot modify)
  std::cout << "\nMethod 7 - Const iterator:" << std::endl;
  std::cout << "Characters: ";
  for (auto it = text.cbegin(); it != text.cend(); ++it) {
    std::cout << "'" << *it << "' ";
    // *it = 'X';  // This would cause a compile error
  }
  std::cout << std::endl;
  
  // Method 8: While loop with pointer arithmetic (C-string)
  std::cout << "\nMethod 8 - Pointer arithmetic (C-string):" << std::endl;
  std::cout << "Characters: ";
  const char* ptr = cstr;
  while (*ptr != '\0') {
    std::cout << "'" << *ptr << "' ";
    ++ptr;
  }
  std::cout << std::endl;
  
  // Method 9: Advanced - Iterate with position tracking
  std::cout << "\nMethod 9 - Position tracking:" << std::endl;
  for (auto it = text.begin(); it != text.end(); ++it) {
    size_t position = std::distance(text.begin(), it);
    std::cout << "Position " << position << ": '" << *it << "'" << std::endl;
  }
  
  // Method 10: Character classification while iterating
  std::cout << "\nMethod 10 - Character analysis:" << std::endl;
  std::string mixed = "Hello123!";
  std::cout << "Analyzing string: '" << mixed << "'" << std::endl;
  
  int letters = 0, digits = 0, others = 0;
  for (char c : mixed) {
    if (std::isalpha(c)) {
      std::cout << "'" << c << "' is a letter" << std::endl;
      letters++;
    } else if (std::isdigit(c)) {
      std::cout << "'" << c << "' is a digit" << std::endl;
      digits++;
    } else {
      std::cout << "'" << c << "' is other" << std::endl;
      others++;
    }
  }
  std::cout << "Summary: " << letters << " letters, " << digits << " digits, " << others << " others" << std::endl;
  
  // Method 11: Searching while iterating
  std::cout << "\nMethod 11 - Search while iterating:" << std::endl;
  std::string searchText = "Programming";
  char target = 'g';
  std::cout << "Searching for '" << target << "' in '" << searchText << "':" << std::endl;
  
  for (size_t i = 0; i < searchText.length(); ++i) {
    if (searchText[i] == target) {
      std::cout << "Found '" << target << "' at position " << i << std::endl;
    }
  }
  
  // Method 12: Processing characters in chunks
  std::cout << "\nMethod 12 - Processing in chunks:" << std::endl;
  std::string longText = "abcdefghijklmnop";
  std::cout << "Original: " << longText << std::endl;
  std::cout << "Chunks of 3: ";
  
  for (size_t i = 0; i < longText.length(); i += 3) {
    std::cout << "[";
    for (size_t j = i; j < i + 3 && j < longText.length(); ++j) {
      std::cout << longText[j];
    }
    std::cout << "] ";
  }
  std::cout << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_string_reversal() {
  std::cout << "=== STRING REVERSAL METHODS ===" << std::endl;
  
  std::string original = "Hello World!";
  std::cout << "Original string: '" << original << "'" << std::endl << std::endl;
  
  // Method 1: Using std::reverse algorithm
  std::cout << "Method 1 - Using std::reverse algorithm:" << std::endl;
  std::string method1 = original;
  std::reverse(method1.begin(), method1.end());
  std::cout << "Result: '" << method1 << "'" << std::endl;
  
  // Method 2: Manual reversal using iterators
  std::cout << "\nMethod 2 - Manual reversal using iterators:" << std::endl;
  std::string method2 = original;
  auto start = method2.begin();
  auto end = method2.end() - 1;
  
  while (start < end) {
    std::swap(*start, *end);
    ++start;
    --end;
  }
  std::cout << "Result: '" << method2 << "'" << std::endl;
  
  // Method 3: Using reverse constructor
  std::cout << "\nMethod 3 - Using reverse iterator constructor:" << std::endl;
  std::string method3(original.rbegin(), original.rend());
  std::cout << "Result: '" << method3 << "'" << std::endl;
  
  // Method 4: Manual reversal with indexing
  std::cout << "\nMethod 4 - Manual reversal with indexing:" << std::endl;
  std::string method4 = original;
  size_t n = method4.length();
  
  for (size_t i = 0; i < n / 2; ++i) {
    std::swap(method4[i], method4[n - 1 - i]);
  }
  std::cout << "Result: '" << method4 << "'" << std::endl;
  
  // Method 5: Building reversed string character by character
  std::cout << "\nMethod 5 - Building reversed string character by character:" << std::endl;
  std::string method5;
  for (int i = static_cast<int>(original.length()) - 1; i >= 0; --i) {
    method5 += original[i];
  }
  std::cout << "Result: '" << method5 << "'" << std::endl;
  
  // Method 6: Using string assignment with reverse iterators
  std::cout << "\nMethod 6 - Using string assignment with reverse iterators:" << std::endl;
  std::string method6;
  method6.assign(original.rbegin(), original.rend());
  std::cout << "Result: '" << method6 << "'" << std::endl;
  
  // Method 7: Recursive reversal (for educational purposes)
  std::cout << "\nMethod 7 - Recursive reversal:" << std::endl;
  std::function<std::string(const std::string&, size_t, size_t)> reverse_recursive = 
    [&](const std::string& str, size_t start, size_t end) -> std::string {
      if (start >= end) {
        return "";
      }
      return str[end] + reverse_recursive(str, start, end - 1);
    };
  
  std::string method7 = reverse_recursive(original, 0, original.length() - 1);
  std::cout << "Result: '" << method7 << "'" << std::endl;
  
  // Method 8: Using stack for reversal
  std::cout << "\nMethod 8 - Using stack for reversal:" << std::endl;
  std::stack<char> char_stack;
  std::string method8;
  
  // Push all characters to stack
  for (char c : original) {
    char_stack.push(c);
  }
  
  // Pop all characters from stack
  while (!char_stack.empty()) {
    method8 += char_stack.top();
    char_stack.pop();
  }
  std::cout << "Result: '" << method8 << "'" << std::endl;
  
  // Demonstrating reversal of C-style strings
  std::cout << "\n--- C-Style String Reversal ---" << std::endl;
  char cstr[] = "C++ Programming";
  std::cout << "Original C-string: '" << cstr << "'" << std::endl;
  
  // Method 9: Manual C-string reversal
  size_t len = strlen(cstr);
  for (size_t i = 0; i < len / 2; ++i) {
    char temp = cstr[i];
    cstr[i] = cstr[len - 1 - i];
    cstr[len - 1 - i] = temp;
  }
  std::cout << "Reversed C-string: '" << cstr << "'" << std::endl;
  
  // Word-wise reversal
  std::cout << "\n--- Word-wise Reversal ---" << std::endl;
  std::string sentence = "Hello Beautiful World";
  std::cout << "Original sentence: '" << sentence << "'" << std::endl;
  
  // Method 10: Reverse words in a sentence
  std::vector<std::string> words;
  std::stringstream ss(sentence);
  std::string word;
  
  while (ss >> word) {
    words.push_back(word);
  }
  
  std::string reversed_sentence;
  for (int i = static_cast<int>(words.size()) - 1; i >= 0; --i) {
    reversed_sentence += words[i];
    if (i > 0) reversed_sentence += " ";
  }
  std::cout << "Words reversed: '" << reversed_sentence << "'" << std::endl;
  
  // Method 11: Reverse each word in place
  std::string sentence_copy = sentence;
  std::stringstream ss2(sentence_copy);
  std::string each_word_reversed;
  
  while (ss2 >> word) {
    std::reverse(word.begin(), word.end());
    each_word_reversed += word;
    if (!ss2.eof()) each_word_reversed += " ";
  }
  std::cout << "Each word reversed: '" << each_word_reversed << "'" << std::endl;
  
  // Performance comparison
  std::cout << "\n--- Performance Notes ---" << std::endl;
  std::cout << "1. std::reverse() - Fastest, optimized by compiler" << std::endl;
  std::cout << "2. Manual swap with iterators - Very fast, good for learning" << std::endl;
  std::cout << "3. Reverse iterator constructor - Fast, creates new string" << std::endl;
  std::cout << "4. Character-by-character building - Slower due to string reallocations" << std::endl;
  std::cout << "5. Recursive approach - Slowest, high memory usage (not recommended for large strings)" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_string_copying() {
  std::cout << "=== STRING COPYING AND MEMORY MANAGEMENT ===" << std::endl;
  
  /*
  STRING COPYING THEORY:
  
  1. DEEP COPY vs SHALLOW COPY:
     - Deep copy: Creates completely independent copy with separate memory
     - Shallow copy: Copies pointer/reference, shares same memory
     - std::string always performs deep copy by default
     - C-style strings require manual deep copying
  
  2. COPY SEMANTICS:
     - Copy constructor: Creates new string from existing string
     - Copy assignment: Assigns one string's content to another
     - Both operations create independent copies
  
  3. MEMORY IMPLICATIONS:
     - Each std::string owns its own memory buffer
     - Copying allocates new memory and copies characters
     - No shared ownership between string objects
     - Safe from dangling pointer issues
  */
  
  // Demonstrating deep copy with std::string
  std::cout << "=== STD::STRING DEEP COPYING ===" << std::endl;
  
  std::string original = "Hello, World!";
  std::cout << "Original string: '" << original << "'" << std::endl;
  std::cout << "Original address: " << static_cast<const void*>(original.c_str()) << std::endl;
  std::cout << "Original size: " << original.size() << std::endl;
  
  // Method 1: Copy constructor (deep copy)
  std::string copy1(original);
  std::cout << "\nCopy constructor:" << std::endl;
  std::cout << "copy1: '" << copy1 << "'" << std::endl;
  std::cout << "copy1 address: " << static_cast<const void*>(copy1.c_str()) << std::endl;
  std::cout << "Same content? " << (original == copy1 ? "Yes" : "No") << std::endl;
  std::cout << "Same memory? " << (original.c_str() == copy1.c_str() ? "Yes" : "No") << std::endl;
  
  // Method 2: Copy assignment (deep copy)
  std::string copy2;
  copy2 = original;
  std::cout << "\nCopy assignment:" << std::endl;
  std::cout << "copy2: '" << copy2 << "'" << std::endl;
  std::cout << "copy2 address: " << static_cast<const void*>(copy2.c_str()) << std::endl;
  std::cout << "Same content? " << (original == copy2 ? "Yes" : "No") << std::endl;
  std::cout << "Same memory? " << (original.c_str() == copy2.c_str() ? "Yes" : "No") << std::endl;
  
  // Method 3: Explicit copy using string constructor
  std::string copy3(original.begin(), original.end());
  std::cout << "\nIterator-based copy:" << std::endl;
  std::cout << "copy3: '" << copy3 << "'" << std::endl;
  std::cout << "copy3 address: " << static_cast<const void*>(copy3.c_str()) << std::endl;
  
  // Method 4: Substring copy (still deep copy)
  std::string copy4 = original.substr(0);  // Copy entire string
  std::cout << "\nSubstring copy:" << std::endl;
  std::cout << "copy4: '" << copy4 << "'" << std::endl;
  std::cout << "copy4 address: " << static_cast<const void*>(copy4.c_str()) << std::endl;
  
  // Demonstrate independence of copies
  std::cout << "\n=== DEMONSTRATING COPY INDEPENDENCE ===" << std::endl;
  std::cout << "Modifying original string..." << std::endl;
  original[0] = 'h';  // Change 'H' to 'h'
  original += " Modified!";
  
  std::cout << "\nAfter modification:" << std::endl;
  std::cout << "original: '" << original << "'" << std::endl;
  std::cout << "copy1:    '" << copy1 << "'" << std::endl;
  std::cout << "copy2:    '" << copy2 << "'" << std::endl;
  std::cout << "copy3:    '" << copy3 << "'" << std::endl;
  std::cout << "copy4:    '" << copy4 << "'" << std::endl;
  
  std::cout << "Copies remain unchanged - proving deep copy!" << std::endl;
  
  // C-style string copying
  std::cout << "\n=== C-STYLE STRING COPYING ===" << std::endl;
  
  const char* c_original = "Hello, C World!";
  std::cout << "Original C-string: '" << c_original << "'" << std::endl;
  std::cout << "Original address: " << static_cast<const void*>(c_original) << std::endl;
  
  // WRONG: Shallow copy (dangerous!)
  const char* shallow_copy = c_original;  // Just copies pointer!
  std::cout << "\nShallow copy (DANGEROUS):" << std::endl;
  std::cout << "shallow_copy: '" << shallow_copy << "'" << std::endl;
  std::cout << "shallow_copy address: " << static_cast<const void*>(shallow_copy) << std::endl;
  std::cout << "Same memory? " << (c_original == shallow_copy ? "Yes (PROBLEM!)" : "No") << std::endl;
  
  // CORRECT: Deep copy using strcpy
  char deep_copy[50];  // Allocate buffer
  strcpy(deep_copy, c_original);
  std::cout << "\nDeep copy using strcpy:" << std::endl;
  std::cout << "deep_copy: '" << deep_copy << "'" << std::endl;
  std::cout << "deep_copy address: " << static_cast<const void*>(deep_copy) << std::endl;
  std::cout << "Same memory? " << (c_original == deep_copy ? "Yes" : "No (CORRECT!)") << std::endl;
  
  // CORRECT: Deep copy using strdup (allocates memory)
  char* dynamic_copy = strdup(c_original);
  std::cout << "\nDeep copy using strdup:" << std::endl;
  std::cout << "dynamic_copy: '" << dynamic_copy << "'" << std::endl;
  std::cout << "dynamic_copy address: " << static_cast<const void*>(dynamic_copy) << std::endl;
  std::cout << "Same memory? " << (c_original == dynamic_copy ? "Yes" : "No (CORRECT!)") << std::endl;
  
  // Demonstrate C-string copy independence
  deep_copy[0] = 'h';  // Modify copy
  std::cout << "\nAfter modifying deep_copy[0] to 'h':" << std::endl;
  std::cout << "c_original: '" << c_original << "'" << std::endl;
  std::cout << "deep_copy:  '" << deep_copy << "'" << std::endl;
  std::cout << "Original unchanged - proving deep copy!" << std::endl;
  
  // Clean up dynamic memory
  free(dynamic_copy);
  
  // String copying with different sizes
  std::cout << "\n=== COPYING WITH DIFFERENT SIZES ===" << std::endl;
  
  std::string small_str = "Hi";
  std::string large_str = "This is a much longer string that will require more memory allocation";
  
  std::cout << "Small string: '" << small_str << "' (size: " << small_str.size() << ")" << std::endl;
  std::cout << "Large string size: " << large_str.size() << std::endl;
  
  // Copy small to large
  std::string copy_small = small_str;
  std::cout << "\nCopy of small: '" << copy_small << "' (size: " << copy_small.size() << ")" << std::endl;
  
  // Copy large to small variable
  copy_small = large_str;  // Assignment resizes automatically
  std::cout << "After assigning large to copy_small:" << std::endl;
  std::cout << "copy_small size: " << copy_small.size() << std::endl;
  std::cout << "copy_small capacity: " << copy_small.capacity() << std::endl;
  
  // Partial copying
  std::cout << "\n=== PARTIAL STRING COPYING ===" << std::endl;
  
  std::string source = "Hello, Beautiful World!";
  std::cout << "Source: '" << source << "'" << std::endl;
  
  // Copy substring
  std::string partial1 = source.substr(7, 9);  // "Beautiful"
  std::cout << "Partial copy 1: '" << partial1 << "'" << std::endl;
  
  // Copy using constructor with count
  std::string partial2(source, 0, 5);  // "Hello"
  std::cout << "Partial copy 2: '" << partial2 << "'" << std::endl;
  
  // Copy using assign method
  std::string partial3;
  partial3.assign(source, 7, 9);
  std::cout << "Partial copy 3: '" << partial3 << "'" << std::endl;
  
  // Performance considerations
  std::cout << "\n=== COPY PERFORMANCE NOTES ===" << std::endl;
  std::cout << "1. std::string copying is O(n) where n is string length" << std::endl;
  std::cout << "2. Short String Optimization (SSO) avoids heap allocation for small strings" << std::endl;
  std::cout << "3. C-style string copying also O(n) but manual memory management" << std::endl;
  std::cout << "4. Move semantics (C++11) can avoid copying when possible" << std::endl;
  std::cout << "5. String views (C++17) provide non-owning references without copying" << std::endl;
  
  // Copy vs Move demonstration
  std::cout << "\n=== COPY vs MOVE SEMANTICS ===" << std::endl;
  
  std::string move_source = "This string will be moved";
  std::cout << "Before move - source: '" << move_source << "'" << std::endl;
  std::cout << "Source size: " << move_source.size() << std::endl;
  
  // Move constructor (transfers ownership, no deep copy)
  std::string moved_to = std::move(move_source);
  std::cout << "\nAfter move:" << std::endl;
  std::cout << "moved_to: '" << moved_to << "'" << std::endl;
  std::cout << "move_source: '" << move_source << "' (moved-from state)" << std::endl;
  std::cout << "move_source size: " << move_source.size() << std::endl;
  
  // Best practices
  std::cout << "\n=== COPYING BEST PRACTICES ===" << std::endl;
  std::cout << "1. Use std::string for automatic deep copying" << std::endl;
  std::cout << "2. Prefer copy initialization: string s = other;" << std::endl;
  std::cout << "3. Use const references to avoid unnecessary copies" << std::endl;
  std::cout << "4. Consider move semantics for temporary objects" << std::endl;
  std::cout << "5. Be careful with C-style string pointers (shallow copy risk)" << std::endl;
  std::cout << "6. Use string_view for read-only access without copying" << std::endl;
  
  std::cout << std::endl;
}

void demonstrate_performance_comparison() {
  std::cout << "=== PERFORMANCE NOTES ===" << std::endl;
  
  std::cout << "C-style strings (char arrays):" << std::endl;
  std::cout << "  + Fast and memory-efficient" << std::endl;
  std::cout << "  + Compatible with C libraries" << std::endl;
  std::cout << "  - Manual memory management" << std::endl;
  std::cout << "  - Buffer overflow risks" << std::endl;
  std::cout << "  - Limited functionality" << std::endl;
  
  std::cout << "\nC++ std::string:" << std::endl;
  std::cout << "  + Automatic memory management" << std::endl;
  std::cout << "  + Rich set of methods" << std::endl;
  std::cout << "  + Safe (bounds checking with at())" << std::endl;
  std::cout << "  + Easy to use and maintain" << std::endl;
  std::cout << "  - Slight overhead compared to char arrays" << std::endl;
  
  std::cout << "\nRecommendation: Use std::string for most applications!" << std::endl;
  
  std::cout << std::endl;
}

int main() {
  std::cout << "C++ STRINGS AND CHARACTER ARRAYS TUTORIAL" << std::endl;
  std::cout << "==========================================" << std::endl << std::endl;
  
  demonstrate_character_types();
  demonstrate_character_arrays();
  demonstrate_cpp_strings();
  demonstrate_string_methods();
  demonstrate_string_comparison();
  demonstrate_string_conversion();
  demonstrate_string_streams();
  demonstrate_string_iterators();
  demonstrate_string_reversal();
  demonstrate_string_copying();
  demonstrate_performance_comparison();
  
  std::cout << "Tutorial completed successfully!" << std::endl;
  return 0;
}