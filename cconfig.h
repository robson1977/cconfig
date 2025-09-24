/*
 * cconfig - A simple configuration file parser
 *
 * This is a single-header library. To use it, do this in one C file:
 * #define CCONFIG_IMPLEMENTATION
 * #include "cconfig.h"
 *
 */

#ifndef CCONFIG_H
#define CCONFIG_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
struct cconfig_table_t;
struct cconfig_array_t;

// Represents a datetime value.
typedef struct cconfig_datetime_t {
    int year, month, day;
    int hour, minute, second;
    char timezone[16];
} cconfig_datetime_t;

// Represents the type of a value.
typedef enum cconfig_value_type_t {
    CCONFIG_VALUE_NONE,
    CCONFIG_VALUE_STRING,
    CCONFIG_VALUE_INTEGER,
    CCONFIG_VALUE_FLOAT,
    CCONFIG_VALUE_BOOLEAN,
    CCONFIG_VALUE_DATETIME,
    CCONFIG_VALUE_ARRAY,
    CCONFIG_VALUE_TABLE
} cconfig_value_type_t;

// Represents a single value.
typedef struct cconfig_value_t {
    cconfig_value_type_t type;
    union {
        char* s;
        int64_t i;
        double d;
        bool b;
        cconfig_datetime_t dt;
        struct cconfig_array_t* a;
        struct cconfig_table_t* t;
    } value;
} cconfig_value_t;

// Represents an array.
typedef struct cconfig_array_t {
    cconfig_value_t** values;
    size_t count;
    size_t capacity;
} cconfig_array_t;

// Represents a key-value pair in a table.
typedef struct cconfig_key_value_t {
    char* key;
    cconfig_value_t* value;
} cconfig_key_value_t;

// Represents a TOML table.
typedef struct cconfig_table_t {
    cconfig_key_value_t** pairs;
    size_t count;
    size_t capacity;
} cconfig_table_t;

// Represents a full document.
typedef struct cconfig_document_t {
    cconfig_table_t* root;
} cconfig_document_t;

// --- Public API ---

#ifndef CCONFIG_API
#define CCONFIG_API
#endif

CCONFIG_API

/**
 * @brief Parses a TOML string and builds a document object model (DOM).
 *
 * @param input The null-terminated string containing the TOML data.
 * @param error_buffer A buffer to write a descriptive error message to if parsing fails.
 * @param error_buffer_size The size of the error_buffer.
 * @return A pointer to a new cconfig_document_t on success, or NULL on failure.
 *         The returned document must be freed with cconfig_free().
 */
cconfig_document_t* cconfig_parse(const char* input, char* error_buffer, size_t error_buffer_size);

CCONFIG_API
/**
 * @brief Serializes a TOML document back into a formatted string.
 *
 * @param doc The document to serialize.
 * @return A newly allocated string containing the TOML representation. The caller is
 *         responsible for freeing this string. Returns NULL if the document is NULL.
 */
char* cconfig_serialize(const cconfig_document_t* doc);

CCONFIG_API
/**
 * @brief Frees all memory associated with a TOML document.
 *
 * @param doc The document to free.
 */
void cconfig_free(cconfig_document_t* doc);

// Convenience functions for querying values

CCONFIG_API
/**
 * @brief Retrieves a table from the document using a dot-separated key path.
 * @param doc The parsed TOML document.
 * @param key_path The dot-separated path to the table (e.g., "database.credentials").
 * @return A pointer to the cconfig_table_t if found, otherwise NULL.
 */
cconfig_table_t* cconfig_get_table(const cconfig_document_t* doc, const char* key_path);

CCONFIG_API
/**
 * @brief Retrieves an array from the document using a dot-separated key path.
 * @param doc The parsed TOML document.
 * @param key_path The dot-separated path to the array (e.g., "database.ports").
 * @return A pointer to the cconfig_array_t if found, otherwise NULL.
 */
cconfig_array_t* cconfig_get_array(const cconfig_document_t* doc, const char* key_path);

CCONFIG_API
/**
 * @brief Retrieves a string value from the document.
 * @param doc The parsed TOML document.
 * @param key_path The dot-separated path to the key.
 * @param default_val The value to return if the key is not found or is not a string.
 * @return The string value if found, otherwise default_val.
 */
const char* cconfig_get_string(const cconfig_document_t* doc, const char* key_path, const char* default_val);

CCONFIG_API
/**
 * @brief Retrieves an integer value from the document.
 * @param doc The parsed TOML document.
 * @param key_path The dot-separated path to the key.
 * @param default_val The value to return if the key is not found or is not an integer.
 * @return The integer value if found, otherwise default_val.
 */
int64_t cconfig_get_int(const cconfig_document_t* doc, const char* key_path, int64_t default_val);

CCONFIG_API
/**
 * @brief Retrieves a double/float value from the document.
 * @param doc The parsed TOML document.
 * @param key_path The dot-separated path to the key.
 * @param default_val The value to return if the key is not found or is not a number.
 * @return The double value if found, otherwise default_val. If the key holds an integer, it will be promoted to a double.
 */
double cconfig_get_double(const cconfig_document_t* doc, const char* key_path, double default_val);

CCONFIG_API
/**
 * @brief Retrieves a boolean value from the document.
 * @param doc The parsed TOML document.
 * @param key_path The dot-separated path to the key.
 * @param default_val The value to return if the key is not found or is not a boolean.
 * @return The boolean value if found, otherwise default_val.
 */
bool cconfig_get_bool(const cconfig_document_t* doc, const char* key_path, bool default_val);

// --- INI Parser ---

/**
 * @brief Callback handler for the INI parser.
 *
 * This function is called for each key-value pair found in the INI string.
 * @param userdata A user-provided pointer, passed through from cconfig_ini_parse.
 * @param section The name of the current section. Will be an empty string "" for keys in the global scope.
 * @param key The key for the current entry.
 * @param value The value for the current entry.
 * @return Return 0 to continue parsing, or any non-zero value to stop.
 */
