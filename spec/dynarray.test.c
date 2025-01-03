#include <stdio.h>
#include <stdbool.h>

#include "errors.h"
#include "dynarray.h"
#include "dynstring.h"

#include "seawitch.h"

// helper function - print int
Error print_int(Int64 i, void* acc, void* item) {
    printf("%lli ", *(Int64*)item);
    return (Error){ .ok = true };
}

typedef struct
{
    Int64 x;
    Int64 y;
} Point;


// Null input
Error create_int_array_and_get_values(FxString *desc) {
    *desc = fxstring_create("Array: Can create an int array and get its items");
    Error err;

    DynArray* arr = dynarray_create(INT64, sizeof(Int64), 8);
    Int64 item_size = (Int64)sizeof(Int64);   //no overflow check required, as the value will be very small

    err = dynarray_push(arr, &(Int64){1}, item_size); if (!err.ok) return err;
    err = dynarray_push(arr, &(Int64){2}, item_size); if (!err.ok) return err;
    err = dynarray_push(arr, &(Int64){3}, item_size); if (!err.ok) return err;
    err = dynarray_push(arr, &(Int64){4}, item_size); if (!err.ok) return err;
    err = dynarray_push(arr, &(Int64){5}, item_size); if (!err.ok) return err;

    // Check if the array is correct
    if (arr->len != 5) return snitch ("Array length is incorrect", __LINE__, __FILE__);

    Int64 val1, val2;
    err = dynarray_get(arr, 0, &val1); if (!err.ok) return err;
    err = dynarray_get(arr, 4, &val2); if (!err.ok) return err;

    if (val1 != 1 && val2 != 5) {
        return snitch("Array values are incorrect", __LINE__, __FILE__);
    }

    err = dynarray_oneach(arr, &(Int64){0}, print_int);
    if (!err.ok) return err;
    printf("\n");

    return (Error){ .ok = true };
}

Error handle_invalid_inputs_for_array_push(FxString *desc) {
    *desc = fxstring_create("Array: Can handle invalid inputs for array push");
    Error err;

    DynArray* arr = dynarray_create(INT64, sizeof(Int64), 8);
    
    // Push a wrong type
    err = dynarray_push(arr, &(Point){1, 2}, 0); 
    if (err.ok) return snitch ("No check for Invalid type", __LINE__, __FILE__);

    return (Error){ .ok = true };
}

Error push_and_pop_from_array(FxString *desc) {
    *desc = fxstring_create("Array: Can push and pop from an int array");
    Error err;

    DynArray* arr = dynarray_create(INT64, (Int64)sizeof(Int64), 8);
    err = dynarray_push(arr, &(Int64){1}, (Int64)sizeof(Int64)); if (!err.ok) return err;
    err = dynarray_push(arr, &(Int64){2}, (Int64)sizeof(Int64)); if (!err.ok) return err;
    if (arr->len != 2) return snitch ("Wrong array length after Push", __LINE__, __FILE__);

    Int64 out;
    err = dynarray_pop(arr, &out); if (!err.ok) return err;
    if (out != 2 || arr->len != 1) return snitch ("Pop operation failed", __LINE__, __FILE__);

    err = dynarray_push(arr, &(Int64){3}, (Int64)sizeof(Int64)); if (!err.ok) return err;
    err = dynarray_push(arr, &(Int64){4}, (Int64)sizeof(Int64)); if (!err.ok) return err;
    if (arr->len != 3) return snitch ("Wrong array length after Push", __LINE__, __FILE__);

    // Pop all items
    Int64 out1, out2, out3;
    err = dynarray_pop(arr, &out1); if (!err.ok) return err;
    err = dynarray_pop(arr, &out2); if (!err.ok) return err;
    err = dynarray_pop(arr, &out3); if (!err.ok) return err;

    if (arr->len != 0) return snitch ("Wrong array length after Pop", __LINE__, __FILE__);
    if (out1 != 4 || out2 != 3 || out3 != 1) return snitch ("Pop operation failed", __LINE__, __FILE__);

    // pop empty array
    Int64 out4;
    err = dynarray_pop(arr, &out4); 
    if (err.ok) return snitch ("No check for Pop from empty array", __LINE__, __FILE__);

    return (Error){ .ok = true };
}

Error get_set_on_byte_array(FxString *desc) {
    *desc = fxstring_create("Array: Can get and set a byte array");
    Error err;

    DynArray* arr = dynarray_create(BYTE, (Int64)sizeof(Byte), 8);
    Byte b1 = 'a', b2 = 'b';
    err = dynarray_push(arr, &b1, (Int64)sizeof(Byte)); if (!err.ok) return err;
    err = dynarray_push(arr, &b2, (Int64)sizeof(Byte)); if (!err.ok) return err;

    Byte out1, out2;
    err = dynarray_get(arr, 0, &out1); if (!err.ok) return err;
    err = dynarray_get(arr, 1, &out2); if (!err.ok) return err;

    if (out1 != 'a' || out2 != 'b') return snitch ("Wrong byte values", __LINE__, __FILE__);

    Byte new_b1 = 'c';
    err = dynarray_set(arr, 0, &new_b1, (Int64)sizeof(Byte)); if (!err.ok) return err;

    Byte out3;
    err = dynarray_get(arr, 0, &out3); if (!err.ok) return err;

    if (out3 != 'c') return snitch ("Wrong byte value after set", __LINE__, __FILE__);

    if (arr->len != 2) return snitch ("Wrong array length after set", __LINE__, __FILE__);

    return (Error){ .ok = true };
}

