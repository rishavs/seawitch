#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "helpers.h"


Buffer* create_buffer(size_t initial_capacity) {
    Buffer* buffer = (Buffer*) malloc(sizeof(Buffer));
    if (buffer == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }
    
    buffer->data = (char*) malloc(initial_capacity);
    if (buffer->data == NULL) {
        free(buffer);
        perror("Memory allocation failed");
        exit(1);
    }

    buffer->size = 0;
    buffer->capacity = initial_capacity;

    return buffer;
}


void add_to_buffer(Buffer* buffer, char ch) {
    if (buffer->size >= buffer->capacity) {
        // Resize the buffer if needed
        buffer->capacity *= 2;
        buffer->data = (char*) realloc(buffer->data, buffer->capacity);
        if (buffer->data == NULL) {
            perror("Memory reallocation failed");
            exit(1);
        }
    }
    
    buffer->data[buffer->size++] = ch;
}

void close_buffer(Buffer* buffer) {
    add_to_buffer(buffer, '\0');
}

void destroy_buffer(Buffer* buffer) {
    free(buffer->data);
    free(buffer);
}

bool are_buffers_equal(Buffer* buffer1, Buffer* buffer2) {
    if (buffer1->size != buffer2->size) {
        return false;
    }

    for (size_t i = 0; i < buffer1->size; i++) {
        if (buffer1->data[i] != buffer2->data[i]) {
            return false;
        }
    }

    return true;
}

bool buffer_has(Buffer* buffer, size_t i, char* frag) {
    size_t frag_len = strlen(frag);
    if (i + frag_len > buffer->size) {
        return false;
    }

    for (size_t j = 0; j < frag_len; j++) {
        if (buffer->data[i + j] != frag[j]) {
            return false;
        }
    }

    return true;
}

// Rest of your code...

// // Use the buffer functions like this:
// Buffer* buffer = create_buffer(16); // Initial capacity of 16
// add_to_buffer(buffer, 'H');
// add_to_buffer(buffer, 'e');
// add_to_buffer(buffer, 'l');
// add_to_buffer(buffer, 'l');
// add_to_buffer(buffer, 'o');
// printf("Buffer: %s\n", buffer->data);
// destroy_buffer(buffer); // Clean up when done