typedef int (*cconfig_ini_handler)(void* userdata, const char* section, const char* key, const char* value);

CCONFIG_API
/**
 * @brief Parses an INI-formatted string using a callback-based approach.
 *
 * @param ini_string The null-terminated string containing the INI data.
 * @param handler The callback function to be invoked for each key-value pair.
 * @param userdata A user-defined pointer that will be passed to the handler function.
 * @return 0 on successful completion, -1 on argument or memory error.
 */
int cconfig_ini_parse(const char* ini_string, cconfig_ini_handler handler, void* userdata);

#ifdef __cplusplus
}
#endif

#endif // CCONFIG_H

#ifdef CCONFIG_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>

// Token types
typedef enum cconfig_token_type_t {
    CCONFIG_TOKEN_EOF,
    CCONFIG_TOKEN_ERROR,
    CCONFIG_TOKEN_IDENT,
    CCONFIG_TOKEN_STRING,
    CCONFIG_TOKEN_INTEGER,
    CCONFIG_TOKEN_FLOAT,
    CCONFIG_TOKEN_BOOLEAN,
    CCONFIG_TOKEN_DATETIME,
    CCONFIG_TOKEN_EQUAL,
    CCONFIG_TOKEN_LBRACKET,
    CCONFIG_TOKEN_RBRACKET,
    CCONFIG_TOKEN_LBRACE,
    CCONFIG_TOKEN_RBRACE,
    CCONFIG_TOKEN_COMMA,
    CCONFIG_TOKEN_DOT,
    CCONFIG_TOKEN_NEWLINE,
    CCONFIG_TOKEN_COMMENT
} cconfig_token_type_t;

// cconfig_token_t structure
typedef struct cconfig_token_t {
    cconfig_token_type_t type;
    const char* start;
    int len;
    int line;
    int col;
} cconfig_token_t;

// Lexer state
typedef struct cconfig_lexer_t {
    const char* input;
    int pos;
    int line;
    int col;
} cconfig_lexer_t;

// Parser state
typedef struct cconfig_parser_t {
    cconfig_lexer_t lexer;
    cconfig_token_t current;
    cconfig_token_t previous;
    char* error_buffer;
    size_t error_buffer_size;
    bool panic_mode;
    cconfig_table_t* current_table;
} cconfig_parser_t;

// --- String Buffer ---

typedef struct cconfig_string_buffer_t {
    char* buffer;
    size_t len;
    size_t capacity;
} cconfig_string_buffer_t;

static void sb_init(cconfig_string_buffer_t* sb) {
    sb->capacity = 128;
    sb->buffer = (char*)malloc(sb->capacity);
    if (sb->buffer) {
        sb->buffer[0] = '\0';
    }
    sb->len = 0;
}

static bool sb_appendf(cconfig_string_buffer_t* sb, const char* format, ...) {
    if (!sb->buffer) return false;

    va_list args, args_copy;
    va_start(args, format);
    va_copy(args_copy, args);

    int needed = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (sb->len + needed + 1 > sb->capacity) {
        size_t new_capacity = sb->capacity;
        while (sb->len + needed + 1 > new_capacity) {
            new_capacity *= 2;
        }
        char* new_buffer = (char*)realloc(sb->buffer, new_capacity);
        if (!new_buffer) {
            va_end(args_copy);
            return false;
        }
        sb->buffer = new_buffer;
        sb->capacity = new_capacity;
    }

    vsnprintf(sb->buffer + sb->len, needed + 1, format, args_copy);
    sb->len += needed;

    va_end(args_copy);
    return true;
}

// --- Portability Helpers ---

static char* cconfig_strndup(const char* s, size_t n) {
    char* p = (char*)malloc(n + 1);
    if (p) {
        memcpy(p, s, n);
        p[n] = '\0';
    }
    return p;
}

static char* cconfig_strdup(const char* s) {
    return cconfig_strndup(s, strlen(s));
}

// --- Lexer Implementation ---

static bool is_at_end(cconfig_lexer_t* lexer) {
    return lexer->input[lexer->pos] == '\0';
}

static char advance(cconfig_lexer_t* lexer) {
    lexer->pos++;
    lexer->col++;
    return lexer->input[lexer->pos - 1];
}

static char peek(cconfig_lexer_t* lexer) {
    return lexer->input[lexer->pos];
}

static cconfig_token_t make_token(cconfig_lexer_t* lexer, cconfig_token_type_t type) {
    cconfig_token_t token;
    token.type = type;
    token.start = NULL; // Will be set by caller
    token.len = 0;      // Will be set by caller
    token.line = lexer->line;
    token.col = lexer->col;
    return token;
}

static cconfig_token_t error_token(cconfig_lexer_t* lexer, const char* message) {
    cconfig_token_t token;
    token.type = CCONFIG_TOKEN_ERROR;
    token.start = message;
    token.len = (int)strlen(message);
    token.line = lexer->line;
    token.col = lexer->col;
    return token;
}

static void skip_whitespace(cconfig_lexer_t* lexer) {
    for (;;) {
        char c = peek(lexer);
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance(lexer);
                break;
            default:
                return;
        }
    }
}

