#include <stdio.h>
#include <stdlib.h>

// Define the implementation in this file
#define CCONFIG_IMPLEMENTATION
#include "../cconfig.h"

// A sample TOML configuration string
const char* toml_string =
    "# This is a cconfig example.\n"
    "title = \"TOML Example\"\n"
    "enabled = true\n"
    "\n"
    "[database]\n"
    "ip = \"192.168.1.1\"\n"
    "ports = [ 8001, 8001, 8002 ]\n"
    "connection_max = 5000\n"
    "data = [ [ 1.0, 2.0 ], [ 3.0, 4.0, 5.0 ] ]\n"
    "\n"
    "[[products]]\n"
    "name = \"Hammer\"\n"
    "sku = 738594937\n"
    "\n"
    "[[products]]\n"
    "name = \"Nail\"\n"
    "sku = 284758393\n";

int main() {
    char error_buffer[256];
    cconfig_document_t* doc = cconfig_parse(toml_string, error_buffer, sizeof(error_buffer));

    if (!doc) {
        fprintf(stderr, "Failed to parse TOML: %s\n", error_buffer);
        return 1;
    }

    printf("Parsing successful!\n\n");

    // --- Querying values ---

    // Get simple values
    const char* title = cconfig_get_string(doc, "title", "Default Title");
    bool enabled = cconfig_get_bool(doc, "enabled", false);

    printf("Title: %s\n", title);
    printf("Enabled: %s\n", enabled ? "true" : "false");

    // Get values from a table
    const char* db_ip = cconfig_get_string(doc, "database.ip", "127.0.0.1");
    int64_t max_conn = cconfig_get_int(doc, "database.connection_max", 100);

    printf("\n[database]\n");
    printf("  IP: %s\n", db_ip);
    printf("  Max Connections: %lld\n", (long long)max_conn);

    // Get an array
    cconfig_array_t* ports = cconfig_get_array(doc, "database.ports");
    if (ports) {
        printf("  Ports: ");
        for (size_t i = 0; i < ports->count; i++) {
            printf("%lld ", (long long)ports->values[i]->value.i);
        }
        printf("\n");
    }

    // --- Querying arrays and tables of arrays with new syntax ---
    printf("\n--- Advanced Queries ---\n");

    // Get a specific port from the array
    int64_t second_port = cconfig_get_int(doc, "database.ports[1]", -1);
    printf("Second port in list: %lld\n", (long long)second_port);

    // Get values from the first product in the array of tables
    const char* first_product_name = cconfig_get_string(doc, "products[0].name", "N/A");
    int64_t first_product_sku = cconfig_get_int(doc, "products[0].sku", 0);
    printf("First product: Name=%s, SKU=%lld\n", first_product_name, (long long)first_product_sku);

    // Get a value from a nested array
    double nested_val = cconfig_get_double(doc, "database.data[1][0]", 0.0);
    printf("Nested array value data[1][0]: %g\n", nested_val);

    // --- Serialize the document back to a string ---
    char* serialized_output = cconfig_serialize(doc);
    if (serialized_output) {
        printf("\n--- Serialized Output ---\n%s", serialized_output);
        free(serialized_output);
    }

    // Clean up
    cconfig_free(doc);

    return 0;
}