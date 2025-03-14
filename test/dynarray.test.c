#include <stdio.h>
#include <stdbool.h>

#include "errors.h"
#include "dynarray.h"
#include "dynstring.h"

#include "seawitch.h"

// helper function - print int
Error print_int(int64_t len, int64_t i, void* acc, void* item) {
    if (i == 0) printf("[ ");
    printf("%lli", *(int64_t*)item);
    if (i != len - 1) printf(", ");
    if (i == len - 1) printf(" ]");

    return (Error){ .ok = true };
}

// Null input
Error create_int_array_and_get_values(FxString *desc) {
    *desc = fxstring_create("Array: Can create an int array and get its items");
    Error err;

    DynArray* arr = dynarray_create(INT64, sizeof(int64_t), 8);
    int64_t item_size = (int64_t)sizeof(int64_t);   //no overflow check required, as the value will be very small

    err = dynarray_push(arr, &(int64_t){1}, item_size); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &(int64_t){2}, item_size); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &(int64_t){3}, item_size); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &(int64_t){4}, item_size); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &(int64_t){5}, item_size); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    // Check if the array is correct
    if (arr->len != 5) return snitch ("Array length is incorrect", __LINE__, __FILE__);

    int64_t val1, val2;
    err = dynarray_get(arr, 0, &val1); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(arr, 4, &val2); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    if (val1 != 1 && val2 != 5) {
        return snitch("Array values are incorrect", __LINE__, __FILE__);
    }

    err = dynarray_oneach(arr, &(int64_t){0}, print_int);
    if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    printf("\n");

    return (Error){ .ok = true };
}

Error handle_invalid_inputs_for_array_push(FxString *desc) {
    *desc = fxstring_create("Array: Can handle invalid inputs for array push");
    Error err;

    DynArray* arr = dynarray_create(INT64, sizeof(int64_t), 8);
    
    // Push a wrong type
    err = dynarray_push(arr, &(Point){1, 2}, 0); 
    if (err.ok) return snitch ("No check for Invalid type", __LINE__, __FILE__);

    return (Error){ .ok = true };
}

