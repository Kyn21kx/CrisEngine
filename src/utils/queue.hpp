#pragma once
#include <cstddef>

//Queue is a circular buffer
struct QueueINT{
  int* base;
  size_t head;
  size_t tail;
  const size_t capacity;  
};

namespace Queue {

QueueINT create(size_t capacity);
void destroy(QueueINT *queue);
//NOTE(cris): Hay un caso en donde ya no se puede hacer push, no se como comunicar ese caso

//returns true if succeeded
bool push(QueueINT *queue, int value);
//should it be named 'pop'? we aren't really popping, we are moving the index forward
bool pop(QueueINT *queue);
//return the value en head and then move the index foward by 1 
int front(QueueINT *queue);
bool is_empty(QueueINT *queue);
bool is_full(QueueINT *queue);
int peek(QueueINT *queue);
//Set the buffer to zero, could be called zero_init as well
// Is it useful to make one that set the buffer given an input parameter?
void flush(QueueINT *queue);
}
