#include <stdio.h>

#include "errors.h"
#include "testing.h"
#include "seawitch.h"

// Import the tests
// #include "dynarray.test.c"
#include "dynstring.test.c"

// Dummy tests

Error must_pass_fun() {
    return (Error) { 
        .ok = true,
    };
}
Error must_fail_fun() {
    return snitch("This is an error message", __LINE__, __FILE__);
}

int main() {

    // Create an array of the tests
    Spec all_specs[] = {
        { .desc = fxstring_create("DUMMY: This test must Pass"), .test = must_pass_fun },
        { .desc = fxstring_create("DUMMY: This test must Fail"), .test = must_fail_fun },

        { .desc = fxstring_create("Create a new dynamic string"), .test = create_basic_dynstring },
        // { .desc = fxstring_do_from_chars("Push cstr and chars to string", false), .test = push_str_chars_to_string },
    
        // End of tests
        (Spec){}
    };

    Int64 i = 0;
    Int64 passed_count = 0;
    printf("----------------------------------------------------\n");
    printf("Running Tests ...\n");
    printf("----------------------------------------------------\n");
    while (all_specs[i].desc.len > 0) {
        Spec current = all_specs[i];
        Error res = current.test();
        if (res.ok) {
            printf("\033[0;32m%lli: \t[ PASSED ]\t%s\n\033[0m", i, current.desc.data); // Green color for PASS
            passed_count++;
        } else {
            printf("\033[0;31m%lli:\t[ FAILED ]\t%s\n\033[0m", i, current.desc.data);

            // If there is also an error message, print it
            if (res.message.len > 0) {
                printf("\033[0;31m\t\t\t-- Error: %s\n\033[0m", res.message.data);
                // yell(test.result.error);
            }
        }
        i++;
    }
    printf("----------------------------------------------------\n");
    printf("Total: %lli, Passed: %lli, Failed: %lli \n", i, passed_count, i-passed_count);
    return 0;
}
