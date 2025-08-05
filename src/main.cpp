#include "ConfigLoader.h"
#include "SIPMessageBuilder.h"
#include "UDPSocket.h"
#include <iostream>
#include <thread>
#include <chrono>


int main() {
    auto config = ConfigLoader::load("config.txt");

    std::string from      = config["from"];
    std::string to        = config["to"];
    std::string localIP   = config["ip"];
    int localPort         = std::stoi(config["port"]);

    std::string serverIP  = config["dest_ip"];
    int serverPort        = std::stoi(config["dest_port"]);

    // Bind to fixed IP and port
    UDPSocket socket(localIP, localPort);  // Create socket bound to your IP and port

    SIPDialog dialog;
    std::string invite = SIPMessageBuilder::generateInvite(from, to, localIP, localPort, dialog);

    std::cout << "Sending SIP INVITE...\n";
    socket.send(invite, serverIP, serverPort);  // Send to SIP server

    std::string response, senderIP;
    int senderPort;

    while (true) {
        response = socket.receive(senderIP, senderPort);
        std::cout << "\nReceived from " << senderIP << ":" << senderPort << "\n" << response << "\n";

        if (response.find("100 Trying") != std::string::npos) {
            std::cout << "100 Trying received...\n";
        } else if (response.find("180 Ringing") != std::string::npos) {
            std::cout << "180 Ringing received...\n";
        } else if (response.find("SIP/2.0 200 OK") != std::string::npos) {
            std::cout << "Got 200 OK — sending ACK...\n";
            SIPMessageBuilder::parse200OK(response, dialog.callID, dialog.toTag, dialog.cseq);
            std::string ack = SIPMessageBuilder::generateAck(from, to, localIP, dialog, localPort);
            socket.send(ack, serverIP, serverPort);
            std::cout << "ACK sent.\n";
            break;
        }
    }

    std::cout << "\nCall ongoing for 10 seconds...\n";
    std::this_thread::sleep_for(std::chrono::seconds(10));

    std::cout << "\nSending BYE...\n";
    std::string bye = SIPMessageBuilder::generateBye(from, to, localIP, dialog, localPort);
    socket.send(bye, serverIP, serverPort);
    std::cout << "BYE sent. Call ended.\n";

    return 0;
}

