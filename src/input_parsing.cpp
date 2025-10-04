/**
 * @file input_parsing.cpp
 * @author GitHub Copilot
 * @brief Demonstrates best practices for reading and cleaning input in C++
 *        Handles format: keyboard = "string", word = "string"
 */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <regex>

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

// Structure to hold parsed input
struct InputData {
    std::string keyboard;
    std::string word;
    bool is_valid;
    
    InputData() : is_valid(false) {}
    
    InputData(const std::string& kb, const std::string& w, bool valid) 
        : keyboard(kb), word(w), is_valid(valid) {}
    
    void display() const {
        if (is_valid) {
            std::cout << Colors::GREEN << "✅ Valid Input:" << Colors::RESET << std::endl;
            std::cout << Colors::CYAN << "  Keyboard: \"" << keyboard << "\"" << Colors::RESET << std::endl;
            std::cout << Colors::CYAN << "  Word: \"" << word << "\"" << Colors::RESET << std::endl;
        } else {
            std::cout << Colors::RED << "❌ Invalid Input" << Colors::RESET << std::endl;
        }
    }
};

// =============================================================================
// METHOD 1: Manual String Parsing (Most Control)
// =============================================================================

class ManualParser {
public:
    static InputData parse(const std::string& input) {
        InputData result;
        std::string cleaned = trim(input);
        
        // Find keyboard = "..."
        size_t keyboard_pos = cleaned.find("keyboard");
        if (keyboard_pos == std::string::npos) return result;
        
        // Find the equals sign after keyboard
        size_t equals_pos = cleaned.find("=", keyboard_pos);
        if (equals_pos == std::string::npos) return result;
        
        // Find opening quote for keyboard value
        size_t quote1_start = cleaned.find("\"", equals_pos);
        if (quote1_start == std::string::npos) return result;
        
        // Find closing quote for keyboard value
        size_t quote1_end = cleaned.find("\"", quote1_start + 1);
        if (quote1_end == std::string::npos) return result;
        
        result.keyboard = cleaned.substr(quote1_start + 1, quote1_end - quote1_start - 1);
        
        // Find word = "..."
        size_t word_pos = cleaned.find("word", quote1_end);
        if (word_pos == std::string::npos) return result;
        
        // Find equals sign after word
        size_t equals_pos2 = cleaned.find("=", word_pos);
        if (equals_pos2 == std::string::npos) return result;
        
        // Find opening quote for word value
        size_t quote2_start = cleaned.find("\"", equals_pos2);
        if (quote2_start == std::string::npos) return result;
        
        // Find closing quote for word value
        size_t quote2_end = cleaned.find("\"", quote2_start + 1);
        if (quote2_end == std::string::npos) return result;
        
        result.word = cleaned.substr(quote2_start + 1, quote2_end - quote2_start - 1);
        result.is_valid = true;
        
        return result;
    }
    
private:
    static std::string trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \t\n\r");
        if (start == std::string::npos) return "";
        size_t end = str.find_last_not_of(" \t\n\r");
        return str.substr(start, end - start + 1);
    }
};

// =============================================================================
// METHOD 2: Regex Parsing (Clean and Robust)
// =============================================================================

class RegexParser {
public:
    static InputData parse(const std::string& input) {
        InputData result;
        
        // Regex pattern to match: keyboard = "...", word = "..."
        std::regex pattern("keyboard\\s*=\\s*\"([^\"]*)\".*word\\s*=\\s*\"([^\"]*)\"");
        std::smatch matches;
        
        if (std::regex_search(input, matches, pattern)) {
            result.keyboard = matches[1].str();
            result.word = matches[2].str();
            result.is_valid = true;
        }
        
        return result;
    }
};

// =============================================================================
// METHOD 3: Stream-based Parsing (Traditional C++ way) - Assumes correct input
// =============================================================================