Error handle_invalid_input_for_get_set_on_byte_array(FxString *desc) {
    *desc = fxstring_create("Array: Can handle invalid inputs for get and set on byte array");
    Error err;

    DynArray* arr = dynarray_create(BYTE, (Int64)sizeof(Byte), 8);
    Byte b1 = 'a', b2 = 'b';
    err = dynarray_push(arr, &b1, (Int64)sizeof(Byte)); if (!err.ok) return err;
    err = dynarray_push(arr, &b2, (Int64)sizeof(Byte)); if (!err.ok) return err;

    // Get a wrong index
    Byte out1;
    err = dynarray_get(arr, -10, &out1);
    if (err.ok) return snitch ("No check for wrong index", __LINE__, __FILE__);

    err = dynarray_get(arr, 100, &out1); 
    if (err.ok) return snitch ("No check for wrong index", __LINE__, __FILE__);

    // Set a wrong index
    Byte new_b1 = 'c';
    err = dynarray_set(arr, -10, &new_b1, (Int64)sizeof(Byte)); 
    if (err.ok) return snitch ("No check for wrong index", __LINE__, __FILE__);

    err = dynarray_set(arr, 100, &new_b1, (Int64)sizeof(Byte));
    if (err.ok) return snitch ("No check for wrong index", __LINE__, __FILE__);

    return (Error){ .ok = true };
}



// Test_Result can_slice_int_array() {
//     Test_Result res = { 
//         .desc = dynstring_create("Array: Can slice an int array"),
//         .passed = false 
//     };

//     DynArray* arr = dynarray_create(INT64, sizeof(Int64), 8);
//     dynarray_push(arr, &(Int64){1});
//     dynarray_push(arr, &(Int64){2});
//     dynarray_push(arr, &(Int64){3});
//     dynarray_push(arr, &(Int64){4});
//     dynarray_push(arr, &(Int64){5});

//     DynArray* sliced = dynarray_slice(arr, 1, 3);

//     if (sliced->len == 3 &&
//         *(Int64*)dynarray_get(sliced, 0, &(Int64){0}) == 2 &&
//         *(Int64*)dynarray_get(sliced, 1, &(Int64){0}) == 3 &&
//         *(Int64*)dynarray_get(sliced, 2, &(Int64){0}) == 4
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

//     DynArray* arr1 = dynarray_create(INT64, sizeof(Int64), 8);
//     dynarray_push(arr1, &(Int64){1});
//     dynarray_push(arr1, &(Int64){2});

//     DynArray* arr2 = dynarray_create(INT64, sizeof(Int64), 8);
//     dynarray_push(arr2, &(Int64){3});
//     dynarray_push(arr2, &(Int64){4});

//     DynArray* arr3 = dynarray_create(INT64, sizeof(Int64), 8);
//     dynarray_push(arr3, &(Int64){5});
    
//     DynArray* joined = dynarray_join(3, arr1, arr2, arr3);

//     if (joined->len == 5 &&
//         *(Int64*)dynarray_get(joined, 0, &(Int64){0}) == 1 &&
//         *(Int64*)dynarray_get(joined, 1, &(Int64){0}) == 2 &&
//         *(Int64*)dynarray_get(joined, 2, &(Int64){0}) == 3 &&
//         *(Int64*)dynarray_get(joined, 3, &(Int64){0}) == 4 &&
//         *(Int64*)dynarray_get(joined, 4, &(Int64){0}) == 5
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

//     DynArray* arr = dynarray_create(INT64, sizeof(Int64), 8);
//     dynarray_push(arr, &(Int64){1});
//     dynarray_push(arr, &(Int64){2});
//     dynarray_push(arr, &(Int64){3});
//     dynarray_push(arr, &(Int64){4});
//     dynarray_push(arr, &(Int64){5});

//     Bool is_even(void* item) {
//         return *(Int64*)item % 2 == 0;
//     }

//     DynArray* filtered = dynarray_filter(arr, is_even);

//     if (filtered->len == 2 &&
//         *(Int64*)dynarray_get(filtered, 0, &(Int64){0}) == 2 &&
//         *(Int64*)dynarray_get(filtered, 1, &(Int64){0}) == 4
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

//     DynArray* arr = dynarray_create(INT64, sizeof(Int64), 8);
//     dynarray_push(arr, &(Int64){5});
//     dynarray_push(arr, &(Int64){3});
//     dynarray_push(arr, &(Int64){1});

