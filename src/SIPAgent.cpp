#include "SIPAgent.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

void SIPAgent::start(const std::string& message, const std::string& ip, int port) {
    // 1. Create UDP socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "❌ Socket creation failed\n";
        return;
    }

    // 2. Bind to local port (so we can receive response)
    sockaddr_in localAddr{};
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(port);  // 5060
    localAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (sockaddr*)&localAddr, sizeof(localAddr)) < 0) {
        std::cerr << "❌ Failed to bind to port " << port << "\n";
        close(sock);
        return;
    }

    // 3. Setup destination
    sockaddr_in remoteAddr{};
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(port);
    remoteAddr.sin_addr.s_addr = inet_addr(ip.c_str());

    // 4. Send SIP message
    ssize_t sent = sendto(sock, message.c_str(), message.size(), 0,
                          (sockaddr*)&remoteAddr, sizeof(remoteAddr));
    if (sent < 0) {
        std::cerr << "❌ Failed to send SIP message\n";
        close(sock);
        return;
    }

    std::cout << "📤 SIP message sent to " << ip << ":" << port << "\n";
    std::cout << "⏳ Waiting for SIP response on port " << port << "...\n";

    // 5. Receive response
    char buffer[2048];
    sockaddr_in fromAddr{};
    socklen_t fromLen = sizeof(fromAddr);
    ssize_t received = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
                                (sockaddr*)&fromAddr, &fromLen);

    if (received > 0) {
        buffer[received] = '\0';
        std::cout << "📥 Received SIP response:\n"
                  << "----------------------------------------\n"
                  << buffer
                  << "\n----------------------------------------\n";
    } else {
        std::cerr << "❌ No SIP response received.\n";
    }

    close(sock);
}
