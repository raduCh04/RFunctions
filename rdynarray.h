#pragma once

/**
 * @file rdynarray.h
 * @author Radu-D. Chira
 * @brief A tiny implementation of generic dynamic array data structure
 * @warning Not really type safe!
 * @version 0.1
 * @date 2024-01-10
 */

#include "rdefines.h"
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

RDynArray rdyn_arr_init(usize item_size);
void rdyn_arr_push_back(RDynArray *array, const void *source);
void rdyn_arr_get(RDynArray array, usize index, void *value);
void rdyn_arr_free(RDynArray *array);