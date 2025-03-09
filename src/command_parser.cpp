//
// Created by kushal bang on 09-03-2025.
//

#include "command_parser.h"
#include <sstream>

std::vector<std::string> parseCommand(const std::string& input) {
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}