class StreamParser {
public:
    static InputData parse(const std::string& input) {
        InputData result;
        std::istringstream iss(input);
        std::string token;
        
        // Read "keyboard" (assumed to be present)
        iss >> token; // "keyboard"
        
        // Read "=" (assumed to be present)
        iss >> token; // "="
        
        // Read keyboard value (skip to first quote and read until closing quote)
        std::getline(iss, token, '"'); // Skip to opening quote
        std::getline(iss, result.keyboard, '"'); // Read keyboard value
        
        // Read "word" (assumed to be present)
        iss >> token; // Skip comma and whitespace, read "word"
        
        // Read "=" (assumed to be present)
        iss >> token; // "="
        
        // Read word value (skip to first quote and read until closing quote)
        std::getline(iss, token, '"'); // Skip to opening quote
        std::getline(iss, result.word, '"'); // Read word value
        
        result.is_valid = true;
        return result;
    }
};

// =============================================================================
// METHOD 4: Custom Token-based Parser (Most Flexible)
// =============================================================================

class TokenParser {
public:
    enum class TokenType {
        IDENTIFIER, EQUALS, STRING, COMMA, END
    };
    
    struct Token {
        TokenType type;
        std::string value;
    };
    
    static InputData parse(const std::string& input) {
        InputData result;
        std::vector<Token> tokens = tokenize(input);
        
        if (parseTokens(tokens, result)) {
            result.is_valid = true;
        }
        
        return result;
    }
    
private:
    static std::vector<Token> tokenize(const std::string& input) {
        std::vector<Token> tokens;
        size_t i = 0;
        
        while (i < input.length()) {
            // Skip whitespace
            while (i < input.length() && std::isspace(input[i])) i++;
            if (i >= input.length()) break;
            
            if (input[i] == '=') {
                tokens.push_back({TokenType::EQUALS, "="});
                i++;
            } else if (input[i] == ',') {
                tokens.push_back({TokenType::COMMA, ","});
                i++;
            } else if (input[i] == '"') {
                // Parse string literal
                i++; // Skip opening quote
                std::string str;
                while (i < input.length() && input[i] != '"') {
                    str += input[i++];
                }
                if (i < input.length()) i++; // Skip closing quote
                tokens.push_back({TokenType::STRING, str});
            } else if (std::isalpha(input[i])) {
                // Parse identifier
                std::string identifier;
                while (i < input.length() && std::isalnum(input[i])) {
                    identifier += input[i++];
                }
                tokens.push_back({TokenType::IDENTIFIER, identifier});
            } else {
                i++; // Skip unknown character
            }
        }
        
        tokens.push_back({TokenType::END, ""});
        return tokens;
    }
    
    static bool parseTokens(const std::vector<Token>& tokens, InputData& result) {
        size_t i = 0;
        
        // Parse: keyboard = "..."
        if (!expectToken(tokens, i, TokenType::IDENTIFIER, "keyboard")) return false;
        if (!expectToken(tokens, i, TokenType::EQUALS)) return false;
        if (!expectToken(tokens, i, TokenType::STRING)) return false;
        result.keyboard = tokens[i-1].value;
        
        // Skip optional comma
        if (i < tokens.size() && tokens[i].type == TokenType::COMMA) i++;
        
        // Parse: word = "..."
        if (!expectToken(tokens, i, TokenType::IDENTIFIER, "word")) return false;
        if (!expectToken(tokens, i, TokenType::EQUALS)) return false;
        if (!expectToken(tokens, i, TokenType::STRING)) return false;
        result.word = tokens[i-1].value;
        
        return true;
    }
    
    static bool expectToken(const std::vector<Token>& tokens, size_t& i, TokenType expected_type, const std::string& expected_value = "") {
        if (i >= tokens.size() || tokens[i].type != expected_type) return false;
        if (!expected_value.empty() && tokens[i].value != expected_value) return false;
        i++;
        return true;
    }
};

// =============================================================================
// INPUT VALIDATION AND CLEANING
// =============================================================================

