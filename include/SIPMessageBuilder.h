#ifndef SIP_MESSAGE_BUILDER_H
#define SIP_MESSAGE_BUILDER_H

#include <string>

struct SIPDialog {
    std::string callID;
    std::string toTag;
    std::string cseq;
};

class SIPMessageBuilder {
public:
    // Build INVITE and fill SIPDialog values
    static std::string generateInvite(const std::string& from, const std::string& to, const std::string& ip, int myPort, SIPDialog& dialog);

    // Generate ACK using dialog info
    static std::string generateAck(const std::string& from, const std::string& to, const std::string& ip, const SIPDialog& dialog, int myPort);

    // Generate BYE using dialog info
    static std::string generateBye(const std::string& from, const std::string& to, const std::string& ip, const SIPDialog& dialog, int myPort);

    // Header parsers
    static std::string extractHeader(const std::string& response, const std::string& headerName);
    static std::string extractTag(const std::string& response, const std::string& headerName);

    // Extract call dialog fields from 200 OK
    static void parse200OK(const std::string& response, std::string& callID, std::string& toTag, std::string& cseq);

private:
    // Utility for header fields
    static std::string generateBranch();
    static std::string generateTag();
};

#endif // SIP_MESSAGE_BUILDER_H
