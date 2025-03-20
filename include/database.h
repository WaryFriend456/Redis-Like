//
// Created by kushal bang on 05-03-2025.
//

#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <map>
#include <mutex>
#include <chrono>

enum class DataType {
    STRING,
    LIST,
    HASH,
    SET
};

struct Value {
    DataType type;
    std::string string_val;
    std::vector<std::string> list_val;
    std::unordered_map<std::string ,std::string> hash_val;
    std::set<std::string> set_val;
    std::chrono::steady_clock::time_point expiry;
    bool has_expiry = false;
};

class Database {
public:
    // String operations
    void set(const std::string &key, const std::string &value);
    std::string get(const std::string &key);
    bool del(const std::string &key);

    // List operations

    long long lpush(const std::string &key, const std::vector<std::string> &values);
    long long rpush(const std::string &key, const std::vector<std::string> &values);
    std::string lpop(const std::string &key);
    std::string rpop(const std::string &key);
    std::vector<std::string> lrange(const std::string &key, int start, int end);

    // Hash operations
    bool hset(const std::string &key, const std::string &field, const std::string &value);
    std::string hget(const std::string &key, const std::string &field);
    bool hdel(const std::string &key, const std::string &field);
    std::unordered_map<std::string, std::string> hgetall(const std::string &key);

    // Set operations
    long long sadd(const std::string &key, const std::set<std::string> &members);
    long long srem(const std::string &key, const std::set<std::string> &members);
    bool sismember(const std::string &key, const std::string &member);
    std::set<std::string> smembers(const std::string &key);

    // Key operations
    bool exists(const std::string &key);
    bool expire(const std::string &key, int seconds);
    long long ttl(const std::string &key);

    // Utility methods
    DataType type(const std::string &key);
    void cleanup_expired_keys();

private:
    std::unordered_map<std::string, Value> data;
    std::mutex mutex;
    // helper function to check if a key exists or correct type
    bool check_or_create_type(const std::string &key, DataType type);
    // helper function to check if a key is of the correct type
    bool check_type(const std::string &key, DataType type);
    // helper function to check if a key expired
    bool is_expired(const std::string &key);
};

#endif //DATABASE_H
