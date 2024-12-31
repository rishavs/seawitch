#include "seawitch.h"

// Create a new dynamic string from a character
String* byte_do_to_string(char c) {
    char temp[2] = {c, '\0'}; // Create a temporary string
    return dynstring_create(temp);
}
