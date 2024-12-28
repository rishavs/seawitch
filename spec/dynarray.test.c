#include <stdio.h>
#include <stdbool.h>

#include "testing.h"
#include "dynarray.h"
#include "dynstring.h"

#include "seawitch.h"

// helper function - print int
void print_int(size_t i, void* acc, void* item) {
    printf("%lli ", *(Int64*)item);
}

typedef struct
{
    Int64 x;
    Int64 y;
} Point;


// Null input
Test_Result can_create_int_array_and_print() {
    Test_Result res = { 
        .desc = string_create("can create an int array and print it"),
        .passed = false 
    };

    DynArray* arr = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr, &(Int64){1});
    dynarray_push(arr, &(Int64){2});
    dynarray_push(arr, &(Int64){3});
    dynarray_push(arr, &(Int64){4});
    dynarray_push(arr, &(Int64){5});

    dynarray_oneach(arr, &(Int64){0}, print_int);
    printf("\n");

    // Check if the array is correct
    if(
        arr->len == 5 &&
        *(Int64*)dynarray_get(arr, 0, &(Int64){0}) == 1 &&
        *(Int64*)dynarray_get(arr, 4, &(Int64){0}) == 5 &&

        // Out of bounds check
        dynarray_get(arr, 5, &(Int64){0}) == NULL &&
        dynarray_get(arr, 100, &(Int64){0}) == NULL

        // *(Int64*)(arr->data) == 1 &&
        // *(Int64*)(arr->data + 1 * sizeof(Int64)) == 2 &&
        // *(Int64*)(arr->data + 2 * sizeof(Int64)) == 3 &&
        // *(Int64*)(arr->data + 3 * sizeof(Int64)) == 4 &&
        // *(Int64*)(arr->data + 4 * sizeof(Int64)) == 5
    ) {
        res.passed = true;
    } 

    return res;
}

Test_Result can_push_and_pop_int_array() {
    Test_Result res = { 
        .desc = string_create("can push and pop an int array"),
        .passed = false 
    };

    DynArray* arr = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr, &(Int64){1});
    dynarray_push(arr, &(Int64){2});

    Int64 out1, out2;
    dynarray_pop(arr, (void*)&out1);

    if (out1 == 2 && arr->len == 1) {
        res.passed = true;
    }

    // push into the array again
    dynarray_push(arr, &(Int64){3});
    dynarray_push(arr, &(Int64){4});
    
    // get the value at the index
    if (arr->len == 3 &&
        *(Int64*)dynarray_get(arr, 0, &(Int64){0}) == 1 &&
        *(Int64*)dynarray_get(arr, 1, &(Int64){0}) == 3 &&
        *(Int64*)dynarray_get(arr, 2, &(Int64){0}) == 4
    ) {
        res.passed = true;
    } else {
        res.passed = false;
    }
   
    return res;
}

Test_Result can_get_set_int_array() {
    Test_Result res = { 
        .desc = string_create("can get and set an int array"),
        .passed = false 
    };

    DynArray* arr = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr, &(Int64){1});
    dynarray_push(arr, &(Int64){2});

    Int64 new_elem = 100;
    dynarray_set(arr, 0, &new_elem);

    Int64 another_elem = 1000;
    dynarray_set(arr, 0, &another_elem);

    if (arr->len == 2 && 
        *(Int64*)dynarray_get(arr, 0, &(Int64){0}) == 1000 &&
        *(Int64*)dynarray_get(arr, 1, &(Int64){0}) == 2 &&

        // Out of bounds check
        dynarray_set(arr, 5, &(Int64){0}) == false &&
        dynarray_set(arr, 100, &(Int64){0}) == false
    ) {
        res.passed = true;
    } else {
        res.passed = false;
    }

    return res;
}

Test_Result can_slice_int_array() {
    Test_Result res = { 
        .desc = string_create("can slice an int array"),
        .passed = false 
    };

    DynArray* arr = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr, &(Int64){1});
    dynarray_push(arr, &(Int64){2});
    dynarray_push(arr, &(Int64){3});
    dynarray_push(arr, &(Int64){4});
    dynarray_push(arr, &(Int64){5});

    DynArray* sliced = dynarray_slice(arr, 1, 3);

    if (sliced->len == 3 &&
        *(Int64*)dynarray_get(sliced, 0, &(Int64){0}) == 2 &&
        *(Int64*)dynarray_get(sliced, 1, &(Int64){0}) == 3 &&
        *(Int64*)dynarray_get(sliced, 2, &(Int64){0}) == 4
    ) {
        res.passed = true;
    } else {
        res.passed = false;
    }

    return res;
}

