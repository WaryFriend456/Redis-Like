# Simple Key-Value Database

A lightweight in-memory key-value database implementation written in C++. This project provides a simple way to store, retrieve, and delete string key-value pairs.

## Features

- In-memory key-value storage
- Basic operations: SET, GET, DELETE
- Command string parsing utility
- Comprehensive test suite using Catch2

## Installation

### Prerequisites

- C++ compiler with C++17 support
- CMake (minimum version 3.15)

### Build Instructions

1. Clone the repository
```bash
git clone https://github.com/YourUsername/key-value-database.git
cd key-value-database
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

### Basic API

```cpp
#include "database.h"

// Create database instance
Database db;

// Store key-value pairs
db.set("name", "John");
db.set("age", "30");

// Retrieve values
std::string name = db.get("name");  // Returns "John"
std::string missing = db.get("unknown");  // Returns "NULL"

// Delete entries
bool deleted = db.del("age");  // Returns true
bool notFound = db.del("unknown");  // Returns false
```

### Command Parsing

The project includes a command parser to help process string inputs:

```cpp
#include "command_parser.h"

std::string input = "SET key1 value1";
std::vector<std::string> tokens = parseCommand(input);
// tokens = {"SET", "key1", "value1"}
```

## Testing

The project uses the Catch2 testing framework. Tests are located in the `tests` directory.

### Running Tests

From the build directory:
```bash
make test
```

Or run the test executable directly:
```bash
./tests/tests
```

## Project Structure

```
.
├── include/
│   ├── database.h          # Database class definition
│   └── command_parser.h    # Command parsing utilities
├── src/
│   ├── database.cpp        # Database implementation
│   ├── command_parser.cpp  # Command parser implementation 
│   └── catch_amalgamated.cpp # Test framework source
├── tests/
│   ├── test.cpp            # Test setup
│   └── test_database.cpp   # Database tests
├── CMakeLists.txt          # Build configuration
└── docs/
    └── Readme.md           # This file
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