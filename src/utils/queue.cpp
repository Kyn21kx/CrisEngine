#include "utils/queue.hpp"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace Queue{
  

QueueINT create(size_t capacity){
  QueueINT result {
    .base = (int*)malloc(sizeof(int) * capacity),
    .head = 0,
    .tail = 0,
    .capacity = capacity    
  };

  return result;
}

void destroy(QueueINT *queue){
  assert(queue != nullptr);
  free(queue->base);
}

bool push(QueueINT *queue, int value){
  assert(queue != nullptr);
  size_t next = (queue->head + 1) % queue->capacity;

if (next == queue->tail) {
    return false; // full
}

queue->base[queue->head] = value;
queue->head = next;

  return true;
  }

bool pop(QueueINT *queue){
  assert(queue != nullptr);

  size_t current_tail = queue->tail;

  //wrap around
  if (current_tail + 1 >= queue->capacity) {
    current_tail = 0;
  
    if (current_tail == queue->head) {
      return false;
    }
    queue->tail = 0;
    return true;
  }
  //check if empty
  if(queue->head == queue->tail){
    return false;
  }

  queue->tail += 1;

  return true;
}

int front(QueueINT *queue){
  assert(queue != nullptr);

  int result = Queue::peek(queue);
  Queue::pop(queue);
  return result;
}

bool is_empty(QueueINT *queue){
  assert(queue != nullptr);
  
  if (queue->head == queue->tail) {
    return true;
  }
  return false;
}

bool is_full(QueueINT *queue){
  assert(queue != nullptr);

  size_t current_head = queue->head;
  
  if (current_head + 1 >= queue->capacity ) {
    current_head = 0;
    if(current_head == queue->tail){
      return true;
    }
  }
  
  if (current_head + 1 == queue->tail) {
    return true;
  }
  
  return false;
}

int peek(QueueINT *queue){
  assert( queue != nullptr);

  return *(queue->base + queue->tail);
}

void flush(QueueINT *queue){
  assert(queue != nullptr);

  memset(queue->base, 0 ,sizeof(int) * queue->capacity);
}
}
