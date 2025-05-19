#include "chunk.h"
#include "debug.h"
#include "common.h"

int main(int argc, const char* argv[]) {
  Chunk chunk;
  init_chunk(&chunk);
  int constant = add_constant(&chunk, 1.2);
  write_chunk(&chunk, constant);
  // QUESTION: are empty enum variants size of 1byte?
  write_chunk(&chunk, OP_RETURN);
  disassemble_chunk(&chunk, "test chunk");
  free_chunk(&chunk);
  return 0;
}
