#include <stdio.h>
#include <stdbool.h>

#include "testing.h"
#include "dynarray.h"
#include "dynstring.h"

#include "seawitch.h"

// helper function - print int
void print_int(size_t i, Gen_ref acc, Gen_ref item) {
    printf("%lli ", *(Int64*)item);
}

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

    // // pop a few elements
    // Int64 out1, out2;
    // dynarray_pop(arr, (void*)&out1);
    // dynarray_pop(arr, (void*)&out2);
    // if (out1 == 5 && out2 == 4 && arr->len == 3) {
    //     dynarray_oneach(arr, &(Int64){0}, print_int);
    //     res.passed = true;
    // } else {
    //     res.passed = false;
    // }
    // printf("\n");


    // // Check for ownership. As this in an array of integers, updating the array should not affect the original values
    // Int64 new_elem = 100;
    // dynarray_set(arr, 0, &new_elem);
    // new_elem = 200;
    // Int64 out3, out4;
    // if (arr->len == 3 && 
    //     dynarray_get(arr, 0, &out3) && out3 == 100 &&
    //     dynarray_get(arr, 1, &out4) && out4 == 2
    // ) {
    //     res.passed = true;
    // } else {
    //     res.passed = false;
    // }
    
    // dynarray_oneach(arr, &(Int64){0}, print_int);


    // printf("\n");
