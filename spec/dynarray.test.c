#include <stdio.h>
#include <stdbool.h>

#include "testing.h"
#include "dynarray.h"
#include "dynstring.h"

#include "seawitch.h"

// Null input
Test_Result empty_failed_test() {
    Test_Result res = { 
        .desc = string_create("empty failed test"),
        .passed = false 
    };

    return res;
}