Test_Result can_join_multiple_int_arrays() {
    Test_Result res = { 
        .desc = string_create("can join multiple int arrays"),
        .passed = false 
    };

    DynArray* arr1 = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr1, &(Int64){1});
    dynarray_push(arr1, &(Int64){2});

    DynArray* arr2 = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr2, &(Int64){3});
    dynarray_push(arr2, &(Int64){4});

    DynArray* arr3 = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr3, &(Int64){5});
    
    DynArray* joined = dynarray_join(3, arr1, arr2, arr3);

    if (joined->len == 5 &&
        *(Int64*)dynarray_get(joined, 0, &(Int64){0}) == 1 &&
        *(Int64*)dynarray_get(joined, 1, &(Int64){0}) == 2 &&
        *(Int64*)dynarray_get(joined, 2, &(Int64){0}) == 3 &&
        *(Int64*)dynarray_get(joined, 3, &(Int64){0}) == 4 &&
        *(Int64*)dynarray_get(joined, 4, &(Int64){0}) == 5
    ) {
        res.passed = true;
    } else {
        res.passed = false;
    }

    return res;
}

Test_Result can_filter_int_array() {
    Test_Result res = { 
        .desc = string_create("can filter an int array"),
        .passed = false 
    };

    DynArray* arr = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr, &(Int64){1});
    dynarray_push(arr, &(Int64){2});
    dynarray_push(arr, &(Int64){3});
    dynarray_push(arr, &(Int64){4});
    dynarray_push(arr, &(Int64){5});

    Bool is_even(void* item) {
        return *(Int64*)item % 2 == 0;
    }

    DynArray* filtered = dynarray_filter(arr, is_even);

    if (filtered->len == 2 &&
        *(Int64*)dynarray_get(filtered, 0, &(Int64){0}) == 2 &&
        *(Int64*)dynarray_get(filtered, 1, &(Int64){0}) == 4
    ) {
        res.passed = true;
    } else {
        res.passed = false;
    }

    return res;
}

Test_Result can_sort_int_array() {
    Test_Result res = { 
        .desc = string_create("can sort an int array"),
        .passed = false 
    };

    DynArray* arr = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr, &(Int64){5});
    dynarray_push(arr, &(Int64){3});
    dynarray_push(arr, &(Int64){1});

    int cmp(const void* a, const void* b) {
        return *(Int64*)a - *(Int64*)b;
    }

    DynArray* sorted = dynarray_sort(arr, cmp);

    if (sorted->len == 3 &&
        *(Int64*)dynarray_get(sorted, 0, &(Int64){0}) == 1 &&
        *(Int64*)dynarray_get(sorted, 1, &(Int64){0}) == 3 &&
        *(Int64*)dynarray_get(sorted, 2, &(Int64){0}) == 5
    ) {
        res.passed = true;
    } else {
        res.passed = false;
    }

    return res;
}

Test_Result can_compare_int_arrays() {
    Test_Result res = { 
        .desc = string_create("can compare int arrays"),
        .passed = false 
    };

    DynArray* arr1 = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr1, &(Int64){1});
    dynarray_push(arr1, &(Int64){2});
    dynarray_push(arr1, &(Int64){3});

    DynArray* arr2 = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr2, &(Int64){1});
    dynarray_push(arr2, &(Int64){2});
    dynarray_push(arr2, &(Int64){3});

    DynArray* arr3 = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr3, &(Int64){1});
    dynarray_push(arr3, &(Int64){2});

    DynArray* arr4 = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr4, &(Int64){1});
    dynarray_push(arr4, &(Int64){3});

    DynArray* arr5 = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr5, &(Int64){1});
    dynarray_push(arr5, &(Int64){2});

    DynArray* arr6 = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr6, &(char[8]){"Hello"});
    dynarray_push(arr6, &(char[8]){"World"});

    Bool cmp(void* a, void* b) {
        return *(Int64*)a == *(Int64*)b;
    }

    if (
        dynarray_compare(arr1, arr2, cmp) &&
        !dynarray_compare(arr3, arr4, cmp) &&
        !dynarray_compare(arr5, arr6, cmp)        
    ) {
        res.passed = true;
    } else {
        res.passed = false;
    }


    return res;
}

