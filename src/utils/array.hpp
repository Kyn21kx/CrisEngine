#pragma once
#include <cstddef>

struct ArrayINT{
  int* data_ptr;
  const size_t length;
};

ArrayINT  array_create(size_t length);
void      array_destroy(ArrayINT *array);
void      array_value_init(ArrayINT *array, int value);
void      array_zero_init(ArrayINT *array);
void      array_set_to(ArrayINT *array, int data, size_t index);
int       array_value_at_index(ArrayINT *array, size_t index);
size_t    array_size(ArrayINT *array);

