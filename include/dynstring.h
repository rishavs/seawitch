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

// Create a new dynamic string
Error dynstring_do_create(DynString* str, const char* data);

// append a fixed-string or char* to a string
Error dynstring_do_push_cstr(DynString* src, char* data);

// Get a substring from a string, given a start and end position
Error dynstring_do_slice (DynString* src, DynString* result, Int64 start, Int64 end);

// Join multiple strings into a single string
Error dynstring_do_join(DynString* result, Int64 n, ...);

// Compare two strings
Error dynstring_do_compare(DynString* str1, DynString* str2);

// Check if a string starts with a fragment
Error dynstring_do_substring_at(DynString* src, Int64 pos, DynString* frag);

// Find a fragment in a string
Error dynstring_do_find(DynString* src, Int64* result_at, DynString* frag);

// Print string
Error dynstring_do_print(DynString* str, bool print_newline);

#endif // SEAWITCH_STRING_H