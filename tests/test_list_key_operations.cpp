//
// Created by kushal bang on 20-03-2025.
//

#include <catch_amalgamated.hpp>
#include "database.h"
#include <thread>

TEST_CASE("List basic operations") {
    Database db;

    SECTION("LPUSH and RPUSH") {
        REQUIRE(db.lpush("mylist", {"value1"}) == 1);
        REQUIRE(db.lpush("mylist", {"value2", "value3"}) == 3);
        REQUIRE(db.rpush("mylist", {"value4"}) == 4);

        auto range = db.lrange("mylist", 0, -1);
        REQUIRE(range.size() == 4);
        REQUIRE(range[0] == "value3");
        REQUIRE(range[1] == "value2");
        REQUIRE(range[2] == "value1");
        REQUIRE(range[3] == "value4");
    }

    SECTION("LPOP and RPOP") {
        db.lpush("poplist", {"one", "two", "three"});

        REQUIRE(db.lpop("poplist") == "three");
        REQUIRE(db.rpop("poplist") == "one");
        REQUIRE(db.lpop("poplist") == "two");
        REQUIRE(db.lpop("poplist") == "NULL");
        REQUIRE(db.rpop("poplist") == "NULL");
    }

    SECTION("LRANGE with different indices") {
        db.rpush("rangelist", {"zero", "one", "two", "three", "four"});

        auto range1 = db.lrange("rangelist", 0, 2);
        REQUIRE(range1.size() == 3);
        REQUIRE(range1[0] == "zero");
        REQUIRE(range1[2] == "two");

        auto range2 = db.lrange("rangelist", -3, -1);
        REQUIRE(range2.size() == 3);
        REQUIRE(range2[0] == "two");
        REQUIRE(range2[2] == "four");

        auto range3 = db.lrange("rangelist", 1, -2);
        REQUIRE(range3.size() == 3);
        REQUIRE(range3[0] == "one");
        REQUIRE(range3[2] == "three");
    }
}

TEST_CASE("List edge cases") {
    Database db;

    SECTION("Empty list operations") {
        REQUIRE(db.lpop("nonexistent") == "NULL");
        REQUIRE(db.rpop("nonexistent") == "NULL");
        REQUIRE(db.lrange("nonexistent", 0, -1).empty());
    }

    SECTION("Out of bounds ranges") {
        db.rpush("boundlist", {"a", "b", "c"});

        REQUIRE(db.lrange("boundlist", 5, 10).empty());
        REQUIRE(db.lrange("boundlist", -10, -5).empty());

        auto range = db.lrange("boundlist", -100, 100);
        REQUIRE(range.size() == 3);
    }

    SECTION("List auto-deletion") {
        db.lpush("dellist", {"single"});
        REQUIRE(db.exists("dellist"));

        db.lpop("dellist");
        REQUIRE_FALSE(db.exists("dellist"));
    }
}

TEST_CASE("Key operations") {
    Database db;

    SECTION("EXISTS command") {
        REQUIRE_FALSE(db.exists("testkey"));

        db.set("testkey", "value");
        REQUIRE(db.exists("testkey"));

        db.del("testkey");
        REQUIRE_FALSE(db.exists("testkey"));
    }

    SECTION("EXPIRE and TTL") {
        db.set("expirekey", "temp-value");

        // Set expiration and check TTL
        REQUIRE(db.expire("expirekey", 5));
        REQUIRE(db.ttl("expirekey") > 0);
        REQUIRE(db.ttl("expirekey") <= 5);

        // Key should still exist
        REQUIRE(db.exists("expirekey"));

        // Non-existent key
        REQUIRE(db.ttl("nonexistentkey") == -2);

        // Key without expiry
        db.set("noexpiry", "forever");
        REQUIRE(db.ttl("noexpiry") == -1);
    }
}

TEST_CASE("Key expiration") {
    Database db;

    SECTION("Key auto-expiration") {
        db.set("shortlife", "gone-soon");
        db.expire("shortlife", 1);

        // Wait for expiration
        std::this_thread::sleep_for(std::chrono::milliseconds(1100));

        REQUIRE_FALSE(db.exists("shortlife"));
        REQUIRE(db.get("shortlife") == "NULL");
        REQUIRE(db.ttl("shortlife") == -2);
    }
}

TEST_CASE("Type checking and interactions") {
    Database db;

    SECTION("Type mismatch handling") {
        // Create string key
        db.set("strkey", "value");

        // Try list operations on string key
        REQUIRE(db.lpush("strkey", {"listval"}) == 0);
        REQUIRE(db.lpop("strkey") == "NULL");
        REQUIRE(db.lrange("strkey", 0, -1).empty());

        // Create list key
        db.lpush("listkey", {"val"});

        // Try string operations
        REQUIRE(db.get("listkey") == "NULL");
    }

    SECTION("Expiring different types") {
        // String expiration
        db.set("str", "value");
        db.expire("str", 1);

        // List expiration
        db.lpush("list", {"item"});
        db.expire("list", 1);

        // Wait for expiration
        std::this_thread::sleep_for(std::chrono::milliseconds(1100));

        REQUIRE_FALSE(db.exists("str"));
        REQUIRE_FALSE(db.exists("list"));
    }
}