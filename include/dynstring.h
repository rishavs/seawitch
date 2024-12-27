#ifndef SEAWITCH_STRING_H
#define SEAWITCH_STRING_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>

#include "seawitch.h"

// Create a new dynamic string
String* string_create(const char* data);

// Get a substring from a string, given a position and length
String* string_get_substring(String* src, Int64 pos, Int64 len);

// append a c-string/char* to a string
String* string_push_cstr(String* src, const char* data);

// Join multiple strings into a single string
String* string_join(Int64 n, ...);

// Compare two strings
Bool string_compare(String* str1, String* str2);

// Check if a string starts with a fragment
Bool string_starts_with(String* src, Int64 pos, String* frag);

#endif // SEAWITCH_STRING_H