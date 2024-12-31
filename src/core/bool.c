#include <stdio.h>
#include <stdlib.h>

#include "seawitch.h"

String* bool_do_to_string(Bool value) {
    return dynstring_create(value ? "true" : "false");
}