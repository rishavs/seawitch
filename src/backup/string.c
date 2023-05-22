#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    char *data;
    size_t length;
    size_t capacity;
} String;

String* init_string() {
    String* string = malloc(sizeof(String));
    if (string == NULL) {
        printf("Error: Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    string->data = malloc(sizeof(char));
    if (string->data == NULL) {
        printf("Error: Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    string->data[0] = '\0';
    string->length = 0;
    string->capacity = 8;

    return string;
} 

String* append_char (String* str,  char ch) {
    if (str->length + 1 > str->capacity) {
        // Reallocate memory with double the capacity
        size_t new_capacity = (str->length + 1) * 2;
        char* new_data = realloc(str->data, new_capacity * sizeof(char));

        if (new_data == NULL) {
            printf("Error: Failed to allocate memory\n");
            exit(EXIT_FAILURE);
        }

        str->data = new_data;
        str->capacity = new_capacity;
    }

    // Append the source string to the destination string
    str->data[str->length] = ch;
    str->length += 1;
    str->data[str->length] = '\0';

    return str;
}