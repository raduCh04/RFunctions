#pragma once

#include "defines.h"
/**
 * @warning Not really secure so be aware of using it
*/
#define FOREACH_DYN(item, array) \
    for (usize _i = 0; _i < (array).size; _i++) \
        if (((item) = (char *)((array).data) + _i * (array).item_size) || 1)

typedef struct RDynArray
{
    void *data;
    usize capacity;
    usize size;
    usize item_size;
} RDynArray;

RDynArray dyn_arr_init(usize item_size);
void dyn_arr_push_back(RDynArray *array, const void *source);
void dyn_arr_get(RDynArray array, usize index, void *value);
void dyn_arr_free(RDynArray *array);