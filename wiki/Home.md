# Welcome to the cconfig Wiki!

`cconfig` is a small, dependency-free, single-header C library for parsing configuration files. It provides a robust parser for **TOML** and a simple, event-driven parser for **INI** files.

## Features

*   **TOML & INI Support**: Parses the full TOML v1.0.0 spec and the simpler INI format.
*   **Zero Dependencies**: Uses only the C standard library for maximum portability.
*   **Single-Header Library**: Just drop `cconfig.h` into your project.
*   **Powerful Query API**: Access nested values, including array elements, with a simple dot-and-bracket notation (e.g., `"database.ports[0]"`).
*   **TOML Serialization**: Convert the parsed TOML data structure back into a string.
*   Clear error reporting with line and column numbers

## Installation

`cconfig` is designed as a single-header library, making integration straightforward. You can also integrate it using CMake's `FetchContent`.

### Single-Header Integration

1.  **Copy `cconfig.h`**: Place the `cconfig.h` file into your project's source directory.
2.  **Define `CCONFIG_IMPLEMENTATION`**: In **one** of your `.c` or `.cpp` source files, define `CCONFIG_IMPLEMENTATION` *before* including `cconfig.h`. This tells the preprocessor to include the implementation details in that specific compilation unit.

    ```c
    // my_main_file.c
    #define CCONFIG_IMPLEMENTATION
    #include "cconfig.h"

    // Your application code...
    ```
3.  **Include `cconfig.h` elsewhere**: In any other source files that need to use `cconfig` functions, simply include `cconfig.h` normally, without defining `CCONFIG_IMPLEMENTATION`.

    ```c
    // another_file.c
    #include "cconfig.h"

    // Use cconfig functions...
    ```

### CMake (FetchContent)

If your project uses CMake, you can integrate `cconfig` directly from its GitHub repository using `FetchContent`.

Add the following to your `CMakeLists.txt`:

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
