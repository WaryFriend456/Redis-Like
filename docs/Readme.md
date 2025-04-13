# Redis-like In-Memory Database

A high-performance, multi-threaded in-memory database implementation in C++ inspired by Redis. This project provides a network-accessible key-value store with support for multiple data types and advanced features.

## Features

- **Multiple Data Types**:
  - Strings: Simple key-value storage
  - Lists: Ordered collections supporting push/pop operations
  - Hashes: Field-value pairs under a single key
  - Sets: Unordered collections of unique strings
  
- **Network Accessibility**: 
  - TCP server implementation using Boost.Asio
  - Multi-threaded connection handling
  
- **Advanced Features**:
  - Key expiration (TTL support)
  - Type checking and validation
  - Thread-safe operations with mutex protection

## Architecture

The project is built with a layered architecture:

1. **Database Layer**: Core data storage and operations
2. **Network Layer**: TCP server implementation for client connections
3. **Session Management**: Handles individual client connections

## Installation

### Prerequisites

- C++17 compatible compiler
- Boost libraries (for Asio networking)
- CMake (minimum version 3.15)

### Build Instructions

1. Clone the repository
```bash
git clone https://github.com/YourUsername/redis_like.git
cd redis_like
```

2. Create and navigate to build directory
```bash
mkdir build && cd build
```

3. Configure and build
```bash
cmake ..
make
```

## Usage

### Running the Server

```bash
./redis_like
```

The server will start on port 6379 (default Redis port) with multithreading enabled.

### Database API

```cpp
#include "database.h"

// Create database instance
Database db;

// String operations
db.set("name", "John");
std::string name = db.get("name");

// List operations
db.rpush("mylist", {"item1", "item2", "item3"});
db.lpush("mylist", {"item0"});
std::string item = db.lpop("mylist");
std::vector<std::string> items = db.lrange("mylist", 0, -1);

// Hash operations
db.hset("user:1000", "name", "John");
db.hset("user:1000", "email", "john@example.com");
std::string email = db.hget("user:1000", "email");
auto userFields = db.hgetall("user:1000");

// Set operations
db.sadd("myset", {"value1", "value2"});
bool isMember = db.sismember("myset", "value1");
auto setMembers = db.smembers("myset");

// Key operations
bool exists = db.exists("name");
db.expire("name", 60); // expire in 60 seconds
long long remaining = db.ttl("name");
```

## Project Structure

```
.
├── include/
│   ├── database.h          # Core database functionality
│   ├── server.h            # TCP server implementation
│   └── session.h           # Client session handling
├── src/
│   ├── database.cpp        # Database implementation
│   ├── server.cpp          # Server implementation
│   ├── session.cpp         # Session implementation
│   └── main.cpp            # Entry point
├── tests/                  # Test files
├── CMakeLists.txt          # Build configuration
└── docs/
    └── Readme.md           # This file
```

## Implementation Details

### Threading Model

The server utilizes a thread pool with size equal to the number of hardware threads available on the system, ensuring optimal performance through parallel processing of client requests.

### Memory Management

All data is stored in-memory using STL containers:
- `std::unordered_map` for the main data store
- `std::vector` for lists
- `std::set` for sets
- `std::unordered_map` for hashes

### Network Protocol

The server uses a TCP-based protocol for communication. Client requests are processed asynchronously using Boost.Asio.

## Testing

The project uses the Catch2 testing framework for unit testing all database operations.

### Running Tests

```bash
./tests/redis_like_tests
```

## License

[MIT License](https://opensource.org/licenses/MIT)

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request