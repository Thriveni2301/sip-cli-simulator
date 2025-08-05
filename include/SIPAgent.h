#ifndef SIPAGENT_H
#define SIPAGENT_H

#include <string>

class SIPAgent {
public:
    void start(const std::string& message, const std::string& ip, int port);
};

#endif // SIPAGENT_H