//     int cmp(const void* a, const void* b) {
//         return *(Int64*)a - *(Int64*)b;
//     }

//     DynArray* sorted = dynarray_sort(arr, cmp);

//     Int64* out1 = dynarray_get(sorted, 0, &(Int64){0});
//     Int64* out2 = dynarray_get(sorted, 1, &(Int64){0});
//     Int64* out3 = dynarray_get(sorted, 2, &(Int64){0});

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

//     DynArray* arr1 = dynarray_create(INT64, sizeof(Int64), 8);
//     dynarray_push(arr1, &(Int64){1});
//     dynarray_push(arr1, &(Int64){2});
//     dynarray_push(arr1, &(Int64){3});

//     DynArray* arr2 = dynarray_create(INT64, sizeof(Int64), 8);
//     dynarray_push(arr2, &(Int64){1});
//     dynarray_push(arr2, &(Int64){2});
//     dynarray_push(arr2, &(Int64){3});

//     DynArray* arr3 = dynarray_create(INT64, sizeof(Int64), 8);
//     dynarray_push(arr3, &(Int64){1});
//     dynarray_push(arr3, &(Int64){2});

//     DynArray* arr4 = dynarray_create(INT64, sizeof(Int64), 8);
//     dynarray_push(arr4, &(Int64){1});
//     dynarray_push(arr4, &(Int64){3});

//     DynArray* arr5 = dynarray_create(INT64, sizeof(Int64), 8);
//     dynarray_push(arr5, &(Int64){1});
//     dynarray_push(arr5, &(Int64){2});

//     DynArray* arr6 = dynarray_create(INT64, sizeof(Int64), 8);
//     dynarray_push(arr6, &(char[8]){"Hello"});
//     dynarray_push(arr6, &(char[8]){"World"});

//     Bool cmp(void* a, void* b) {
//         return *(Int64*)a == *(Int64*)b;
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

//     DynArray* arr1 = dynarray_create(INT64, sizeof(Int64), 8);
//     dynarray_push(arr1, &(Int64){1});
//     dynarray_push(arr1, &(Int64){2});
//     dynarray_push(arr1, &(Int64){3});
//     dynarray_push(arr1, &(Int64){4});
//     dynarray_push(arr1, &(Int64){5});

//     DynArray* arr2 = dynarray_create(INT64, sizeof(Int64), 8);
//     dynarray_push(arr2, &(Int64){3});
//     dynarray_push(arr2, &(Int64){4});

//     DynArray* arr3 = dynarray_create(INT64, sizeof(Int64), 8);
//     dynarray_push(arr3, &(Int64){1});
//     dynarray_push(arr3, &(Int64){2});

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

//     DynArray* arr = dynarray_create(INT64, sizeof(Int64), 8);
//     dynarray_push(arr, &(Int64){1});
//     dynarray_push(arr, &(Int64){2});
//     dynarray_push(arr, &(Int64){3});
//     dynarray_push(arr, &(Int64){4});
//     dynarray_push(arr, &(Int64){5});

//     Bool cmp(void* a, void* b) {
//         return *(Int64*)a == *(Int64*)b;
//     }

//     size_t index;
//     if (
//         dynarray_find(arr, &index, &(Int64){3}, cmp) &&
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

//     DynArray* arr = dynarray_create(INT64, sizeof(Int64), 8);
//     dynarray_push(arr, &(Int64){1});
//     dynarray_push(arr, &(Int64){2});
//     dynarray_push(arr, &(Int64){3});
//     dynarray_push(arr, &(Int64){4});
//     dynarray_push(arr, &(Int64){5});

//     DynArray* new_arr = dynarray_create(INT64, sizeof(Int64), 8);

//     // Take the new_arr as acc and add all items  with value * 2 to it
//     void fn(size_t i, void* acc, void* item) {
//         Int64 val = *(Int64*)item;
//         dynarray_push((DynArray*)acc, &(Int64){val * 2});
//     };

//     dynarray_oneach(arr, (DynArray*)new_arr, fn);

//     Int64* out1 = dynarray_get(new_arr, 0, &(Int64){0});
//     Int64* out2 = dynarray_get(new_arr, 1, &(Int64){0});
//     Int64* out3 = dynarray_get(new_arr, 2, &(Int64){0});
//     Int64* out4 = dynarray_get(new_arr, 3, &(Int64){0});
//     Int64* out5 = dynarray_get(new_arr, 4, &(Int64){0});

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

//     DynArray* arr = dynarray_create(INT64, sizeof(Int64), 8);
//     dynarray_push(arr, &(Int64){1});
//     dynarray_push(arr, &(Int64){2});
//     dynarray_push(arr, &(Int64){3});

//     Int64 sum = 0;

//     // Take the sum as acc and add all items to it
//     void fn(size_t i, void* acc, void* item) {
//         *(Int64*)acc += *(Int64*)item;
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