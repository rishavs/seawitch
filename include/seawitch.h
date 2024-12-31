#ifndef SEAWITCH_H
#define SEAWITCH_H

#include <stdint.h>
#include <stdbool.h>

#define SEAWITCH_VERSION "0.0.1"

#define INITIAL_CAPACITY 8
#define FIXED_STRING_SIZE 1024

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
    FXSTRING,
    DYNSTRING,
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
    char*   data;
    Int64   len;
    Int64   capacity;
} DynString;
// typedef struct {
//     Int64   len;
//     char    data[FIXED_STRING_SIZE];
// } FxString;

typedef char FxString[FIXED_STRING_SIZE];


// Error uses fixed strings to avoid dynamic memory allocation
typedef struct {
    FxString    message;                // Error message

    FxString    name;                   // Error header. Optional
    FxString    details;                // Optional
    FxString    hint;                   // Optional

    Int64       pos;                    // Error position provided by user. Optional
    Int64       line;                   // Error line number provided by user. Optional
    Int64       column;                 // Error column number provided by user. Optional
    FxString    filepath;               // Error file path provided by user. Optional
    
    Int64       raised_at_line;         // Internal compilar path for error.
    FxString    raised_at_filepath;     // Internal compilar path for error.
} Error;

#define DEFINE_RESULT(type, name) \
    typedef struct { \
        Bool    ok; \
        union { \
            type    data; \
            Error   error; \
        }; \
    } Result_with_##name; 

DEFINE_RESULT(Int64, int64)
DEFINE_RESULT(Bool, bool)
DEFINE_RESULT(DynString*, dynstring)
    
#endif