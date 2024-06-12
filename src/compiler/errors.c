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