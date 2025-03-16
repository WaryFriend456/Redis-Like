//
// Created by kushal bang on 05-03-2025.
//

#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <unordered_map>
#include <mutex>

class Database {
public:
    // stores the key-value pair in the database
    void set(const std::string &key, const std::string &value);

    // returns the value of the key in the database (returns NULL if the key is not present)
    std::string get(const std::string &key);

    // deletes the key-value pair from the database, returns true if the key was present in the database
    bool del(const std::string &key);

private:
    std::unordered_map<std::string, std::string> data;
    std::mutex mutex;
};

#endif //DATABASE_H