static cconfig_token_t string_token(cconfig_lexer_t* lexer, char quote_char) {
    const char* start = &lexer->input[lexer->pos - 1];
    bool is_multiline = false;

    // Check for multiline string `"""` or `'''`
    if (peek(lexer) == quote_char && lexer->input[lexer->pos + 1] == quote_char) {
        is_multiline = true;
        advance(lexer);
        advance(lexer);
    }

    while (!is_at_end(lexer)) {
        if (peek(lexer) == quote_char) {
            if (is_multiline) {
                if (lexer->input[lexer->pos + 1] == quote_char && lexer->input[lexer->pos + 2] == quote_char) {
                    advance(lexer);
                    advance(lexer);
                    advance(lexer); // Consume closing `"""` or `'''`
                    goto end_string;
                }
            } else {
                advance(lexer); // Consume closing `"` or `'`
                goto end_string;
            }
        }

        if (peek(lexer) == '\n' && !is_multiline) {
            return error_token(lexer, "Unterminated string (newline in single-line string).");
        }

        if (peek(lexer) == '\n') {
            lexer->line++;
            lexer->col = 0; // Will be incremented by advance()
        }
        // Handle escape characters only in non-literal (double-quoted) strings
        if (quote_char == '"' && peek(lexer) == '\\') {
            advance(lexer); // Consume '\'
        }
        advance(lexer);
    }

    return error_token(lexer, "Unterminated string.");

end_string:;
    cconfig_token_t tok = make_token(lexer, CCONFIG_TOKEN_STRING);
    tok.start = start;
    tok.len = (int)(&lexer->input[lexer->pos] - start);
    return tok;
}

// Processes a string literal, handling escape sequences and removing quotes.
// Returns a newly allocated string.
static char* process_string_literal(const char* start, int len) {
    char quote_char = start[0];
    bool is_multiline = (len > 5 && start[1] == quote_char && start[2] == quote_char);
    int content_start = is_multiline ? 3 : 1;
    int content_end = len - (is_multiline ? 3 : 1);

    // Simple case for literal strings (no escapes)
    if (quote_char == '\'') {
        return cconfig_strndup(start + content_start, content_end - content_start);
    }

    // Handle escapes for basic strings
    cconfig_string_buffer_t sb;
    sb_init(&sb);
    if (!sb.buffer) return NULL; // OOM

    for (int i = content_start; i < content_end; ++i) {
        bool appended = true;
        if (start[i] == '\n') {
            // Normalize line endings
            appended = sb_appendf(&sb, "%c", '\n');
        } else
        if (start[i] == '\\') {
            i++; // Move to the character after '\'
            switch (start[i]) {
                case 'b': sb_appendf(&sb, "%c", '\b'); break;
                case 't': sb_appendf(&sb, "%c", '\t'); break;
                case 'n': sb_appendf(&sb, "%c", '\n'); break;
                case 'f': sb_appendf(&sb, "%c", '\f'); break;
                case 'r': sb_appendf(&sb, "%c", '\r'); break;
                case '"': sb_appendf(&sb, "%c", '"'); break;
                case '\\': sb_appendf(&sb, "%c", '\\'); break;
                // TODO: Add support for \uXXXX and \UXXXXXXXX
                default:
                    // Invalid escape sequence, just append the backslash and the char
                    appended = sb_appendf(&sb, "\\%c", start[i]);
                    break;
            }
        } else {
            appended = sb_appendf(&sb, "%c", start[i]);
        }
        if (!appended) {
            free(sb.buffer);
            return NULL; // OOM
        }
    }

    // Shrink buffer to fit and return
    char* final_str = (char*)realloc(sb.buffer, sb.len + 1);
    if (final_str) {
        final_str[sb.len] = '\0';
        return final_str;
    }

    free(sb.buffer);
    return NULL; // OOM
}

static cconfig_token_t identifier_token(cconfig_lexer_t* lexer) {
    const char* start = &lexer->input[lexer->pos - 1];
    while (isalnum(peek(lexer)) || peek(lexer) == '_' || peek(lexer) == '-') {
        advance(lexer);
    }
    
    cconfig_token_t token = make_token(lexer, CCONFIG_TOKEN_IDENT);
    token.start = start;
    token.len = (int)(&lexer->input[lexer->pos] - start);

    // Check for boolean keywords
    if (token.len == 4 && strncmp(token.start, "true", 4) == 0) {
        token.type = CCONFIG_TOKEN_BOOLEAN;
    } else if (token.len == 5 && strncmp(token.start, "false", 5) == 0) {
        token.type = CCONFIG_TOKEN_BOOLEAN;
    }

    return token;
}

// Function prototypes
static cconfig_token_t next_token(cconfig_lexer_t* lexer);
static cconfig_value_t* parse_value(cconfig_parser_t* parser);
static void cconfig_free_value(cconfig_value_t* value);
static bool cconfig_table_add_pair(cconfig_table_t* table, cconfig_key_value_t* pair);
static bool cconfig_array_add_value(cconfig_array_t* array, cconfig_value_t* value);
static cconfig_table_t* find_or_create_table(cconfig_table_t* parent, const char* key, int key_len);

static void cconfig_free_table(cconfig_table_t* table);

// Forward declarations for serializer
static void cconfig_serialize_table_contents(cconfig_string_buffer_t* sb, const cconfig_table_t* table, const char* table_path);

// Helper functions
static void parser_error(cconfig_parser_t* parser, const char* message) {
    // Preserve the first error encountered. If panic_mode is already set,
    // do not overwrite the existing error message — first error wins.
    if (parser->panic_mode) return;

    if (parser->error_buffer && parser->error_buffer_size > 0) {
        snprintf(parser->error_buffer, parser->error_buffer_size, "Error at line %d, col %d: %s",
                 parser->previous.line, parser->previous.col, message);
    }
    parser->panic_mode = true;
}

static void advance_token(cconfig_parser_t* parser) {
    parser->previous = parser->current;
    parser->current = next_token(&parser->lexer);
    // Skip error tokens until we find something we can parse.
    while (parser->current.type == CCONFIG_TOKEN_ERROR) {
        parser_error(parser, parser->current.start);
        parser->current = next_token(&parser->lexer);
    }
}

static bool consume_token(cconfig_parser_t* parser, cconfig_token_type_t type, const char* message) {
    if (parser->current.type == type) {
        advance_token(parser);
        return true;
    }
    if (message) {
        parser_error(parser, message);
    }
    return false;
}

