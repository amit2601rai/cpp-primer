/**
 * @file telnet_server.cpp
 * @brief Basic Telnet server implementation
 * 
 * This implements a simple Telnet server that:
 * - Accepts multiple client connections
 * - Provides a basic shell-like interface
 * - Handles Telnet protocol basics
 * - Supports simple commands
 */

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

const int TELNET_PORT = 2323;  // Using non-standard port (standard is 23)
const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 10;

// Telnet protocol constants
const unsigned char TELNET_IAC = 255;   // Interpret As Command
const unsigned char TELNET_WILL = 251;  // Will option
const unsigned char TELNET_WONT = 252;  // Won't option
const unsigned char TELNET_DO = 253;    // Do option
const unsigned char TELNET_DONT = 254;  // Don't option
const unsigned char TELNET_SE = 240;    // Subnegotiation End
const unsigned char TELNET_SB = 250;    // Subnegotiation Begin

// Telnet options
const unsigned char TELNET_ECHO = 1;          // Echo option
const unsigned char TELNET_SUPPRESS_GA = 3;  // Suppress Go Ahead
const unsigned char TELNET_TERMINAL_TYPE = 24; // Terminal Type
const unsigned char TELNET_WINDOW_SIZE = 31;  // Window Size

// Global variables for server management
int server_socket = -1;
std::vector<int> client_sockets;
std::mutex clients_mutex;
bool server_running = true;

// Client session structure
struct TelnetSession {
    int socket;
    std::string client_ip;
    int client_port;
    bool echo_enabled;
    std::string current_directory;
    
    TelnetSession(int sock, const std::string& ip, int port) 
        : socket(sock), client_ip(ip), client_port(port), echo_enabled(true), current_directory("/") {}
};

// Signal handler for graceful shutdown
void signal_handler(int signum) {
    std::cout << "\n\nShutting down Telnet server..." << std::endl;
    server_running = false;
    
    // Close all client connections
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        for (int client_sock : client_sockets) {
            close(client_sock);
        }
        client_sockets.clear();
    }
    
    if (server_socket != -1) {
        close(server_socket);
    }
    exit(0);
}

// Send data to client with error checking
bool send_to_client(int socket, const std::string& data) {
    ssize_t bytes_sent = send(socket, data.c_str(), data.length(), 0);
    return bytes_sent > 0;
}

// Send Telnet command
void send_telnet_command(int socket, unsigned char command, unsigned char option) {
    unsigned char cmd[3] = {TELNET_IAC, command, option};
    send(socket, cmd, 3, 0);
}

// Process Telnet protocol commands
void process_telnet_command(TelnetSession& session, const std::vector<unsigned char>& command) {
    if (command.size() < 3) return;
    
    unsigned char cmd = command[1];
    unsigned char option = command[2];
    
    std::cout << "📡 Telnet command from " << session.client_ip << ": ";
    
    switch (cmd) {
        case TELNET_WILL:
            std::cout << "WILL " << (int)option << std::endl;
            // Client will do something - respond with DO or DONT
            if (option == TELNET_ECHO || option == TELNET_SUPPRESS_GA) {
                send_telnet_command(session.socket, TELNET_DO, option);
            } else {
                send_telnet_command(session.socket, TELNET_DONT, option);
            }
            break;
            
        case TELNET_WONT:
            std::cout << "WONT " << (int)option << std::endl;
            // Client won't do something - acknowledge
            send_telnet_command(session.socket, TELNET_DONT, option);
            break;
            
        case TELNET_DO:
            std::cout << "DO " << (int)option << std::endl;
            // Client wants us to do something
            if (option == TELNET_ECHO) {
                send_telnet_command(session.socket, TELNET_WILL, option);
                session.echo_enabled = true;
            } else {
                send_telnet_command(session.socket, TELNET_WONT, option);
            }
            break;
            
        case TELNET_DONT:
            std::cout << "DONT " << (int)option << std::endl;
            // Client doesn't want us to do something
            send_telnet_command(session.socket, TELNET_WONT, option);
            if (option == TELNET_ECHO) {
                session.echo_enabled = false;
            }
            break;
            
        default:
            std::cout << "Unknown command " << (int)cmd << std::endl;
            break;
    }
}

// Execute a simple command
std::string execute_command(TelnetSession& session, const std::string& command) {
    std::string cmd = command;
    std::string response;
    
    // Remove trailing whitespace
    while (!cmd.empty() && (cmd.back() == '\r' || cmd.back() == '\n' || cmd.back() == ' ')) {
        cmd.pop_back();
    }
    
    if (cmd.empty()) {
        return "";
    }
    
    std::cout << "🔧 Command from " << session.client_ip << ": " << cmd << std::endl;
    
    if (cmd == "help" || cmd == "?") {
        response = "Available commands:\r\n";
        response += "  help, ?     - Show this help\r\n";
        response += "  date        - Show current date/time\r\n";
        response += "  whoami      - Show current user info\r\n";
        response += "  pwd         - Show current directory\r\n";
        response += "  echo <text> - Echo text back\r\n";
        response += "  uptime      - Show server uptime\r\n";
        response += "  clients     - Show connected clients\r\n";
        response += "  quit, exit  - Disconnect\r\n";
        
    } else if (cmd == "date") {
        time_t now = time(0);
        char* time_str = ctime(&now);
        response = "Current date/time: ";
        response += time_str;  // ctime includes \n
        
    } else if (cmd == "whoami") {
        response = "You are: telnet_user@" + session.client_ip + "\r\n";
        response += "Session: " + std::to_string(session.socket) + "\r\n";
        
    } else if (cmd == "pwd") {
        response = "Current directory: " + session.current_directory + "\r\n";
        
    } else if (cmd.substr(0, 5) == "echo ") {
        response = cmd.substr(5) + "\r\n";
        
    } else if (cmd == "uptime") {
        response = "Server is running (simplified uptime)\r\n";
        
    } else if (cmd == "clients") {
        std::lock_guard<std::mutex> lock(clients_mutex);
        response = "Connected clients: " + std::to_string(client_sockets.size()) + "\r\n";
        
    } else if (cmd == "quit" || cmd == "exit") {
        response = "Goodbye!\r\n";
        return "QUIT:" + response;  // Special marker for quit
        
    } else {
        response = "Unknown command: " + cmd + "\r\n";
        response += "Type 'help' for available commands.\r\n";
    }
    
    return response;
}

