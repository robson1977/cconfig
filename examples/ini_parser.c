#include <stdio.h>

// Define the implementation in this file
#define CCONFIG_IMPLEMENTATION
#include "../cconfig.h"

// A sample INI configuration string
const char* ini_string =
    "; global settings\n"
    "user = \"default\"\n"
    "\n"
    "[database]\n"
    "host = localhost\n"
    "port = 5432\n"
    "user = admin\n"
    "\n"
    "# network settings\n"
    "[network]\n"
    "timeout = 30\n";

// The callback function that will be called by the INI parser
int ini_handler(void* userdata, const char* section, const char* key, const char* value) {
    // userdata can be used to pass context, e.g., a struct to populate.
    // Here, we just print the values.
    (void)userdata; // Unused for this simple example

    if (section[0] == '\0') {
        printf("Global: %s = %s\n", key, value);
    } else {
        printf("Section '%s': %s = %s\n", section, key, value);
    }

    // Return 0 to continue parsing
    return 0;
}

int main() {
    printf("--- Parsing INI string ---\n");
    int result = cconfig_ini_parse(ini_string, ini_handler, NULL);

    printf("\nParsing finished with code: %d\n", result);
    return result;
}