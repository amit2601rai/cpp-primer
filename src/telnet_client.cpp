/**
 * @file telnet_client.cpp
 * @brief Basic Telnet client implementation
 * 
 * This implements a simple Telnet client that:
 * - Connects to a Telnet server
 * - Handles basic Telnet protocol negotiation
 * - Provides interactive terminal interface
 * - Supports keyboard input and server responses
 */

#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <atomic>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <signal.h>

// Telnet protocol constants
const unsigned char TELNET_IAC = 255;   // Interpret As Command
const unsigned char TELNET_WILL = 251;  // Will option
const unsigned char TELNET_WONT = 252;  // Won't option
const unsigned char TELNET_DO = 253;    // Do option
const unsigned char TELNET_DONT = 254;  // Don't option

// Telnet options
const unsigned char TELNET_ECHO = 1;          // Echo option
const unsigned char TELNET_SUPPRESS_GA = 3;  // Suppress Go Ahead
const unsigned char TELNET_TERMINAL_TYPE = 24; // Terminal Type

// Global variables
int client_socket = -1;
std::atomic<bool> connected{false};
struct termios original_termios;
bool terminal_modified = false;

// Restore terminal settings on exit
void cleanup_terminal() {
    if (terminal_modified) {
        tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
        terminal_modified = false;
    }
}

// Signal handler for graceful shutdown
void signal_handler(int signum) {
    std::cout << "\n\nDisconnecting from Telnet server..." << std::endl;
    connected = false;
    
    if (client_socket != -1) {
        close(client_socket);
    }
    
    cleanup_terminal();
    exit(0);
}

// Set terminal to raw mode for character-by-character input
void set_terminal_raw_mode() {
    if (tcgetattr(STDIN_FILENO, &original_termios) == -1) {
        std::cerr << "⚠️  Warning: Failed to get terminal attributes" << std::endl;
        return;
    }
    
    terminal_modified = true;
    
    struct termios raw = original_termios;
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);  // Turn off echo, canonical mode, etc.
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);  // Turn off input processing
    raw.c_cflag |= CS8;  // 8-bit characters
    raw.c_oflag &= ~(OPOST);  // Turn off output processing
    raw.c_cc[VMIN] = 1;   // Minimum characters to read
    raw.c_cc[VTIME] = 0;  // No timeout
    
    if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) == -1) {
        std::cerr << "⚠️  Warning: Failed to set terminal to raw mode" << std::endl;
    }
}

// Send Telnet command
void send_telnet_command(int socket, unsigned char command, unsigned char option) {
    unsigned char cmd[3] = {TELNET_IAC, command, option};
    send(socket, cmd, 3, 0);
}

// Process Telnet protocol commands
void process_telnet_command(const std::vector<unsigned char>& command) {
    if (command.size() < 3) return;
    
    unsigned char cmd = command[1];
    unsigned char option = command[2];
    
    std::cout << "\r📡 Telnet command: ";
    
    switch (cmd) {
        case TELNET_WILL:
            std::cout << "Server WILL " << (int)option << std::endl;
            // Server will do something - respond with DO or DONT
            if (option == TELNET_ECHO || option == TELNET_SUPPRESS_GA) {
                send_telnet_command(client_socket, TELNET_DO, option);
            } else {
                send_telnet_command(client_socket, TELNET_DONT, option);
            }
            break;
            
        case TELNET_WONT:
            std::cout << "Server WONT " << (int)option << std::endl;
            // Server won't do something - acknowledge
            send_telnet_command(client_socket, TELNET_DONT, option);
            break;
            
        case TELNET_DO:
            std::cout << "Server wants us to DO " << (int)option << std::endl;
            // Server wants us to do something
            if (option == TELNET_TERMINAL_TYPE) {
                send_telnet_command(client_socket, TELNET_WILL, option);
            } else {
                send_telnet_command(client_socket, TELNET_WONT, option);
            }
            break;
            
        case TELNET_DONT:
            std::cout << "Server wants us to NOT DO " << (int)option << std::endl;
            // Server doesn't want us to do something
            send_telnet_command(client_socket, TELNET_WONT, option);
            break;
            
        default:
            std::cout << "Unknown command " << (int)cmd << std::endl;
            break;
    }
}