class InputValidator {
public:
    static bool validateKeyboard(const std::string& keyboard) {
        // Check if keyboard contains only unique lowercase letters
        if (keyboard.length() != 26) return false;
        
        bool seen[26] = {false};
        for (char c : keyboard) {
            if (c < 'a' || c > 'z') return false;
            if (seen[c - 'a']) return false; // Duplicate letter
            seen[c - 'a'] = true;
        }
        return true;
    }
    
    static bool validateWord(const std::string& word) {
        // Check if word contains only lowercase letters
        return std::all_of(word.begin(), word.end(), [](char c) {
            return c >= 'a' && c <= 'z';
        });
    }
    
    static InputData validateAndClean(const InputData& input) {
        InputData result = input;
        
        if (!input.is_valid) {
            result.is_valid = false;
            return result;
        }
        
        // Clean and validate keyboard
        std::string cleaned_keyboard = input.keyboard;
        cleaned_keyboard.erase(std::remove_if(cleaned_keyboard.begin(), cleaned_keyboard.end(), 
                                            [](char c) { return !std::isalpha(c); }), 
                             cleaned_keyboard.end());
        std::transform(cleaned_keyboard.begin(), cleaned_keyboard.end(), 
                      cleaned_keyboard.begin(), ::tolower);
        
        // Clean and validate word
        std::string cleaned_word = input.word;
        cleaned_word.erase(std::remove_if(cleaned_word.begin(), cleaned_word.end(), 
                                        [](char c) { return !std::isalpha(c); }), 
                         cleaned_word.end());
        std::transform(cleaned_word.begin(), cleaned_word.end(), 
                      cleaned_word.begin(), ::tolower);
        
        result.keyboard = cleaned_keyboard;
        result.word = cleaned_word;
        result.is_valid = validateKeyboard(cleaned_keyboard) && validateWord(cleaned_word);
        
        return result;
    }
};

// =============================================================================
// DEMONSTRATION FUNCTIONS
// =============================================================================

void testParsers() {
    std::vector<std::string> test_inputs = {
        "keyboard = \"abcdefghijklmnopqrstuvwxyz\", word = \"cba\"",
        "keyboard=\"qwertyuiopasdfghjklzxcvbnm\", word=\"hello\"",
        "  keyboard   =   \"abcdefghijklmnopqrstuvwxyz\"  ,  word   =   \"world\"  ",
        "keyboard = \"abcdefghijklmnopqrstuvwxyz\" word = \"test\"", // No comma
        "word = \"first\", keyboard = \"abcdefghijklmnopqrstuvwxyz\"", // Reversed order
        "invalid input format",
        "keyboard = \"abc\", word = \"def\"" // Invalid keyboard length
    };
    
    std::cout << Colors::BOLD << Colors::BLUE << "🧪 Testing Different Parsing Methods" << Colors::RESET << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    for (size_t i = 0; i < test_inputs.size(); i++) {
        std::cout << Colors::YELLOW << "\nTest Case " << (i + 1) << ":" << Colors::RESET << std::endl;
        std::cout << "Input: " << test_inputs[i] << std::endl;
        
        // Test Manual Parser
        std::cout << Colors::CYAN << "Manual Parser: " << Colors::RESET;
        auto manual_result = ManualParser::parse(test_inputs[i]);
        manual_result.display();
        
        // Test Regex Parser
        std::cout << Colors::CYAN << "Regex Parser: " << Colors::RESET;
        auto regex_result = RegexParser::parse(test_inputs[i]);
        regex_result.display();
        
        // Test Stream Parser
        std::cout << Colors::CYAN << "Stream Parser: " << Colors::RESET;
        auto stream_result = StreamParser::parse(test_inputs[i]);
        stream_result.display();
        
        // Test Token Parser
        std::cout << Colors::CYAN << "Token Parser: " << Colors::RESET;
        auto token_result = TokenParser::parse(test_inputs[i]);
        token_result.display();
        
        std::cout << std::string(40, '-') << std::endl;
    }
}

