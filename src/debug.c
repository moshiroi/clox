#include <stdint.h>
#include <stdio.h>
#include "debug.h"
#include "chunk.h"
#include "value.h"

void disassemble_chunk(Chunk *chunk, const char *name){
  printf("=== %s ===\n", name);

  for (int offset = 0; offset < chunk->count;) {
    offset = disassemble_instruction(chunk, offset);
  }
}

int disassemble_instruction(Chunk *chunk, int offset){
  printf("%04d ", offset);

  uint8_t instruction = chunk->code[offset];

  switch (instruction) {
    case OP_RETURN:
      return simple_instruction("OPRETURN", offset);
    case OP_CONSTANT:
      return constant_instruction("OP_CONSTANT", chunk, offset);
    default:
      printf("Unknown instruction - %d\n", instruction);
      return offset + 1;
  }  
}

int simple_instruction(const char* name, int offset) {
  printf("%s\n", name);
  // **pointing to the next instr**
  return offset + 1; 
}

int constant_instruction(const char* name, Chunk* chunk, int offset) {
  uint8_t constant = chunk->code[offset+1]; //+1?
  printf("%-16s %4d '", name, constant);
  print_value(chunk->constants.values[constant]);
  printf("\n");
  // **pointing to the next instr**
  return offset + 2; // +2 because -> 1 for instruction, 1 for operand
  }
