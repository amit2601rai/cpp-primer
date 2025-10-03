/**
 * @file udp_server.cpp
 * @brief Standalone UDP server implementation
 */

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

const int PORT = 9999;
const int BUFFER_SIZE = 1024;
int server_socket = -1;

// Signal handler for graceful shutdown
void signal_handler(int signum) {
    std::cout << "\n\nShutting down UDP server..." << std::endl;
    if (server_socket != -1) {
        close(server_socket);
    }
    exit(0);
}

int main() {
    std::cout << "=== UDP SERVER ===" << std::endl;
    std::cout << "Starting UDP server on port " << PORT << std::endl;
    
    // Set up signal handler for Ctrl+C
    signal(SIGINT, signal_handler);
    
    // Create UDP socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        std::cerr << "Error: Failed to create socket" << std::endl;
        return 1;
    }
    
    // Configure server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept from any IP
    server_addr.sin_port = htons(PORT);
    
    // Bind socket to address
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error: Failed to bind socket to port " << PORT << std::endl;
        close(server_socket);
        return 1;
    }
    
    std::cout << "✓ Server bound to port " << PORT << std::endl;
    std::cout << "✓ Waiting for client messages..." << std::endl;
    std::cout << "  (Press Ctrl+C to stop)" << std::endl << std::endl;
    
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    // Main server loop
    while (true) {
        // Clear buffer
        memset(buffer, 0, BUFFER_SIZE);
        
        // Receive message from client
        ssize_t bytes_received = recvfrom(server_socket, buffer, BUFFER_SIZE - 1,
                                         0, (struct sockaddr*)&client_addr, &client_len);
        
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            
            // Get client IP and port
            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
            int client_port = ntohs(client_addr.sin_port);
            
            std::cout << "📨 Received from " << client_ip << ":" << client_port 
                      << " (" << bytes_received << " bytes): \"" << buffer << "\"" << std::endl;
            
            // Create echo response
            std::string response = "Echo: " + std::string(buffer);
            
            // Send response back to client
            ssize_t bytes_sent = sendto(server_socket, response.c_str(), response.length(),
                                       0, (struct sockaddr*)&client_addr, client_len);
            
            if (bytes_sent > 0) {
                std::cout << "📤 Sent response to " << client_ip << ":" << client_port 
                          << " (" << bytes_sent << " bytes): \"" << response << "\"" << std::endl;
            } else {
                std::cerr << "❌ Failed to send response to client" << std::endl;
            }
            
            std::cout << std::endl;
        } else if (bytes_received < 0) {
            std::cerr << "❌ Error receiving data" << std::endl;
        }
    }
    
    return 0;
}