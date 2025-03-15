//
// Created by kushal bang on 16-03-2025.
//

#ifndef SESSION_H
#define SESSION_H

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <memory>
#include "database.h"

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, Database& db);
    void start();

private:
    void doRead();
    void doWrite(const std::string& response);

    tcp::socket socket_;
    static constexpr std::size_t max_length = 1024;
    char data_[max_length];
    Database& db_;
    std::string buffer_;
    std::string write_buffer_;
};

#endif //SESSION_H
