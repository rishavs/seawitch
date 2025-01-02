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
Error dynstring_slice (DynString* src, DynString* result, Int64 start, Int64 end);

// Join multiple strings into a single string
Error dynstring_join(DynString* result, Int64 n, ...);

// Compare two strings
Error dynstring_compare(DynString* str1, DynString* str2, Bool* result);

// Check if a string starts with a fragment
Error dynstring_substring_at(DynString* src, DynString* frag, Int64 pos, Bool* result);

// Find a fragment in a string
// Error dynstring_find(DynString* src, Int64* result_at, DynString* frag);

// Print string
// Error dynstring_print(DynString* str, bool print_newline);

#endif // SEAWITCH_STRING_H