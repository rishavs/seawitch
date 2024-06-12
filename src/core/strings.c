
// String structure
struct String
{
    struct List list; // Dynamic and resizable array of characters
    size_t len;        // Length of the string
};


// Initialize a string instance with the given capacity
void string_init(struct String *str, size_t capacity)
{
    vector_init(&str->vec, sizeof(char), capacity);
    str->len = 0;
}

// Append a character to the string
void string_append_char(struct String *str, char c)
{
    vector_append(&str->vec, &c);
    str->len++;
}

// Append a null-terminated string to the string
void string_append_str(struct String *str, const char *s)
{
    size_t s_len = strlen(s);
    for (size_t i = 0; i < s_len; i++)
    {
        string_append_char(str, s[i]);
    }
}

// // Get a pointer to the character at the given index in the string
// char *string_at(
