#ifndef SEAWITCH_TESTS_H
#define SEAWITCH_TESTS_H

#include <stdbool.h>

#include "seawitch.h"

typedef struct Test_Result {
    String* desc;
    bool passed;
} Test_Result;

// Define the function pointer type
typedef Test_Result (*Test_Fun)();

#endif // SEAWITCH_TESTS_H