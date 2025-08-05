#include "UDPSocket.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>


UDPSocket::UDPSocket(const std::string& ip, int port) {
    socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in myAddr{};
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &myAddr.sin_addr);

    if (bind(socketfd, (sockaddr*)&myAddr, sizeof(myAddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "[+] Bound to IP: " << ip << ", Port: " << port << std::endl;
}


UDPSocket::~UDPSocket() {
    close(socketfd);
}

void UDPSocket::send(const std::string& message, const std::string& destIP, int destPort) {
    sockaddr_in destAddr{};
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(destPort);
    destAddr.sin_addr.s_addr = inet_addr(destIP.c_str());
    std::cout << "[SEND] To IP: " << destIP << ", Port: " << destPort << "\n";
    std::cout << "[SEND] Message:\n" << message << "\n";

    ssize_t sent = sendto(socketfd, message.c_str(), message.size(), 0, (struct sockaddr*)&destAddr, sizeof(destAddr));

    if (sent < 0) {
    perror("sendto failed");
    } else {
    std::cout << "sendto success, bytes sent: " << sent << std::endl;
    }
}

std::string UDPSocket::receive(std::string& senderIP, int& senderPort) {
    char buffer[4096];
    sockaddr_in senderAddr{};
    socklen_t senderLen = sizeof(senderAddr);
    std::cout<<"THRIVENI - 1"<<std::endl;
    struct timeval timeout;
    timeout.tv_sec = 5;  // 5 seconds timeout
    timeout.tv_usec = 0;
    setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    ssize_t bytes = recvfrom(socketfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&senderAddr, &senderLen);

    if (bytes >= 0) {
        buffer[bytes] = '\0';
        senderIP = inet_ntoa(senderAddr.sin_addr);
        senderPort = ntohs(senderAddr.sin_port);
        std::cout<<"THRIVENI - 2"<<std::endl;
        return std::string(buffer);
    }
std::cout<<"THRIVENI-3"<<std::endl;
    return "";
}

int UDPSocket::getBoundPort() const {
    return ntohs(localAddr.sin_port);
}

std::string UDPSocket::getBoundIP() const {
    return inet_ntoa(localAddr.sin_addr);
}
