//
// Created by kushal bang on 16-03-2025.
//

#include "session.h"
#include "command_handler.h"
#include "command_parser.h"
#include <iostream>

Session::Session(tcp::socket socket, Database& db)
    : socket_(std::move(socket)), db_(db) {}

void Session::start() {
    std::flush(std::cout);
    doWrite("> ");
    doRead();
}

void Session::doRead() {
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
                        return;  // Return after processing one command
                    }
                }

                // Continue reading if no complete command was found
                doRead();
            }
        }
    );
}

void Session::doWrite(const std::string& response) {
    auto self(shared_from_this());
    // Store the response in the member variable
    write_buffer_ = response;

    boost::asio::async_write(
        socket_,
        boost::asio::buffer(write_buffer_),
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                doRead(); // Wait for the next command
            }
        }
    );
}