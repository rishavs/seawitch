#include <stdio.h>
#include <stdlib.h>

#include "errors.h"
#include "seawitch.h"

// FATAL! Print error message and exit
void fatal(Outcome code, Int64 line, char* filepath) {
    fprintf(stderr, "\033[0;31m");
    perror(Outcome_message[code]);
    
    if (line)       fprintf(stderr, "At Line: %zu\n", line);
    if (filepath)   fprintf(stderr, "In File: %s\n", filepath);

    fprintf(stderr, "\n\033[0m");
    exit(EXIT_FAILURE);
}

// Non-fatal error. Show error message
void yell_at_clouds(Outcome code, Int64 line, char* filepath) {
    fprintf(stderr, "\033[0;31m");
    perror(Outcome_message[code]);
    
    if (line)       fprintf(stderr, "At Line: %zu\n", line);
    if (filepath)   fprintf(stderr, "In File: %s\n", filepath);

    fprintf(stderr, "\n\033[0m");
}

// // Print error message and continue
// void yell(Error err, FxString filepath, Int64 line) {
//     fprintf(stderr, "\033[0;31m");
//     perror("[ ERROR ]");
//     fprintf(stderr, "Error Name: %s\n", err.name);
//     fprintf(stderr, "Error Message: %s\n", err.message);
    
//     if (err.details) fprintf(stderr, "Error Details: %s\n", err.details);
//     if (err.hint) fprintf(stderr, "Error Hint: %s\n", err.hint);

//     if (err.hint) fprintf(stderr, "Error Position: %zu:%zu\n", err.line, err.pos);
//     if (err.hint) fprintf(stderr, "Error File: %s\n", filepath);

//     fprintf(stderr, "\n\033[0m");
// }

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