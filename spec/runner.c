#include <stdio.h>

#include "errors.h"
// #include "testing.h"
#include "seawitch.h"

// Define a type for the test functions
typedef Error (*Spec)();

// Dummy tests
Error must_pass_fun(FxString *desc) {
    *desc = fxstring_create("This test should pass");
    return (Error) { .ok = true };
}
Error must_fail_fun(FxString *desc) {
    *desc = fxstring_create("This test should fail");
    return snitch("This is a dummy error message", __LINE__, __FILE__);
}

// Helper types
typedef struct
{
    Int64 x;
    Int64 y;
} Point;


// create a list of test functions like must_pass_fun, must_fail_fun, etc.
// Import the tests
#include "dynstring.test.c"
#include "dynarray.test.c"
#include "dynhashmap.test.c"

Spec all_specs[] = {
    // must_pass_fun,
    // must_fail_fun,

    // -----------------------------------------------
    // Dyn string tests
    // -----------------------------------------------
    create_basic_dynstring,
    push_chars_to_string,
    slice_string,
    handle_invalid_inputs_for_slicing_string,
    join_multiple_strings,
    join_string_with_empty_string,
    join_single_string,
    handle_invalid_inputs_for_joining_strings,
    compare_strings,
    compare_empty_strings,
    check_substring_at_given_pos,
    handle_invalid_inputs_for_checking_substring,
    // -----------------------------------------------
    // Dyn array tests
    // -----------------------------------------------    
    create_int_array_and_get_values,
    handle_invalid_inputs_for_array_push,
    push_and_pop_from_array,
    get_set_on_byte_array,
    handle_invalid_input_for_get_set_on_byte_array,
    slice_int_array,
    handle_invalid_input_for_slicing_array,
    join_multiple_arrays,
    append_an_array_to_another_array,
    can_filter_an_array,
    sort_an_array,
    compare_arrays,
    find_an_item_in_array,
    create_object_array,
    create_object_ref_array,

    create_int_hashmap_and_get_values,
    handle_invalid_input_for_get_set_on_byte_hashmap,
    get_set_remove_from_object_hashmap,

    // End of tests
    NULL
};

int main() {
    FxString desc;
    Int64 i = 0;
    Int64 passed_count = 0;

    printf("----------------------------------------------------\n");
    printf("Running Tests ...\n");
    printf("----------------------------------------------------\n");

    while (all_specs[i] != NULL) {
        Spec current = all_specs[i];
        Error res = current(&desc);
        if (res.ok) {
            printf("\033[0;32m%lli: \t[ PASSED ]\t%s\n\033[0m", i, desc.data); // Green color for PASS
            passed_count++;
        } else {
            printf("\033[0;31m%lli:\t[ FAILED ]\t%s\n\033[0m", i, desc.data);

            // If there is also an error message, print it
            if (res.message.len > 0) {
                yell(res);
            }
        }
        i++;
    }
    printf("----------------------------------------------------\n");
    printf("Total: %lli, Passed: %lli, Failed: %lli \n", i, passed_count, i-passed_count);
    return 0;
}
