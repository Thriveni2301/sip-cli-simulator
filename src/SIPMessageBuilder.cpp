#include "SIPMessageBuilder.h"
#include <sstream>
#include <random>
#include <ctime>
#include <iostream>

std::string SIPMessageBuilder::generateBranch() {
    return "z9hG4bK" + std::to_string(rand() % 1000000);
}

std::string SIPMessageBuilder::generateTag() {
    return "tag" + std::to_string(rand() % 1000000);
}

std::string SIPMessageBuilder::extractHeader(const std::string& response, const std::string& headerName) {
    size_t pos = response.find(headerName + ":");
    if (pos == std::string::npos) return "";

    size_t start = pos + headerName.length() + 1;
    size_t end = response.find("\r\n", start);
    return response.substr(start, end - start);
}

std::string SIPMessageBuilder::extractTag(const std::string& response, const std::string& headerName) {
    size_t headerPos = response.find(headerName + ":");
    if (headerPos == std::string::npos) return "";

    size_t tagPos = response.find("tag=", headerPos);
    if (tagPos == std::string::npos) return "";

    size_t tagEnd = response.find("\r\n", tagPos);
    return response.substr(tagPos + 4, tagEnd - tagPos - 4);  // skip "tag="
}

void SIPMessageBuilder::parse200OK(const std::string& response, std::string& callID, std::string& toTag, std::string& cseq) {
    callID = extractHeader(response, "Call-ID");
    toTag = extractTag(response, "To");
    cseq = extractHeader(response, "CSeq");
}


std::string SIPMessageBuilder::generateInvite(
    const std::string& from,
    const std::string& to,
    const std::string& ip,
    int myPort,
    SIPDialog& dialog)
{
    std::string branch = generateBranch();
    std::string tag = generateTag();
    std::string callID = std::to_string(std::time(nullptr)) + "@" + ip;

    dialog.callID = callID;
    dialog.toTag = "";  // will be filled after 200 OK
    dialog.cseq = "1 INVITE";

    // Compose SDP body
    std::ostringstream sdp;
    sdp << "v=0\r\n"
        << "o=- 12345 12345 IN IP4 " << ip << "\r\n"
        << "s=Simple SIP Call\r\n"
        << "c=IN IP4 " << ip << "\r\n"
        << "t=0 0\r\n"
        << "m=audio 4000 RTP/AVP 0\r\n"
        << "a=rtpmap:0 PCMU/8000\r\n";

    std::string sdpBody = sdp.str();

    // Build SIP INVITE
    std::ostringstream msg;
    msg << "INVITE sip:" << to << "@" << ip << " SIP/2.0\r\n"
        << "Via: SIP/2.0/UDP " << ip << ":" << myPort << ";branch=" << branch << "\r\n"
        << "Max-Forwards: 70\r\n"
        << "To: <sip:" << to << "@" << ip << ">\r\n"
        << "From: <sip:" << to << "@" << ip << ">;tag=" << tag << "\r\n"
        << "Call-ID: " << callID << "\r\n"
        << "CSeq: " << dialog.cseq << "\r\n"
        << "Contact: <sip:" << to << "@" << ip << ":" << myPort << ">\r\n"
        << "Content-Type: application/sdp\r\n"
        << "Content-Length: " << sdpBody.size() << "\r\n\r\n"
        << sdpBody;

    return msg.str();
}


std::string SIPMessageBuilder::generateAck(const std::string& from, const std::string& to, const std::string& ip, const SIPDialog& dialog, int myPort) {
    std::ostringstream ss;
    ss << "ACK sip:" << to << " SIP/2.0\r\n"
       << "Via: SIP/2.0/UDP " << ip << ":" << myPort << ";branch=" << generateBranch() << "\r\n"
       << "Max-Forwards: 70\r\n"
       << "To: <sip:" << to << ">;tag=" << dialog.toTag << "\r\n"
       << "From: <sip:" << from << ">;tag=123456\r\n"
       << "Call-ID: " << dialog.callID << "\r\n"
       << "CSeq: " << dialog.cseq << "\r\n"
       << "Contact: <sip:" << from << "@" << ip << ":" << myPort << ">\r\n"
       << "Content-Length: 0\r\n\r\n";

    return ss.str();
}

std::string SIPMessageBuilder::generateBye(const std::string& from, const std::string& to, const std::string& ip, const SIPDialog& dialog, int myPort) {
    std::ostringstream msg;
    msg << "BYE sip:" << to << " SIP/2.0\r\n"
        << "Via: SIP/2.0/UDP " << ip << ":" << myPort << ";branch=" << generateBranch() << "\r\n"
        << "Max-Forwards: 70\r\n"
        << "To: <sip:" << to << ">;tag=" << dialog.toTag << "\r\n"
        << "From: <sip:" << from << ">;tag=123456\r\n"
        << "Call-ID: " << dialog.callID << "\r\n"
        << "CSeq: 2 BYE\r\n"
        << "Content-Length: 0\r\n\r\n";

    return msg.str();
}
