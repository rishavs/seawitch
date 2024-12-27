#ifndef SEAWITCH_H
#define SEAWITCH_H

#include <stdint.h>
#include <stdbool.h>

#define SEAWITCH_VERSION "0.0.1"

#define INITIAL_CAPACITY 8

typedef enum {
    // Internal types
    GEN_REF,
    
    // Value types
    BOOL,
    BYTE,
    INT64,
    FLOAT64,
    // RUNE,

    REF_BOOL,   
    REF_BYTE,  
    REF_INT64,
    REF_FLOAT64,

    // Complex types. Composed of other individual types. Only pass by reference.
    // STRING,
    // LIST,
    // DICT,
    // FUNC,
    // OBJECT,
    // RESULT,
    // ERROR,

    // Special types
    // UNKNOWN,
    // NOTHING,
    // ANY,
    // SOME,

} Types;



typedef uint8_t     Byte;
// typedef Int64_t     Rune;
typedef int64_t     Int64;
typedef double      Float64;
typedef bool        Bool;

typedef void*       Gen_ref;

typedef struct {
    char*       data;
    Int64     len;
    Int64     capacity;
} String;

typedef struct {
    String      type;
    String      description;
    String      hint;

    Int64       pos;
    Int64       line;
    Int64       column;
    String      file;
} Error;

#define DEFINE_RESULT(type) \
    typedef struct { \
        Bool    ok; \
        union { \
            type    data; \
            Error   error; \
        }; \
    } Result_of_##type##_t; \
    
#endif