// Helper to add a key-value pair to a table, handling reallocation.
static bool cconfig_table_add_pair(cconfig_table_t* table, cconfig_key_value_t* pair) {
    if (table->count >= table->capacity) {
        size_t new_capacity = table->capacity == 0 ? 8 : table->capacity * 2;
        cconfig_key_value_t** new_pairs = (cconfig_key_value_t**)realloc(table->pairs, new_capacity * sizeof(cconfig_key_value_t*));
        if (!new_pairs) {
            return false; // OOM
        }
        table->pairs = new_pairs;
        table->capacity = new_capacity;
    }
    table->pairs[table->count++] = pair;
    return true;
}

// Helper to add a value to an array, handling reallocation.
static bool cconfig_array_add_value(cconfig_array_t* array, cconfig_value_t* value) {
    if (array->count >= array->capacity) {
        size_t new_capacity = array->capacity == 0 ? 8 : array->capacity * 2;
        cconfig_value_t** new_values = (cconfig_value_t**)realloc(array->values, new_capacity * sizeof(cconfig_value_t*));
        if (!new_values) {
            return false; // OOM
        }
        array->values = new_values;
        array->capacity = new_capacity;
    }
    array->values[array->count++] = value;
    return true;
}

// Parsing functions
static cconfig_key_value_t* parse_key_value(cconfig_parser_t* parser) {
    cconfig_table_t* table = parser->current_table;

    // Handle dotted keys by traversing/creating tables
    while (parser->current.type == CCONFIG_TOKEN_IDENT) {
        cconfig_token_t key_token = parser->current;
        advance_token(parser);

        if (parser->current.type == CCONFIG_TOKEN_DOT) {
            // This is a nested table path, e.g., the 'a' in 'a.b = 1'
            table = find_or_create_table(table, key_token.start, key_token.len);
            if (!table) {
                parser_error(parser, "Failed to create table for dotted key.");
                return NULL;
            }
            advance_token(parser); // Consume '.'
        } else if (parser->current.type == CCONFIG_TOKEN_EQUAL) {
            // This is the final key in the path
            const char* key = key_token.start;
            int key_len = key_token.len;

            if (!consume_token(parser, CCONFIG_TOKEN_EQUAL, "Expected '=' after key.")) return NULL;

            cconfig_value_t* value = parse_value(parser);
            if (!value) return NULL;

            cconfig_key_value_t* pair = (cconfig_key_value_t*)malloc(sizeof(cconfig_key_value_t));
            if (!pair) { parser_error(parser, "Out of memory."); return NULL; }

            pair->key = cconfig_strndup(key, key_len);
            if (!pair->key) { free(pair); parser_error(parser, "Out of memory."); return NULL; }
            pair->value = value;

            // Add pair to the correct (potentially nested) table
            if (!cconfig_table_add_pair(table, pair)) {
                cconfig_free_value(pair->value);
                free(pair->key);
                free(pair);
                parser_error(parser, "Out of memory.");
                return NULL;
            }
            return pair; // Return a dummy pair to signal success, even though it's already added
        }
    }

    return NULL;
}

static cconfig_array_t* find_or_create_array_of_tables(cconfig_parser_t* parser, cconfig_table_t* parent, const char* key, int key_len) {
    // Look for an existing array of tables
    for (size_t i = 0; i < parent->count; i++) {
        cconfig_key_value_t* pair = parent->pairs[i];
        if (strncmp(pair->key, key, key_len) == 0 && pair->key[key_len] == '\0') {
            if (pair->value->type == CCONFIG_VALUE_ARRAY) {
                // TOML spec: Once a key is used for an array of tables, it cannot be used for anything else.
                // We can add a check here to ensure all elements are tables if needed.
                return pair->value->value.a;
            }
            // Error: key exists but is not an array.
            parser_error(parser, "Key redefined as an array of tables.");
            return NULL;
        }
    }

    // Not found, create a new one
    cconfig_key_value_t* pair = (cconfig_key_value_t*)malloc(sizeof(cconfig_key_value_t));
    if (!pair) { parser_error(parser, "Out of memory."); return NULL; }

    pair->key = cconfig_strndup(key, key_len);
    if (!pair->key) { free(pair); parser_error(parser, "Out of memory."); return NULL; }

    pair->value = (cconfig_value_t*)malloc(sizeof(cconfig_value_t));
    if (!pair->value) { free(pair->key); free(pair); parser_error(parser, "Out of memory."); return NULL; }

    pair->value->type = CCONFIG_VALUE_ARRAY;
    pair->value->value.a = (cconfig_array_t*)calloc(1, sizeof(cconfig_array_t));
    if (!pair->value->value.a) {
        free(pair->value); free(pair->key); free(pair);
        parser_error(parser, "Out of memory.");
        return NULL;
    }

    if (!cconfig_table_add_pair(parent, pair)) {
        cconfig_free_value(pair->value);
        free(pair->key);
        free(pair);
        parser_error(parser, "Out of memory.");
        return NULL;
    }
    return pair->value->value.a;
}

static cconfig_table_t* find_or_create_table(cconfig_table_t* parent, const char* key, int key_len) {
    // Look for an existing table
    for (size_t i = 0; i < parent->count; i++) {
        if (strncmp(parent->pairs[i]->key, key, key_len) == 0 && parent->pairs[i]->key[key_len] == '\0') {
            if (parent->pairs[i]->value->type == CCONFIG_VALUE_TABLE) {
                return parent->pairs[i]->value->value.t;
            }
            // TODO: Error: key exists but is not a table
            return NULL;
        }
    }

    // Not found, create a new one
    cconfig_key_value_t* pair = (cconfig_key_value_t*)malloc(sizeof(cconfig_key_value_t));
    if (!pair) return NULL;
    pair->key = cconfig_strndup(key, key_len);
    if (!pair->key) { free(pair); return NULL; }
    pair->value = (cconfig_value_t*)malloc(sizeof(cconfig_value_t));
    if (!pair->value) { free(pair->key); free(pair); return NULL; }
    pair->value->type = CCONFIG_VALUE_TABLE;
    pair->value->value.t = (cconfig_table_t*)calloc(1, sizeof(cconfig_table_t)); // Use calloc for zero-initialization
    if (!pair->value->value.t) { free(pair->value); free(pair->key); free(pair); return NULL; }

    if (!cconfig_table_add_pair(parent, pair)) {
        cconfig_free_value(pair->value);
        free(pair->key);
        free(pair);
        return NULL;
    }
    return pair->value->value.t;
}

