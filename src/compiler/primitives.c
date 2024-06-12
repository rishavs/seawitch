
typedef long double float64;

typedef enum  {
    Nothing,
    Something,
    Anything,
    Bool,
    Number,
    Byte,
    Grapheme,
    String,
} Primitives;

typedef struct Number {
    /* data */
    Primitives type;
    float64 value;
    float64 refcount;
} Number;

typedef struct String {
    /* data */
    Primitives type;
    char* value;
    float64 refcount;
} String;
