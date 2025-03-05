#include <iostream>

#include "database.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Database db;

    db.set("Hello", "World");
    std::cout << db.get("Hello") << std::endl;

    db.del("Hello");
    std::cout << db.get("Hello") << std::endl;
    return 0;
}
