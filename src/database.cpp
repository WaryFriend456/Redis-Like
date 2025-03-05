//
// Created by kushal bang on 05-03-2025.
//
#include "database.h"

void Database::set(const std::string &key, const std::string &value) {
    data[key] = value;
}

std::string Database::get(const std::string &key) {
    auto it = data.find(key);
    if (it != data.end()) {
        return it->second;
    }
    return "NULL";
}

bool Database::del(const std::string &key) {
    return data.erase(key) > 0;
}