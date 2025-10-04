/**
 * @file class_vs_struct.cpp
 * @author GitHub Copilot
 * @brief Demonstrates the differences between class and struct in C++
 */

#include <iostream>
#include <string>

// ANSI Color codes for better output visualization
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
// STRUCT EXAMPLES - Default PUBLIC access
// =============================================================================

// Example 1: Simple struct (all public by default)
struct Point {
    double x, y;  // PUBLIC by default
    
    // Constructor
    Point(double x = 0.0, double y = 0.0) : x(x), y(y) {}
    
    // Method (public by default)
    void display() const {
        std::cout << Colors::GREEN << "Point(" << x << ", " << y << ")" << Colors::RESET << std::endl;
    }
    
    double distance_from_origin() const {
        return sqrt(x*x + y*y);
    }
};

// Example 2: Struct with mixed access levels
struct Rectangle {
    // PUBLIC by default
    double width, height;
    
    Rectangle(double w, double h) : width(w), height(h) {}
    
private:
    // Explicitly private section
    bool is_valid() const {
        return width > 0 && height > 0;
    }
    
public:
    // Back to public
    double area() const {
        return is_valid() ? width * height : 0.0;
    }
    
    void display() const {
        std::cout << Colors::CYAN << "Rectangle: " << width << " x " << height 
                  << " (Area: " << area() << ")" << Colors::RESET << std::endl;
    }
};

// =============================================================================
// CLASS EXAMPLES - Default PRIVATE access
// =============================================================================

// Example 1: Simple class (all private by default)
class Circle {
    // PRIVATE by default
    double radius;
    static const double PI;
    
public:
    // Explicitly public section
    Circle(double r = 1.0) : radius(r > 0 ? r : 1.0) {}
    
    // Getter methods
    double get_radius() const { return radius; }
    double get_area() const { return PI * radius * radius; }
    double get_circumference() const { return 2 * PI * radius; }
    
    // Setter method with validation
    void set_radius(double r) {
        if (r > 0) {
            radius = r;
        } else {
            std::cout << Colors::RED << "Error: Radius must be positive!" << Colors::RESET << std::endl;
        }
    }
    
    void display() const {
        std::cout << Colors::YELLOW << "Circle with radius " << radius 
                  << " (Area: " << get_area() << ")" << Colors::RESET << std::endl;
    }
};

const double Circle::PI = 3.14159;

// Example 2: Class with encapsulation
class BankAccount {
private:
    // All private by default - data hiding
    std::string account_number;
    std::string owner_name;
    double balance;
    bool is_active;
    
    // Private helper method
    bool validate_amount(double amount) const {
        return amount > 0;
    }
    
public:
    // Constructor
    BankAccount(const std::string& account_num, const std::string& owner, double initial_balance = 0.0) 
        : account_number(account_num), owner_name(owner), balance(initial_balance), is_active(true) {}
    
    // Public interface methods
    bool deposit(double amount) {
        if (!is_active) {
            std::cout << Colors::RED << "Account is inactive!" << Colors::RESET << std::endl;
            return false;
        }
        if (validate_amount(amount)) {
            balance += amount;
            std::cout << Colors::GREEN << "Deposited $" << amount << ". New balance: $" << balance << Colors::RESET << std::endl;
            return true;
        }
        std::cout << Colors::RED << "Invalid deposit amount!" << Colors::RESET << std::endl;
        return false;
    }
    
    bool withdraw(double amount) {
        if (!is_active) {
            std::cout << Colors::RED << "Account is inactive!" << Colors::RESET << std::endl;
            return false;
        }
        if (validate_amount(amount) && amount <= balance) {
            balance -= amount;
            std::cout << Colors::GREEN << "Withdrew $" << amount << ". New balance: $" << balance << Colors::RESET << std::endl;
            return true;
        }
        std::cout << Colors::RED << "Invalid withdrawal amount or insufficient funds!" << Colors::RESET << std::endl;
        return false;
    }
    
    // Read-only access to private data
    double get_balance() const { return balance; }
    std::string get_owner() const { return owner_name; }
    std::string get_account_number() const { return account_number; }
    bool is_account_active() const { return is_active; }
    
    void display_info() const {
        std::cout << Colors::MAGENTA << "Account: " << account_number 
                  << " | Owner: " << owner_name 
                  << " | Balance: $" << balance 
                  << " | Status: " << (is_active ? "Active" : "Inactive") << Colors::RESET << std::endl;
    }
};

// =============================================================================
// COMPARISON EXAMPLES
// =============================================================================

// Same functionality implemented as struct vs class
struct PersonStruct {
    std::string name;  // Public by default
    int age;          // Public by default
    
    PersonStruct(const std::string& n, int a) : name(n), age(a) {}
    
    void introduce() const {
        std::cout << Colors::CYAN << "Hi! I'm " << name << " and I'm " << age << " years old." << Colors::RESET << std::endl;
    }
};

class PersonClass {
private:
    std::string name;  // Private by default
    int age;          // Private by default
    
public:
    PersonClass(const std::string& n, int a) : name(n), age(a) {}
    
    // Need public methods to access private data
    std::string get_name() const { return name; }
    int get_age() const { return age; }
    
    void set_name(const std::string& n) { name = n; }
    void set_age(int a) { if (a >= 0) age = a; }
    
    void introduce() const {
        std::cout << Colors::BLUE << "Hello! I'm " << name << " and I'm " << age << " years old." << Colors::RESET << std::endl;
    }
};

