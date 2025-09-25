# cconfig

A small, dependency-free, single-header C library for parsing configuration files. It provides a robust parser for **TOML** and a simple, event-driven parser for **INI** files.

[![License](https://img.shields.io/github/license/Nouridin/cconfig)](LICENSE)
![Stars](https://img.shields.io/github/stars/Nouridin/cconfig?style=social)

---

## Features

*   **TOML & INI Support**: Parses the full TOML v1.0.0 spec and the simpler INI format.
*   **Zero Dependencies**: Uses only the C standard library for maximum portability.
*   **Single-Header Library**: Just drop `cconfig.h` into your project.
*   **Powerful Query API**: Access nested values, including array elements, with a simple dot-and-bracket notation (e.g., `"database.ports[0]"`).
*   **TOML Serialization**: Convert the parsed TOML data structure back into a string.
*   Clear error reporting with line and column numbers

## Integration

This is a single-header library. To use it, copy `cconfig.h` into your project and include it.

Then, in **one** C or C++ source file, define `CCONFIG_IMPLEMENTATION` before the include to create the implementation.

```c
#define CCONFIG_IMPLEMENTATION
#include "cconfig.h"
```

All other source files can include `cconfig.h` normally without the define.

## Package Manager Integration

You can easily add `cconfig` to your project using common C++ package managers.

### CMake (FetchContent)

If you are using CMake, you can use `FetchContent` to download and use `cconfig` directly from this repository.

```cmake
# In your CMakeLists.txt

include(FetchContent)

FetchContent_Declare(
  cconfig
  GIT_REPOSITORY https://github.com/Nouridin/cconfig.git
  GIT_TAG        main # Or a specific version tag like v1.0.0
)

FetchContent_MakeAvailable(cconfig)

# Now you can use the header in your targets
target_include_directories(your_target_name PRIVATE ${cconfig_SOURCE_DIR})
```

### vcpkg

`cconfig` is officially in VCPKG you can install it:

```bash
vcpkg install cconfig
```

And use it in your CMake project with `find_package(cconfig CONFIG REQUIRED)`.

## Quick Start: Parsing TOML

Here is a minimal example of parsing a TOML string and retrieving values.

```c
#include <stdio.h>
#include <stdlib.h>

// In one C file, do this:
#define CCONFIG_IMPLEMENTATION
#include "cconfig.h"

const char* my_config =
    "title = \"My App\"\n"
    "port = 8080\n"
    "\n"
    "[database]\n"
    "host = \"127.0.0.1\"\n"
    "enabled = true\n";

int main() {
    char error_buffer[256];
    cconfig_document_t* doc = cconfig_parse(my_config, error_buffer, sizeof(error_buffer));

    if (!doc) {
        fprintf(stderr, "Parse error: %s\n", error_buffer);
        return 1;
    }

    // Get top-level values
    const char* title = cconfig_get_string(doc, "title", "Default");
    int64_t port = cconfig_get_int(doc, "port", 9000);

    // Get nested values using dot notation
    const char* db_host = cconfig_get_string(doc, "database.host", "localhost");
    bool db_enabled = cconfig_get_bool(doc, "database.enabled", false);

    printf("Title: %s\n", title);
    printf("Port: %lld\n", (long long)port);
    printf("Database Host: %s\n", db_host);
    printf("Database Enabled: %s\n", db_enabled ? "true" : "false");

    // Clean up
    cconfig_free(doc);

    return 0;
}
```

## Contributing

Contributions are welcome! Whether it's bug reports, feature requests, or code contributions, all are appreciated.

Please read our **Contributing Guidelines** to get started. Also, be sure to review our **Code of Conduct**.

---
