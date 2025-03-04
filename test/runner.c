#include <stdio.h>
#include <stdint.h>

#include "sw_core.h"

#include "seawitch.h"
#include "sw_compiler.h"

typedef struct {
    Compiler_error* error;
    char* desc;
} Test_result;

// Define a type for the test functions
typedef Test_result (*Spec)();

// Dummy tests
Test_result must_pass_fun() {
    return (Test_result){
        .error = NULL,
        .desc = "This test should pass"
    };
}
Test_result must_fail_fun() {
    // desc = dynstring_from_cstr("This test should fail");
    // return snitch("This is a dummy error message", __LINE__, __FILE__);
    return (Test_result) {
        .error = snitch("This is a dummy error message", __FILE__, __LINE__),
        .desc = "This test should fail"
    };
}

Test_result simple_var_declaration_as_int() {
    Test_result res = { 
        .desc = "simple var declaration as int",
        .error = NULL
    };

    // Transpile the source
    Transpiler_context* ctx = transpiler_ctx_do_init(
        "specs/basic.test.c",
        "var x = 3\n var y = 13"
    );
    
    transpile_code(ctx);
    
    return res;
}

// Helper types
typedef struct
{
    int64_t x;
    int64_t y;
} Point;


// create a list of test functions like must_pass_fun, must_fail_fun, etc.
// Import the tests
// #include "dynstring.test.c"
// #include "dynarray.test.c"
// #include "dynhashmap.test.c"

Spec all_specs[] = {
    must_fail_fun,
    must_pass_fun,

    simple_var_declaration_as_int,
    // -----------------------------------------------
    // Dyn string tests
    // -----------------------------------------------
    // create_basic_dynstring,
    // push_chars_to_string,
    // slice_string,
    // handle_invalid_inputs_for_slicing_string,
    // join_multiple_strings,
    // join_string_with_empty_string,
    // join_single_string,
    // handle_invalid_inputs_for_joining_strings,
    // compare_strings,
    // compare_empty_strings,
    // check_substring_at_given_pos,
    // handle_invalid_inputs_for_checking_substring,
    // // -----------------------------------------------
    // // Dyn array tests
    // // -----------------------------------------------    
    // create_int_array_and_get_values,
    // handle_invalid_inputs_for_array_push,
    // push_and_pop_from_array,
    // get_set_on_byte_array,
    // handle_invalid_input_for_get_set_on_byte_array,
    // slice_int_array,
    // handle_invalid_input_for_slicing_array,
    // join_multiple_arrays,
    // append_an_array_to_another_array,
    // can_filter_an_array,
    // sort_an_array,
    // compare_arrays,
    // find_an_item_in_array,
    // create_object_array,
    // create_object_ref_array,

    // create_int_hashmap_and_get_values,
    // handle_invalid_input_for_get_set_on_byte_hashmap,
    // get_set_remove_from_object_hashmap,

    // End of tests
    NULL
};

int main() {
    int64_t i = 0;
    int64_t passed_count = 0;

    printf("----------------------------------------------------\n");
    printf("Running Tests ...\n");
    printf("----------------------------------------------------\n");

    while (all_specs[i] != NULL) {
        Spec current = all_specs[i];
        Test_result res = current();
        if (res.error) {
            printf("\033[0;31m%lli:\t[ FAILED ]\t%s\n\033[0m", i, res.desc); // Red color for FAIL
            yell(res.error);
        } else {
            printf("\033[0;32m%lli: \t[ PASSED ]\t%s\n\033[0m", i, res.desc); // Green color for PASS
            passed_count++;
        }
        i++;
    }
    printf("----------------------------------------------------\n");
    printf("Total: %lli, Passed: %lli, Failed: %lli \n", i, passed_count, i-passed_count);
    return 0;
}
