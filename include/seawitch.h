#ifndef SEAWITCH_H
#define SEAWITCH_H

#include <stdint.h>
#include <stdbool.h>

#define SEAWITCH_VERSION "0.0.1"

#define INITIAL_CAPACITY 8
#define FIXED_STRING_SIZE 64

typedef enum {
    // Generic Reference
    REF,
    
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
    OBJECT,
    REF_OBJECT,

} Value_type;

typedef uint8_t     Byte;
// typedef Int64_t     Rune;
typedef int64_t     Int64;
typedef double      Float64;
typedef bool        Bool;

typedef void*       Ref;

typedef struct {
    char*   data;
    Int64   len;
    Int64   capacity;
} DynString;

typedef struct {
    DynString*  message;                // Error message

    DynString*  name;                   // Error header. Optional
    DynString*  details;                // Optional. cause. hint. etc.

    Int64       pos;                    // Error position provided by user. Optional
    Int64       line;                   // Error line number provided by user. Optional
    Int64       column;                 // Error column number provided by user. Optional
    DynString*  filepath;               // Error file path provided by user. Optional
    
    Int64       raised_on_line;         // Internal compiler path for error.
    DynString*  raised_in_file;         // Internal compiler path for error.
} Compiler_error;

#endif