//
// Created by kushal bang on 05-03-2025.
//
#include "database.h"

bool Database::check_or_create_type(const std::string &key, DataType type) {
    auto it = data.find(key);
    if (it == data.end()) {
        // Key does not exist, create a new entry with the specified type
        Value val;
        val.type = type;
        data[key] = val;
        return true;
    }
    // Key exists, check if the type matches
    return it->second.type == type;
}

bool Database::check_type(const std::string &key, DataType type) {
    auto it = data.find(key);
    if (it == data.end()) {
        // Key does not exist
        return false;
    }
    // Key exists, check if the type matches
    return it->second.type == type;
}

// bool operator>(const std::chrono::system_clock::time_point & now, const std::chrono::steady_clock::time_point & rhs);

bool Database::is_expired(const std::string &key) {
    auto it = data.find(key);
    if (it != data.end() && it->second.has_expiry) {
        // If the expiry is set with steady_clock, we need to compare against steady_clock
        auto now = std::chrono::steady_clock::now();
        if (now > it->second.expiry) {
            data.erase(it); // Remove expired key
            return true;
        }
    }
    return false;
}

// String operations
void Database::set(const std::string &key, const std::string &value) {
    std::lock_guard lock(mutex);
    Value val;
    val.type = DataType::STRING;
    val.string_val = value;
    data[key] = val;
}

std::string Database::get(const std::string &key) {
    std::lock_guard lock(mutex);
    if (is_expired(key)) {
        return "NULL";
    }
    auto it = data.find(key);
    if (it != data.end() && it->second.type == DataType::STRING) {
        return it->second.string_val;
    }
    return "NULL";
}

bool Database::del(const std::string &key) {
    std::lock_guard lock(mutex);
    return data.erase(key) > 0;
}

// List operations implementation (partial)
long long Database::lpush(const std::string &key, const std::vector<std::string> &values) {
    std::lock_guard lock(mutex);

    if (!check_or_create_type(key, DataType::LIST)) {
        return 0; // Type mismatch
    }

    auto& list = data[key].list_val;
    for (const auto &value : values) {
        list.insert(list.begin(), value);
    }

    return list.size();
}

std::string Database::lpop(const std::string &key) {
    std::lock_guard lock(mutex);

    if (is_expired(key)) {
        return "NULL";
    }

    if (!check_type(key, DataType::LIST)) {
        return "NULL"; // Type mismatch
    }

    auto& list = data[key].list_val;
    if (list.empty()) {
        return "NULL"; // List is empty
    }

    std::string value = list.front();
    list.erase(list.begin());

    if (list.empty()) {
        data.erase(key);
    }

    return value;
}

long long Database::rpush(const std::string &key, const std::vector<std::string> &values) {
    std::lock_guard lock(mutex);

    if (!check_or_create_type(key, DataType::LIST)) {
        return 0; // Type mismatch
    }
    auto& list = data[key].list_val;
    for (const auto &value : values) {
        list.push_back(value);
    }
    return list.size();
}

std::string Database::rpop(const std::string &key) {
    std::lock_guard lock(mutex);
    if (is_expired(key)) {
        return "NULL";
    }

    if (!check_type(key, DataType::LIST)) {
        return "NULL"; // Type mismatch
    }

    auto& list = data[key].list_val;
    if (list.empty()) {
        return "NULL"; // List is empty
    }
    std::string value = list.back();
    list.pop_back();
    if (list.empty()) {
        data.erase(key);
    }
    return value;
}

std::vector<std::string> Database::lrange(const std::string &key, int start, int end) {
    std::lock_guard lock(mutex);
    if (is_expired(key)) {
        return {}; // Return empty vector if key is expired
    }
    if (!check_type(key, DataType::LIST)) {
        return {}; // Type mismatch
    }
    auto& list = data[key].list_val;
    int orig_start = start;
    int orig_end = end;
    if (start < 0) {
        start = std::max(0, static_cast<int>(list.size()) + start);
    }
    if (end < 0) {
        end = std::max(0, static_cast<int>(list.size()) + end);
    }
    if (start == end && orig_start < 0 && orig_end < 0 &&
        -orig_start > static_cast<int>(list.size()) &&
        -orig_end > static_cast<int>(list.size())) {
        return {};
    }
    if (start > end || start >= static_cast<int>(list.size())) {
        return {}; // Invalid range
    }
    end = std::min(end, static_cast<int>(list.size()) - 1);
    std::vector<std::string> result(list.begin() + start, list.begin() + end + 1);
    return result;
}


// Key operations
bool Database::exists(const std::string &key) {
    std::lock_guard lock(mutex);

    if (is_expired(key)) {
        return false;
    }

    return data.find(key) != data.end();
}


bool Database::expire(const std::string &key, int seconds) {
    std::lock_guard lock(mutex);

    auto it = data.find(key);
    if (it == data.end()) {
        return false;
    }

    it->second.has_expiry = true;
    it->second.expiry = std::chrono::steady_clock::now() + std::chrono::seconds(seconds);
    return true;
}

long long Database::ttl(const std::string &key) {
    std::lock_guard lock(mutex);

    auto it = data.find(key);
    if (it == data.end()) {
        return -2; // Key does not exist
    }
    if (!it->second.has_expiry) {
        return -1; // Key exists but has no expiry
    }

    auto now = std::chrono::steady_clock::now();
    if (now > it->second.expiry) {
        data.erase(it);
        return -2; // Key has expired
    }

    auto remaining = std::chrono::duration_cast<std::chrono::seconds>(it->second.expiry - now).count();
    return remaining;
}
