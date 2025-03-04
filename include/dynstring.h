#ifndef SEAWITCH_STRING_H
#define SEAWITCH_STRING_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>

#include "seawitch.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#include "errors.h"
#include "seawitch.h"

// Create an empty dynamic string
DynString* dynstring_create();

// append a fixed-string or char* to a string
Error dynstring_push_chars(DynString* src, char* data);

// Get a substring from a string, given a start and end position
Error dynstring_slice (DynString* src, DynString* result, int64_t start, int64_t end);

// Join multiple strings into a single string
Error dynstring_join(DynString* result, int64_t n, ...);

// Compare two strings
Error dynstring_compare(DynString* str1, DynString* str2, Bool* result);

// Check if a string starts with a fragment
Error dynstring_substring_at(DynString* src, DynString* frag, int64_t pos, Bool* result);

// Find a fragment in a string
// Error dynstring_find(DynString* src, int64_t* result_at, DynString* frag);

// Print string
// Error dynstring_print(DynString* str, bool print_newline);

#endif // SEAWITCH_STRING_H