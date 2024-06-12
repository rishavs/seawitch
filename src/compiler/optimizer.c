    // Create a new dynamic array with an initial capacity of 5
    List* myArray = list_new(5);

    // Push references to items (of different types) into the array
    int item1 = 42;
    double item2 = 3.14;
    char* item3 = "Hello, World!";

    list_push(myArray, &item1);
    list_push(myArray, &item2);
    list_push(myArray, item3); // Note: You can push items of different types

    // Get and print items from the array by index
    for (size_t i = 0; i < myArray->size; i++) {
        void* item = list_get(myArray, i);
        if (item != NULL) {
            if (item == &item1) {
                printf("Item at index %zu (int): %d\n", i, *((int*)item));
            } else if (item == &item2) {
                printf("Item at index %zu (double): %lf\n", i, *((double*)item));
            } else if (item == item3) {
                printf("Item at index %zu (string): %s\n", i, (char*)item);
            }
        }
    }

    // Destroy the dynamic array and free its memory
    list_destroy(myArray);

    return 0;