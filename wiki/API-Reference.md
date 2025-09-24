# API Reference

This page documents the public API for `cconfig`.

## Data Structures

`cconfig` uses several data structures to represent TOML data. These are the most important ones:

*   `cconfig_document_t`: Represents a full TOML document.
*   `cconfig_table_t`: Represents a TOML table.
*   `cconfig_array_t`: Represents a TOML array.
*   `cconfig_value_t`: Represents a single value.

## Functions

### `cconfig_parse`

```c
cconfig_document_t* cconfig_parse(const char* input, char* error_buffer, size_t error_buffer_size);
```

Parses a TOML string and builds a document object model (DOM).

*   `input`: The null-terminated string containing the TOML data.
*   `error_buffer`: A buffer to write a descriptive error message to if parsing fails.
*   `error_buffer_size`: The size of the `error_buffer`.

Returns a pointer to a new `cconfig_document_t` on success, or `NULL` on failure. The returned document must be freed with `cconfig_free()`.

### `cconfig_serialize`

```c
char* cconfig_serialize(const cconfig_document_t* doc);
```

Serializes a TOML document back into a formatted string.

*   `doc`: The document to serialize.

Returns a newly allocated string containing the TOML representation. The caller is responsible for freeing this string. Returns `NULL` if the document is `NULL`.

### `cconfig_free`

```c
void cconfig_free(cconfig_document_t* doc);
```

Frees all memory associated with a TOML document.

*   `doc`: The document to free.

### `cconfig_get_table`

```c
cconfig_table_t* cconfig_get_table(const cconfig_document_t* doc, const char* key_path);
```

Retrieves a table from the document using a dot-separated key path.

*   `doc`: The parsed TOML document.
*   `key_path`: The dot-separated path to the table (e.g., `"database.credentials"`).

Returns a pointer to the `cconfig_table_t` if found, otherwise `NULL`.

### `cconfig_get_array`

```c
cconfig_array_t* cconfig_get_array(const cconfig_document_t* doc, const char* key_path);
```

Retrieves an array from the document using a dot-separated key path.

*   `doc`: The parsed TOML document.
*   `key_path`: The dot-separated path to the array (e.g., `"database.ports"`).

Returns a pointer to the `cconfig_array_t` if found, otherwise `NULL`.

### `cconfig_get_string`

```c
const char* cconfig_get_string(const cconfig_document_t* doc, const char* key_path, const char* default_val);
```

Retrieves a string value from the document.

*   `doc`: The parsed TOML document.
*   `key_path`: The dot-separated path to the key.
*   `default_val`: The value to return if the key is not found or is not a string.

Returns the string value if found, otherwise `default_val`.

### `cconfig_get_int`

```c
int64_t cconfig_get_int(const cconfig_document_t* doc, const char* key_path, int64_t default_val);
```

Retrieves an integer value from the document.

*   `doc`: The parsed TOML document.
*   `key_path`: The dot-separated path to the key.
*   `default_val`: The value to return if the key is not found or is not an integer.

Returns the integer value if found, otherwise `default_val`.

### `cconfig_get_double`

```c
double cconfig_get_double(const cconfig_document_t* doc, const char* key_path, double default_val);
```

Retrieves a double/float value from the document.

*   `doc`: The parsed TOML document.
*   `key_path`: The dot-separated path to the key.
*   `default_val`: The value to return if the key is not found or is not a number.

Returns the double value if found, otherwise `default_val`. If the key holds an integer, it will be promoted to a double.

### `cconfig_get_bool`

```c
bool cconfig_get_bool(const cconfig_document_t* doc, const char* key_path, bool default_val);
```

Retrieves a boolean value from the document.

*   `doc`: The parsed TOML document.
*   `key_path`: The dot-separated path to the key.
*   `default_val`: The value to return if the key is not found or is not a boolean.

Returns the boolean value if found, otherwise `default_val`.

### `cconfig_ini_parse`

```c
int cconfig_ini_parse(const char* ini_string, cconfig_ini_handler handler, void* userdata);
```

Parses an INI-formatted string using a callback-based approach.

*   `ini_string`: The null-terminated string containing the INI data.
*   `handler`: The callback function to be invoked for each key-value pair.
*   `userdata`: A user-defined pointer that will be passed to the handler function.

Returns 0 on successful completion, -1 on argument or memory error.
