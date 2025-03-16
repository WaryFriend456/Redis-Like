#ifndef CATCH_CONFIG_MAIN
#include <iostream>
#include "database.h"
#include <boost/asio.hpp>
#include "server.h"
#include <vector>
#include <thread>

// Main function to start the server
int main() {
    try {
        boost::asio::io_context io_context;

        // Create a work guard to keep the io_context running
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
        work_guard = boost::asio::make_work_guard(io_context);

        // Create a thread pool
        std::vector<std::thread> thread_pool;
        unsigned int thread_count = std::thread::hardware_concurrency();
        thread_count = thread_count > 1 ? thread_count : 2; // Ensure at least 2 threads

        Database db;

        Server server(io_context, 6379, db);

        // std::cout << "Server started at port 6379" << std::endl;
        std::cout << "Server started at port 6379 with " << thread_count << " threads" << std::endl;
        // Start the thread pool
        thread_pool.reserve(thread_count);
        for (unsigned int i = 0; i < thread_count; i++) {
            thread_pool.emplace_back([&io_context]() {
                io_context.run();
            });
        }

        for (std::thread &thread : thread_pool) {
            thread.join();
        }
        // io_context.run();
    }catch (std::exception& e) {
        std::cerr << "Exception : " << e.what() << std::endl;
    }
    return 0;
}

#endif