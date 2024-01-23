#ifndef HX_DATATYPES_H
#define HX_DATATYPES_H

#include <stdlib.h>

// TODO: change key char*s to dynamic strings

typedef struct {
    char* type;
    void* __value;
    char* (*to_string)();
} Nothing;

typedef struct {
    char* type;
    char* id;
    char* description;
    char* hint;
    char* (*to_string)();
} Error;

Error Error_create_default() {
    Error e;
    e.type = "Error";
    e.id = "";
    e.description = "";
    e.hint = "";
    e.to_string = Error_to_string;
    return e;
}

typedef struct {
    char* type;
    bool __value;
} Bool;

Bool Bool_create_default() {
    Bool b;
    b.type = "Bool";
    b.__value = 0;
    return b;
}

void Bool_free (Bool* b) {
    return
}

typedef struct {
    char* type;
    _Bool isInteger;
    double __value;
} Num;

Num Num_create_default() {
    Num n;
    n.type = "Num";
    n.__value = 0;
    n.isInteger = 1;
    return n;
}

void Num_free (Num* n) {
    return
}

#endif // HX_DATATYPES_H