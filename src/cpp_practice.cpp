#include <iostream>
#include <string>
#include <iomanip>

// ANSI Color codes for console output
namespace Colors {
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";
    const std::string BOLD = "\033[1m";
}

// Enhanced Car struct with color and additional functionality
struct Car {
    std::string make;
    std::string model;
    int year;
    std::string color;
    double mileage;
    bool is_electric;
    
    // Constructor
    Car(const std::string& make, const std::string& model, int year, 
        const std::string& color = "Unknown", double mileage = 0.0, bool electric = false)
        : make(make), model(model), year(year), color(color), mileage(mileage), is_electric(electric) {}
    
    // Default constructor
    Car() : make("Unknown"), model("Unknown"), year(0), color("Unknown"), mileage(0.0), is_electric(false) {}
    
    // Method to display car info with colors
    void display() const {
        std::cout << Colors::BOLD << Colors::CYAN << "=== Car Information ===" << Colors::RESET << std::endl;
        std::cout << Colors::GREEN << "Make: " << Colors::WHITE << make << Colors::RESET << std::endl;
        std::cout << Colors::GREEN << "Model: " << Colors::WHITE << model << Colors::RESET << std::endl;
        std::cout << Colors::GREEN << "Year: " << Colors::WHITE << year << Colors::RESET << std::endl;
        std::cout << Colors::GREEN << "Color: " << Colors::YELLOW << color << Colors::RESET << std::endl;
        std::cout << Colors::GREEN << "Mileage: " << Colors::WHITE << std::fixed << std::setprecision(1) 
                  << mileage << " miles" << Colors::RESET << std::endl;
        std::cout << Colors::GREEN << "Type: " << Colors::WHITE 
                  << (is_electric ? "Electric" : "Gas") << Colors::RESET << std::endl;
    }
    
    // Method to calculate car age
    int get_age() const {
        return 2024 - year;
    }
    
    // Method to check if car is vintage (>25 years old)
    bool is_vintage() const {
        return get_age() > 25;
    }
};

// Enhanced Vehicle class with color functionality
class Vehicle {
private:
    std::string make;
    std::string vehicle_color;
    int max_speed;
    bool is_running;
    
public:
    // Constructor
    Vehicle(const std::string& make = "Unknown", const std::string& color = "White", int speed = 0)
        : make(make), vehicle_color(color), max_speed(speed), is_running(false) {}
    
    // Getter methods
    std::string get_make() const { return make; }
    std::string get_color() const { return vehicle_color; }
    int get_max_speed() const { return max_speed; }
    bool get_running_status() const { return is_running; }
    
    // Setter methods
    void set_make(const std::string& new_make) { make = new_make; }
    void set_color(const std::string& new_color) { vehicle_color = new_color; }
    void set_max_speed(int speed) { max_speed = speed; }
    
    // Vehicle operations
    void start_engine() {
        is_running = true;
        std::cout << Colors::GREEN << "🚗 " << make << " engine started!" << Colors::RESET << std::endl;
    }
    
    void stop_engine() {
        is_running = false;
        std::cout << Colors::RED << "🛑 " << make << " engine stopped!" << Colors::RESET << std::endl;
    }
    
    void display_info() const {
        std::cout << Colors::BOLD << Colors::MAGENTA << "=== Vehicle Information ===" << Colors::RESET << std::endl;
        std::cout << Colors::BLUE << "Make: " << Colors::WHITE << make << Colors::RESET << std::endl;
        std::cout << Colors::BLUE << "Color: " << Colors::YELLOW << vehicle_color << Colors::RESET << std::endl;
        std::cout << Colors::BLUE << "Max Speed: " << Colors::WHITE << max_speed << " mph" << Colors::RESET << std::endl;
        std::cout << Colors::BLUE << "Status: " << Colors::WHITE 
                  << (is_running ? "Running" : "Stopped") << Colors::RESET << std::endl;
    }
    