static cconfig_value_t* parse_value(cconfig_parser_t* parser) {
    cconfig_value_t* val = (cconfig_value_t*)malloc(sizeof(cconfig_value_t));
    val->type = CCONFIG_VALUE_NONE;
    if (!val) {
        parser_error(parser, "Out of memory.");
        return NULL;
    }

    switch (parser->current.type) {
        case CCONFIG_TOKEN_STRING: {
            val->type = CCONFIG_VALUE_STRING;
            val->value.s = process_string_literal(parser->current.start, parser->current.len);
            if (!val->value.s) {
                free(val);
                parser_error(parser, "Out of memory.");
                return NULL;
            }
            break;
        }
        case CCONFIG_TOKEN_INTEGER: {
            val->type = CCONFIG_VALUE_INTEGER;
            val->value.i = strtoll(parser->current.start, NULL, 10);
            break;
        }
        case CCONFIG_TOKEN_FLOAT: {
            val->type = CCONFIG_VALUE_FLOAT;
            val->value.d = strtod(parser->current.start, NULL);
            break;
        }
        case CCONFIG_TOKEN_BOOLEAN: {
            val->type = CCONFIG_VALUE_BOOLEAN;
            val->value.b = strncmp(parser->current.start, "true", 4) == 0;
            break;
        }
        case CCONFIG_TOKEN_LBRACKET: {
            advance_token(parser); // Consume '['
            val->type = CCONFIG_VALUE_ARRAY;
            val->value.a = (cconfig_array_t*)malloc(sizeof(cconfig_array_t));
            if (!val->value.a) {
                free(val);
                parser_error(parser, "Out of memory.");
                return NULL;
            }
            val->value.a->count = 0;
            val->value.a->values = NULL;
            val->value.a->capacity = 0;

            // Handle empty array
            if (parser->current.type == CCONFIG_TOKEN_RBRACKET) {
                advance_token(parser);
                return val;
            }

            while (parser->current.type != CCONFIG_TOKEN_RBRACKET && parser->current.type != CCONFIG_TOKEN_EOF) {
                cconfig_value_t* elem = parse_value(parser);
                if (!elem) { cconfig_free_value(val); return NULL; }

                if (!cconfig_array_add_value(val->value.a, elem)) {
                    cconfig_free_value(elem);
                    cconfig_free_value(val);
                    parser_error(parser, "Out of memory.");
                    return NULL;
                }

                // Skip optional trailing comma before ']'
                if (parser->current.type == CCONFIG_TOKEN_RBRACKET) {
                    break;
                }

                if (!consume_token(parser, CCONFIG_TOKEN_COMMA, "Expected ',' or ']' in array.")) {
                    // Error, cleanup and return
                    cconfig_free_value(val);
                    return NULL;
                }

                // After a comma, we MUST have another value. A closing bracket is invalid according to strict TOML.
                // We also need to skip past any comments or newlines before checking.
                while (parser->current.type == CCONFIG_TOKEN_COMMENT || parser->current.type == CCONFIG_TOKEN_NEWLINE) advance_token(parser);
                if (parser->current.type == CCONFIG_TOKEN_RBRACKET) {
                    // Use the standard message so callers/tests that look for this text match.
                    parser_error(parser, "Expected ']' to close array.");
                    cconfig_free_value(val);
                    return NULL;
                }
            }

            if (!consume_token(parser, CCONFIG_TOKEN_RBRACKET, "Expected ']' to close array.")) {
                cconfig_free_value(val);
                return NULL;
            }
            return val; // Return early, advance_token is handled by sub-calls
        }
        default: {
            free(val);
            return NULL;
        }
    }

    advance_token(parser);
    return val;
}

