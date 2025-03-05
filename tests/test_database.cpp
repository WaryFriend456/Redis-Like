//
// Created by kushal bang on 05-03-2025.
//
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "database.h"

TEST_CASE("Database SET and GET") {
    Database db;
    db.set("foo", "bar");
    REQUIRE(db.get("foo") == "bar");
}

TEST_CASE("Database DEL") {
    Database db;
    db.set("key", "value");
    REQUIRE(db.del("key") == true);
    REQUIRE(db.get("key") == "");
}
