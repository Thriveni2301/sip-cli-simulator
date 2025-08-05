#pragma once
#include <string>
#include <unordered_map>

class ConfigLoader {
public:
    static std::unordered_map<std::string, std::string> load(const std::string& filename);
};
