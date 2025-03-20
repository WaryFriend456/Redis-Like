//
// Created by kushal bang on 16-03-2025.
//
#include "command_handler.h"
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>

// Helper function to convert string to uppercase
std::string to_upper(const std::string &input) {
    std::string res = input;
    std::transform(res.begin(), res.end(), res.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return res;
}

// helper function to parse integer with error handling
bool try_parse_int(const std::string &str, int &value) {
    try {
        value = std::stoi(str);
        return true;
    } catch (...) {
        return false;
    }
}


std::string handleCommand(const std::vector<std::string> &tokens, Database &db) {
    if (tokens.empty()) {
        return "Unknown Command";
    }

    const std::string& command = to_upper(tokens[0]);

    // String operations
    if (command == "SET") {
        if (tokens.size() < 3) {
            return "Usage: Set <key> <value>";
        }
        db.set(tokens[1], tokens[2]);
        return "OK";
    }
    else if (command == "GET") {
        if (tokens.size() < 2) {
            return "Usage: GET <key>";
        }
        std::string value = db.get(tokens[1]);
        return value.empty() ? "NULL" : value;
    }
    else if (command == "DEL") {
        if (tokens.size() < 2) {
            return "Usage: DEL <key>";
        }
        return db.del(tokens[1]) ? "OK" : "ERROR";
    }
    // List operstions
    else if (command == "LPUSH") {
        if (tokens.size() < 3) {
            return "Usage: LPUSH <key> <value> [value ...]";
        }
        std::vector<std::string> values(tokens.begin() + 2, tokens.end());
        return std::to_string(db.lpush(tokens[1], values));
    }
    else if (command == "RPUSH") {
        if (tokens.size() < 3) {
            return "Usage: RPUSH <key> <value> [value ...]";
        }
        std::vector<std::string> values(tokens.begin() + 2, tokens.end());
        return std::to_string(db.rpush(tokens[1], values));
    }
    else if (command == "LPOP") {
        if (tokens.size() < 2) {
            return "Usage: LPOP <key>";
        }
        return db.lpop(tokens[1]);
    }
    else if (command == "RPOP") {
        if (tokens.size() < 2) {
            return "Usage: RPOP <key>";
        }
        return db.rpop(tokens[1]);
    }
    else if (command == "LRANGE") {
        if (tokens.size() < 4) {
            return "Usage: LRANGE <key> <start> <end>";
        }
        int start, end;
        if (!try_parse_int(tokens[2], start) || !try_parse_int(tokens[3], end)) {
            return "Invalid range values";
        }
        std::vector<std::string> result = db.lrange(tokens[1], start, end);
        std::ostringstream oss;
        for (const auto &item : result) {
            oss << item << " ";
        }
        return oss.str();
    }
    // Key operations
    else if (command == "EXISTS") {
        if (tokens.size() < 2) {
            return "Usage: EXISTS <key>";
        }
        return db.exists(tokens[1]) ? "OK" : "NOT EXISTS";
    }
    else if (command == "EXPIRE") {
        if (tokens.size() < 3) {
            return "Usage: EXPIRE <key> <seconds>";
        }
        int seconds;
        if (!try_parse_int(tokens[2], seconds) || seconds < 0) {
            return "Value is not a valid integer or out of range";
        }
        return db.expire(tokens[1], seconds) ? "OK" : "ERROR";
    }
    else if (command == "TTL") {
        if (tokens.size() < 2) {
            return "Usage: TTL <key>";
        }
        return std::to_string(db.ttl(tokens[1]));
    }
    else if (command == "PING") {
        return "PONG";
    }
    return "Unknown Command: " + command;
}
