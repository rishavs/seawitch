#include <stdio.h>
#include <stdbool.h>

#include "testing.h"
#include "dynstring.h"
#include "seawitch.h"


// // Create a new dynamic string
// Outcome dynstring_do_create(DynString* str, char* data);

// // append a fixed-string or char* to a string
// Outcome dynstring_do_push_cstr(DynString* src, char* data);

// // Get a substring from a string, given a start and end position
// Outcome dynstring_do_slice (DynString* src, DynString* result, Int64 start, Int64 end);

// // Join multiple strings into a single string
// Outcome dynstring_do_join(DynString* result, Int64 n, ...);

// // Compare two strings
// Outcome dynstring_do_compare(DynString* str1, DynString* str2);

// // Check if a string starts with a fragment
// Outcome dynstring_do_substring_at(DynString* src, Int64 pos, DynString* frag);

// // Find a fragment in a string
// Outcome dynstring_do_find(DynString* src, Int64* result_at, DynString* frag);

// // Print string
// Outcome dynstring_do_print(DynString* str, bool print_newline);


// create a new string and check length
Test_Result create_basic_dynstring() {
    Test_Result res;
    res.desc = calloc(1, sizeof(DynString));
    if (dynstring_do_create(res.desc, "Hello, World!") != OUT_OK) {
        yell_at_clouds(OUT_MEMORY_ERROR, __LINE__, __FILE__);
        return res;
    }
    res.passed = false;

    DynString* str = calloc(1, sizeof(DynString));
    if (dynstring_do_create(str, "Hello, World!") != OUT_OK) {
        yell_at_clouds(OUT_MEMORY_ERROR, __LINE__, __FILE__);
        return res;
    }

    // Check for length
    size_t str_size = strlen(str->data);
    if (str_size > INT_MAX) yell_at_clouds(OUT_INTEGER_OVERFLOW, __LINE__, __FILE__);
    Int64 len_via_strlen = (Int64)str_size;

    Int64 len_via_loop = 0;
    while (str->data[len_via_loop] != '\0') {
        len_via_loop++;
    }
    if (str->len == len_via_loop && str->len == len_via_strlen && str->len == 13) {
        res.passed = true;
    } else {
        fprintf(stderr, "Length mismatch: %lli != %lli != %lli \n", str->len, len_via_strlen, len_via_loop);
    }

    return res;
}


// Test_Result push_str_chars_to_string() {
//     Test_Result res = { 
//         .desc = dynstring_create("can push cstr and chars to string"),
//         .passed = false 
//     };

//     String* str = dynstring_create("Hell");
//     if (!str) {
//         return res;
//     }

//     // Push a c-string
//     str = dynstring_push_cstr(str, "o, ");
//     if (!str) return res;

//     // Push a char
//     str = dynstring_push_cstr(str, (char[]){'W', '\0'});
//     if (!str) return res;

//     // Push the rest of the string
//     str = dynstring_push_cstr(str, "orld!");
//     if (!str) return res;

//     // Check for length
//     Int64 j = 0;
//     while (str->data[j] != '\0') {
//         j++;
//     }
//     if (j == str->len && j == 13) {
//         res.passed = true;
//         printf("%s\n", str->data);
//     } else {
//         fprintf(stderr, "Length mismatch: %lli != %lli\n", j, str->len);
//     }

//     return res;
// }

// Test_Result get_substring() {
//     Test_Result res = { 
//         .desc = dynstring_create("can get substring from string"),
//         .passed = false 
//     };

//     String* str = dynstring_create("Hello, World!");
//     if (!str) {
//         return res;
//     }

//     // Get substring
//     String* sub = dynstring_get_substring(str, 7, 5);
//     if (!sub) return res;

//     // Check for length
//     Int64 j = 0;
//     while (sub->data[j] != '\0') {
//         j++;
//     }
//     if (j == sub->len && j == 5) {
//         res.passed = true;
//         printf("%s\n", sub->data);
//     } else {
//         fprintf(stderr, "Length mismatch: %lli != %lli\n", j, sub->len);
//     }

//     String* all_string = dynstring_get_substring(str, 0, str->len);
//     if (!all_string) return res;

//     // Check for length
//     j = 0;
//     while (all_string->data[j] != '\0') {
//         j++;
//     }
//     if (j == all_string->len && j == 13) {
//         res.passed = true;
//         printf("%s\n", all_string->data);
//     } else {
//         fprintf(stderr, "Length mismatch: %lli != %lli\n", j, all_string->len);
//     }

//     String* empty_string = dynstring_get_substring(str, 0, 0);
//     if (!empty_string) return res;
    
//     // Check for length
//     j = 0;
//     while (empty_string->data[j] != '\0') {
//         j++;
//     }
//     if (j == empty_string->len && j == 0) {
//         res.passed = true;
//         printf("%s\n", empty_string->data);
//     } else {
//         fprintf(stderr, "Length mismatch: %lli != %lli\n", j, empty_string->len);
//     }

//     String* out_of_bounds = dynstring_get_substring(str, 0, 100);
//     if (out_of_bounds) {
//         fprintf(stderr, "Out of bounds error not caught\n");
//     } else {
//         res.passed = true;
//     }



//     return res;
// }