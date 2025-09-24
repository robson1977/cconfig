#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define the implementation in the test runner
#define CCONFIG_IMPLEMENTATION
#include "../cconfig.h"

// --- Simple Test Framework ---
static int total_tests = 0;
static int tests_passed = 0;

#define TEST_CASE(name) static void name()
#define RUN_TEST(name) \
    do { \
        printf("--- Running %s ---\n", #name); \
        name(); \
    } while (0)

// Assertion macros
#define ASSERT(condition) \
    do { \
        total_tests++; \
        if (condition) { \
            tests_passed++; \
            printf("  [PASS] %s\n", #condition); \
        } else { \
            fprintf(stderr, "  [FAIL] %s, file %s, line %d\n", #condition, __FILE__, __LINE__); \
        } \
    } while (0)

#define ASSERT_STR_EQ(s1, s2) \
    do { \
        total_tests++; \
        const char* _s1 = (s1); \
        const char* _s2 = (s2); \
        if (_s1 && _s2 && strcmp(_s1, _s2) == 0) { \
            tests_passed++; \
            printf("  [PASS] \"%s\" == \"%s\"\n", _s1, _s2); \
        } else { \
            fprintf(stderr, "  [FAIL] Expected \"%s\", got \"%s\", file %s, line %d\n", _s2, _s1, __FILE__, __LINE__); \
        } \
    } while (0)


// --- Test Data ---
const char* valid_toml =
    "title = \"TOML Example\"\n"
    "owner.name = \"Tom\"\n"
    "\n"
    "[database]\n"
    "ip = \"192.168.1.1\"\n"
    "ports = [ 8001, 8002, 8003 ]\n"
    "enabled = true\n"
    "\n"
    "[[products]]\n"
    "name = \"Hammer\"\n"
    "sku = 738594937\n"
    "\n"
    "[[products]]\n"
    "name = \"Nail\"\n"
    "sku = 284758393\n";

const char* invalid_toml = "key = [ 1, 2, ] # Invalid trailing comma";

// --- Test Cases ---

TEST_CASE(test_basic_parsing) {
    char err[256];
    cconfig_document_t* doc = cconfig_parse(valid_toml, err, sizeof(err));
    ASSERT(doc != NULL);

    ASSERT_STR_EQ(cconfig_get_string(doc, "title", ""), "TOML Example");
    ASSERT_STR_EQ(cconfig_get_string(doc, "owner.name", ""), "Tom");
    ASSERT(cconfig_get_bool(doc, "database.enabled", false) == true);
    ASSERT(cconfig_get_int(doc, "products[1].sku", 0) == 284758393);

    cconfig_free(doc);
}

TEST_CASE(test_advanced_queries) {
    char err[256];
    cconfig_document_t* doc = cconfig_parse(valid_toml, err, sizeof(err));
    ASSERT(doc != NULL);

    // Query array element
    ASSERT(cconfig_get_int(doc, "database.ports[0]", 0) == 8001);
    ASSERT(cconfig_get_int(doc, "database.ports[2]", 0) == 8003);

    // Query array of tables
    ASSERT_STR_EQ(cconfig_get_string(doc, "products[0].name", ""), "Hammer");
    ASSERT_STR_EQ(cconfig_get_string(doc, "products[1].name", ""), "Nail");

    // Test default values for non-existent keys
    ASSERT(cconfig_get_int(doc, "database.ports[3]", -1) == -1); // Out of bounds
    ASSERT_STR_EQ(cconfig_get_string(doc, "products[0].color", "default"), "default");

    cconfig_free(doc);
}

TEST_CASE(test_parse_errors) {
    char err[256];
    cconfig_document_t* doc = cconfig_parse(invalid_toml, err, sizeof(err));

    ASSERT(doc == NULL);
    ASSERT(strstr(err, "Expected ']' to close array") != NULL);
    printf("  Correctly caught error: %s\n", err);

    cconfig_free(doc); // Should be safe to call on NULL
}

TEST_CASE(test_serialization) {
    char err[256];
    cconfig_document_t* doc = cconfig_parse("a = 1\nb = \"hello\"", err, sizeof(err));
    ASSERT(doc != NULL);

    char* serialized = cconfig_serialize(doc);
    ASSERT(serialized != NULL);

    // Simple check, serialization order isn't guaranteed
    ASSERT(strstr(serialized, "a = 1") != NULL);
    ASSERT(strstr(serialized, "b = \"hello\"") != NULL);

    free(serialized);
    cconfig_free(doc);
}


// --- Test Runner Main ---

int main() {
    RUN_TEST(test_basic_parsing);
    RUN_TEST(test_advanced_queries);
    RUN_TEST(test_parse_errors);
    RUN_TEST(test_serialization);

    printf("\n--- Test Summary ---\n");
    printf("Total tests: %d\n", total_tests);
    printf("Passed: %d\n", tests_passed);
    printf("Failed: %d\n", total_tests - tests_passed);

    if (total_tests == tests_passed) {
        printf("\nAll tests passed!\n");
        return 0; // Success
    } else {
        fprintf(stderr, "\nSome tests failed!\n");
        return 1; // Failure
    }
}