// =============================================================================
// DEMONSTRATION FUNCTIONS
// =============================================================================

void demonstrate_struct_usage() {
    std::cout << Colors::BOLD << Colors::GREEN << "\n=== STRUCT EXAMPLES (Default PUBLIC) ===" << Colors::RESET << std::endl;
    
    // Point struct - direct access to members
    Point p1(3.0, 4.0);
    std::cout << "Direct access to struct members:" << std::endl;
    std::cout << "p1.x = " << p1.x << ", p1.y = " << p1.y << std::endl;
    p1.display();
    
    // Rectangle struct
    Rectangle rect(5.0, 3.0);
    std::cout << "\nDirect access to Rectangle members:" << std::endl;
    std::cout << "Width: " << rect.width << ", Height: " << rect.height << std::endl;
    rect.display();
    
    // PersonStruct - all public
    PersonStruct person_s("Alice", 25);
    std::cout << "\nDirect access to PersonStruct:" << std::endl;
    std::cout << "Name: " << person_s.name << ", Age: " << person_s.age << std::endl;
    person_s.introduce();
    
    // Can modify directly (good or bad depending on design intent)
    person_s.age = 26;
    std::cout << "After direct modification - Age: " << person_s.age << std::endl;
}

void demonstrate_class_usage() {
    std::cout << Colors::BOLD << Colors::BLUE << "\n=== CLASS EXAMPLES (Default PRIVATE) ===" << Colors::RESET << std::endl;
    
    // Circle class - encapsulated data
    Circle c1(5.0);
    std::cout << "Access through public methods only:" << std::endl;
    // std::cout << c1.radius; // ❌ ERROR - private member
    std::cout << "Radius: " << c1.get_radius() << std::endl;
    c1.display();
    
    // BankAccount class - strong encapsulation
    BankAccount account("ACC123", "Bob", 1000.0);
    account.display_info();
    account.deposit(500.0);
    account.withdraw(200.0);
    // account.balance = 5000; // ❌ ERROR - private member
    
    // PersonClass - controlled access
    PersonClass person_c("Charlie", 30);
    std::cout << "\nAccess through getters/setters:" << std::endl;
    // std::cout << person_c.name; // ❌ ERROR - private member
    std::cout << "Name: " << person_c.get_name() << ", Age: " << person_c.get_age() << std::endl;
    person_c.introduce();
    
    // Controlled modification
    person_c.set_age(31);
    std::cout << "After setter modification - Age: " << person_c.get_age() << std::endl;
}

void demonstrate_access_levels() {
    std::cout << Colors::BOLD << Colors::YELLOW << "\n=== ACCESS LEVEL COMPARISON ===" << Colors::RESET << std::endl;
    
    std::cout << Colors::CYAN << "\nSTRUCT - Default PUBLIC access:" << Colors::RESET << std::endl;
    std::cout << "struct MyStruct {" << std::endl;
    std::cout << "    int data;        // PUBLIC by default" << std::endl;
    std::cout << "    void method();   // PUBLIC by default" << std::endl;
    std::cout << "};" << std::endl;
    
    std::cout << Colors::MAGENTA << "\nCLASS - Default PRIVATE access:" << Colors::RESET << std::endl;
    std::cout << "class MyClass {" << std::endl;
    std::cout << "    int data;        // PRIVATE by default" << std::endl;
    std::cout << "    void method();   // PRIVATE by default" << std::endl;
    std::cout << "public:" << std::endl;
    std::cout << "    int public_data; // Explicitly PUBLIC" << std::endl;
    std::cout << "};" << std::endl;
}

void show_when_to_use_which() {
    std::cout << Colors::BOLD << Colors::RED << "\n=== WHEN TO USE WHICH? ===" << Colors::RESET << std::endl;
    
    std::cout << Colors::GREEN << "\nUse STRUCT when:" << Colors::RESET << std::endl;
    std::cout << "  • Simple data containers" << std::endl;
    std::cout << "  • All members should be accessible" << std::endl;
    std::cout << "  • Plain Old Data (POD) types" << std::endl;
    std::cout << "  • C-style structures" << std::endl;
    std::cout << "  • Mathematical types (Point, Vector, etc.)" << std::endl;
    
    std::cout << Colors::BLUE << "\nUse CLASS when:" << Colors::RESET << std::endl;
    std::cout << "  • Complex objects with behavior" << std::endl;
    std::cout << "  • Need data encapsulation/hiding" << std::endl;
    std::cout << "  • Want to control access to data" << std::endl;
    std::cout << "  • Object-oriented design" << std::endl;
    std::cout << "  • Need validation or business logic" << std::endl;
}

// =============================================================================
// MAIN FUNCTION
// =============================================================================

int main() {
    std::cout << Colors::BOLD << Colors::CYAN << "🎯 C++ CLASS vs STRUCT Demonstration" << Colors::RESET << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    demonstrate_struct_usage();
    demonstrate_class_usage();
    demonstrate_access_levels();
    show_when_to_use_which();
    
    std::cout << Colors::BOLD << Colors::GREEN << "\n✅ Key Takeaway:" << Colors::RESET << std::endl;
    std::cout << "The ONLY difference is default access level:" << std::endl;
    std::cout << "• struct = public by default" << std::endl;
    std::cout << "• class = private by default" << std::endl;
    std::cout << "Everything else is identical!" << std::endl;
    
    return 0;
}