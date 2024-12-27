#include "testing.h"
#include "seawitch.h"

// Import the tests
#include "dynarray.test.c"

// Create an array of the tests
Test_Fun all_tests[] = {
    // basic_string_create,
    // push_str_chars_to_string,
    // get_substring,

    can_create_int_array_and_print,
    can_push_and_pop_int_array,
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
        Test_Result res = all_tests[i]();
        if (res.passed) {
            printf("\033[0;32m%lli: \t[ PASSED ]\t%s\n\033[0m", i, res.desc->data); // Green color for PASS
            passed_count++;
        } else {
            printf("\033[0;31m%lli:\t[ FAILED ]\t%s\n\033[0m", i, res.desc->data);
        }
        i++;
    }
    printf("----------------------------------------------------\n");
    printf("Total: %lli, Passed: %lli, Failed: %lli \n", i, passed_count, i-passed_count);
    return 0;
}
