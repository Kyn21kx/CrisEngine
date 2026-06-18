#include "utils/array.hpp"
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <stdlib.h>

// Allocates memory given a constant length
ArrayINT array_create(size_t length){
  
  //NOTE(cris): Probablemente sea pesima idea usar un malloc aqui 
  ArrayINT result { .data_ptr = (int*)malloc(sizeof(int)* length),
                    .length = length
                };

  return result;
}
//Initializes an array given a value
void array_value_init(ArrayINT *array, int value){
memset(array->data_ptr, value,sizeof(int) *  array->length);
  /*
  for(size_t i = 0; i < array->length; i++){
    *(array->data_ptr + i) = value
  }
  */
}

void array_destroy(ArrayINT *array){
  free(array);
}

void array_zero_init(ArrayINT *array){
memset(array->data_ptr, 0,sizeof(int) * array->length);
}

int array_value_at_index(ArrayINT *array, size_t index){
  assert(index < array->length);
  assert( array != nullptr);
  
    return *(array->data_ptr + index);
}

void array_set_to(ArrayINT *array, int data, size_t index){
  assert(index < array->length);
  assert( array != nullptr);

 *(array->data_ptr + index) = data;
}

size_t array_size(ArrayINT *array){
  return array->length;
}