    // Virtual destructor for proper inheritance
    virtual ~Vehicle() = default;
};

// Enhanced union with proper handling (Note: unions with non-trivial types like std::string need special care)
union ColorValue {
    int rgb_int;        // RGB as integer (0xRRGGBB)
    float hsv_hue;      // HSV hue value (0.0-360.0)
    char hex_code[8];   // Hex color code "#RRGGBB"
    
    // Constructor
    ColorValue() : rgb_int(0) {}
    
    // Destructor (needed when union contains non-trivial types)
    ~ColorValue() {}
};

// Wrapper class for the union to make it safer to use
class ColorManager {
private:
    ColorValue value;
    enum class ColorType { RGB_INT, HSV_HUE, HEX_CODE } current_type;
    
public:
    ColorManager() : current_type(ColorType::RGB_INT) {
        value.rgb_int = 0;
    }
    
    void set_rgb(int rgb) {
        current_type = ColorType::RGB_INT;
        value.rgb_int = rgb;
    }
    
    void set_hsv_hue(float hue) {
        current_type = ColorType::HSV_HUE;
        value.hsv_hue = hue;
    }
    
    void set_hex(const std::string& hex) {
        current_type = ColorType::HEX_CODE;
        strncpy(value.hex_code, hex.c_str(), 7);
        value.hex_code[7] = '\0';
    }
    
    void display() const {
        std::cout << Colors::BOLD << Colors::YELLOW << "=== Color Information ===" << Colors::RESET << std::endl;
        switch (current_type) {
            case ColorType::RGB_INT:
                std::cout << Colors::CYAN << "RGB Integer: " << Colors::WHITE 
                          << "0x" << std::hex << value.rgb_int << std::dec << Colors::RESET << std::endl;
                break;
            case ColorType::HSV_HUE:
                std::cout << Colors::CYAN << "HSV Hue: " << Colors::WHITE 
                          << value.hsv_hue << "°" << Colors::RESET << std::endl;
                break;
            case ColorType::HEX_CODE:
                std::cout << Colors::CYAN << "Hex Code: " << Colors::WHITE 
                          << value.hex_code << Colors::RESET << std::endl;
                break;
        }
    }
};

int main() {
    std::cout << Colors::BOLD << Colors::BLUE << "🎨 C++ Practice: Structs, Classes, and Unions with Colors!" << Colors::RESET << std::endl;
    std::cout << std::string(60, '=') << std::endl << std::endl;
    
    // Testing enhanced Car struct
    Car my_car("Toyota", "Camry", 2020, "Red", 15000.5, false);
    Car electric_car("Tesla", "Model 3", 2023, "Blue", 5000.0, true);
    
    my_car.display();
    std::cout << Colors::GREEN << "Car age: " << Colors::WHITE << my_car.get_age() << " years" << Colors::RESET << std::endl;
    std::cout << Colors::GREEN << "Is vintage: " << Colors::WHITE << (my_car.is_vintage() ? "Yes" : "No") << Colors::RESET << std::endl;
    std::cout << std::endl;
    
    electric_car.display();
    std::cout << std::endl;
    
    // Testing enhanced Vehicle class
    Vehicle my_vehicle("BMW", "Black", 180);
    my_vehicle.display_info();
    my_vehicle.start_engine();
    my_vehicle.stop_engine();
    std::cout << std::endl;
    
    // Testing enhanced union with ColorManager
    ColorManager color1, color2, color3;
    
    color1.set_rgb(0xFF5733);  // Orange color
    color2.set_hsv_hue(240.0); // Blue hue
    color3.set_hex("#00FF00");  // Green
    
    color1.display();
    color2.display();
    color3.display();
    
    std::cout << std::endl << Colors::BOLD << Colors::GREEN 
              << "✅ All examples completed successfully!" << Colors::RESET << std::endl;
    
    return 0;
}