// Handle individual client connection
void handle_client(int client_socket, const std::string& client_ip, int client_port) {
    TelnetSession session(client_socket, client_ip, client_port);
    
    std::cout << "🔗 New Telnet client connected: " << client_ip << ":" << client_port << std::endl;
    
    // Send welcome message
    std::string welcome = "\r\n";
    welcome += "=========================================\r\n";
    welcome += "  Welcome to Basic Telnet Server\r\n";
    welcome += "=========================================\r\n";
    welcome += "Connected from: " + client_ip + ":" + std::to_string(client_port) + "\r\n";
    welcome += "Type 'help' for available commands.\r\n";
    welcome += "\r\n";
    
    send_to_client(client_socket, welcome);
    
    // Initial Telnet negotiation
    send_telnet_command(client_socket, TELNET_WILL, TELNET_ECHO);
    send_telnet_command(client_socket, TELNET_WILL, TELNET_SUPPRESS_GA);
    
    char buffer[BUFFER_SIZE];
    std::string input_buffer;
    
    while (server_running) {
        // Send prompt
        std::string prompt = session.current_directory + "$ ";
        send_to_client(client_socket, prompt);
        
        // Read data from client
        ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytes_received <= 0) {
            break;  // Client disconnected or error
        }
        
        // Process received data
        for (int i = 0; i < bytes_received; i++) {
            unsigned char byte = buffer[i];
            
            if (byte == TELNET_IAC && i + 2 < bytes_received) {
                // Telnet command sequence
                std::vector<unsigned char> command = {
                    static_cast<unsigned char>(buffer[i]), 
                    static_cast<unsigned char>(buffer[i+1]), 
                    static_cast<unsigned char>(buffer[i+2])
                };
                process_telnet_command(session, command);
                i += 2;  // Skip the next two bytes
                
            } else if (byte == '\r' || byte == '\n') {
                // End of command
                if (!input_buffer.empty()) {
                    std::string response = execute_command(session, input_buffer);
                    
                    if (response.substr(0, 5) == "QUIT:") {
                        send_to_client(client_socket, response.substr(5));
                        goto cleanup;  // Break out of nested loops
                    }
                    
                    if (!response.empty()) {
                        send_to_client(client_socket, response);
                    }
                    
                    input_buffer.clear();
                }
                
            } else if (byte >= 32 && byte <= 126) {
                // Printable character
                input_buffer += byte;
                
                // Echo back if echo is enabled
                if (session.echo_enabled) {
                    char echo_char = byte;
                    send(client_socket, &echo_char, 1, 0);
                }
                
            } else if (byte == 8 || byte == 127) {
                // Backspace or delete
                if (!input_buffer.empty()) {
                    input_buffer.pop_back();
                    if (session.echo_enabled) {
                        send_to_client(client_socket, "\b \b");  // Backspace, space, backspace
                    }
                }
            }
        }
    }
    
cleanup:
    // Remove client from list
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        client_sockets.erase(
            std::remove(client_sockets.begin(), client_sockets.end(), client_socket),
            client_sockets.end()
        );
    }
    
    close(client_socket);
    std::cout << "🔌 Client disconnected: " << client_ip << ":" << client_port << std::endl;
}

int main() {
    std::cout << "=== BASIC TELNET SERVER ===" << std::endl;
    std::cout << "Starting Telnet server on port " << TELNET_PORT << std::endl;
    
    // Set up signal handler
    signal(SIGINT, signal_handler);
    
    // Create TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "❌ Error: Failed to create socket" << std::endl;
        return 1;
    }
    
    // Set socket options
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "⚠️  Warning: Failed to set SO_REUSEADDR" << std::endl;
    }
    
    // Configure server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(TELNET_PORT);
    
    // Bind socket to address
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "❌ Error: Failed to bind socket to port " << TELNET_PORT << std::endl;
        close(server_socket);
        return 1;
    }
    
    // Listen for connections
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        std::cerr << "❌ Error: Failed to listen on socket" << std::endl;
        close(server_socket);
        return 1;
    }
    
    std::cout << "✓ Server listening on port " << TELNET_PORT << std::endl;
    std::cout << "✓ Maximum clients: " << MAX_CLIENTS << std::endl;
    std::cout << "✓ Ready to accept connections..." << std::endl;
    std::cout << "  (Press Ctrl+C to stop)" << std::endl;
    std::cout << "\n📋 To connect: telnet localhost " << TELNET_PORT << std::endl << std::endl;
    
    // Main server loop
    while (server_running) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        // Accept client connection
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        
        if (client_socket < 0) {
            if (server_running) {
                std::cerr << "❌ Error accepting client connection" << std::endl;
            }
            continue;
        }
        
        // Get client IP and port
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        int client_port = ntohs(client_addr.sin_port);
        
        // Add client to list
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            client_sockets.push_back(client_socket);
        }
        
        // Handle client in separate thread
        std::thread client_thread(handle_client, client_socket, std::string(client_ip), client_port);
        client_thread.detach();
    }
    
    return 0;
}