cconfig_token_t next_token(cconfig_lexer_t* lexer) {
    skip_whitespace(lexer);

    int start_pos = lexer->pos;
    int start_col = lexer->col;

    if (is_at_end(lexer)) return make_token(lexer, CCONFIG_TOKEN_EOF);

    char c = advance(lexer);

    if (isalpha(c) || c == '_') return identifier_token(lexer);
    if (isdigit(c) || c == '-' || c == '+') {
        bool is_float = false;
        // Check for datetime-like format first
        if (isdigit(lexer->input[start_pos]) && lexer->input[start_pos+1] && lexer->input[start_pos+2] && lexer->input[start_pos+3] && lexer->input[start_pos+4] == '-') {
             // Potentially a datetime, let's check further
             // This is a simplified check for YYYY-MM-DD
             if (isdigit(lexer->input[start_pos+5]) && isdigit(lexer->input[start_pos+6]) && lexer->input[start_pos+7] == '-' && isdigit(lexer->input[start_pos+8]) && isdigit(lexer->input[start_pos+9])) {
                 // For now, we only support date part.
                 lexer->pos = start_pos + 10;
                 // TODO: Parse full datetime with time and timezone
                 cconfig_token_t token = make_token(lexer, CCONFIG_TOKEN_DATETIME);
                 token.start = &lexer->input[start_pos];
                 token.len = lexer->pos - start_pos;
                 token.col = start_col;
                 return token;
             }
        }

        while (isdigit(peek(lexer))) advance(lexer);
        if (peek(lexer) == '.') {
            is_float = true;
            advance(lexer); // consume '.'
            while (isdigit(peek(lexer))) advance(lexer);
        }
        if (peek(lexer) == 'e' || peek(lexer) == 'E') {
            is_float = true;
            advance(lexer); // consume 'e' or 'E'
            if (peek(lexer) == '+' || peek(lexer) == '-') advance(lexer);
            while (isdigit(peek(lexer))) advance(lexer);
        }
        cconfig_token_t token = make_token(lexer, is_float ? CCONFIG_TOKEN_FLOAT : CCONFIG_TOKEN_INTEGER);
        token.start = &lexer->input[start_pos];
        token.len = lexer->pos - start_pos;
        token.col = start_col;
        return token;
    }

    cconfig_token_t token;
    switch (c) {
        case '=': token = make_token(lexer, CCONFIG_TOKEN_EQUAL); break;
        case '[': token = make_token(lexer, CCONFIG_TOKEN_LBRACKET); break;
        case ']': token = make_token(lexer, CCONFIG_TOKEN_RBRACKET); break;
        case '{': token = make_token(lexer, CCONFIG_TOKEN_LBRACE); break;
        case '}': token = make_token(lexer, CCONFIG_TOKEN_RBRACE); break;
        case ',': token = make_token(lexer, CCONFIG_TOKEN_COMMA); break;
        case '.': token = make_token(lexer, CCONFIG_TOKEN_DOT); break;
        case '\n':
            token = make_token(lexer, CCONFIG_TOKEN_NEWLINE);
            lexer->line++;
            lexer->col = 1;
            break;
        case '"': return string_token(lexer, '"');
        case '\'': return string_token(lexer, '\'');
        case '#':
            while (peek(lexer) != '\n' && !is_at_end(lexer)) {
                advance(lexer);
            }
            token = make_token(lexer, CCONFIG_TOKEN_COMMENT);
            break;
        default:
            return error_token(lexer, "Unexpected character.");
    }

    token.start = &lexer->input[start_pos];
    token.len = lexer->pos - start_pos;
    token.col = start_col;
    return token;
}


// --- Public API ---

// Main entry point for parsing
cconfig_document_t* cconfig_parse(const char* input, char* error_buffer, size_t error_buffer_size) {
    cconfig_parser_t parser = {
        .lexer = { .input = input, .pos = 0, .line = 1, .col = 1 },
        .error_buffer = error_buffer,
        .error_buffer_size = error_buffer_size,
        .panic_mode = false,
        .current_table = NULL
    };

    advance_token(&parser);

    cconfig_document_t* doc = (cconfig_document_t*)malloc(sizeof(cconfig_document_t));
    if (!doc) { parser_error(&parser, "Out of memory."); return NULL; }

    doc->root = (cconfig_table_t*)malloc(sizeof(cconfig_table_t));
    if (!doc->root) {
        free(doc);
        parser_error(&parser, "Out of memory.");
        return NULL;
    }

    doc->root->count = 0;
    doc->root->pairs = NULL;
    doc->root->capacity = 0;
    parser.current_table = doc->root;

    while (parser.current.type != CCONFIG_TOKEN_EOF) {
        if (parser.current.type == CCONFIG_TOKEN_NEWLINE || parser.current.type == CCONFIG_TOKEN_COMMENT) {
            advance_token(&parser);
            continue;
        }

        if (parser.current.type == CCONFIG_TOKEN_LBRACKET) {
            advance_token(&parser); // Consume '['

            bool is_array_of_tables = consume_token(&parser, CCONFIG_TOKEN_LBRACKET, NULL);

            if (is_array_of_tables) {
                // This is an array of tables [[...]]
                cconfig_table_t* parent_table = doc->root;
                cconfig_array_t* target_array = NULL;

                // Traverse dotted keys like [[a.b.c]]
                while (parser.current.type == CCONFIG_TOKEN_IDENT) {
                    const char* key = parser.current.start;
                    int key_len = parser.current.len;
                    advance_token(&parser);

                    if (parser.current.type == CCONFIG_TOKEN_DOT) {
                        // This is an intermediate table, e.g., 'a' in [[a.b]]
                        parent_table = find_or_create_table(parent_table, key, key_len);
                        if (!parent_table) { parser_error(&parser, "Failed to create table path."); cconfig_free(doc); return NULL; }
                        advance_token(&parser); // consume '.'
                    } else {
                        // This is the final key, which holds the array
                        target_array = find_or_create_array_of_tables(&parser, parent_table, key, key_len);
                        break;
                    }
                }

                if (!target_array) { parser_error(&parser, "Invalid array of tables declaration."); cconfig_free(doc); return NULL; }

                // Create the new table and add it to the array
                cconfig_table_t* new_table = (cconfig_table_t*)calloc(1, sizeof(cconfig_table_t));
                if (!new_table) { parser_error(&parser, "Out of memory."); cconfig_free(doc); return NULL; }

                cconfig_value_t* new_table_value = (cconfig_value_t*)malloc(sizeof(cconfig_value_t));
                if (!new_table_value) { free(new_table); parser_error(&parser, "Out of memory."); cconfig_free(doc); return NULL; }

                new_table_value->type = CCONFIG_VALUE_TABLE;
                new_table_value->value.t = new_table;

                if (!cconfig_array_add_value(target_array, new_table_value)) {
                    cconfig_free_value(new_table_value);
                    parser_error(&parser, "Out of memory.");
                    cconfig_free(doc);
                    return NULL;
                }
                parser.current_table = new_table;

                if (!consume_token(&parser, CCONFIG_TOKEN_RBRACKET, "Expected ']]' to close array of tables.")) { cconfig_free(doc); return NULL; }
                if (!consume_token(&parser, CCONFIG_TOKEN_RBRACKET, "Expected ']]' to close array of tables.")) { cconfig_free(doc); return NULL; }

            } else {
                // This is a standard table [...]
                cconfig_table_t* table = doc->root;
                while (parser.current.type == CCONFIG_TOKEN_IDENT) {
                    table = find_or_create_table(table, parser.current.start, parser.current.len);
                    if (!table) { parser_error(&parser, "Failed to create or find table."); cconfig_free(doc); return NULL; }
                    advance_token(&parser);
                    if (parser.current.type == CCONFIG_TOKEN_DOT) {
                        advance_token(&parser); // Consume '.'
                    } else {
                        break;
                    }
                }
                parser.current_table = table;

                if (!consume_token(&parser, CCONFIG_TOKEN_RBRACKET, "Expected ']' after table name.")) {
                    cconfig_free(doc);
                    return NULL;
                }
            }
            continue;
        }

        cconfig_key_value_t* pair = parse_key_value(&parser);
        if (pair) {
            // The pair is already added to the correct table inside parse_key_value.
            // We just need to know it was successful. A non-NULL return indicates success.
            // After a key-value pair, we expect a newline or EOF.
            if (parser.current.type != CCONFIG_TOKEN_NEWLINE && parser.current.type != CCONFIG_TOKEN_EOF && parser.current.type != CCONFIG_TOKEN_COMMENT) {
                parser_error(&parser, "Unexpected token after key-value pair.");
                advance_token(&parser);
            }
        } else {
            // If we failed to parse a key-value pair, it's a syntax error.
            // Report it and advance to avoid an infinite loop.
            parser_error(&parser, "Invalid syntax. Expected key-value pair or table definition.");
            advance_token(&parser);
        }
    }

    // If we hit a parse error, free the partially constructed document and return NULL.
    if (parser.panic_mode) {
        cconfig_free(doc);
        return NULL;
    }

    return doc;
}