Test_Result can_check_for_subarray() {
    Test_Result res = { 
        .desc = string_create("can check for subarray"),
        .passed = false 
    };

    DynArray* arr1 = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr1, &(Int64){1});
    dynarray_push(arr1, &(Int64){2});
    dynarray_push(arr1, &(Int64){3});
    dynarray_push(arr1, &(Int64){4});
    dynarray_push(arr1, &(Int64){5});

    DynArray* arr2 = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr2, &(Int64){3});
    dynarray_push(arr2, &(Int64){4});

    DynArray* arr3 = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr3, &(Int64){1});
    dynarray_push(arr3, &(Int64){2});

    if (
        dynarray_has_subarray(arr1, 2, arr2) &&
        !dynarray_has_subarray(arr1, 2, arr3)
    ) {
        res.passed = true;
    } else {
        res.passed = false;
    }

    return res;
}

Test_Result can_find_item_in_array() {
    Test_Result res = { 
        .desc = string_create("can find an item in an array"),
        .passed = false 
    };

    DynArray* arr = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr, &(Int64){1});
    dynarray_push(arr, &(Int64){2});
    dynarray_push(arr, &(Int64){3});
    dynarray_push(arr, &(Int64){4});
    dynarray_push(arr, &(Int64){5});

    Bool cmp(void* a, void* b) {
        return *(Int64*)a == *(Int64*)b;
    }

    size_t index;
    if (
        dynarray_find(arr, &index, &(Int64){3}, cmp) &&
        index == 2
    ) {
        res.passed = true;
    } else {
        res.passed = false;
    }

    return res;
}

Test_Result can_map_through_array() {
    Test_Result res = { 
        .desc = string_create("can map through an array"),
        .passed = false 
    };

    DynArray* arr = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr, &(Int64){1});
    dynarray_push(arr, &(Int64){2});
    dynarray_push(arr, &(Int64){3});
    dynarray_push(arr, &(Int64){4});
    dynarray_push(arr, &(Int64){5});

    DynArray* new_arr = dynarray_create(INT64, sizeof(Int64), 8);

    // Take the new_arr as acc and add all items  with value * 2 to it
    void fn(size_t i, void* acc, void* item) {
        Int64 val = *(Int64*)item;
        dynarray_push((DynArray*)acc, &(Int64){val * 2});
    };

    dynarray_oneach(arr, (DynArray*)new_arr, fn);

    if (
        new_arr->len == 5 &&
        *(Int64*)dynarray_get(new_arr, 0, &(Int64){0}) == 2 &&
        *(Int64*)dynarray_get(new_arr, 1, &(Int64){0}) == 4 &&
        *(Int64*)dynarray_get(new_arr, 2, &(Int64){0}) == 6 &&
        *(Int64*)dynarray_get(new_arr, 3, &(Int64){0}) == 8 &&
        *(Int64*)dynarray_get(new_arr, 4, &(Int64){0}) == 10
    ) {
        res.passed = true;
    } else {
        res.passed = false;
    }

    return res;
}

Test_Result can_reduce_array() {
    Test_Result res = { 
        .desc = string_create("can reduce an array"),
        .passed = false 
    };

    DynArray* arr = dynarray_create(INT64, sizeof(Int64), 8);
    dynarray_push(arr, &(Int64){1});
    dynarray_push(arr, &(Int64){2});
    dynarray_push(arr, &(Int64){3});

    Int64 sum = 0;

    // Take the sum as acc and add all items to it
    void fn(size_t i, void* acc, void* item) {
        *(Int64*)acc += *(Int64*)item;
    };

    dynarray_oneach(arr, &sum, fn);

    if (sum == 6) {
        res.passed = true;
    } else {
        res.passed = false;
    }

    return res;
}