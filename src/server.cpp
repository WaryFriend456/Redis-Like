//
// Created by kushal bang on 16-03-2025.
//

#include "server.h"
#include "session.h"
#include<memory>

Server::Server(boost::asio::io_context &io_context, short port, Database &db)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), db_(db) {
    doAccept();
}

void Server::doAccept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::make_shared<Session>(std::move(socket), db_)->start();
            }
            doAccept();
        }
    );
}

