//
// Created by kushal bang on 16-03-2025.
//

#ifndef SERVER_H
#define SERVER_H

#include<boost/asio.hpp>
#include "database.h"

using boost::asio::ip::tcp;

class Server {
public:
    Server(boost::asio::io_context& io_context, short port, Database& db);

private:
    void doAccept();

    tcp::acceptor acceptor_;
    Database& db_;
};

#endif //SERVER_H
