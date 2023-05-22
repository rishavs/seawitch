#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// Define a parser result struct
typedef struct {
    bool success;
    const char* remainder;
} ParseResult;

// Define a type for parser functions
typedef ParseResult (*Parser)(const char*);

// Parser combinator: Sequencing
ParseResult seq(Parser p1, Parser p2, const char* input) {
    ParseResult result1 = p1(input);
    if (!result1.success) {
        return result1;
    }
    return p2(result1.remainder);
}

// Parser combinator: Alternation
ParseResult alt(Parser p1, Parser p2, const char* input) {
    ParseResult result1 = p1(input);
    if (result1.success) {
        return result1;
    }
    return p2(input);
}

// Parser combinator: Satisfy
ParseResult satisfy(bool (*predicate)(char), const char* input) {
    if (*input == '\0' || !predicate(*input)) {
        return (ParseResult){ .success = false, .remainder = input };
    }
    return (ParseResult){ .success = true, .remainder = input + 1 };
}

// Parser combinator: Literal string
ParseResult lit(const char* str, const char* input) {
    size_t len = strlen(str);
    if (strncmp(str, input, len) == 0) {
        return (ParseResult){ .success = true, .remainder = input + len };
    }
    return (ParseResult){ .success = false, .remainder = input };
}

// Parser combinator: Zero or more
ParseResult zero_or_more(Parser p, const char* input) {
    ParseResult result = p(input);
    while (result.success) {
        input = result.remainder;
        result = p(input);
    }
    return (ParseResult){ .success = true, .remainder = input };
}

// Parser combinator: One or more
ParseResult one_or_more(Parser p, const char* input) {
    ParseResult result = p(input);
    if (!result.success) {
        return result;
    }
    return zero_or_more(p, result.remainder);
}

// Example parser functions

ParseResult parse_digit(const char* input) {
    return satisfy(isdigit, input);
}

ParseResult parse_integer(const char* input) {
    return one_or_more(parse_digit, input);
}

ParseResult parse_variable(const char* input) {
    return seq(parse_digit, parse_integer, input);
}

// Usage example

int main() {
    const char* input = "123abc";
    ParseResult result = parse_variable(input);
    if (result.success) {
        printf("Parsing succeeded. Remaining input: '%s'\n", result.remainder);
    } else {
        printf("Parsing failed. Remaining input: '%s'\n", result.remainder);
    }
    return 0;
}