// Free the memory allocated for the TOML document
static void cconfig_free_value(cconfig_value_t* value) {
    if (!value) return;

    switch (value->type) {
        case CCONFIG_VALUE_STRING:
            free(value->value.s);
            break;
        case CCONFIG_VALUE_ARRAY: {
            for (size_t i = 0; i < value->value.a->count; i++) {
                cconfig_free_value(value->value.a->values[i]);
            }
            free(value->value.a->values);
            free(value->value.a);
            break;
        }
        case CCONFIG_VALUE_TABLE:
            cconfig_free_table(value->value.t);
            break;
        default:
            // No dynamic allocation for other types
            break;
    }
    free(value);
}

static void cconfig_free_table(cconfig_table_t* table) {
    if (!table) return;
    for (size_t i = 0; i < table->count; i++) {
        cconfig_key_value_t* pair = table->pairs[i];
        free(pair->key);
        cconfig_free_value(pair->value);
        free(pair);
    }
    free(table->pairs);
    free(table);
}

void cconfig_free(cconfig_document_t* doc) {
    if (doc) {
        cconfig_free_table(doc->root);
        free(doc);
    }
}

// --- Convenience Getters ---

static const cconfig_value_t* cconfig_get_value_from_table(const cconfig_table_t* table, const char* key) {
    if (!table) return NULL;
    for (size_t i = 0; i < table->count; i++) {
        if (strcmp(table->pairs[i]->key, key) == 0) {
            return table->pairs[i]->value;
        }
    }
    return NULL;
}

static const cconfig_value_t* cconfig_get_value(const cconfig_document_t* doc, const char* key_path) {
    if (!doc || !doc->root || !key_path) return NULL;
    
    const cconfig_value_t* current_value = &(cconfig_value_t){ .type = CCONFIG_VALUE_TABLE, .value.t = doc->root };
    const char* p = key_path;

    while (p && *p) {
        if (current_value == NULL) return NULL;

        if (*p == '[') { // Array access
            if (current_value->type != CCONFIG_VALUE_ARRAY) return NULL;

            p++; // Skip '['
            char* end;
            long index = strtol(p, &end, 10);
            if (p == end || *end != ']') return NULL; // Invalid index
            p = end + 1;

            if (index < 0 || (size_t)index >= current_value->value.a->count) return NULL;
            current_value = current_value->value.a->values[index];

        } else { // Table access
            if (current_value->type != CCONFIG_VALUE_TABLE) return NULL;

            const char* key_start = p;
            const char* key_end = strpbrk(p, ".[");
            size_t key_len;

            if (key_end) {
                key_len = key_end - key_start;
                p = key_end;
            } else {
                key_len = strlen(key_start);
                p = NULL;
            }

            char key[256];
            if (key_len >= sizeof(key)) return NULL; // Key too long
            memcpy(key, key_start, key_len);
            key[key_len] = '\0';

            current_value = cconfig_get_value_from_table(current_value->value.t, key);
        }

        // Skip dot separator
        if (p && *p == '.') {
            p++;
        }
    }

    return current_value;
}

cconfig_table_t* cconfig_get_table(const cconfig_document_t* doc, const char* key_path) {
    const cconfig_value_t* value = cconfig_get_value(doc, key_path);
    if (value && value->type == CCONFIG_VALUE_TABLE) {
        return value->value.t;
    }
    return NULL;
}

cconfig_array_t* cconfig_get_array(const cconfig_document_t* doc, const char* key_path) {
    const cconfig_value_t* value = cconfig_get_value(doc, key_path);
    if (value && value->type == CCONFIG_VALUE_ARRAY) {
        return value->value.a;
    }
    return NULL;
}

const char* cconfig_get_string(const cconfig_document_t* doc, const char* key_path, const char* default_val) {
    const cconfig_value_t* value = cconfig_get_value(doc, key_path);
    return (value && value->type == CCONFIG_VALUE_STRING) ? value->value.s : default_val;
}

int64_t cconfig_get_int(const cconfig_document_t* doc, const char* key_path, int64_t default_val) {
    const cconfig_value_t* value = cconfig_get_value(doc, key_path);
    return (value && value->type == CCONFIG_VALUE_INTEGER) ? value->value.i : default_val;
}

