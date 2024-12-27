#include <stdio.h>
#include <stdbool.h>

#include "testing.h"
#include "dynstring.h"
#include "seawitch.h"


// // Create a new dynamic string
// String* string_create(const char* data);

// // Get a substring from a string, given a position and length
// String* string_get_substring(String* src, Int64 pos, Int64 len);

// // append a c-string/char* to a string
// String* string_push_cstr(String* src, const char* data);

// // Join multiple strings into a single string
// String* string_join(Int64 n, ...);

// // Compare two strings
// Bool string_compare(String* str1, String* str2);

// // Check if a string starts with a fragment
// Bool string_starts_with(String* src, Int64 pos, String* frag);

// Null input
Test_Result basic_string_create() {
    Test_Result res = { 
        .desc = string_create("can create basic string"),
        .passed = false 
    };

    String* str = string_create("Hello, World!");
    if (str) {
        printf("\n");
        for (Int64 i = 0; i < str->len; i++) {
            printf("%c ", str->data[i]);
        }
        printf("\n");

        res.passed = true;
    }

    // Check for length
    Int64 j = 0;
    while (str->data[j] != '\0') {
        j++;
    }
    if (j == str->len) {
        res.passed = true;
    } else {
        fprintf(stderr, "Length mismatch: %lli != %lli\n", j, str->len);
    }

    return res;
}


Test_Result push_str_chars_to_string() {
    Test_Result res = { 
        .desc = string_create("can push cstr and chars to string"),
        .passed = false 
    };

    String* str = string_create("Hell");
    if (!str) {
        return res;
    }

    // Push a c-string
    str = string_push_cstr(str, "o, ");
    if (!str) return res;

    // Push a char
    str = string_push_cstr(str, (char[]){'W', '\0'});
    if (!str) return res;

    // Push the rest of the string
    str = string_push_cstr(str, "orld!");
    if (!str) return res;

    // Check for length
    Int64 j = 0;
    while (str->data[j] != '\0') {
        j++;
    }
    if (j == str->len && j == 13) {
        res.passed = true;
        printf("%s\n", str->data);
    } else {
        fprintf(stderr, "Length mismatch: %lli != %lli\n", j, str->len);
    }

    return res;
}

Test_Result get_substring() {
    Test_Result res = { 
        .desc = string_create("can get substring from string"),
        .passed = false 
    };

    String* str = string_create("Hello, World!");
    if (!str) {
        return res;
    }

    // Get substring
    String* sub = string_get_substring(str, 7, 5);
    if (!sub) return res;

    // Check for length
    Int64 j = 0;
    while (sub->data[j] != '\0') {
        j++;
    }
    if (j == sub->len && j == 5) {
        res.passed = true;
        printf("%s\n", sub->data);
    } else {
        fprintf(stderr, "Length mismatch: %lli != %lli\n", j, sub->len);
    }

    String* all_string = string_get_substring(str, 0, str->len);
    if (!all_string) return res;

    // Check for length
    j = 0;
    while (all_string->data[j] != '\0') {
        j++;
    }
    if (j == all_string->len && j == 13) {
        res.passed = true;
        printf("%s\n", all_string->data);
    } else {
        fprintf(stderr, "Length mismatch: %lli != %lli\n", j, all_string->len);
    }

    String* empty_string = string_get_substring(str, 0, 0);
    if (!empty_string) return res;
    
    // Check for length
    j = 0;
    while (empty_string->data[j] != '\0') {
        j++;
    }
    if (j == empty_string->len && j == 0) {
        res.passed = true;
        printf("%s\n", empty_string->data);
    } else {
        fprintf(stderr, "Length mismatch: %lli != %lli\n", j, empty_string->len);
    }

    String* out_of_bounds = string_get_substring(str, 0, 100);
    if (out_of_bounds) {
        fprintf(stderr, "Out of bounds error not caught\n");
    } else {
        res.passed = true;
    }



    return res;
}