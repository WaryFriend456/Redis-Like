#ifndef CATCH_CONFIG_MAIN
#include <iostream>
#include <string>

#include "database.h"
#include <command_parser.h>
#include <boost/asio.hpp>
#include <memory>

using boost::asio::ip::tcp;

std::string handleCommand(const std::vector<std::string>& tokens, Database& db) {
    if (tokens.empty()) {
        return "Unknown Command";
    }

    const std::string& command = tokens[0];

    if (command == "SET") {
        if (tokens.size() < 3) {
            return "Usage: SET <key> <value>";
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
    else {
        if (command == "DEL") {
            if (tokens.size() < 2) {
                return "Usage: DEL <key>";
            }
            return db.del(tokens[1]) ? "OK" : "ERROR";
        }
        return "Unknown Command: " + command;
    }
}

// Represents a session with a client
class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, Database& db)
        : socket_(std::move(socket)), db_(db) {}

    void start() {
        std::flush(std::cout);
        doWrite("> ");
        doRead();
    }

private:
    std::string buffer_; // Store partial input
    std::string write_buffer_;

    void doRead() {

        auto self(shared_from_this());
        socket_.async_read_some(
            boost::asio::buffer(data_, max_length),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    // Append received data to buffer
                    buffer_.append(data_, length);

                    // Process complete commands
                    size_t pos;
                    while ((pos = buffer_.find("\r\n")) != std::string::npos ||
                           (pos = buffer_.find("\n")) != std::string::npos) {

                        // Extract the command (exclude the newline)
                        std::string command = buffer_.substr(0, pos);

                        // Remove the processed command from buffer (including newline)
                        size_t chars_to_remove = pos;
                        if (buffer_[pos] == '\r' && pos + 1 < buffer_.length() && buffer_[pos + 1] == '\n') {
                            chars_to_remove += 2;  // \r\n case
                        } else {
                            chars_to_remove += 1;  // \n case
                        }
                        buffer_.erase(0, chars_to_remove);

                        // Process the command if it's not empty
                        if (!command.empty()) {
                            auto tokens = parseCommand(command);
                            std::string response = handleCommand(tokens, db_) + "\r\n";

                            // Send response
                            doWrite(response);
                            doWrite("> ");
                            return;  // Return after processing one command to avoid overlapping writes
                        }
                    }

                    // Continue reading if no complete command was found
                    // doWrite("> ");
                    doRead();
                }
            }
        );
    }

    void doWrite(const std::string& response) {
        auto self(shared_from_this());
        // Store the response in the member variable
        write_buffer_ = response;

        boost::asio::async_write(
            socket_,
            boost::asio::buffer(write_buffer_), // Use the member variable instead
            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    doRead(); // Wait for the next command
                }
            }
        );
    }

    tcp::socket socket_;
    static constexpr std::size_t max_length = 1024;
    char data_[max_length];
    Database& db_;
};


class Server {
public:
    Server(boost::asio::io_context& io_context, short port, Database& db)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), db_(db) {
        doAccept();
    }

private:
    void doAccept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    // Create a session and run it
                    std::make_shared<Session>(std::move(socket), db_)->start();
                }
                // Accept the next connection
                doAccept();
            }
        );
    }

    tcp::acceptor acceptor_;
    Database& db_;
};

int main() {

    try {
        boost::asio::io_context io_context;

        Database db;

        Server server(io_context, 6379, db);

        std::cout << "Server started at port 6379" << std::endl;

        io_context.run();

    }catch (std::exception& e) {
        std::cerr << "Exception : " << e.what() << std::endl;
    }


    // std::string input;
    //
    // std::cout << "Welcome to MyRedisClone CLI. Type commands (SET, GET, DEL). Type EXIT to quit." << std::endl;
    //
    // while (true) {
    //     std::cout << "> ";
    //     if (!std::getline(std::cin, input) || input == "EXIT") {
    //         break;  // exit the loop if user types EXIT or ctrl-dA
    //     }
    //
    //     // Parse the command
    //     auto tokens = parseCommand(input);
    //     if (tokens.empty()) {
    //         continue; // if the user enters an empty line, skip to the next iteration
    //     }
    //
    //     const std::string& command = tokens[0];
    //
    //     if (command == "SET") {
    //         if (tokens.size() < 3) {
    //             std::cout << "Usage: SET <key> <value>" << std::endl;
    //         } else {
    //             db.set(tokens[1], tokens[2]);
    //             std::cout << "OK" << std::endl;
    //         }
    //     }else if (command == "GET") {
    //         if (tokens.size() < 2) {
    //             std::cout << "Usage: GET <key>" << std::endl;
    //         } else {
    //             std::string value = db.get(tokens[1]);
    //             if (!value.empty()) {
    //                 std::cout << value << std::endl;
    //             } else {
    //                 std::cout << "NULL" << std::endl;
    //             }
    //         }
    //     } else if (command == "DEL") {
    //         if (tokens.size() < 2) {
    //             std::cout << "Usage: DEL <key>" << std::endl;
    //         } else {
    //             bool deleted = db.del(tokens[1]);
    //             std::cout << (deleted ? "OK" : "ERROR") << std::endl;
    //         }
    //     }
    //     else {
    //         std::cout << "Unknown Command: " << command << std::endl;
    //     }
    // }

    return 0;
}

#endif