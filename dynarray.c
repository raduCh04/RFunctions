#include "dynarray.h"

//TODO: Replace all standard functions with OS Specific ones
#include <stdlib.h>
#include <string.h>

enum { INITIAL_SIZE = 4 };

RDynArray dyn_arr_init(usize item_size)
{
    RDynArray result = {0};

    result.data = malloc(INITIAL_SIZE * item_size); //TODO: Check for error
    result.capacity = INITIAL_SIZE * item_size;
    result.item_size = item_size;
    return result;
}

static void dyn_arr_resize(RDynArray *array, usize new_capacity)
{
    array->data = realloc(array->data, new_capacity * array->item_size); //TODO: Check for error
    array->capacity = new_capacity;
}

void dyn_arr_push_back(RDynArray *array, const void *source)
{
    if (array->size == array->capacity)
        dyn_arr_resize(array, array->capacity * 2);
    memcpy((char *)array->data + array->size * array->item_size, source, array->item_size); //TODO: Check for error
    array->size++;
}

void dyn_arr_pop_back(RDynArray *array)
{
    memset((char *)array->data, 0, array->item_size);  //TODO: Check for error
    array->size--;
}

void dyn_arr_get(RDynArray array, usize index, void *value)
{
    memcpy(value, (char *)array.data + index * array.item_size, array.item_size); //TODO: Check for error
}

void dyn_arr_free(RDynArray *array)
{
    if (array != NULL)
    {
        if (array->data != NULL)
            free(array->data);
        array->size = 0;
        array->capacity = 0;
        array->item_size = 0;
    }
}
