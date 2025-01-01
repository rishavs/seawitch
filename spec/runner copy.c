#include <stdio.h>

#include "errors.h"
#include "testing.h"
#include "seawitch.h"

// Import the tests
// #include "dynarray.test.c"
#include "dynstring.test.c"

// Dummy tests
Test_Result must_pass () {
    return (Test_Result) { 
        .desc = fxstring_do_from_chars("DUMMY: This test must Pass", false),
        .passed = true
    };
}

Test_Result must_fail () {
    return (Test_Result) { 
        .desc = fxstring_do_from_chars("DUMMY: This test must Fail", false) ,
        .passed = false, 
        .error = {
            .message = fxstring_do_from_chars("This is an error message", false),
            .line = __LINE__,
            .filepath = fxstring_do_from_chars(__FILE__, true)
        }
    };
}

// Create an array of the tests
Test_Fun all_tests[] = {
    must_pass,
    must_fail,
    create_basic_dynstring,
    push_str_chars_to_string,
    // basic_string_create,
    // push_str_chars_to_string,
    // get_substring,

    // can_create_int_array_and_print,
    // can_push_and_pop_int_array,
    // can_get_set_int_array,
    // can_slice_int_array,
    // can_join_multiple_int_arrays,
    // can_filter_int_array,
    // can_sort_int_array,
    // can_compare_int_arrays,
    // can_check_for_subarray,
    // can_find_item_in_array,
    // can_map_through_array,
    // can_reduce_array,
    // can_create_array_of_structs_as_values,
    // can_create_array_of_refs_to_structs,

    // End of tests
    NULL
};


int main() {

    Int64 i = 0;
    Int64 passed_count = 0;

    printf("----------------------------------------------------\n");
    printf("Running Tests ...\n");
    printf("----------------------------------------------------\n");
    while (all_tests[i] != NULL) {
        Test_Result test = all_tests[i]();
        if (test.passed) {
            printf("\033[0;32m%lli: \t[ PASSED ]\t%s\n\033[0m", i, test.desc.data); // Green color for PASS
            passed_count++;
        } else {
            printf("\033[0;31m%lli:\t[ FAILED ]\t%s\n\033[0m", i, test.desc.data);

            // If there is also an error message, print it
            if (test.error.message.len > 0) {
                printf("\033[0;31m\t\t\t-- Error: %s\n\033[0m", test.error.message.data);
                // yell(test.result.error);
            }
        }
        i++;
    }
    printf("----------------------------------------------------\n");
    printf("Total: %lli, Passed: %lli, Failed: %lli \n", i, passed_count, i-passed_count);
    return 0;
}
