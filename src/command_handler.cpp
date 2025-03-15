//
// Created by kushal bang on 16-03-2025.
//
#include "command_handler.h"
#include <string>

std::string handleCommand(const std::vector<std::string> &tokens, Database &db) {
    if (tokens.empty()) {
        return "Unknown Command";
    }

    const std::string& command = tokens[0];

    if (command == "SET" || command == "set") {
        if (tokens.size() < 3) {
            return "Usage: GET <key> <value>";
        }
        db.set(tokens[1], tokens[2]);
        return "OK";
    }
    else if (command == "GET" || command == "get") {
        if (tokens.size() < 2) {
            return "Usage: GET <key>";
        }
        std::string value = db.get(tokens[1]);
        return value.empty() ? "NULL" : value;
    }
    else if (command == "DEL" || command == "del") {
        if (tokens.size() < 2) {
            return "Usage: DEL <key>";
        }
        return db.del(tokens[1]) ? "OK" : "ERROR";
    }
    return "Unknown Command: " + command;
}
