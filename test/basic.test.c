#include <stdbool.h>

#include "dyn_string.h"
#include "errors.h"
#include "tests.h"
#include "transpiler.h"

// Null input
Test_Result empty_failed_test() {
    Test_Result res = { 
        .desc = dyn_dynstring_do_init("empty failed test"),
        .passed = false 
    };

    return res;
}

Test_Result empty_passed_test() {
    Test_Result res = { 
        .desc = dyn_dynstring_do_init("empty passed test"),
        .passed = false 
    };
    res.passed = true;

    return res;
}

Test_Result simple_var_declaration_as_int() {
    Test_Result res = { 
        .desc = dyn_dynstring_do_init("simple var declaration as int"),
        .passed = false 
    };

    // Transpile the source
    Transpiler_ctx_t* ctx = transpiler_ctx_do_init(
        dyn_dynstring_do_init("specs/basic.test.c"),
        dyn_dynstring_do_init("let x = 3\n let y = 13")
    );
    
    transpile_file(ctx);
    
    res.passed = true;
    return res;
}