// Thread function to handle server responses
void handle_server_responses() {
    char buffer[1024];
    
    while (connected) {
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes_received <= 0) {
            if (connected) {
                std::cout << "\r\n🔌 Server disconnected" << std::endl;
                connected = false;
            }
            break;
        }
        
        // Process received data
        for (int i = 0; i < bytes_received; i++) {
            unsigned char byte = static_cast<unsigned char>(buffer[i]);
            
            if (byte == TELNET_IAC && i + 2 < bytes_received) {
                // Telnet command sequence
                std::vector<unsigned char> command = {
                    static_cast<unsigned char>(buffer[i]), 
                    static_cast<unsigned char>(buffer[i+1]), 
                    static_cast<unsigned char>(buffer[i+2])
                };
                process_telnet_command(command);
                i += 2;  // Skip the next two bytes
                
            } else {
                // Regular character - display it
                std::cout << static_cast<char>(byte) << std::flush;
            }
        }
    }
}

// Thread function to handle user input
void handle_user_input() {
    char ch;
    
    while (connected) {
        if (read(STDIN_FILENO, &ch, 1) == 1) {
            // Send character to server
            if (send(client_socket, &ch, 1, 0) <= 0) {
                if (connected) {
                    std::cout << "\r\n❌ Failed to send data to server" << std::endl;
                    connected = false;
                }
                break;
            }
            
            // Handle special characters locally if needed
            if (ch == '\r') {
                // Some terminals need both \r and \n
                send(client_socket, "\n", 1, 0);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    std::cout << "=== BASIC TELNET CLIENT ===" << std::endl;
    
    std::string server_ip = "127.0.0.1";
    int server_port = 2323;  // Default to our custom port
    
    // Parse command line arguments
    if (argc >= 2) {
        server_ip = argv[1];
        if (argc >= 3) {
            server_port = std::stoi(argv[2]);
        }
    } else {
        std::cout << "Usage: " << argv[0] << " [server_ip] [port]" << std::endl;
        std::cout << "Using defaults: " << server_ip << ":" << server_port << std::endl;
    }
    
    std::cout << "Connecting to Telnet server at " << server_ip << ":" << server_port << std::endl;
    
    // Set up signal handler
    signal(SIGINT, signal_handler);
    
    // Create TCP socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        std::cerr << "❌ Error: Failed to create socket" << std::endl;
        return 1;
    }
    
    // Configure server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    
    // Convert IP address
    if (inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "❌ Error: Invalid server IP address: " << server_ip << std::endl;
        close(client_socket);
        return 1;
    }
    
    // Connect to server
    std::cout << "🔗 Connecting..." << std::endl;
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "❌ Error: Failed to connect to " << server_ip << ":" << server_port << std::endl;
        std::cerr << "   Make sure the Telnet server is running" << std::endl;
        close(client_socket);
        return 1;
    }
    
    connected = true;
    std::cout << "✅ Connected to Telnet server!" << std::endl;
    std::cout << "📋 Commands: Type normally, Ctrl+C to quit" << std::endl;
    std::cout << "─────────────────────────────────────────" << std::endl;
    
    // Set terminal to raw mode for character-by-character input
    set_terminal_raw_mode();
    
    // Initial Telnet negotiation
    send_telnet_command(client_socket, TELNET_DO, TELNET_ECHO);
    send_telnet_command(client_socket, TELNET_DO, TELNET_SUPPRESS_GA);
    
    // Start threads for handling server responses and user input
    std::thread response_thread(handle_server_responses);
    std::thread input_thread(handle_user_input);
    
    // Wait for disconnection
    response_thread.join();
    
    // Cleanup
    connected = false;
    
    if (input_thread.joinable()) {
        input_thread.detach();  // Input thread will exit when connected becomes false
    }
    
    cleanup_terminal();
    close(client_socket);
    
    std::cout << "\n🔌 Disconnected from server" << std::endl;
    return 0;
}

/*
Usage Examples:
  ./telnet_client                        # Connect to localhost:2323
  ./telnet_client 192.168.1.100          # Connect to specific IP
  ./telnet_client 127.0.0.1 23           # Connect to standard Telnet port
  ./telnet_client example.com 2323       # Connect to remote server
*/