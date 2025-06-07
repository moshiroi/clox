#ifndef clox_chunk_h
#define clox_chunk_h

#include "value.h"

// Bytecode instructions - optional accompanying operand
typedef enum {
  // Expects a constant operand to follow it.
  // Must provide the index of the constant value in the corresponding chunks ValueArray
  OP_CONSTANT,
  OP_NEGATE,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_RETURN
} OpCode;

typedef struct {
  int count;
  int capacity;
  uint8_t *code;
  int *lines;
  // NOTE: Since the OP_CONSTANT instruction uses a single byte for the index operand, we can store and load only up to 256 constants in a chunk.
  ValueArray constants;
} Chunk;

void init_chunk(Chunk *chunk);
void write_chunk(Chunk *chunk, uint8_t byte, int line);
void free_chunk(Chunk *chunk);

int add_constant(Chunk *chunk, Value value);

#endif