double cconfig_get_double(const cconfig_document_t* doc, const char* key_path, double default_val) {
    const cconfig_value_t* value = cconfig_get_value(doc, key_path);
    if (value && value->type == CCONFIG_VALUE_FLOAT) return value->value.d;
    if (value && value->type == CCONFIG_VALUE_INTEGER) return (double)value->value.i; // Promote int to double
    return default_val;
}

bool cconfig_get_bool(const cconfig_document_t* doc, const char* key_path, bool default_val) {
    const cconfig_value_t* value = cconfig_get_value(doc, key_path);
    return (value && value->type == CCONFIG_VALUE_BOOLEAN) ? value->value.b : default_val;
}

// --- Serializer ---

static void cconfig_serialize_value(cconfig_string_buffer_t* sb, const cconfig_value_t* value) {
    switch (value->type) {
        case CCONFIG_VALUE_STRING:
            // For now, always serialize as a basic string.
            // A more advanced serializer could choose literal strings if no escapes are needed.
            sb_appendf(sb, "\"");
            for (const char* p = value->value.s; *p; p++) {
                if (*p == '"' || *p == '\\') sb_appendf(sb, "\\");
                sb_appendf(sb, "%c", *p);
            }
            sb_appendf(sb, "\"");
            break;
        case CCONFIG_VALUE_INTEGER:
            sb_appendf(sb, "%lld", (long long)value->value.i);
            break;
        case CCONFIG_VALUE_FLOAT:
            sb_appendf(sb, "%g", value->value.d);
            break;
        case CCONFIG_VALUE_BOOLEAN:
            sb_appendf(sb, "%s", value->value.b ? "true" : "false");
            break;
        case CCONFIG_VALUE_ARRAY: {
            sb_appendf(sb, "[ ");
            for (size_t i = 0; i < value->value.a->count; i++) {
                cconfig_serialize_value(sb, value->value.a->values[i]);
                if (i < value->value.a->count - 1) {
                    sb_appendf(sb, ", ");
                }
            }
            sb_appendf(sb, " ]");
            break;
        }
        default: // Tables are handled separately
            break;
    }
}

static void cconfig_serialize_table_contents(cconfig_string_buffer_t* sb, const cconfig_table_t* table, const char* table_path) {
    bool wrote_header = false;
    char new_path[256];

    // First, print all simple key-value pairs in the current table
    for (size_t i = 0; i < table->count; i++) {
        cconfig_key_value_t* pair = table->pairs[i];
        if (pair->value->type != CCONFIG_VALUE_TABLE && pair->value->type != CCONFIG_VALUE_ARRAY) {
            if (!wrote_header && table_path && table_path[0] != '\0') {
                sb_appendf(sb, "[%s]\n", table_path);
                wrote_header = true;
            }
            sb_appendf(sb, "%s = ", pair->key);
            cconfig_serialize_value(sb, pair->value);
            sb_appendf(sb, "\n");
        }
    }
    if (wrote_header) sb_appendf(sb, "\n");

    // Then, recursively serialize sub-tables and arrays of tables
    for (size_t i = 0; i < table->count; i++) {
        cconfig_key_value_t* pair = table->pairs[i];
        snprintf(new_path, sizeof(new_path), "%s%s%s", table_path ? table_path : "", table_path ? "." : "", pair->key);

        if (pair->value->type == CCONFIG_VALUE_TABLE) {
            cconfig_serialize_table_contents(sb, pair->value->value.t, new_path);
        } else if (pair->value->type == CCONFIG_VALUE_ARRAY && pair->value->value.a->count > 0 && pair->value->value.a->values[0]->type == CCONFIG_VALUE_TABLE) {
            // Array of tables
            for (size_t j = 0; j < pair->value->value.a->count; j++) {
                sb_appendf(sb, "[[%s]]\n", new_path);
                cconfig_serialize_table_contents(sb, pair->value->value.a->values[j]->value.t, NULL);
                sb_appendf(sb, "\n");
            }
        }
    }
}

char* cconfig_serialize(const cconfig_document_t* doc) {
    if (!doc) return NULL;
    cconfig_string_buffer_t sb;
    sb_init(&sb);
    if (!sb.buffer) return NULL; // OOM
    cconfig_serialize_table_contents(&sb, doc->root, "");
    return sb.buffer;
}

// --- INI Parser Implementation ---

// Helper to trim leading/trailing whitespace from a string in-place
static char* trim_whitespace(char* str) {
    char* end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end + 1) = 0;

    return str;
}

int cconfig_ini_parse(const char* ini_string, cconfig_ini_handler handler, void* userdata) {
    if (!ini_string || !handler) {
        return -1;
    }

    char* buffer = cconfig_strdup(ini_string);
    if (!buffer) {
        return -1; // Out of memory
    }

    char section[128] = "";
    char* line = strtok(buffer, "\n\r");
    int line_num = 0;

    while (line != NULL) {
        line_num++;
        char* trimmed_line = trim_whitespace(line);

        if (trimmed_line[0] == '#' || trimmed_line[0] == ';') {
            // Comment line
        } else if (trimmed_line[0] == '[') {
            // Section line
            char* end = strchr(trimmed_line, ']');
            if (end) {
                *end = '\0';
                strncpy(section, trimmed_line + 1, sizeof(section) - 1);
                section[sizeof(section) - 1] = '\0';
                trim_whitespace(section);
            }
        } else if (strchr(trimmed_line, '=')) {
            // Key-value pair
            char* key = trimmed_line;
            char* value = strchr(trimmed_line, '=');
            *value = '\0'; // Split the string
            value++;

            key = trim_whitespace(key);
            value = trim_whitespace(value);

            if (handler(userdata, section, key, value) != 0) {
                break; // Handler requested to stop
            }
        }

        line = strtok(NULL, "\n\r");
    }

    free(buffer);
    return 0;
}

#endif // CCONFIG_IMPLEMENTATION
