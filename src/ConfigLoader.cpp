#include "ConfigLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::unordered_map<std::string, std::string> ConfigLoader::load(const std::string& filename) {
    std::unordered_map<std::string, std::string> config;
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open config file: " << filename << std::endl;
        return config;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        size_t eq = line.find('=');
        if (eq != std::string::npos) {
            std::string key = line.substr(0, eq);
            std::string value = line.substr(eq + 1);
            config[key] = value;
        }
    }

    return config;
}
