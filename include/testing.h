#ifndef SEAWITCH_TESTS_H
#define SEAWITCH_TESTS_H

#include <stdbool.h>

#include "seawitch.h"

typedef struct Test_Result {
    FxString desc;
    Bool passed;
    Error error;
} Test_Result;

typedef struct Spec {
    FxString desc;
    Error (*test)();
} Spec;

// Define the function pointer type
typedef Test_Result (*Test_Fun)();

#endif // SEAWITCH_TESTS_H
