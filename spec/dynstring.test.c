#include <stdio.h>
#include <stdbool.h>

// #include "testing.h"
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
Error create_basic_dynstring(FxString* desc) {
    *desc = fxstring_create("Dynstring: can create a new dynamic string");

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

Error push_chars_to_string(FxString *desc) {
    *desc = fxstring_create("Dynstring: can push cstr and chars to string");

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
    if (str->len != 13) {
        return snitch ("Length of new string is wrong", __LINE__, __FILE__);
    }

    return (Error){ .ok = true };
}

Error slice_string (FxString *desc) {
    *desc = fxstring_create("Dynstring: can slice a substring from a source string");
    
    DynString* str = dynstring_create();
    Error err = dynstring_push_chars(str, "Hello, World!");
    if (!err.ok) return err;

    // Get substring
    DynString* sub = dynstring_create();
    err = dynstring_slice(str, sub, 7, 11);
    if (!err.ok) return err;

    // Check for length
    if (sub->len != 5) {
        return snitch ("Length of substring is wrong", __LINE__, __FILE__);
    }

    // Check for content
    if (strncmp(sub->data, "World", 5) != 0) {
        return snitch ("Content of substring is wrong", __LINE__, __FILE__);
    }

    return (Error){ .ok = true };
}

Error check_user_inputs_for_slicing_string(FxString* desc ) {
    *desc = fxstring_create("Dynstring: can handle invalid user inputs for slicing string");
    
    DynString* str = dynstring_create();
    Error err = dynstring_push_chars(str, "Hello, World!");
    if (!err.ok) return err;

    // Get substring
    DynString* sub = dynstring_create();

    // Give invalid inputs
    err = dynstring_slice(str, sub, -1, 10);
    if (err.ok) return snitch ("Negative start index not caught", __LINE__, __FILE__);

    err = dynstring_slice(str, sub, 7, 100);
    if (err.ok) return snitch ("End index out of bounds not caught", __LINE__, __FILE__);

    err = dynstring_slice(str, sub, 7, 6);
    if (err.ok) return snitch ("Start index greater than end index not caught", __LINE__, __FILE__);

    // Check for length
    if (sub->len != 0 || str->len != 13) {
        return snitch ("Length of substring after failed slicing is wrong", __LINE__, __FILE__);
    }

    return (Error){ .ok = true };
}

Error join_multiple_strings(FxString* desc) {
    *desc = fxstring_create("Dynstring: can join multiple strings into a single string");
    
    DynString* str1 = dynstring_create();
    Error err = dynstring_push_chars(str1, "Hello, ");
    if (!err.ok) return err;

    DynString* str2 = dynstring_create();
    err = dynstring_push_chars(str2, "World!");
    if (!err.ok) return err;

    // Join strings
    DynString* result = dynstring_create();
    err = dynstring_join(result, 2, str1, str2);
    if (!err.ok) return err;

    // Check for length
    if (result->len != 13) {
        return snitch ("Length of joined string is wrong", __LINE__, __FILE__);
    }

    // Check for content
    if (strncmp(result->data, "Hello, World!", 13) != 0) {
        return snitch ("Content of joined string is wrong", __LINE__, __FILE__);
    }

    return (Error){ .ok = true };

}

Error join_string_with_empty_string(FxString* desc) {
    *desc = fxstring_create("Dynstring: can join a string with an empty string");

    DynString* content_string = dynstring_create();
    Error err = dynstring_push_chars(content_string, "Hello");
    if (!err.ok) return err;

    DynString* empty_str1 = dynstring_create();
    DynString* empty_str2 = dynstring_create();

    // Join 2 empty strings
    DynString* res1 = dynstring_create();
    err = dynstring_join(res1, 2, empty_str1, empty_str2);
    if (!err.ok) return err;

    // Check for length
    if (res1->len != 0 || empty_str1->len != 0 || empty_str2->len != 0) {
        return snitch ("Length of joined string is wrong", __LINE__, __FILE__);
    }

    // Check for content
    if (strncmp(res1->data, "", 0) != 0) {
        return snitch ("Content of joined string is wrong", __LINE__, __FILE__);
    }

    // now join 2 strings where 1st string is empty
    DynString* res2 = dynstring_create();
    err = dynstring_join(res2, 2, empty_str1, content_string);
    if (!err.ok) return err;

    // Check for length
    if (res2->len != 5) {
        return snitch ("Length of joined string is wrong", __LINE__, __FILE__);
    }
    // Check for content
    if (strncmp(res2->data, "Hello", 5) != 0) {
        return snitch ("Content of joined string is wrong", __LINE__, __FILE__);
    }

    // now join 2 strings where 2nd string is empty
    DynString* res3 = dynstring_create();
    err = dynstring_join(res3, 2, content_string, empty_str1);
    if (!err.ok) return err;

    // Check for length
    if (res3->len != 5) {
        return snitch ("Length of joined string is wrong", __LINE__, __FILE__);
    }

    // Check for content
    if (strncmp(res3->data, "Hello", 7) != 0) {
        return snitch ("Content of joined string is wrong", __LINE__, __FILE__);
    }

    return (Error){ .ok = true };
}

Error join_single_string(FxString* desc) {
    *desc = fxstring_create("Dynstring: can join a single string into a clone string");
    
    // check if we can join only 1 string
    DynString* str = dynstring_create();
    Error err = dynstring_push_chars(str, "Hello");
    if (!err.ok) return err;

    DynString* res = dynstring_create();
    err = dynstring_join(res, 1, str);
    if (!err.ok) return err;

    // Check for length
    if (res->len != 5) {
        return snitch ("Length of joined string is wrong", __LINE__, __FILE__);
    }
    // Check for content
    if (strncmp(res->data, "Hello, ", 5) != 0) {
        return snitch ("Content of joined string is wrong", __LINE__, __FILE__);
    }

    return (Error){ .ok = true };
}

Error check_user_inputs_for_joining_strings(FxString* desc) {
    *desc = fxstring_create("Dynstring: can handle invalid user inputs for joining strings");
    Error err;

    DynString* null_str;
    DynString* str1 = dynstring_create();
    err = dynstring_push_chars(str1, "Hello, ");
    if (!err.ok) return err;

    DynString* str2 = dynstring_create();
    err = dynstring_push_chars(str2, "World!");
    if (!err.ok) return err;

    DynString* res = dynstring_create();

    // check if null result string is caught
    err = dynstring_join(null_str, 2, str1, str2);
    if (err.ok) return snitch ("Null input not caught", __LINE__, __FILE__);

    // check if null string is caught
    err = dynstring_join(res, 2, str1, null_str);
    if (err.ok) return snitch ("Null input not caught", __LINE__, __FILE__);

    // check if zero strings are caught
    err = dynstring_join(res, 0);
    if (err.ok) return snitch ("Joining zero strings not caught", __LINE__, __FILE__);

    // check if negative number of strings is caught
    err = dynstring_join(res, -1, str1, str2);
    if (err.ok) return snitch ("Negative number of strings not caught", __LINE__, __FILE__);

    return (Error){ .ok = true };
}

Error compare_strings(FxString* desc) {
    *desc = fxstring_create("Dynstring: can compare two strings");
    
    DynString* str1 = dynstring_create();
    Error err = dynstring_push_chars(str1, "foo");
    if (!err.ok) return err;

    DynString* str2 = dynstring_create();
    err = dynstring_push_chars(str2, "bar");
    if (!err.ok) return err;

    // Compare strings
    Bool result;
    err = dynstring_compare(str1, str2, &result);
    if (!err.ok) return err;

    // Check for length
    if (result != false) {
        return snitch ("Strings should not be equal", __LINE__, __FILE__);
    }

    // Compare same strings
    err = dynstring_compare(str1, str1, &result);
    if (!err.ok) return err;

    // Check for length
    if (result != true) {
        return snitch ("Strings should be equal", __LINE__, __FILE__);
    }

    return (Error){ .ok = true };
}

Error compare_empty_strings(FxString* desc) {
    *desc = fxstring_create("Dynstring: can compare two empty strings");
    
    DynString* str1 = dynstring_create();
    DynString* str2 = dynstring_create();

    // Compare strings
    Bool result;
    Error err = dynstring_compare(str1, str2, &result);
    if (!err.ok) return err;

    // Check for length
    if (result != true) {
        return snitch ("Strings should be equal", __LINE__, __FILE__);
    }

    return (Error){ .ok = true };
}

Error check_user_inputs_for_comparing_strings(FxString* desc) {
    *desc = fxstring_create("Dynstring: check invalid user inputs for comparing strings");
    Error err;

    DynString* str1 = dynstring_create();
    err = dynstring_push_chars(str1, "foo");
    if (!err.ok) return err;

    DynString* str2;
    DynString* str3;

    // Compare strings
    Bool res1, res2, res3;
    err = dynstring_compare(str1, str2, &res1);
    if (err.ok) return snitch ("Null input not caught", __LINE__, __FILE__);

    // err = dynstring_compare(str2, str1, &res2);
    // if (err.ok) return snitch ("Null input not caught", __LINE__, __FILE__);

    // err = dynstring_compare(str2, str3, &res3);
    // if (err.ok) return snitch ("Null input not caught", __LINE__, __FILE__);

    return (Error){ .ok = true };
}

Error get_substring_at_given_pos(FxString* desc) {
    *desc = fxstring_create("Dynstring: can get substring from a string at given position");
    
    DynString* str = dynstring_create();
    Error err = dynstring_push_chars(str, "Hello, World!");
    if (!err.ok) return err;

    // Get substring from the middle of the string
    DynString* frag1 = dynstring_create();
    err = dynstring_push_chars(frag1, "World!");
    if (!err.ok) return err;

    Bool found1;

    err = dynstring_substring_at(str, frag1, 7, &found1);
    if (!err.ok) return err;

    if (!found1) {
        return snitch ("Substring not found", __LINE__, __FILE__);
    }

    // Check if we can get substring from the start of the string
    Bool found2;
    err = dynstring_substring_at(str, str, 0, &found2);
    if (!err.ok) return err;

    // Check if we can get empty substring from string. Should be always true
    Bool found3;
    DynString* frag3 = dynstring_create();
    err = dynstring_substring_at(str, frag3, 0, &found3);
    if (!err.ok) return err;

    if (!found3) {
        return snitch ("Substring not found", __LINE__, __FILE__);
    }

    return (Error){ .ok = true };
}

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