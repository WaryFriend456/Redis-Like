#ifndef CATCH_CONFIG_MAIN
#include <iostream>
#include <string>

#include "database.h"
#include <command_parser.h>
#include <boost/asio.hpp>
#include <memory>


int main() {

    Database db;
    std::string input;

    std::cout << "Welcome to MyRedisClone CLI. Type commands (SET, GET, DEL). Type EXIT to quit." << std::endl;

    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, input) || input == "EXIT") {
            break;  // exit the loop if user types EXIT or ctrl-dA
        }

        // Parse the command
        auto tokens = parseCommand(input);
        if (tokens.empty()) {
            continue; // if the user enters an empty line, skip to the next iteration
        }

        const std::string& command = tokens[0];

        if (command == "SET") {
            if (tokens.size() < 3) {
                std::cout << "Usage: SET <key> <value>" << std::endl;
            } else {
                db.set(tokens[1], tokens[2]);
                std::cout << "OK" << std::endl;
            }
        }else if (command == "GET") {
            if (tokens.size() < 2) {
                std::cout << "Usage: GET <key>" << std::endl;
            } else {
                std::string value = db.get(tokens[1]);
                if (!value.empty()) {
                    std::cout << value << std::endl;
                } else {
                    std::cout << "NULL" << std::endl;
                }
            }
        } else if (command == "DEL") {
            if (tokens.size() < 2) {
                std::cout << "Usage: DEL <key>" << std::endl;
            } else {
                bool deleted = db.del(tokens[1]);
                std::cout << (deleted ? "OK" : "ERROR") << std::endl;
            }
        }
        else {
            std::cout << "Unknown Command: " << command << std::endl;
        }
    }

    return 0;
}

#endif