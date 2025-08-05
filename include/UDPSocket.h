#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H

#include <string>
#include <netinet/in.h>

class UDPSocket {
public:
    //UDPSocket(int bindPort = 0, const std::string& bindIP = "0.0.0.0"); // default: bind to any IP, any port
    UDPSocket(const std::string& bindIP = "0.0.0.0", int bindPort = 0);

    ~UDPSocket();

    void send(const std::string& message, const std::string& destIP, int destPort);
    std::string receive(std::string& senderIP, int& senderPort); // return message and update sender info

    int getBoundPort() const; // helpful for ACK logic
    std::string getBoundIP() const;

private:
    int socketfd;
    sockaddr_in localAddr;
};

#endif

