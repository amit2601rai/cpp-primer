/**
 * @file networking.cpp
 * @author GitHub Copilot
 * @brief Tutorial code for networking concepts, protocols, sockets, and network programming.
 */

// This program provides a comprehensive overview of networking including:
// - OSI and TCP/IP protocol stack
// - Sockets and ports
// - TCP vs UDP protocols
// - Network programming with sockets
// - Key protocols: HTTP, SMTP, SSH, Telnet, FTP
// - Network addressing and routing
// - Practical socket programming examples
// - Security considerations and protocols

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

void demonstrate_networking_overview() {
    std::cout << "=== NETWORKING FUNDAMENTALS OVERVIEW ===" << std::endl;
    
    // Network Protocol Stack:
    // ┌─ OSI MODEL vs TCP/IP STACK ─────────────────────────────┐
    // │                                                         │
    // │ OSI 7-Layer Model          TCP/IP 4-Layer Model        │
    // │ ┌─ Layer 7: Application ─┐  ┌─ Application Layer ────┐  │
    // │ │ HTTP, SMTP, SSH, FTP   │  │ HTTP, SMTP, SSH, FTP,  │  │
    // │ │ Telnet, DNS, DHCP      │  │ Telnet, DNS, DHCP      │  │
    // │ └────────────────────────┘  └────────────────────────┘  │
    // │ ┌─ Layer 6: Presentation ┐                              │
    // │ │ SSL/TLS, Encryption    │  (Handled by Application)   │
    // │ └────────────────────────┘                              │
    // │ ┌─ Layer 5: Session ─────┐                              │
    // │ │ Session Management     │  (Handled by Application)   │
    // │ └────────────────────────┘                              │
    // │ ┌─ Layer 4: Transport ───┐  ┌─ Transport Layer ──────┐  │
    // │ │ TCP, UDP               │  │ TCP, UDP               │  │
    // │ │ Port Numbers           │  │ Port Numbers           │  │
    // │ └────────────────────────┘  └────────────────────────┘  │
    // │ ┌─ Layer 3: Network ─────┐  ┌─ Internet Layer ──────┐  │
    // │ │ IP, ICMP, IGMP         │  │ IP, ICMP, IGMP         │  │
    // │ │ Routing                │  │ Routing                │  │
    // │ └────────────────────────┘  └────────────────────────┘  │
    // │ ┌─ Layer 2: Data Link ───┐  ┌─ Network Access Layer ─┐  │
    // │ │ Ethernet, WiFi         │  │ Ethernet, WiFi         │  │
    // │ │ MAC Addresses          │  │ MAC Addresses          │  │
    // │ └────────────────────────┘  │                        │  │
    // │ ┌─ Layer 1: Physical ────┐  │ (Physical Medium)      │  │
    // │ │ Cables, Radio Waves    │  │                        │  │
    // │ └────────────────────────┘  └────────────────────────┘  │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- Protocol Stack Layers ---" << std::endl;
    std::cout << "Application Layer:" << std::endl;
    std::cout << "• User applications and services" << std::endl;
    std::cout << "• HTTP (web), SMTP (email), SSH (secure shell)" << std::endl;
    std::cout << "• FTP (file transfer), Telnet (remote terminal)" << std::endl;
    std::cout << "• DNS (domain names), DHCP (IP assignment)" << std::endl;
    
    std::cout << "\nTransport Layer:" << std::endl;
    std::cout << "• End-to-end communication" << std::endl;
    std::cout << "• TCP: Reliable, connection-oriented" << std::endl;
    std::cout << "• UDP: Fast, connectionless" << std::endl;
    std::cout << "• Port numbers (0-65535)" << std::endl;
    
    std::cout << "\nInternet Layer:" << std::endl;
    std::cout << "• Host-to-host routing" << std::endl;
    std::cout << "• IP addresses (IPv4/IPv6)" << std::endl;
    std::cout << "• Packet forwarding" << std::endl;
    std::cout << "• ICMP (error reporting)" << std::endl;
    
    std::cout << "\nNetwork Access Layer:" << std::endl;
    std::cout << "• Physical network access" << std::endl;
    std::cout << "• Ethernet, WiFi protocols" << std::endl;
    std::cout << "• MAC addresses" << std::endl;
    std::cout << "• Frame transmission" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_ip_addressing() {
    std::cout << "=== IP ADDRESSING AND PORTS ===" << std::endl;
    
    // IP Address Structure:
    // ┌─ IPv4 ADDRESS STRUCTURE ────────────────────────────────┐
    // │                                                         │
    // │ 32-bit Address: 192.168.1.100                          │
    // │ ┌─ Binary Representation ─────────────────────────────┐ │
    // │ │ 11000000.10101000.00000001.01100100                │ │
    // │ │    192      168      1       100                   │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ Address Classes (Traditional):                          │
    // │ ┌─ Class A ───────────────────────────────────────────┐ │
    // │ │ 0.0.0.0 - 127.255.255.255                          │ │
    // │ │ Network: 8 bits, Host: 24 bits                     │ │
    // │ │ Example: 10.0.0.0/8 (Private)                      │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │ ┌─ Class B ───────────────────────────────────────────┐ │
    // │ │ 128.0.0.0 - 191.255.255.255                        │ │
    // │ │ Network: 16 bits, Host: 16 bits                    │ │
    // │ │ Example: 172.16.0.0/12 (Private)                   │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │ ┌─ Class C ───────────────────────────────────────────┐ │
    // │ │ 192.0.0.0 - 223.255.255.255                        │ │
    // │ │ Network: 24 bits, Host: 8 bits                     │ │
    // │ │ Example: 192.168.0.0/16 (Private)                  │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ CIDR Notation (Modern):                                │
    // │ 192.168.1.0/24 = Network mask 255.255.255.0           │
    // │ 172.16.0.0/16  = Network mask 255.255.0.0             │
    // │ 10.0.0.0/8     = Network mask 255.0.0.0               │
    // └─────────────────────────────────────────────────────────┘
    
    // Port Number Structure:
    // ┌─ PORT NUMBER RANGES ────────────────────────────────────┐
    // │                                                         │
    // │ 16-bit Port Number (0-65535):                          │
    // │ ┌─ Well-Known Ports (0-1023) ─────────────────────────┐ │
    // │ │ 20/21: FTP (Data/Control)                           │ │
    // │ │ 22:    SSH (Secure Shell)                           │ │
    // │ │ 23:    Telnet                                       │ │
    // │ │ 25:    SMTP (Email)                                 │ │
    // │ │ 53:    DNS                                          │ │
    // │ │ 80:    HTTP                                         │ │
    // │ │ 110:   POP3 (Email)                                 │ │
    // │ │ 143:   IMAP (Email)                                 │ │
    // │ │ 443:   HTTPS (Secure HTTP)                          │ │
    // │ │ 993:   IMAPS (Secure IMAP)                          │ │
    // │ │ 995:   POP3S (Secure POP3)                          │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │ ┌─ Registered Ports (1024-49151) ─────────────────────┐ │
    // │ │ 3306:  MySQL Database                               │ │
    // │ │ 5432:  PostgreSQL Database                          │ │
    // │ │ 6379:  Redis                                        │ │
    // │ │ 8080:  Alternative HTTP                             │ │
    // │ │ 27017: MongoDB                                      │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │ ┌─ Dynamic/Private Ports (49152-65535) ───────────────┐ │
    // │ │ Used by applications for outbound connections       │ │
    // │ │ Assigned automatically by OS                        │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- IP Address Types ---" << std::endl;
    std::cout << "IPv4 (32-bit addresses):" << std::endl;
    std::cout << "• Format: 192.168.1.100" << std::endl;
    std::cout << "• Private ranges: 10.0.0.0/8, 172.16.0.0/12, 192.168.0.0/16" << std::endl;
    std::cout << "• Loopback: 127.0.0.1 (localhost)" << std::endl;
    std::cout << "• Broadcast: 255.255.255.255" << std::endl;
    
    std::cout << "\nIPv6 (128-bit addresses):" << std::endl;
    std::cout << "• Format: 2001:0db8:85a3:0000:0000:8a2e:0370:7334" << std::endl;
    std::cout << "• Compressed: 2001:db8:85a3::8a2e:370:7334" << std::endl;
    std::cout << "• Loopback: ::1" << std::endl;
    
    std::cout << "\n--- Port Numbers ---" << std::endl;
    std::cout << "Well-Known Ports (0-1023):" << std::endl;
    std::cout << "• HTTP: 80, HTTPS: 443" << std::endl;
    std::cout << "• SSH: 22, Telnet: 23" << std::endl;
    std::cout << "• SMTP: 25, DNS: 53" << std::endl;
    std::cout << "• FTP: 20/21, POP3: 110, IMAP: 143" << std::endl;
    
    std::cout << "\nRegistered Ports (1024-49151):" << std::endl;
    std::cout << "• Application-specific services" << std::endl;
    std::cout << "• Database ports: MySQL (3306), PostgreSQL (5432)" << std::endl;
    std::cout << "• Alternative HTTP: 8080, 8443" << std::endl;
    
    std::cout << "\nDynamic Ports (49152-65535):" << std::endl;
    std::cout << "• Client-side connections" << std::endl;
    std::cout << "• Automatically assigned by OS" << std::endl;
    std::cout << "• Ephemeral/temporary connections" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_sockets() {
    std::cout << "=== SOCKETS: NETWORK PROGRAMMING INTERFACE ===" << std::endl;
    
    // Socket Communication Model:
    // ┌─ SOCKET COMMUNICATION MODEL ────────────────────────────┐
    // │                                                         │
    // │ Client Application          Server Application          │
    // │ ┌─────────────────────┐     ┌─────────────────────────┐ │
    // │ │ socket()            │     │ socket()                │ │
    // │ │ ↓                   │     │ ↓                       │ │
    // │ │ connect()           │     │ bind()                  │ │
    // │ │ ↓                   │     │ ↓                       │ │
    // │ │ send()/recv()       │ ←───┤ listen()                │ │
    // │ │ ↓                   │     │ ↓                       │ │
    // │ │ close()             │     │ accept() → new socket   │ │
    // │ └─────────────────────┘     │ ↓                       │ │
    // │                             │ send()/recv()           │ │
    // │                             │ ↓                       │ │
    // │                             │ close()                 │ │
    // │                             └─────────────────────────┘ │
    // │                                                         │
    // │ Socket Addressing:                                      │
    // │ ┌─ Socket Address Structure ──────────────────────────┐ │
    // │ │ struct sockaddr_in {                                │ │
    // │ │     short sin_family;        // AF_INET             │ │
    // │ │     unsigned short sin_port; // Port number         │ │
    // │ │     struct in_addr sin_addr; // IP address          │ │
    // │ │     char sin_zero[8];        // Padding             │ │
    // │ │ };                                                  │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ Socket Types:                                           │
    // │ • SOCK_STREAM (TCP): Reliable, ordered, bidirectional  │
    // │ • SOCK_DGRAM (UDP):  Fast, connectionless, unreliable  │
    // │ • SOCK_RAW:          Direct IP access (admin only)     │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- Socket Fundamentals ---" << std::endl;
    std::cout << "Socket Definition:" << std::endl;
    std::cout << "• Endpoint for network communication" << std::endl;
    std::cout << "• Identified by IP address + port number" << std::endl;
    std::cout << "• Abstraction for network programming" << std::endl;
    std::cout << "• Available in multiple programming languages" << std::endl;
    
    std::cout << "\nSocket Types:" << std::endl;
    std::cout << "TCP Sockets (SOCK_STREAM):" << std::endl;
    std::cout << "• Reliable, ordered delivery" << std::endl;
    std::cout << "• Connection-oriented" << std::endl;
    std::cout << "• Error detection and correction" << std::endl;
    std::cout << "• Used by HTTP, SMTP, SSH, etc." << std::endl;
    
    std::cout << "\nUDP Sockets (SOCK_DGRAM):" << std::endl;
    std::cout << "• Fast, lightweight" << std::endl;
    std::cout << "• Connectionless" << std::endl;
    std::cout << "• No delivery guarantee" << std::endl;
    std::cout << "• Used by DNS, DHCP, streaming" << std::endl;
    
    std::cout << "\n--- Socket Programming Steps ---" << std::endl;
    std::cout << "Server Side:" << std::endl;
    std::cout << "1. socket() - Create socket" << std::endl;
    std::cout << "2. bind() - Bind to address/port" << std::endl;
    std::cout << "3. listen() - Listen for connections" << std::endl;
    std::cout << "4. accept() - Accept client connections" << std::endl;
    std::cout << "5. send()/recv() - Exchange data" << std::endl;
    std::cout << "6. close() - Close connection" << std::endl;
    
    std::cout << "\nClient Side:" << std::endl;
    std::cout << "1. socket() - Create socket" << std::endl;
    std::cout << "2. connect() - Connect to server" << std::endl;
    std::cout << "3. send()/recv() - Exchange data" << std::endl;
    std::cout << "4. close() - Close connection" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_tcp_protocol() {
    std::cout << "=== TCP (Transmission Control Protocol) ===" << std::endl;
    
    // TCP Header Structure:
    // ┌─ TCP HEADER STRUCTURE ──────────────────────────────────┐
    // │                                                         │
    // │  0                   15                   31            │
    // │ ┌─────────────────────┬─────────────────────┐           │
    // │ │   Source Port       │  Destination Port   │           │
    // │ ├─────────────────────┴─────────────────────┤           │
    // │ │           Sequence Number                 │           │
    // │ ├───────────────────────────────────────────┤           │
    // │ │        Acknowledgment Number              │           │
    // │ ├─────┬───┬─┬─┬─┬─┬─┬─┴─────────────────────┤           │
    // │ │Offset│Res│U│A│P│R│S│F│    Window Size      │           │
    // │ │     │   │R│C│S│S│Y│I│                     │           │
    // │ │     │   │G│K│H│T│N│N│                     │           │
    // │ ├─────┴───┴─┴─┴─┴─┴─┴─┬─────────────────────┤           │
    // │ │      Checksum       │   Urgent Pointer    │           │
    // │ ├─────────────────────┴─────────────────────┤           │
    // │ │              Options (optional)           │           │
    // │ └───────────────────────────────────────────┘           │
    // │                                                         │
    // │ Key Fields:                                             │
    // │ • Sequence Number: Order packets, detect duplicates    │
    // │ • ACK Number: Confirm received data                     │
    // │ • Flags: SYN, ACK, FIN, RST, PSH, URG                  │
    // │ • Window Size: Flow control                             │
    // │ • Checksum: Error detection                             │
    // └─────────────────────────────────────────────────────────┘
    
    // TCP Connection Establishment (3-Way Handshake):
    // ┌─ TCP 3-WAY HANDSHAKE ───────────────────────────────────┐
    // │                                                         │
    // │ Client                                    Server        │
    // │ ┌─────┐                                  ┌─────┐        │
    // │ │     │ ──── SYN (seq=x) ──────────────→ │     │        │
    // │ │     │                                  │     │        │
    // │ │     │ ←── SYN+ACK (seq=y, ack=x+1) ─── │     │        │
    // │ │     │                                  │     │        │
    // │ │     │ ──── ACK (seq=x+1, ack=y+1) ───→ │     │        │
    // │ └─────┘                                  └─────┘        │
    // │                                                         │
    // │ State Changes:                                          │
    // │ Client: CLOSED → SYN_SENT → ESTABLISHED                │
    // │ Server: CLOSED → LISTEN → SYN_RCVD → ESTABLISHED       │
    // │                                                         │
    // │ Connection Termination (4-Way Handshake):              │
    // │ ┌─────┐                                  ┌─────┐        │
    // │ │     │ ──── FIN (seq=x) ──────────────→ │     │        │
    // │ │     │                                  │     │        │
    // │ │     │ ←── ACK (ack=x+1) ────────────── │     │        │
    // │ │     │                                  │     │        │
    // │ │     │ ←── FIN (seq=y) ────────────────│ │     │        │
    // │ │     │                                  │     │        │
    // │ │     │ ──── ACK (ack=y+1) ────────────→ │     │        │
    // │ └─────┘                                  └─────┘        │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- TCP Characteristics ---" << std::endl;
    std::cout << "Reliable Transport:" << std::endl;
    std::cout << "• Guaranteed delivery of data" << std::endl;
    std::cout << "• In-order packet delivery" << std::endl;
    std::cout << "• Error detection and correction" << std::endl;
    std::cout << "• Duplicate detection and removal" << std::endl;
    
    std::cout << "\nConnection-Oriented:" << std::endl;
    std::cout << "• 3-way handshake to establish connection" << std::endl;
    std::cout << "• Maintains connection state" << std::endl;
    std::cout << "• 4-way handshake to close connection" << std::endl;
    std::cout << "• Full-duplex communication" << std::endl;
    
    std::cout << "\nFlow Control:" << std::endl;
    std::cout << "• Window-based flow control" << std::endl;
    std::cout << "• Prevents receiver buffer overflow" << std::endl;
    std::cout << "• Dynamic window size adjustment" << std::endl;
    
    std::cout << "\nCongestion Control:" << std::endl;
    std::cout << "• Slow start algorithm" << std::endl;
    std::cout << "• Congestion avoidance" << std::endl;
    std::cout << "• Fast retransmit and recovery" << std::endl;
    
    std::cout << "\n--- TCP Connection States ---" << std::endl;
    std::cout << "CLOSED → LISTEN → SYN_RCVD → ESTABLISHED" << std::endl;
    std::cout << "ESTABLISHED → FIN_WAIT_1 → FIN_WAIT_2 → TIME_WAIT → CLOSED" << std::endl;
    
    std::cout << "\n--- TCP Use Cases ---" << std::endl;
    std::cout << "Perfect for:" << std::endl;
    std::cout << "• Web browsing (HTTP/HTTPS)" << std::endl;
    std::cout << "• Email (SMTP, POP3, IMAP)" << std::endl;
    std::cout << "• File transfer (FTP, SFTP)" << std::endl;
    std::cout << "• Remote access (SSH, Telnet)" << std::endl;
    std::cout << "• Database connections" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_udp_protocol() {
    std::cout << "=== UDP (User Datagram Protocol) ===" << std::endl;
    
    // UDP Header Structure:
    // ┌─ UDP HEADER STRUCTURE ──────────────────────────────────┐
    // │                                                         │
    // │  0                   15                   31            │
    // │ ┌─────────────────────┬─────────────────────┐           │
    // │ │   Source Port       │  Destination Port   │           │
    // │ ├─────────────────────┼─────────────────────┤           │
    // │ │      Length         │      Checksum       │           │
    // │ └─────────────────────┴─────────────────────┘           │
    // │ ┌───────────────────────────────────────────┐           │
    // │ │                Data                       │           │
    // │ │             (Variable Length)             │           │
    // │ └───────────────────────────────────────────┘           │
    // │                                                         │
    // │ Header Size: Only 8 bytes (vs 20 bytes for TCP)        │
    // │ Simple Structure: Minimal overhead                      │
    // │ No Connection State: Stateless protocol                │
    // └─────────────────────────────────────────────────────────┘
    
    // UDP Communication Model:
    // ┌─ UDP COMMUNICATION MODEL ───────────────────────────────┐
    // │                                                         │
    // │ Client                                    Server        │
    // │ ┌─────┐                                  ┌─────┐        │
    // │ │     │ ──── Datagram 1 ───────────────→ │     │        │
    // │ │     │                                  │     │        │
    // │ │     │ ──── Datagram 2 ───────────────→ │     │        │
    // │ │     │                                  │     │        │
    // │ │     │ ←─── Response 1 ────────────────  │     │        │
    // │ │     │                                  │     │        │
    // │ │     │ ──── Datagram 3 ───────────────→ │     │        │
    // │ └─────┘           ↑                      └─────┘        │
    // │                   │                                     │
    // │           May be lost, duplicated,                      │
    // │           or arrive out of order                        │
    // │                                                         │
    // │ Characteristics:                                        │
    // │ • No connection establishment                           │
    // │ • No delivery guarantee                                 │
    // │ • No ordering guarantee                                 │
    // │ • Minimal processing overhead                           │
    // │ • Application handles reliability                       │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- UDP Characteristics ---" << std::endl;
    std::cout << "Connectionless:" << std::endl;
    std::cout << "• No connection establishment" << std::endl;
    std::cout << "• Each datagram is independent" << std::endl;
    std::cout << "• No connection state maintained" << std::endl;
    std::cout << "• Send and forget model" << std::endl;
    
    std::cout << "\nUnreliable:" << std::endl;
    std::cout << "• No delivery guarantee" << std::endl;
    std::cout << "• No acknowledgments" << std::endl;
    std::cout << "• No retransmission" << std::endl;
    std::cout << "• Packets may be lost, duplicated, or reordered" << std::endl;
    
    std::cout << "\nLightweight:" << std::endl;
    std::cout << "• Minimal 8-byte header" << std::endl;
    std::cout << "• Low processing overhead" << std::endl;
    std::cout << "• Fast transmission" << std::endl;
    std::cout << "• No flow control" << std::endl;
    
    std::cout << "\n--- UDP vs TCP Comparison ---" << std::endl;
    std::cout << "┌─────────────┬─────────────┬─────────────┐" << std::endl;
    std::cout << "│ Feature     │ TCP         │ UDP         │" << std::endl;
    std::cout << "├─────────────┼─────────────┼─────────────┤" << std::endl;
    std::cout << "│ Reliability │ Reliable    │ Unreliable  │" << std::endl;
    std::cout << "│ Connection  │ Oriented    │ Less        │" << std::endl;
    std::cout << "│ Speed       │ Slower      │ Faster      │" << std::endl;
    std::cout << "│ Overhead    │ High        │ Low         │" << std::endl;
    std::cout << "│ Header Size │ 20 bytes    │ 8 bytes     │" << std::endl;
    std::cout << "│ Ordering    │ Guaranteed  │ None        │" << std::endl;
    std::cout << "│ Flow Control│ Yes         │ No          │" << std::endl;
    std::cout << "└─────────────┴─────────────┴─────────────┘" << std::endl;
    
    std::cout << "\n--- UDP Use Cases ---" << std::endl;
    std::cout << "Perfect for:" << std::endl;
    std::cout << "• DNS queries (fast lookup)" << std::endl;
    std::cout << "• DHCP (IP address assignment)" << std::endl;
    std::cout << "• Live streaming (video/audio)" << std::endl;
    std::cout << "• Online gaming (low latency)" << std::endl;
    std::cout << "• IoT sensor data" << std::endl;
    std::cout << "• Network discovery protocols" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_http_protocol() {
    std::cout << "=== HTTP (HyperText Transfer Protocol) ===" << std::endl;
    
    // HTTP Request/Response Model:
    // ┌─ HTTP REQUEST/RESPONSE MODEL ───────────────────────────┐
    // │                                                         │
    // │ Client (Browser)                    Server (Web Server) │
    // │ ┌─────────────────┐                ┌─────────────────┐  │
    // │ │                 │ ── Request ──→ │                 │  │
    // │ │ HTTP Request:   │                │ HTTP Response:  │  │
    // │ │ GET /index.html │                │ 200 OK          │  │
    // │ │ Host: example.com│               │ Content-Type:   │  │
    // │ │ User-Agent: ... │                │ text/html       │  │
    // │ │                 │ ←─ Response ── │ <html>...</html>│  │
    // │ └─────────────────┘                └─────────────────┘  │
    // │                                                         │
    // │ HTTP Request Structure:                                 │
    // │ ┌─ Request Line ──────────────────────────────────────┐ │
    // │ │ METHOD /path/resource HTTP/version                  │ │
    // │ │ GET /api/users HTTP/1.1                             │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │ ┌─ Headers ───────────────────────────────────────────┐ │
    // │ │ Host: www.example.com                               │ │
    // │ │ User-Agent: Mozilla/5.0 ...                         │ │
    // │ │ Accept: text/html,application/json                  │ │
    // │ │ Content-Type: application/json                      │ │
    // │ │ Content-Length: 123                                 │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │ ┌─ Body (Optional) ───────────────────────────────────┐ │
    // │ │ {"name": "John", "email": "john@example.com"}       │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ HTTP Response Structure:                                │
    // │ ┌─ Status Line ───────────────────────────────────────┐ │
    // │ │ HTTP/version STATUS_CODE Reason-Phrase              │ │
    // │ │ HTTP/1.1 200 OK                                     │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │ ┌─ Headers ───────────────────────────────────────────┐ │
    // │ │ Content-Type: text/html                             │ │
    // │ │ Content-Length: 1234                                │ │
    // │ │ Server: Apache/2.4.41                               │ │
    // │ │ Set-Cookie: sessionid=abc123                        │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │ ┌─ Body ──────────────────────────────────────────────┐ │
    // │ │ <html><head><title>Example</title></head>...        │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- HTTP Methods ---" << std::endl;
    std::cout << "GET: Retrieve data from server" << std::endl;
    std::cout << "• Safe and idempotent" << std::endl;
    std::cout << "• No request body" << std::endl;
    std::cout << "• Cacheable" << std::endl;
    
    std::cout << "\nPOST: Submit data to server" << std::endl;
    std::cout << "• Create new resources" << std::endl;
    std::cout << "• Has request body" << std::endl;
    std::cout << "• Not idempotent" << std::endl;
    
    std::cout << "\nPUT: Update/replace resource" << std::endl;
    std::cout << "• Complete resource replacement" << std::endl;
    std::cout << "• Idempotent" << std::endl;
    
    std::cout << "\nDELETE: Remove resource" << std::endl;
    std::cout << "• Remove specified resource" << std::endl;
    std::cout << "• Idempotent" << std::endl;
    
    std::cout << "\n--- HTTP Status Codes ---" << std::endl;
    std::cout << "1xx Informational:" << std::endl;
    std::cout << "• 100 Continue, 101 Switching Protocols" << std::endl;
    
    std::cout << "\n2xx Success:" << std::endl;
    std::cout << "• 200 OK, 201 Created, 204 No Content" << std::endl;
    
    std::cout << "\n3xx Redirection:" << std::endl;
    std::cout << "• 301 Moved Permanently, 302 Found, 304 Not Modified" << std::endl;
    
    std::cout << "\n4xx Client Error:" << std::endl;
    std::cout << "• 400 Bad Request, 401 Unauthorized, 404 Not Found" << std::endl;
    
    std::cout << "\n5xx Server Error:" << std::endl;
    std::cout << "• 500 Internal Server Error, 502 Bad Gateway, 503 Service Unavailable" << std::endl;
    
    std::cout << "\n--- HTTP Versions ---" << std::endl;
    std::cout << "HTTP/1.0:" << std::endl;
    std::cout << "• One request per connection" << std::endl;
    std::cout << "• No persistent connections" << std::endl;
    
    std::cout << "\nHTTP/1.1:" << std::endl;
    std::cout << "• Persistent connections" << std::endl;
    std::cout << "• Request pipelining" << std::endl;
    std::cout << "• Host header required" << std::endl;
    
    std::cout << "\nHTTP/2:" << std::endl;
    std::cout << "• Binary protocol" << std::endl;
    std::cout << "• Multiplexing" << std::endl;
    std::cout << "• Header compression" << std::endl;
    std::cout << "• Server push" << std::endl;
    
    std::cout << "\nHTTP/3:" << std::endl;
    std::cout << "• Based on QUIC (UDP)" << std::endl;
    std::cout << "• Improved performance" << std::endl;
    std::cout << "• Better mobile support" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_other_protocols() {
    std::cout << "=== OTHER NETWORK PROTOCOLS ===" << std::endl;
    
    // Protocol Comparison:
    // ┌─ PROTOCOL COMPARISON ───────────────────────────────────┐
    // │                                                         │
    // │ Protocol │ Port │ Transport │ Purpose                  │
    // │ ─────────┼──────┼───────────┼─────────────────────────  │
    // │ SSH      │  22  │    TCP    │ Secure remote access     │
    // │ Telnet   │  23  │    TCP    │ Insecure remote access   │
    // │ SMTP     │  25  │    TCP    │ Email sending            │
    // │ DNS      │  53  │   UDP     │ Domain name resolution   │
    // │ HTTP     │  80  │    TCP    │ Web browsing             │
    // │ POP3     │ 110  │    TCP    │ Email retrieval          │
    // │ IMAP     │ 143  │    TCP    │ Email access             │
    // │ HTTPS    │ 443  │    TCP    │ Secure web browsing      │
    // │ SMTPS    │ 465  │    TCP    │ Secure email sending     │
    // │ IMAPS    │ 993  │    TCP    │ Secure email access      │
    // │ POP3S    │ 995  │    TCP    │ Secure email retrieval   │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- SSH (Secure Shell) - Port 22 ---" << std::endl;
    std::cout << "Purpose: Secure remote access and file transfer" << std::endl;
    std::cout << "Features:" << std::endl;
    std::cout << "• Encrypted communication" << std::endl;
    std::cout << "• Authentication (password/key-based)" << std::endl;
    std::cout << "• Remote command execution" << std::endl;
    std::cout << "• File transfer (SCP, SFTP)" << std::endl;
    std::cout << "• Port forwarding/tunneling" << std::endl;
    
    std::cout << "\n--- Telnet - Port 23 ---" << std::endl;
    std::cout << "Purpose: Remote terminal access (INSECURE)" << std::endl;
    std::cout << "Characteristics:" << std::endl;
    std::cout << "• Plain text communication" << std::endl;
    std::cout << "• No encryption" << std::endl;
    std::cout << "• Passwords sent in clear text" << std::endl;
    std::cout << "• Largely replaced by SSH" << std::endl;
    std::cout << "• Still used for debugging" << std::endl;
    
    std::cout << "\n--- SMTP (Simple Mail Transfer Protocol) - Port 25 ---" << std::endl;
    std::cout << "Purpose: Email sending between servers" << std::endl;
    std::cout << "Process:" << std::endl;
    std::cout << "1. HELO/EHLO (greeting)" << std::endl;
    std::cout << "2. MAIL FROM (sender)" << std::endl;
    std::cout << "3. RCPT TO (recipient)" << std::endl;
    std::cout << "4. DATA (message content)" << std::endl;
    std::cout << "5. QUIT (disconnect)" << std::endl;
    
    std::cout << "\n--- DNS (Domain Name System) - Port 53 ---" << std::endl;
    std::cout << "Purpose: Translate domain names to IP addresses" << std::endl;
    std::cout << "Record Types:" << std::endl;
    std::cout << "• A: IPv4 address" << std::endl;
    std::cout << "• AAAA: IPv6 address" << std::endl;
    std::cout << "• CNAME: Canonical name (alias)" << std::endl;
    std::cout << "• MX: Mail exchange" << std::endl;
    std::cout << "• TXT: Text records" << std::endl;
    
    std::cout << "\n--- FTP (File Transfer Protocol) - Ports 20/21 ---" << std::endl;
    std::cout << "Purpose: File transfer between client and server" << std::endl;
    std::cout << "Modes:" << std::endl;
    std::cout << "• Active Mode: Server connects to client" << std::endl;
    std::cout << "• Passive Mode: Client connects to server" << std::endl;
    std::cout << "• Port 21: Control connection" << std::endl;
    std::cout << "• Port 20: Data connection" << std::endl;
    
    std::cout << "\n--- Email Protocols ---" << std::endl;
    std::cout << "POP3 (Post Office Protocol 3) - Port 110:" << std::endl;
    std::cout << "• Download and delete model" << std::endl;
    std::cout << "• Single device access" << std::endl;
    std::cout << "• Simple protocol" << std::endl;
    
    std::cout << "\nIMAP (Internet Message Access Protocol) - Port 143:" << std::endl;
    std::cout << "• Server-side message storage" << std::endl;
    std::cout << "• Multi-device synchronization" << std::endl;
    std::cout << "• Folder support" << std::endl;
    std::cout << "• Partial message download" << std::endl;
    
    std::cout << "\n--- Security Considerations ---" << std::endl;
    std::cout << "Secure Variants:" << std::endl;
    std::cout << "• HTTPS (HTTP over TLS): Port 443" << std::endl;
    std::cout << "• SMTPS (SMTP over SSL): Port 465" << std::endl;
    std::cout << "• IMAPS (IMAP over SSL): Port 993" << std::endl;
    std::cout << "• POP3S (POP3 over SSL): Port 995" << std::endl;
    std::cout << "• FTPS (FTP over SSL): Port 990" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_socket_programming() {
    std::cout << "=== PRACTICAL SOCKET PROGRAMMING ===" << std::endl;
    
    std::cout << "\n--- TCP Server Example (Simplified) ---" << std::endl;
    std::cout << "// Create TCP server socket" << std::endl;
    std::cout << "int server_socket = socket(AF_INET, SOCK_STREAM, 0);" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "// Configure server address" << std::endl;
    std::cout << "struct sockaddr_in server_addr;" << std::endl;
    std::cout << "server_addr.sin_family = AF_INET;" << std::endl;
    std::cout << "server_addr.sin_addr.s_addr = INADDR_ANY;" << std::endl;
    std::cout << "server_addr.sin_port = htons(8080);" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "// Bind socket to address" << std::endl;
    std::cout << "bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "// Listen for connections" << std::endl;
    std::cout << "listen(server_socket, 5);" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "// Accept client connections" << std::endl;
    std::cout << "int client_socket = accept(server_socket, NULL, NULL);" << std::endl;
    
    std::cout << "\n--- TCP Client Example (Simplified) ---" << std::endl;
    std::cout << "// Create TCP client socket" << std::endl;
    std::cout << "int client_socket = socket(AF_INET, SOCK_STREAM, 0);" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "// Configure server address" << std::endl;
    std::cout << "struct sockaddr_in server_addr;" << std::endl;
    std::cout << "server_addr.sin_family = AF_INET;" << std::endl;
    std::cout << "server_addr.sin_port = htons(8080);" << std::endl;
    std::cout << "inet_pton(AF_INET, \"127.0.0.1\", &server_addr.sin_addr);" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "// Connect to server" << std::endl;
    std::cout << "connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));" << std::endl;
    
    // Demonstrate simple HTTP client
    std::cout << "\n--- Simple HTTP Request Demonstration ---" << std::endl;
    
    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cout << "Failed to create socket" << std::endl;
        return;
    }
    
    // Configure server address (Google DNS for demo)
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    
    // Convert IP address
    if (inet_pton(AF_INET, "8.8.8.8", &server_addr.sin_addr) <= 0) {
        std::cout << "Invalid IP address" << std::endl;
        close(sock);
        return;
    }
    
    // Connect to server (this will fail for 8.8.8.8:80, but demonstrates the process)
    std::cout << "Attempting to connect to server..." << std::endl;
    
    // Set socket timeout for demonstration
    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cout << "Connection failed (expected for this demo)" << std::endl;
    } else {
        std::cout << "Connection successful!" << std::endl;
        
        // Send HTTP request
        std::string request = "GET / HTTP/1.1\r\nHost: example.com\r\n\r\n";
        send(sock, request.c_str(), request.length(), 0);
        
        // Receive response
        char buffer[1024] = {0};
        recv(sock, buffer, 1024, 0);
        std::cout << "Response: " << buffer << std::endl;
    }
    
    close(sock);
    
    std::cout << "\n--- Socket Programming Best Practices ---" << std::endl;
    std::cout << "Error Handling:" << std::endl;
    std::cout << "• Always check return values" << std::endl;
    std::cout << "• Handle EAGAIN/EWOULDBLOCK for non-blocking sockets" << std::endl;
    std::cout << "• Use errno for detailed error information" << std::endl;
    
    std::cout << "\nResource Management:" << std::endl;
    std::cout << "• Always close sockets when done" << std::endl;
    std::cout << "• Use RAII in C++ for automatic cleanup" << std::endl;
    std::cout << "• Set socket options (SO_REUSEADDR, etc.)" << std::endl;
    
    std::cout << "\nPerformance:" << std::endl;
    std::cout << "• Use non-blocking I/O for scalability" << std::endl;
    std::cout << "• Consider epoll/kqueue for event-driven programming" << std::endl;
    std::cout << "• Buffer management for large data transfers" << std::endl;
    
    std::cout << "\nSecurity:" << std::endl;
    std::cout << "• Validate all input data" << std::endl;
    std::cout << "• Use TLS/SSL for secure communication" << std::endl;
    std::cout << "• Implement rate limiting and DOS protection" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_udp_server_client() {
    std::cout << "=== UDP SERVER AND CLIENT IMPLEMENTATION ===" << std::endl;
    
    // UDP Communication Flow:
    // ┌─ UDP SERVER/CLIENT COMMUNICATION ───────────────────────┐
    // │                                                         │
    // │ UDP SERVER              UDP CLIENT                      │
    // │ ┌─────────────────┐     ┌─────────────────────────────┐ │
    // │ │ 1. socket()     │     │ 1. socket()                 │ │
    // │ │ 2. bind()       │     │ 2. sendto() → Message       │ │
    // │ │ 3. recvfrom()   │ ←── │ 3. recvfrom() ← Response    │ │
    // │ │ 4. sendto()     │ ──→ │ 4. close()                  │ │
    // │ │ 5. close()      │     │                             │ │
    // │ └─────────────────┘     └─────────────────────────────┘ │
    // │                                                         │
    // │ Key Differences from TCP:                               │
    // │ • No listen() or accept() - connectionless             │
    // │ • Use sendto()/recvfrom() instead of send()/recv()     │
    // │ • Each message is independent                           │
    // │ • No connection state maintained                        │
    // │ • Address must be specified for each message           │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- UDP Server Implementation ---" << std::endl;
    
    // UDP Server Code
    std::cout << "Creating UDP Server on port 9999..." << std::endl;
    
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        std::cout << "Failed to create UDP server socket" << std::endl;
        return;
    }
    
    // Configure server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP
    server_addr.sin_port = htons(9999);       // Port 9999
    
    // Bind socket to address
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cout << "Failed to bind UDP server socket" << std::endl;
        close(server_socket);
        return;
    }
    
    std::cout << "UDP Server bound to port 9999, waiting for messages..." << std::endl;
    
    // Server message handling (demonstrate concept)
    char buffer[1024];
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    
    std::cout << "\n--- UDP Server Code Example ---" << std::endl;
    std::cout << "// Create UDP socket" << std::endl;
    std::cout << "int server_socket = socket(AF_INET, SOCK_DGRAM, 0);" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "// Configure and bind to address" << std::endl;
    std::cout << "struct sockaddr_in server_addr;" << std::endl;
    std::cout << "server_addr.sin_family = AF_INET;" << std::endl;
    std::cout << "server_addr.sin_addr.s_addr = INADDR_ANY;" << std::endl;
    std::cout << "server_addr.sin_port = htons(9999);" << std::endl;
    std::cout << "bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "// Receive messages from clients" << std::endl;
    std::cout << "while (true) {" << std::endl;
    std::cout << "    char buffer[1024];" << std::endl;
    std::cout << "    struct sockaddr_in client_addr;" << std::endl;
    std::cout << "    socklen_t client_len = sizeof(client_addr);" << std::endl;
    std::cout << "    " << std::endl;
    std::cout << "    // Receive message from any client" << std::endl;
    std::cout << "    int bytes_received = recvfrom(server_socket, buffer, sizeof(buffer)," << std::endl;
    std::cout << "                                  0, (struct sockaddr*)&client_addr, &client_len);" << std::endl;
    std::cout << "    " << std::endl;
    std::cout << "    if (bytes_received > 0) {" << std::endl;
    std::cout << "        buffer[bytes_received] = '\\0';" << std::endl;
    std::cout << "        std::cout << \"Received: \" << buffer << std::endl;" << std::endl;
    std::cout << "        " << std::endl;
    std::cout << "        // Echo message back to client" << std::endl;
    std::cout << "        std::string response = \"Echo: \" + std::string(buffer);" << std::endl;
    std::cout << "        sendto(server_socket, response.c_str(), response.length()," << std::endl;
    std::cout << "               0, (struct sockaddr*)&client_addr, client_len);" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "}" << std::endl;
    
    std::cout << "\n--- UDP Client Implementation ---" << std::endl;
    
    // Create UDP client socket for demonstration
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        std::cout << "Failed to create UDP client socket" << std::endl;
        close(server_socket);
        return;
    }
    
    // Configure server address for client
    struct sockaddr_in target_server_addr;
    target_server_addr.sin_family = AF_INET;
    target_server_addr.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &target_server_addr.sin_addr);
    
    std::cout << "\n--- UDP Client Code Example ---" << std::endl;
    std::cout << "// Create UDP client socket" << std::endl;
    std::cout << "int client_socket = socket(AF_INET, SOCK_DGRAM, 0);" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "// Configure server address" << std::endl;
    std::cout << "struct sockaddr_in server_addr;" << std::endl;
    std::cout << "server_addr.sin_family = AF_INET;" << std::endl;
    std::cout << "server_addr.sin_port = htons(9999);" << std::endl;
    std::cout << "inet_pton(AF_INET, \"127.0.0.1\", &server_addr.sin_addr);" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "// Send message to server" << std::endl;
    std::cout << "std::string message = \"Hello UDP Server!\";" << std::endl;
    std::cout << "sendto(client_socket, message.c_str(), message.length()," << std::endl;
    std::cout << "       0, (struct sockaddr*)&server_addr, sizeof(server_addr));" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "// Receive response from server" << std::endl;
    std::cout << "char response[1024];" << std::endl;
    std::cout << "socklen_t server_len = sizeof(server_addr);" << std::endl;
    std::cout << "int bytes_received = recvfrom(client_socket, response, sizeof(response)," << std::endl;
    std::cout << "                              0, (struct sockaddr*)&server_addr, &server_len);" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "if (bytes_received > 0) {" << std::endl;
    std::cout << "    response[bytes_received] = '\\0';" << std::endl;
    std::cout << "    std::cout << \"Server response: \" << response << std::endl;" << std::endl;
    std::cout << "}" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "close(client_socket);" << std::endl;
    
    // Demonstrate actual communication (simplified)
    std::cout << "\n--- Live UDP Communication Test ---" << std::endl;
    
    // Send a test message from client to server
    std::string test_message = "Hello from UDP client!";
    std::cout << "Client sending: \"" << test_message << "\"" << std::endl;
    
    ssize_t sent_bytes = sendto(client_socket, test_message.c_str(), test_message.length(),
                               0, (struct sockaddr*)&target_server_addr, sizeof(target_server_addr));
    
    if (sent_bytes > 0) {
        std::cout << "Message sent successfully (" << sent_bytes << " bytes)" << std::endl;
        
        // Try to receive the message on server side (with timeout)
        struct timeval timeout;
        timeout.tv_sec = 1;  // 1 second timeout
        timeout.tv_usec = 0;
        setsockopt(server_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        
        ssize_t received_bytes = recvfrom(server_socket, buffer, sizeof(buffer) - 1,
                                         0, (struct sockaddr*)&client_addr, &client_addr_len);
        
        if (received_bytes > 0) {
            buffer[received_bytes] = '\0';
            std::cout << "Server received: \"" << buffer << "\"" << std::endl;
            
            // Echo response back to client
            std::string echo_response = "Echo: " + std::string(buffer);
            sendto(server_socket, echo_response.c_str(), echo_response.length(),
                   0, (struct sockaddr*)&client_addr, client_addr_len);
            std::cout << "Server echoed back: \"" << echo_response << "\"" << std::endl;
            
            // Client receives the echo
            char client_buffer[1024];
            setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
            ssize_t echo_bytes = recvfrom(client_socket, client_buffer, sizeof(client_buffer) - 1,
                                         0, (struct sockaddr*)&target_server_addr, &client_addr_len);
            
            if (echo_bytes > 0) {
                client_buffer[echo_bytes] = '\0';
                std::cout << "Client received echo: \"" << client_buffer << "\"" << std::endl;
            } else {
                std::cout << "Client did not receive echo (timeout or error)" << std::endl;
            }
        } else {
            std::cout << "Server did not receive message (timeout)" << std::endl;
        }
    } else {
        std::cout << "Failed to send message from client" << std::endl;
    }
    
    // Cleanup
    close(client_socket);
    close(server_socket);
    
    std::cout << "\n--- UDP Programming Key Points ---" << std::endl;
    std::cout << "Advantages:" << std::endl;
    std::cout << "• Fast and lightweight" << std::endl;
    std::cout << "• No connection overhead" << std::endl;
    std::cout << "• Suitable for real-time applications" << std::endl;
    std::cout << "• Broadcast and multicast support" << std::endl;
    std::cout << "• Lower latency than TCP" << std::endl;
    
    std::cout << "\nDisadvantages:" << std::endl;
    std::cout << "• No delivery guarantee" << std::endl;
    std::cout << "• No error recovery" << std::endl;
    std::cout << "• No flow control" << std::endl;
    std::cout << "• Messages may arrive out of order" << std::endl;
    std::cout << "• Application must handle reliability" << std::endl;
    
    std::cout << "\nBest Practices:" << std::endl;
    std::cout << "• Set appropriate timeouts" << std::endl;
    std::cout << "• Implement retry logic if needed" << std::endl;
    std::cout << "• Handle partial message reads" << std::endl;
    std::cout << "• Validate message integrity" << std::endl;
    std::cout << "• Consider message size limits (MTU)" << std::endl;
    
    std::cout << "\nCommon Use Cases:" << std::endl;
    std::cout << "• DNS queries and responses" << std::endl;
    std::cout << "• DHCP client/server communication" << std::endl;
    std::cout << "• Real-time gaming" << std::endl;
    std::cout << "• Live video/audio streaming" << std::endl;
    std::cout << "• IoT sensor data transmission" << std::endl;
    std::cout << "• Network discovery protocols" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_network_security() {
    std::cout << "=== NETWORK SECURITY CONCEPTS ===" << std::endl;
    
    // Security Layers:
    // ┌─ NETWORK SECURITY LAYERS ───────────────────────────────┐
    // │                                                         │
    // │ Application Layer Security:                             │
    // │ ┌─ HTTPS (HTTP over TLS) ─────────────────────────────┐ │
    // │ │ • Certificate-based authentication                  │ │
    // │ │ • Public key cryptography                           │ │
    // │ │ • Session encryption                                │ │
    // │ │ • Data integrity verification                       │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ Transport Layer Security:                               │
    // │ ┌─ TLS/SSL Protocol ──────────────────────────────────┐ │
    // │ │ TLS Handshake Process:                              │ │
    // │ │ 1. Client Hello (supported ciphers)                │ │
    // │ │ 2. Server Hello (chosen cipher, certificate)       │ │
    // │ │ 3. Certificate verification                         │ │
    // │ │ 4. Key exchange (ephemeral keys)                   │ │
    // │ │ 5. Encrypted communication begins                  │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // │                                                         │
    // │ Network Layer Security:                                 │
    // │ ┌─ IPSec (IP Security) ───────────────────────────────┐ │
    // │ │ • Authentication Header (AH)                        │ │
    // │ │ • Encapsulating Security Payload (ESP)              │ │
    // │ │ • VPN implementations                               │ │
    // │ │ • Site-to-site encryption                           │ │
    // │ └─────────────────────────────────────────────────────┘ │
    // └─────────────────────────────────────────────────────────┘
    
    std::cout << "\n--- Common Security Threats ---" << std::endl;
    std::cout << "Man-in-the-Middle (MITM):" << std::endl;
    std::cout << "• Attacker intercepts communication" << std::endl;
    std::cout << "• Can read/modify data in transit" << std::endl;
    std::cout << "• Prevention: Use TLS/SSL, certificate pinning" << std::endl;
    
    std::cout << "\nEavesdropping:" << std::endl;
    std::cout << "• Passive monitoring of network traffic" << std::endl;
    std::cout << "• Unencrypted protocols vulnerable" << std::endl;
    std::cout << "• Prevention: Encryption at all layers" << std::endl;
    
    std::cout << "\nDenial of Service (DoS):" << std::endl;
    std::cout << "• Overwhelming server with requests" << std::endl;
    std::cout << "• Resource exhaustion attacks" << std::endl;
    std::cout << "• Prevention: Rate limiting, load balancing" << std::endl;
    
    std::cout << "\nSQL Injection:" << std::endl;
    std::cout << "• Malicious SQL code in input" << std::endl;
    std::cout << "• Database compromise" << std::endl;
    std::cout << "• Prevention: Parameterized queries, input validation" << std::endl;
    
    std::cout << "\n--- Encryption and Authentication ---" << std::endl;
    std::cout << "Symmetric Encryption:" << std::endl;
    std::cout << "• Same key for encryption/decryption" << std::endl;
    std::cout << "• Fast for large data" << std::endl;
    std::cout << "• Key distribution challenge" << std::endl;
    std::cout << "• Examples: AES, ChaCha20" << std::endl;
    
    std::cout << "\nAsymmetric Encryption:" << std::endl;
    std::cout << "• Public/private key pairs" << std::endl;
    std::cout << "• Slower than symmetric" << std::endl;
    std::cout << "• Solves key distribution" << std::endl;
    std::cout << "• Examples: RSA, ECDSA" << std::endl;
    
    std::cout << "\nDigital Certificates:" << std::endl;
    std::cout << "• Verify identity of parties" << std::endl;
    std::cout << "• Certificate Authorities (CAs)" << std::endl;
    std::cout << "• Chain of trust" << std::endl;
    std::cout << "• X.509 standard" << std::endl;
    
    std::cout << "\n--- Secure Protocol Examples ---" << std::endl;
    std::cout << "HTTPS (HTTP over TLS):" << std::endl;
    std::cout << "• TLS 1.2/1.3 encryption" << std::endl;
    std::cout << "• Server authentication" << std::endl;
    std::cout << "• Optional client authentication" << std::endl;
    
    std::cout << "\nSSH (Secure Shell):" << std::endl;
    std::cout << "• Host key verification" << std::endl;
    std::cout << "• Password or key-based auth" << std::endl;
    std::cout << "• Encrypted command execution" << std::endl;
    
    std::cout << "\nVPN (Virtual Private Network):" << std::endl;
    std::cout << "• Encrypted tunnel over internet" << std::endl;
    std::cout << "• Remote access to private networks" << std::endl;
    std::cout << "• IPSec, OpenVPN, WireGuard protocols" << std::endl;
    
    std::cout << "\n--- Security Best Practices ---" << std::endl;
    std::cout << "Development:" << std::endl;
    std::cout << "• Use secure protocols (HTTPS, SSH)" << std::endl;
    std::cout << "• Validate all input data" << std::endl;
    std::cout << "• Implement proper authentication" << std::endl;
    std::cout << "• Keep software updated" << std::endl;
    
    std::cout << "\nDeployment:" << std::endl;
    std::cout << "• Use firewalls and intrusion detection" << std::endl;
    std::cout << "• Regular security audits" << std::endl;
    std::cout << "• Monitor network traffic" << std::endl;
    std::cout << "• Principle of least privilege" << std::endl;
    
    std::cout << std::endl;
}

int main() {
    std::cout << "COMPREHENSIVE NETWORKING TUTORIAL" << std::endl;
    std::cout << "=================================" << std::endl << std::endl;
    
    demonstrate_networking_overview();
    demonstrate_ip_addressing();
    demonstrate_sockets();
    demonstrate_tcp_protocol();
    demonstrate_udp_protocol();
    demonstrate_http_protocol();
    demonstrate_other_protocols();
    demonstrate_socket_programming();
    demonstrate_udp_server_client();
    demonstrate_network_security();
    
    std::cout << "Networking tutorial completed successfully!" << std::endl;
    std::cout << "\nKey Takeaways:" << std::endl;
    std::cout << "━━━━━━━━━━━━━━" << std::endl;
    
    std::cout << "\nPROTOCOL STACK:" << std::endl;
    std::cout << "• Application Layer: HTTP, SMTP, SSH, FTP" << std::endl;
    std::cout << "• Transport Layer: TCP (reliable), UDP (fast)" << std::endl;
    std::cout << "• Internet Layer: IP routing and addressing" << std::endl;
    std::cout << "• Network Access: Ethernet, WiFi physical protocols" << std::endl;
    
    std::cout << "\nSOCKETS AND PORTS:" << std::endl;
    std::cout << "• Sockets are endpoints for network communication" << std::endl;
    std::cout << "• Ports identify specific services (0-65535)" << std::endl;
    std::cout << "• Well-known ports for standard services" << std::endl;
    std::cout << "• Socket programming provides network API" << std::endl;
    
    std::cout << "\nTCP vs UDP:" << std::endl;
    std::cout << "• TCP: Reliable, ordered, connection-oriented" << std::endl;
    std::cout << "• UDP: Fast, lightweight, connectionless" << std::endl;
    std::cout << "• Choose based on application requirements" << std::endl;
    
    std::cout << "\nSECURITY:" << std::endl;
    std::cout << "• Always use encryption for sensitive data" << std::endl;
    std::cout << "• TLS/SSL for transport layer security" << std::endl;
    std::cout << "• Validate all input and implement authentication" << std::endl;
    std::cout << "• Regular security updates and monitoring" << std::endl;
    
    std::cout << "\nPRACTICAL APPLICATIONS:" << std::endl;
    std::cout << "• Web development: HTTP/HTTPS protocols" << std::endl;
    std::cout << "• System administration: SSH for remote access" << std::endl;
    std::cout << "• Email systems: SMTP, POP3, IMAP protocols" << std::endl;
    std::cout << "• Network programming: Socket APIs for custom applications" << std::endl;
    std::cout << "• Real-time systems: UDP for gaming, streaming, IoT" << std::endl;
    
    return 0;
}