void demonstrateValidation() {
    std::cout << Colors::BOLD << Colors::GREEN << "\n🔍 Input Validation and Cleaning" << Colors::RESET << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::vector<InputData> test_data = {
        {"abcdefghijklmnopqrstuvwxyz", "hello", true},
        {"ABCDEFGHIJKLMNOPQRSTUVWXYZ", "WORLD", true}, // Uppercase
        {"abc123defghijklmnopqrstuvwxyz", "he!!o", true}, // With numbers/symbols
        {"abcdefghijklmnopqrstuvwxy", "test", true}, // Missing letter
        {"abcdefghijklmnopqrstuvwxyza", "duplicate", true}, // Duplicate letter
    };
    
    for (auto& data : test_data) {
        std::cout << Colors::YELLOW << "\nOriginal: " << Colors::RESET;
        data.display();
        
        auto validated = InputValidator::validateAndClean(data);
        std::cout << Colors::GREEN << "After validation/cleaning: " << Colors::RESET;
        validated.display();
        
        std::cout << std::string(40, '-') << std::endl;
    }
}

void interactiveDemo() {
    std::cout << Colors::BOLD << Colors::MAGENTA << "\n🎮 Interactive Input Demo" << Colors::RESET << std::endl;
    std::cout << "Enter input in format: keyboard = \"...\", word = \"...\"" << std::endl;
    std::cout << "Or press Enter to use default example" << std::endl;
    std::cout << "Type 'quit' to exit" << std::endl;
    
    std::string input;
    while (true) {
        std::cout << Colors::CYAN << "\n> " << Colors::RESET;
        std::getline(std::cin, input);
        
        if (input == "quit") break;
        
        if (input.empty()) {
            input = "keyboard = \"abcdefghijklmnopqrstuvwxyz\", word = \"cba\"";
            std::cout << "Using default: " << input << std::endl;
        }
        
        // Parse with regex (most robust for user input)
        auto result = RegexParser::parse(input);
        auto validated = InputValidator::validateAndClean(result);
        
        std::cout << Colors::GREEN << "Parsed result: " << Colors::RESET;
        validated.display();
        
        if (validated.is_valid) {
            std::cout << Colors::BLUE << "✅ Ready for processing!" << Colors::RESET << std::endl;
        }
    }
}

// =============================================================================
// MAIN FUNCTION
// =============================================================================

int main() {
    std::cout << Colors::BOLD << Colors::CYAN << "📝 C++ Input Parsing and Cleaning Demo" << Colors::RESET << std::endl;
    std::cout << "Format: keyboard = \"string\", word = \"string\"" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    // Show different parsing approaches
    testParsers();
    
    // Demonstrate validation and cleaning
    demonstrateValidation();
    
    // Best practices summary
    std::cout << Colors::BOLD << Colors::GREEN << "\n📋 Best Practices Summary:" << Colors::RESET << std::endl;
    std::cout << Colors::YELLOW << "1. Regex Parser" << Colors::RESET << " - Best for simple, well-defined formats" << std::endl;
    std::cout << Colors::YELLOW << "2. Token Parser" << Colors::RESET << " - Most flexible for complex parsing" << std::endl;
    std::cout << Colors::YELLOW << "3. Manual Parser" << Colors::RESET << " - Most control, good for performance" << std::endl;
    std::cout << Colors::YELLOW << "4. Stream Parser" << Colors::RESET << " - Traditional C++ approach" << std::endl;
    std::cout << Colors::GREEN << "\n✅ Always validate and clean input!" << Colors::RESET << std::endl;
    std::cout << Colors::GREEN << "✅ Handle edge cases and errors gracefully!" << Colors::RESET << std::endl;
    
    // Interactive demo (uncomment to enable)
    // interactiveDemo();
    
    return 0;
}