#ifndef clox_value_h
#define clox_value_h

#include "common.h"

typedef double Value;

void print_value(Value value);

typedef struct {
  int capacity;
  int count;
  Value* values;  
} ValueArray;


void init_value_array(ValueArray* value_array);
void write_value_array(ValueArray* value_array, Value value);
void free_value_array(ValueArray* value_array);

#endif
