#ifndef CATCH_CONFIG_MAIN
#include <iostream>
#include "database.h"
#include <boost/asio.hpp>
#include "server.h"

// Main function to start the server
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
    return 0;
}

#endif