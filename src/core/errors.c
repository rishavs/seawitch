#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "errors.h"
#include "seawitch.h"


// Build a short error object with minimal ceremony
Error snitch(char* msg, Int64 line, char* filepath) {
    return (Error){ 
        .ok = false,
        .message = fxstring_create(msg),
        .raised_on_line = line,
        .raised_in_file = fxstring_create_from_behind(filepath)
    };
}

// Print error message and continue
void yell(Error err) {
    // printing in format <filename>:<line>:<column>: <message> makes it clickable in IDEs and editors
    fprintf(stderr, "\033[0;31m%s:%lli:%lli: [ ERROR ] %s\n", 
        err.raised_in_file.data, err.raised_on_line, err.pos, err.message.data);
    
    if (errno != 0) {
        fprintf(stderr, "INTERNAL");
    }
        
    // if(err.message.len > 0) fprintf(stderr, "Error Message: %s\n", err.message.data);
    fprintf(stderr, "ERROR DETAILS:\n");

    if (err.name.len > 0) fprintf(stderr, "\tError Name: %s\n", err.name.data);
    if (err.details.len > 0) fprintf(stderr, "\tError Details: %s\n", err.details.data);
    if (err.hint.len > 0) fprintf(stderr, "\tError Hint: %s\n", err.hint.data);
    
    if (err.pos > 0) fprintf(stderr, "\tError Position: %lli\n", err.pos);
    if (err.column > 0) fprintf(stderr, "\tError Column: %lli\n", err.column);
    if (err.line > 0) fprintf(stderr, "\tError Line: %lli\n", err.line);
    if (err.filepath.len > 0) fprintf(stderr, "\tError File: %s\n", err.filepath.data);

    if (err.raised_on_line > 0)     fprintf(stderr, "\tRaised on line: %lli\n", err.raised_on_line);
    if (err.raised_in_file.len > 0) fprintf(stderr, "\tRaised in file: %s\n", err.raised_in_file.data);

    fprintf(stderr, "\n\033[0m");
}

// Print error message and exit
void fatal(Error err) {
    yell(err);
    exit(EXIT_FAILURE);
}

// // Create a new error
// Error* error_create(Error_category category, FxString name, FxString message) {
//     Error* err = calloc(1, sizeof(Error));
//     if (!err) fatal_memory_allocation(__FILE__, __LINE__);

//     err->category = category;
//     if (name) strcpy(err->name, name);
//     if (message) strcpy(err->message, message);
//     if (details) strcpy(err->details, details);
//     if (hint) strcpy(err->hint, hint);
//     err->pos = pos;
//     err->line = line;
//     if (filepath) strcpy(err->filepath, filepath);

//     return err;
// }

// // Some pre built error messages
// // throw memory allocation failure and exit
// void fatal_memory_allocation (FxString filepath, Int64 line) {
//     fatal((Error) {
//         .category = MEMORY_ALLOCATION_FAILURE,
//         .name = "Memory Allocation Failure",
//         .message = "Failed to allocate memory during compilation",
//         .pos = 0,
//         .line = 0,
//         .column = 0,
//         .filepath = ""
//     }, filepath, line);
// }

// // throw null input error and exit
// void fatal_null_input (FxString filepath, Int64 line) {
//     fatal((Error) {
//         .category = NULL_INPUT,
//         .name = "Null Input Error",
//         .message = "Null input detected during compilation",
//         .pos = 0,
//         .line = 0,
//         .column = 0,
//         .filepath = ""
//     }, filepath, line);
// }

// // throw integer overflow error and exit
// void fatal_integer_overflow (FxString filepath, Int64 line) {
//     fatal((Error) {
//         .category = INTEGER_OVERFLOW,
//         .name = "Integer Overflow Error",
//         .message = "Integer overflow detected during compilation",
//         .pos = 0,
//         .line = 0,
//         .column = 0,
//         .filepath = ""
//     }, filepath, line);
// }


// // Primitive library function failed. Non-fatal error. Show error message and return false
// void* print_error_return_null(char* transpiler_file, size_t transpiler_line) {
//     fprintf(stderr, "\033[0;31m%s\n", "[ ERROR ] Primitive function failed");
//     perror("Internal Note");
//     fprintf(stderr, "Internal Note: Failure occurred in the compiler code at line %zu, in the file \"%s\"", transpiler_line, transpiler_file);
//     fprintf(stderr, "\n\033[0m");

//     return NULL;
// }

// // Primitive library function failed. Non-fatal error. Show error message and return false
// Bool print_error_return_false(char* transpiler_file, size_t transpiler_line) {
//     fprintf(stderr, "\033[0;31m%s\n", "[ ERROR ] Primitive function failed");
//     perror("Internal Note");
//     fprintf(stderr, "Internal Note: Failure occurred in the compiler code at line %zu, in the file \"%s\"", transpiler_line, transpiler_file);
//     fprintf(stderr, "\n\033[0m");

//     return false;
// }
// // Memory allocaltion failed. Fatal failure.
// void memory_allocation_failure(Int64 pos, Int64 line, char* filepath, char* transpiler_file, Int64 transpiler_line) {
//     fprintf(stderr, "\033[0;31m%s\n", "[ FATAL ] Memory Failure! Failed to allocate memory during compilation.");
//     if (pos && line && filepath) { // memory failures need not be tied to user code
//         fprintf(stderr, "Problem found at %zu:%zu in the file \"%s\"\n", pos, line, filepath);
//     };   
//     perror("Internal Note");
//     fprintf(stderr, "Internal Note: Failed occurred in the compiler code at line %zu, in the file \"%s\"", transpiler_line, transpiler_file);
//     fprintf(stderr, "\n\033[0m");
//     exit(EXIT_FAILURE);
// }


// Here we will define all the compiler errors

// Let's list all possible compiler error types
// BaseException: Base class for all built-in exceptions. It is not meant to be directly inherited by user-defined classes.

// Exception: All built-in, non-system-exiting exceptions are derived from this class. All user-defined exceptions should also be derived from this class.

// ArithmeticError: Base class for those built-in exceptions that are raised for various arithmetic errors such as:

// OverflowError: Raised when the result of an arithmetic operation is too large to be expressed by the usual number type.
// ZeroDivisionError: Raised when the second argument of a division or modulo operation is zero.
// BufferError: Raised when a buffer related operation cannot be performed.

// LookupError: Base class for those exceptions that are raised when a key or index used on a mapping or sequence is invalid such as:

// IndexError: Raised when a sequence subscript is out of range.
// KeyError: Raised when a dictionary key is not found.
// EOFError: Raised when the input() function hits an end-of-file condition (EOF) without reading any data.

// ImportError: Raised when the import statement has troubles trying to load a module.

// OSError: Base class for those built-in exceptions that are raised for various system-related errors such as:

// FileNotFoundError: Raised when a file or directory is requested but doesn’t exist.
// PermissionError: Raised when trying to open a file in write mode where write access is not allowed.
// SyntaxError: Raised when the parser encounters a syntax error.

// TypeError: Raised when an operation or function is applied to an object of inappropriate type.

// ValueError: Raised when a built-in operation or function receives an argument that has the right type but an inappropriate value.

// RuntimeError: Raised when an error is detected that doesn’t fall in any of the other categories.


// Compiler_Error raise(char* type, char* message, int atPos, int atLine) {
//     Compiler_Error error;
//     error.type = type;
//     error.message = message;
//     error.atPos = atPos;
//     error.atLine = atLine;
//     return error;
// }