/**
 * @file udp_test.cpp
 * @brief Simple UDP server and client test
 */

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void test_udp_communication() {
    std::cout << "=== UDP SERVER AND CLIENT TEST ===" << std::endl;
    
    // Create UDP server socket
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        std::cout << "Failed to create UDP server socket" << std::endl;
        return;
    }
    
    // Configure server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(9999);
    
    // Bind socket to address
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cout << "Failed to bind UDP server socket" << std::endl;
        close(server_socket);
        return;
    }
    
    std::cout << "✓ UDP Server created and bound to port 9999" << std::endl;
    
    // Create UDP client socket
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        std::cout << "Failed to create UDP client socket" << std::endl;
        close(server_socket);
        return;
    }
    
    // Configure target server address for client
    struct sockaddr_in target_addr;
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &target_addr.sin_addr);
    
    std::cout << "✓ UDP Client created" << std::endl;
    
    // Test communication
    std::string message = "Hello UDP Server from C++!";
    std::cout << "\n→ Client sending: \"" << message << "\"" << std::endl;
    
    // Send message from client to server
    ssize_t sent = sendto(client_socket, message.c_str(), message.length(),
                         0, (struct sockaddr*)&target_addr, sizeof(target_addr));
    
    if (sent > 0) {
        std::cout << "✓ Message sent (" << sent << " bytes)" << std::endl;
        
        // Receive on server side
        char buffer[1024];
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        // Set timeout for receive
        struct timeval timeout;
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;
        setsockopt(server_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        
        ssize_t received = recvfrom(server_socket, buffer, sizeof(buffer) - 1,
                                   0, (struct sockaddr*)&client_addr, &client_len);
        
        if (received > 0) {
            buffer[received] = '\0';
            std::cout << "← Server received: \"" << buffer << "\"" << std::endl;
            
            // Echo back to client
            std::string echo = "Echo: " + std::string(buffer);
            sendto(server_socket, echo.c_str(), echo.length(),
                   0, (struct sockaddr*)&client_addr, client_len);
            std::cout << "→ Server echoed: \"" << echo << "\"" << std::endl;
            
            // Client receives echo
            char echo_buffer[1024];
            setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
            ssize_t echo_received = recvfrom(client_socket, echo_buffer, sizeof(echo_buffer) - 1,
                                           0, nullptr, nullptr);
            
            if (echo_received > 0) {
                echo_buffer[echo_received] = '\0';
                std::cout << "← Client received: \"" << echo_buffer << "\"" << std::endl;
                std::cout << "\n✅ UDP communication successful!" << std::endl;
            } else {
                std::cout << "⚠️  Client did not receive echo" << std::endl;
            }
        } else {
            std::cout << "⚠️  Server did not receive message" << std::endl;
        }
    } else {
        std::cout << "❌ Failed to send message" << std::endl;
    }
    
    // Cleanup
    close(client_socket);
    close(server_socket);
    
    std::cout << "\n=== UDP Programming Summary ===" << std::endl;
    std::cout << "Server Steps:" << std::endl;
    std::cout << "1. socket(AF_INET, SOCK_DGRAM, 0)" << std::endl;
    std::cout << "2. bind() to local address and port" << std::endl;
    std::cout << "3. recvfrom() to receive messages" << std::endl;
    std::cout << "4. sendto() to send responses" << std::endl;
    std::cout << "5. close() when done" << std::endl;
    
    std::cout << "\nClient Steps:" << std::endl;
    std::cout << "1. socket(AF_INET, SOCK_DGRAM, 0)" << std::endl;
    std::cout << "2. sendto() with server address" << std::endl;
    std::cout << "3. recvfrom() to receive responses" << std::endl;
    std::cout << "4. close() when done" << std::endl;
    
    std::cout << "\nKey Differences from TCP:" << std::endl;
    std::cout << "• No listen() or accept() calls" << std::endl;
    std::cout << "• Use sendto()/recvfrom() instead of send()/recv()" << std::endl;
    std::cout << "• Each message includes destination address" << std::endl;
    std::cout << "• No connection state maintained" << std::endl;
    std::cout << "• Messages may be lost or reordered" << std::endl;
}

int main() {
    test_udp_communication();
    return 0;
}