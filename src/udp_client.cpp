/**
 * @file udp_client.cpp
 * @brief Standalone UDP client implementation
 */

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

const int SERVER_PORT = 9999;
const int BUFFER_SIZE = 1024;
const char* SERVER_IP = "127.0.0.1";  // localhost

int main(int argc, char* argv[]) {
    std::cout << "=== UDP CLIENT ===" << std::endl;
    
    std::string server_ip = SERVER_IP;
    std::string message;
    
    // Parse command line arguments
    if (argc >= 2) {
        message = argv[1];
        if (argc >= 3) {
            server_ip = argv[2];
        }
    } else {
        std::cout << "Enter message to send to server: ";
        std::getline(std::cin, message);
        if (message.empty()) {
            message = "Hello from UDP client!";
        }
    }
    
    std::cout << "Connecting to UDP server at " << server_ip << ":" << SERVER_PORT << std::endl;
    std::cout << "Message to send: \"" << message << "\"" << std::endl << std::endl;
    
    // Create UDP socket
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        std::cerr << "❌ Error: Failed to create socket" << std::endl;
        return 1;
    }
    
    // Configure server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    
    // Convert IP address string to binary
    if (inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "❌ Error: Invalid server IP address: " << server_ip << std::endl;
        close(client_socket);
        return 1;
    }
    
    // Send message to server
    std::cout << "📤 Sending message..." << std::endl;
    ssize_t bytes_sent = sendto(client_socket, message.c_str(), message.length(),
                               0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    if (bytes_sent < 0) {
        std::cerr << "❌ Error: Failed to send message" << std::endl;
        close(client_socket);
        return 1;
    }
    
    std::cout << "✓ Sent " << bytes_sent << " bytes to server" << std::endl;
    
    // Set timeout for receiving response
    struct timeval timeout;
    timeout.tv_sec = 5;  // 5 second timeout
    timeout.tv_usec = 0;
    if (setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        std::cerr << "⚠️  Warning: Failed to set socket timeout" << std::endl;
    }
    
    // Receive response from server
    char response[BUFFER_SIZE];
    struct sockaddr_in response_addr;
    socklen_t response_len = sizeof(response_addr);
    
    std::cout << "📨 Waiting for server response..." << std::endl;
    ssize_t bytes_received = recvfrom(client_socket, response, BUFFER_SIZE - 1,
                                     0, (struct sockaddr*)&response_addr, &response_len);
    
    if (bytes_received > 0) {
        response[bytes_received] = '\0';
        
        // Verify response came from the correct server
        char response_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &response_addr.sin_addr, response_ip, INET_ADDRSTRLEN);
        int response_port = ntohs(response_addr.sin_port);
        
        std::cout << "✓ Received response from " << response_ip << ":" << response_port 
                  << " (" << bytes_received << " bytes)" << std::endl;
        std::cout << "📨 Server response: \"" << response << "\"" << std::endl;
        
        // Check if it's an echo response
        if (std::string(response).find("Echo: " + message) != std::string::npos) {
            std::cout << "✅ Echo response confirmed!" << std::endl;
        }
    } else if (bytes_received == 0) {
        std::cout << "⚠️  No response received (empty message)" << std::endl;
    } else {
        std::cerr << "❌ Error receiving response (timeout or network error)" << std::endl;
        std::cerr << "   Make sure the UDP server is running on " << server_ip << ":" << SERVER_PORT << std::endl;
    }
    
    // Close socket
    close(client_socket);
    std::cout << "\n🔌 Connection closed" << std::endl;
    
    return 0;
}

/*
Usage Examples:
  ./udp_client                           # Interactive mode
  ./udp_client "Hello Server"            # Send specific message to localhost
  ./udp_client "Hello" 192.168.1.100     # Send to specific IP
*/