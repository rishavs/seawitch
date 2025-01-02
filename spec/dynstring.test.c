#include <stdio.h>
#include <stdbool.h>

#include "testing.h"
#include "dynstring.h"
#include "seawitch.h"


// // Create a new dynamic string
// Error dynstring_do_create(DynString* str, char* data);

// // append a fixed-string or char* to a string
// Error dynstring_do_push_cstr(DynString* src, char* data);

// // Get a substring from a string, given a start and end position
// Error dynstring_do_slice (DynString* src, DynString* result, Int64 start, Int64 end);

// // Join multiple strings into a single string
// Error dynstring_do_join(DynString* result, Int64 n, ...);

// // Compare two strings
// Error dynstring_do_compare(DynString* str1, DynString* str2);

// // Check if a string starts with a fragment
// Error dynstring_do_substring_at(DynString* src, Int64 pos, DynString* frag);

// // Find a fragment in a string
// Error dynstring_do_find(DynString* src, Int64* result_at, DynString* frag);

// // Print string
// Error dynstring_do_print(DynString* str, bool print_newline);


// create a new string and check length
Error create_basic_dynstring() {
    DynString* str = dynstring_create();
    if (str == NULL || str->data == NULL) return snitch ("Failed to create new string", __LINE__, __FILE__);

    // Check for length
    size_t str_size = strlen(str->data);
    Int64 len_via_strlen = (Int64)str_size;
    if (str->len != 0 || str->len != len_via_strlen) {
        return snitch ("Length of new string is wrong", __LINE__, __FILE__);
    }

    Int64 len_via_loop = 0;
    while (str->data[len_via_loop] != '\0') {
        len_via_loop++;
    }
    if (len_via_loop != 0) {
        return snitch ("Length of new string is wrong", __LINE__, __FILE__);
    }

    return (Error){ .ok = true };
}

Error push_chars_to_string() {
    DynString* str = dynstring_create();
    if (str == NULL || str->data == NULL) return snitch ("Failed to create new string", __LINE__, __FILE__);

    // Push a c-string
    Error err = dynstring_push_chars(str, "Hello, ");
    if (!err.ok) return err;

    // Push a char
    err = dynstring_push_chars(str, (char[]){'W', '\0'});
    if (!err.ok) return err;

    // Push the rest of the string
    err = dynstring_push_chars(str, "orld!");
    if (!err.ok) return err;

    // Check for length
    if (str->len == 13) {
        printf("%s\n", str->data);
        return (Error){ .ok = true };
    } else {
        return snitch ("Length of new string is wrong", __LINE__, __FILE__);
    }
}

// Test_Result push_str_chars_to_string() {
//     Test_Result res = { 
//         .desc = dynstring_create("Dynstring: can push cstr and chars to string"),
//         .passed = false 
//     };

    
//     DynString* str = calloc(1, sizeof(DynString));
//     Error err = dynstring_do_create(str, "Hell");
//     if (!err.ok) return err;

//     // Push a c-string
//     err = dynstring_push_cstr(str, "o, ");
//     if (!err.ok) return err;

//     // Push a char
//     err = dynstring_push_cstr(str, (char[]){'W', '\0'});
//     if (!err.ok) return err;

//     // Push the rest of the string
//     err = dynstring_push_cstr(str, "orld!");
//     if (!err.ok) return err;

//     // Check for length
//     if (str->len == 13) {
//         res.passed = true;
//         printf("%s\n", str->data);
//     } else {
//         res.passed = false;
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