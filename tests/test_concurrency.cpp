//
// Created by kushal bang on 16-03-2025.
//

#include <catch_amalgamated.hpp>
#include "database.h"
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>

TEST_CASE("Database concurrent access to different keys") {
    Database db;
    std::vector<std::thread> threads;
    const int num_threads = 10;
    const int operations_per_thread = 1000;

    std::vector<bool> results(num_threads, true);

    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back([&db, i, operations_per_thread, &results]() {
            bool thread_success = true;
            std::string key_prefix = "key_" + std::to_string(i) + "_";
            for (int j = 0; j < operations_per_thread; j++) {
                std::string key = key_prefix + std::to_string(j);
                std::string value = "value_" + std::to_string(j);
                db.set(key, value);
                if (db.get(key) != value) {
                    thread_success = false;
                    break;
                }
            }
            results[i] = thread_success;
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // Check results after threads complete
    for (bool result : results) {
        REQUIRE(result);
    }
}

TEST_CASE("Database concurrent access to same key") {
    Database db;
    std::vector<std::thread> threads;
    std::atomic<int> successful_sets = 0;
    const int num_threads = 10;

    // Initialize key
    db.set("shared_key", "initial");

    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back([&db, i, &successful_sets]() {
            std::string new_value = "thread_" + std::to_string(i);
            db.set("shared_key", new_value);
            std::string retrieved = db.get("shared_key");
            if (retrieved == new_value) {
                successful_sets++;
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // After threads complete, verify results
    REQUIRE(successful_sets > 0);
    std::string final_value = db.get("shared_key");
    REQUIRE(final_value.find("thread_") == 0);
}

TEST_CASE("Database concurrent sets and deletes") {
    Database db;
    std::vector<std::thread> threads;
    std::atomic<int> counter = 0;
    const int num_threads = 20;

    // Set writers
    for (int i = 0; i < num_threads/2; i++) {
        threads.emplace_back([&db, i]() {
            for (int j = 0; j < 100; j++) {
                db.set("contested_key", "value_" + std::to_string(i) + "_" + std::to_string(j));
                std::this_thread::yield();
            }
        });
    }

    // Readers/deleters
    for (int i = 0; i < num_threads/2; i++) {
        threads.emplace_back([&db, i, &counter]() {
            for (int j = 0; j < 100; j++) {
                std::string val = db.get("contested_key");
                if (val != "NULL") {
                    counter++;
                }
                if (j % 10 == 0) {
                    db.del("contested_key");
                }
                std::this_thread::yield();
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // Test completes successfully if no crashes
    SUCCEED("Concurrent operation completed without crashes");
}