Error push_and_pop_from_array(FxString *desc) {
    *desc = fxstring_create("Array: Can push and pop from an int array");
    Error err;

    DynArray* arr = dynarray_create(INT64, (int64_t)sizeof(int64_t), 8);
    err = dynarray_push(arr, &(int64_t){1}, (int64_t)sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &(int64_t){2}, (int64_t)sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    if (arr->len != 2) return snitch ("Wrong array length after Push", __LINE__, __FILE__);

    int64_t out;
    err = dynarray_pop(arr, &out); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    if (out != 2 || arr->len != 1) return snitch ("Pop operation failed", __LINE__, __FILE__);

    err = dynarray_push(arr, &(int64_t){3}, (int64_t)sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &(int64_t){4}, (int64_t)sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    if (arr->len != 3) return snitch ("Wrong array length after Push", __LINE__, __FILE__);

    // Pop all items
    int64_t out1, out2, out3;
    err = dynarray_pop(arr, &out1); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_pop(arr, &out2); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_pop(arr, &out3); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    if (arr->len != 0) return snitch ("Wrong array length after Pop", __LINE__, __FILE__);
    if (out1 != 4 || out2 != 3 || out3 != 1) return snitch ("Pop operation failed", __LINE__, __FILE__);

    // pop empty array
    int64_t out4;
    err = dynarray_pop(arr, &out4); 
    if (err.ok) return snitch ("No check for Pop from empty array", __LINE__, __FILE__);

    return (Error){ .ok = true };
}

Error get_set_on_byte_array(FxString *desc) {
    *desc = fxstring_create("Array: Can get and set a byte array");
    Error err;

    DynArray* arr = dynarray_create(BYTE, (int64_t)sizeof(Byte), 8);
    Byte b1 = 'a', b2 = 'b';
    err = dynarray_push(arr, &b1, (int64_t)sizeof(Byte)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &b2, (int64_t)sizeof(Byte)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    Byte out1, out2;
    err = dynarray_get(arr, 0, &out1); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(arr, 1, &out2); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    if (out1 != 'a' || out2 != 'b') return snitch ("Wrong byte values", __LINE__, __FILE__);

    Byte new_b1 = 'c';
    err = dynarray_set(arr, 0, &new_b1, (int64_t)sizeof(Byte)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    Byte out3;
    err = dynarray_get(arr, 0, &out3); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    if (out3 != 'c') return snitch ("Wrong byte value after set", __LINE__, __FILE__);

    if (arr->len != 2) return snitch ("Wrong array length after set", __LINE__, __FILE__);

    return (Error){ .ok = true };
}

Error handle_invalid_input_for_get_set_on_byte_array(FxString *desc) {
    *desc = fxstring_create("Array: Can handle invalid inputs for get and set on byte array");
    Error err;

    DynArray* arr = dynarray_create(BYTE, (int64_t)sizeof(Byte), 8);
    Byte b1 = 'a', b2 = 'b';
    err = dynarray_push(arr, &b1, (int64_t)sizeof(Byte)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &b2, (int64_t)sizeof(Byte)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    // Get a wrong index
    Byte out1;
    err = dynarray_get(arr, -10, &out1);
    if (err.ok) return snitch ("No check for wrong index", __LINE__, __FILE__);

    err = dynarray_get(arr, 100, &out1); 
    if (err.ok) return snitch ("No check for wrong index", __LINE__, __FILE__);

    // Set a wrong index
    Byte new_b1 = 'c';
    err = dynarray_set(arr, -10, &new_b1, (int64_t)sizeof(Byte)); 
    if (err.ok) return snitch ("No check for wrong index", __LINE__, __FILE__);

    err = dynarray_set(arr, 100, &new_b1, (int64_t)sizeof(Byte));
    if (err.ok) return snitch ("No check for wrong index", __LINE__, __FILE__);

    return (Error){ .ok = true };
}

Error slice_int_array(FxString *desc) {
    *desc = fxstring_create("Array: Can slice an int array");
    Error err;

    DynArray* arr = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_push(arr, &(int64_t){1}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__); // 0
    err = dynarray_push(arr, &(int64_t){2}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__); // 1
    err = dynarray_push(arr, &(int64_t){3}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__); // 2
    err = dynarray_push(arr, &(int64_t){4}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__); // 3
    err = dynarray_push(arr, &(int64_t){5}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__); // 4

    DynArray* sliced1 = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_slice(arr, sliced1, 1, 3); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    int64_t val1, val2, val3;
    err = dynarray_get(sliced1, 0, &val1); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(sliced1, 1, &val2); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(sliced1, 2, &val3); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    if (sliced1->len != 3 || val1 != 2 || val2 != 3 || val3 != 4) {
        return snitch("Array slicing failed", __LINE__, __FILE__);
    }

    // now slice entire array
    DynArray* sliced2 = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_slice(arr, sliced2, 0, 4); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    int64_t val4, val5, val6, val7, val8;
    err = dynarray_get(sliced2, 0, &val4); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(sliced2, 1, &val5); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(sliced2, 2, &val6); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(sliced2, 3, &val7); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(sliced2, 4, &val8); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    if (sliced2->len != 5 || val4 != 1 || val5 != 2 || val6 != 3 || val7 != 4 || val8 != 5) {
        return snitch("Array slicing failed", __LINE__, __FILE__);
    }

    // Can slice only 1 item
    DynArray* sliced3 = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_slice(arr, sliced3, 2, 2); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    int64_t val9;
    err = dynarray_get(sliced3, 0, &val9); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    if (sliced3->len != 1 || val9 != 3) {
        return snitch("Array slicing failed", __LINE__, __FILE__);
    }

    return (Error){ .ok = true };
}

Error handle_invalid_input_for_slicing_array(FxString *desc) {
    *desc = fxstring_create("Array: Can handle invalid inputs for slicing array");
    Error err;

    DynArray* arr = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_push(arr, &(int64_t){1}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &(int64_t){2}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &(int64_t){3}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &(int64_t){4}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &(int64_t){5}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    // Slice with wrong start index
    DynArray* sliced1 = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_slice(arr, sliced1, -1, 3);
    if (err.ok) return snitch ("No check for wrong start index", __LINE__, __FILE__);

    // Slice with wrong end index
    DynArray* sliced2 = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_slice(arr, sliced2, 1, 100);
    if (err.ok) return snitch ("No check for wrong end index", __LINE__, __FILE__);

    // Slice with start index greater than end index
    DynArray* sliced3 = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_slice(arr, sliced3, 3, 1);
    if (err.ok) return snitch ("No check for start index greater than end index", __LINE__, __FILE__);

    // Slice with empty slice array
    DynArray* empty_slice = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_slice(arr, empty_slice, 0, 4); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    
    // Check if the original array is not modified
    int64_t val1;
    err = dynarray_get(arr, 0, &val1); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    
    if (val1 != 1) return snitch ("Original array is modified after slicing", __LINE__, __FILE__);

    return (Error){ .ok = true };
}

Error join_multiple_arrays(FxString *desc) {
    *desc = fxstring_create("Array: Can join multiple int arrays");
    Error err;

    DynArray* arr1 = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_push(arr1, &(int64_t){1}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr1, &(int64_t){2}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    DynArray* arr2 = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_push(arr2, &(int64_t){3}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr2, &(int64_t){4}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    DynArray* arr3 = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_push(arr3, &(int64_t){5}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    DynArray* joined = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_join(joined, 3, arr1, arr2, arr3); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    int64_t val1, val2, val3, val4, val5;
    err = dynarray_get(joined, 0, &val1); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(joined, 1, &val2); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(joined, 2, &val3); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(joined, 3, &val4); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(joined, 4, &val5); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    if (joined->len != 5 || val1 != 1 || val2 != 2 || val3 != 3 || val4 != 4 || val5 != 5) {
        return snitch("Array joining failed", __LINE__, __FILE__);
    }

    // Can join with 0 arrays

    // Can join with 1 array

    // Can join empty arrays

    // check if n is <= 0
    DynArray* joined2 = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_join(joined2, 0, arr1, arr2, arr3);
    if (err.ok) return snitch ("No check for n <= 0", __LINE__, __FILE__);

    // check if n > actual number of items
    DynArray* joined3 = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_join(joined3, 5, arr1, arr2, arr3);
    if (err.ok) return snitch ("No check for n > actual number of items", __LINE__, __FILE__);

    return (Error){ .ok = true };
}

Error append_an_array_to_another_array(FxString *desc) {
    *desc = fxstring_create("Array: Can append an array to another array");
    Error err;

    DynArray* arr1 = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_push(arr1, &(int64_t){1}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr1, &(int64_t){2}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    DynArray* arr2 = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_push(arr2, &(int64_t){3}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr2, &(int64_t){4}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    err = dynarray_append(arr1, arr2); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    int64_t val1, val2, val3, val4;
    err = dynarray_get(arr1, 0, &val1); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(arr1, 1, &val2); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(arr1, 2, &val3); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(arr1, 3, &val4); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    if (arr1->len != 4 || val1 != 1 || val2 != 2 || val3 != 3 || val4 != 4) {
        return snitch("Array appending failed", __LINE__, __FILE__);
    }

    // check that the original array is not modified
    int64_t val5, val6;
    err = dynarray_get(arr2, 0, &val5); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(arr2, 1, &val6); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    if (val5 != 3 || val6 != 4) {
        return snitch("Original array is modified after appending", __LINE__, __FILE__);
    }

    return (Error){ .ok = true };
}

Error is_even(int64_t len, int64_t i, void* item, Bool* res) {
    *res = (*(int64_t*)item % 2 == 0);
    return (Error){ .ok = true };
}

Error can_filter_an_array(FxString *desc) {
    *desc = fxstring_create("Array: Can filter an int array");
    Error err;

    DynArray* arr = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_push(arr, &(int64_t){1}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &(int64_t){2}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &(int64_t){3}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &(int64_t){4}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &(int64_t){5}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    // Error dynarray_filter(DynArray* source, DynArray* filtered, Error (*fn)(int64_t, int64_t, void*, Bool*));

    DynArray* filtered = dynarray_create(INT64, sizeof(int64_t), 8);

    err = dynarray_filter(arr, filtered, is_even); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    int64_t val1, val2;
    err = dynarray_get(filtered, 0, &val1); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(filtered, 1, &val2); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    if (filtered->len != 2 || val1 != 2 || val2 != 4) {
        return snitch("Array filtering failed", __LINE__, __FILE__);
    }

    return (Error){ .ok = true };
}


int cmp(const void* a, const void* b) {
    return *(int64_t*)a - *(int64_t*)b;
}

Error sort_an_array(FxString *desc) {
    *desc = fxstring_create("Array: Can sort an int array");
    Error err;

    DynArray* arr = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_push(arr, &(int64_t){5}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &(int64_t){1}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &(int64_t){3}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    DynArray* sorted = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_sort(arr, sorted, cmp); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    int64_t val1, val2, val3;
    err = dynarray_get(sorted, 0, &val1); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(sorted, 1, &val2); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(sorted, 2, &val3); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    if (sorted->len != 3 || val1 != 1 || val2 != 3 || val3 != 5) {
        return snitch("Array sorting failed", __LINE__, __FILE__);
    }

    return (Error){ .ok = true };
}

Error is_eql(void* item1, void* item2, Bool* res) {
    *res = (*(int64_t*)item1 == *(int64_t*)item2);
    return (Error){ .ok = true };
}

Error compare_arrays(FxString *desc) {
    *desc = fxstring_create("Array: Can compare arrays");
    Error err;

    DynArray* arr1 = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_push(arr1, &(int64_t){1}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr1, &(int64_t){2}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    DynArray* arr2 = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_push(arr2, &(int64_t){1}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr2, &(int64_t){2}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    Bool result;
    err = dynarray_compare(arr1, arr2, is_eql, &result); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    if (!result) return snitch("Array comparison failed", __LINE__, __FILE__);

    // check for different arrays
    DynArray* arr3 = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_push(arr3, &(int64_t){1}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr3, &(int64_t){3}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    Bool result2;
    err = dynarray_compare(arr1, arr3, is_eql, &result2); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    if (result2) return snitch("Array comparison failed", __LINE__, __FILE__);

    // check for different lengths
    DynArray* arr4 = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_push(arr4, &(int64_t){1}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    Bool result3;
    err = dynarray_compare(arr1, arr4, is_eql, &result3); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    if (result3) return snitch("Array comparison failed", __LINE__, __FILE__);

    // check for different types
    DynArray* arr5 = dynarray_create(OBJECT, sizeof(Point), 8);
    err = dynarray_push(arr5, &(Point){1, 2}, sizeof(Point)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    Bool result4;
    err = dynarray_compare(arr1, arr5, is_eql, &result4); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    if (result4) return snitch("Array comparison failed", __LINE__, __FILE__);

    return (Error){ .ok = true };
}

Error find_an_item_in_array(FxString *desc) {
    *desc = fxstring_create("Array: Can find an item in an int array");
    Error err;

    DynArray* arr = dynarray_create(INT64, sizeof(int64_t), 8);
    err = dynarray_push(arr, &(int64_t){1}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &(int64_t){2}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &(int64_t){3}, sizeof(int64_t)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    int64_t index;
    int64_t item_to_find = 2;

    err = dynarray_find_first(arr, 0, &item_to_find, &index, is_eql); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    if (index != 1) return snitch("Array find failed", __LINE__, __FILE__);

    // find at exact index
    err = dynarray_find_first(arr, 1, &item_to_find, &index, is_eql); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    if (index != 1) return snitch("Array find failed", __LINE__, __FILE__);

    // check for not found
    int64_t not_found_item = 4;
    err = dynarray_find_first(arr, 0, &not_found_item, &index, is_eql); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    if (index != -1) return snitch("Array find failed", __LINE__, __FILE__);

    // check not found at index
    err = dynarray_find_first(arr, 2, &item_to_find, &index, is_eql); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    if (index != -1) return snitch("Array find failed", __LINE__, __FILE__);

    return (Error){ .ok = true };
}

Error create_object_array(FxString *desc) {
    *desc = fxstring_create("Array: Can create an object array");
    Error err;

    DynArray* arr = dynarray_create(OBJECT, sizeof(Point), 8);
    Point p1 = {1, 2}, p2 = {3, 4};
    err = dynarray_push(arr, &p1, sizeof(Point)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &p2, sizeof(Point)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    Point out1, out2;
    err = dynarray_get(arr, 0, &out1); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(arr, 1, &out2); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    if (arr->len != 2 || out1.x != 1 || out1.y != 2 || out2.x != 3 || out2.y != 4) {
        return snitch("Object array creation failed", __LINE__, __FILE__);
    }

    return (Error){ .ok = true };
}

Error create_object_ref_array(FxString *desc) {
    *desc = fxstring_create("Array: Can create an object reference array");
    Error err;

    DynArray* arr = dynarray_create(REF_OBJECT, sizeof(Point*), 8);
    Point* p1 = calloc(1, sizeof(Point)); p1->x = 1; p1->y = 2;
    Point* p2 = calloc(1, sizeof(Point)); p2->x = 3; p2->y = 4;

    err = dynarray_push(arr, &p1, sizeof(Point*)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_push(arr, &p2, sizeof(Point*)); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    
    Point* out1, *out2;
    err = dynarray_get(arr, 0, &out1); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);
    err = dynarray_get(arr, 1, &out2); if (!err.ok) return snitch(err.message.data, __LINE__, __FILE__);

    if (arr->len != 2 || out1->x != 1 || out1->y != 2 || out2->x != 3 || out2->y != 4) {
        return snitch("Object reference array creation failed", __LINE__, __FILE__);
    }

    return (Error){ .ok = true };
}

// Test_Result can_slice_int_array() {
//     Test_Result res = { 
//         .desc = dynstring_create("Array: Can slice an int array"),
//         .passed = false 
//     };

//     DynArray* arr = dynarray_create(INT64, sizeof(int64_t), 8);
//     dynarray_push(arr, &(int64_t){1});
//     dynarray_push(arr, &(int64_t){2});
//     dynarray_push(arr, &(int64_t){3});
//     dynarray_push(arr, &(int64_t){4});
//     dynarray_push(arr, &(int64_t){5});

//     DynArray* sliced = dynarray_slice(arr, 1, 3);

//     if (sliced->len == 3 &&
//         *(int64_t*)dynarray_get(sliced, 0, &(int64_t){0}) == 2 &&
//         *(int64_t*)dynarray_get(sliced, 1, &(int64_t){0}) == 3 &&
//         *(int64_t*)dynarray_get(sliced, 2, &(int64_t){0}) == 4
//     ) {
//         res.passed = true;
//     } else {
//         res.passed = false;
//     }

//     return res;
// }

// Test_Result can_join_multiple_int_arrays() {
//     Test_Result res = { 
//         .desc = dynstring_create("Array: Can join multiple int arrays"),
//         .passed = false 
//     };

//     DynArray* arr1 = dynarray_create(INT64, sizeof(int64_t), 8);
//     dynarray_push(arr1, &(int64_t){1});
//     dynarray_push(arr1, &(int64_t){2});

//     DynArray* arr2 = dynarray_create(INT64, sizeof(int64_t), 8);
//     dynarray_push(arr2, &(int64_t){3});
//     dynarray_push(arr2, &(int64_t){4});

//     DynArray* arr3 = dynarray_create(INT64, sizeof(int64_t), 8);
//     dynarray_push(arr3, &(int64_t){5});
    
//     DynArray* joined = dynarray_join(3, arr1, arr2, arr3);

//     if (joined->len == 5 &&
//         *(int64_t*)dynarray_get(joined, 0, &(int64_t){0}) == 1 &&
//         *(int64_t*)dynarray_get(joined, 1, &(int64_t){0}) == 2 &&
//         *(int64_t*)dynarray_get(joined, 2, &(int64_t){0}) == 3 &&
//         *(int64_t*)dynarray_get(joined, 3, &(int64_t){0}) == 4 &&
//         *(int64_t*)dynarray_get(joined, 4, &(int64_t){0}) == 5
//     ) {
//         res.passed = true;
//     } else {
//         res.passed = false;
//     }

//     return res;
// }

// Test_Result can_filter_int_array() {
//     Test_Result res = { 
//         .desc = dynstring_create("Array: Can filter an int array"),
//         .passed = false 
//     };

//     DynArray* arr = dynarray_create(INT64, sizeof(int64_t), 8);
//     dynarray_push(arr, &(int64_t){1});
//     dynarray_push(arr, &(int64_t){2});
//     dynarray_push(arr, &(int64_t){3});
//     dynarray_push(arr, &(int64_t){4});
//     dynarray_push(arr, &(int64_t){5});

//     Bool is_even(void* item) {
//         return *(int64_t*)item % 2 == 0;
//     }

//     DynArray* filtered = dynarray_filter(arr, is_even);

//     if (filtered->len == 2 &&
//         *(int64_t*)dynarray_get(filtered, 0, &(int64_t){0}) == 2 &&
//         *(int64_t*)dynarray_get(filtered, 1, &(int64_t){0}) == 4
//     ) {
//         res.passed = true;
//     } else {
//         res.passed = false;
//     }

//     return res;
// }

// Test_Result can_sort_int_array() {
//     Test_Result res = { 
//         .desc = dynstring_create("Array: Can sort an int array"),
//         .passed = false 
//     };

//     DynArray* arr = dynarray_create(INT64, sizeof(int64_t), 8);
//     dynarray_push(arr, &(int64_t){5});
//     dynarray_push(arr, &(int64_t){3});
//     dynarray_push(arr, &(int64_t){1});

//     int cmp(const void* a, const void* b) {
//         return *(int64_t*)a - *(int64_t*)b;
//     }

//     DynArray* sorted = dynarray_sort(arr, cmp);

//     int64_t* out1 = dynarray_get(sorted, 0, &(int64_t){0});
//     int64_t* out2 = dynarray_get(sorted, 1, &(int64_t){0});
//     int64_t* out3 = dynarray_get(sorted, 2, &(int64_t){0});

//     if (sorted->len == 3 &&
//         *out1 == 1 &&
//         *out2 == 3 &&
//         *out3 == 5
//     ) {
//         res.passed = true;
//     } else {
//         res.passed = false;
//     }

//     return res;
// }

// Test_Result can_compare_int_arrays() {
//     Test_Result res = { 
//         .desc = dynstring_create("Array: Can compare int arrays"),
//         .passed = false 
//     };

//     DynArray* arr1 = dynarray_create(INT64, sizeof(int64_t), 8);
//     dynarray_push(arr1, &(int64_t){1});
//     dynarray_push(arr1, &(int64_t){2});
//     dynarray_push(arr1, &(int64_t){3});

//     DynArray* arr2 = dynarray_create(INT64, sizeof(int64_t), 8);
//     dynarray_push(arr2, &(int64_t){1});
//     dynarray_push(arr2, &(int64_t){2});
//     dynarray_push(arr2, &(int64_t){3});

//     DynArray* arr3 = dynarray_create(INT64, sizeof(int64_t), 8);
//     dynarray_push(arr3, &(int64_t){1});
//     dynarray_push(arr3, &(int64_t){2});

//     DynArray* arr4 = dynarray_create(INT64, sizeof(int64_t), 8);
//     dynarray_push(arr4, &(int64_t){1});
//     dynarray_push(arr4, &(int64_t){3});

//     DynArray* arr5 = dynarray_create(INT64, sizeof(int64_t), 8);
//     dynarray_push(arr5, &(int64_t){1});
//     dynarray_push(arr5, &(int64_t){2});

//     DynArray* arr6 = dynarray_create(INT64, sizeof(int64_t), 8);
//     dynarray_push(arr6, &(char[8]){"Hello"});
//     dynarray_push(arr6, &(char[8]){"World"});

//     Bool cmp(void* a, void* b) {
//         return *(int64_t*)a == *(int64_t*)b;
//     }

//     if (
//         dynarray_compare(arr1, arr2, cmp) &&
//         !dynarray_compare(arr3, arr4, cmp) &&
//         !dynarray_compare(arr5, arr6, cmp)        
//     ) {
//         res.passed = true;
//     } else {
//         res.passed = false;
//     }


//     return res;
// }

// Test_Result can_check_for_subarray() {
//     Test_Result res = { 
//         .desc = dynstring_create("Array: Can check for subarray"),
//         .passed = false 
//     };

//     DynArray* arr1 = dynarray_create(INT64, sizeof(int64_t), 8);
//     dynarray_push(arr1, &(int64_t){1});
//     dynarray_push(arr1, &(int64_t){2});
//     dynarray_push(arr1, &(int64_t){3});
//     dynarray_push(arr1, &(int64_t){4});
//     dynarray_push(arr1, &(int64_t){5});

//     DynArray* arr2 = dynarray_create(INT64, sizeof(int64_t), 8);
//     dynarray_push(arr2, &(int64_t){3});
//     dynarray_push(arr2, &(int64_t){4});

//     DynArray* arr3 = dynarray_create(INT64, sizeof(int64_t), 8);
//     dynarray_push(arr3, &(int64_t){1});
//     dynarray_push(arr3, &(int64_t){2});

//     if (
//         dynarray_has_subarray(arr1, 2, arr2) &&
//         !dynarray_has_subarray(arr1, 2, arr3)
//     ) {
//         res.passed = true;
//     } else {
//         res.passed = false;
//     }

//     return res;
// }


// Test_Result can_find_item_in_array() {
//     Test_Result res = { 
//         .desc = dynstring_create("Array: Can find an item in an array"),
//         .passed = false 
//     };

//     DynArray* arr = dynarray_create(INT64, sizeof(int64_t), 8);
//     dynarray_push(arr, &(int64_t){1});
//     dynarray_push(arr, &(int64_t){2});
//     dynarray_push(arr, &(int64_t){3});
//     dynarray_push(arr, &(int64_t){4});
//     dynarray_push(arr, &(int64_t){5});

//     Bool cmp(void* a, void* b) {
//         return *(int64_t*)a == *(int64_t*)b;
//     }

//     size_t index;
//     if (
//         dynarray_find(arr, &index, &(int64_t){3}, cmp) &&
//         index == 2
//     ) {
//         res.passed = true;
//     } else {
//         res.passed = false;
//     }

//     return res;
// }

// Test_Result can_map_through_array() {
//     Test_Result res = { 
//         .desc = dynstring_create("Array: Can map through an array"),
//         .passed = false 
//     };

//     DynArray* arr = dynarray_create(INT64, sizeof(int64_t), 8);
//     dynarray_push(arr, &(int64_t){1});
//     dynarray_push(arr, &(int64_t){2});
//     dynarray_push(arr, &(int64_t){3});
//     dynarray_push(arr, &(int64_t){4});
//     dynarray_push(arr, &(int64_t){5});

//     DynArray* new_arr = dynarray_create(INT64, sizeof(int64_t), 8);

//     // Take the new_arr as acc and add all items  with value * 2 to it
//     void fn(size_t i, void* acc, void* item) {
//         int64_t val = *(int64_t*)item;
//         dynarray_push((DynArray*)acc, &(int64_t){val * 2});
//     };

//     dynarray_oneach(arr, (DynArray*)new_arr, fn);

//     int64_t* out1 = dynarray_get(new_arr, 0, &(int64_t){0});
//     int64_t* out2 = dynarray_get(new_arr, 1, &(int64_t){0});
//     int64_t* out3 = dynarray_get(new_arr, 2, &(int64_t){0});
//     int64_t* out4 = dynarray_get(new_arr, 3, &(int64_t){0});
//     int64_t* out5 = dynarray_get(new_arr, 4, &(int64_t){0});

//     if (
//         new_arr->len == 5 &&
//         *out1 == 2 &&
//         *out2 == 4 &&
//         *out3 == 6 &&
//         *out4 == 8 &&
//         *out5 == 10
//     ) {
//         res.passed = true;
//     } else {
//         res.passed = false;
//     }

//     return res;
// }

// Test_Result can_reduce_array() {
//     Test_Result res = { 
//         .desc = dynstring_create("Array: Can reduce an array"),
//         .passed = false 
//     };

//     DynArray* arr = dynarray_create(INT64, sizeof(int64_t), 8);
//     dynarray_push(arr, &(int64_t){1});
//     dynarray_push(arr, &(int64_t){2});
//     dynarray_push(arr, &(int64_t){3});

//     int64_t sum = 0;

//     // Take the sum as acc and add all items to it
//     void fn(size_t i, void* acc, void* item) {
//         *(int64_t*)acc += *(int64_t*)item;
//     };

//     dynarray_oneach(arr, &sum, fn);

//     if (sum == 6) {
//         res.passed = true;
//     } else {
//         res.passed = false;
//     }

//     return res;
// }

// Test_Result can_create_array_of_structs_as_values() {
//     Test_Result res = { 
//         .desc = dynstring_create("Array: Can create an array of structs as values"),
//         .passed = false 
//     };

//     DynArray* arr = dynarray_create(GEN_REF, sizeof(Point), 8);
//     Point p1 = {1, 2};
//     Point p2 = {3, 4};
//     Point p3 = {5, 6};

//     dynarray_push(arr, &p1);
//     dynarray_push(arr, &p2);
//     dynarray_push(arr, &p3);

//     // make a change in a struct
//     p3.x = 500;
//     p3.y = 600;

//     Point* p1_out = dynarray_get(arr, 0, &(Point){0, 0});
//     Point* p2_out = dynarray_get(arr, 1, &(Point){0, 0});
//     Point* p3_out = dynarray_get(arr, 2, &(Point){0, 0});

//     if (
//         arr->len == 3 &&
//         p1_out->x == 1 && p1_out->y == 2 &&
//         p2_out->x == 3 && p2_out->y == 4 &&
//         p3_out->x == 5 && p3_out->y == 6
//     ) {
//         res.passed = true;
//     } else {
//         res.passed = false;
//     }

//     return res;
// }


// Test_Result can_create_array_of_refs_to_structs() {
//     Test_Result res = {
//         .desc = dynstring_create("Array: Can create an array of refs to structs"),
//         .passed = false
//     };

//     // Create array to store Point pointers
//     DynArray* arr = dynarray_create(GEN_REF, sizeof(Point*), 8);

//     // Allocate and initialize points
//     Point* p1 = malloc(sizeof(Point));  // Changed from calloc for clarity
//     *p1 = (Point){1, 2};               // Direct initialization
    
//     Point* p2 = malloc(sizeof(Point));
//     *p2 = (Point){3, 4};
    
//     Point* p3 = malloc(sizeof(Point));
//     *p3 = (Point){5, 6};

//     // Push pointers to the points into array
//     dynarray_push(arr, &p1);  // Note: pushing address of the pointer
//     dynarray_push(arr, &p2);
//     dynarray_push(arr, &p3);

//     // Make a change in the original struct
//     p3->x = 500;
//     p3->y = 600;

//     // Get pointers back out
//     Point* p1_out = NULL;
//     Point* p2_out = NULL;
//     Point* p3_out = NULL;
    
//     dynarray_get(arr, 0, &p1_out);  // Get pointer from array into p1_out
//     dynarray_get(arr, 1, &p2_out);
//     dynarray_get(arr, 2, &p3_out);

//     if (
//         arr->len == 3 &&
//         p1_out->x == 1 && p1_out->y == 2 &&
//         p2_out->x == 3 && p2_out->y == 4 &&
//         p3_out->x == 500 && p3_out->y == 600
//     ) {
//         res.passed = true;
//     }

//     return res;
// }