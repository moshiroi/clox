#include "chunk.h"
#include "common.h"

int main(int argc, const char* argv[]) {
  Chunk chunk;
  init_chunk(&chunk);
  // QUESTION: are empty enum variants size of 1byte?
  write_chunk(&chunk, OP_RETURN);
  free_chunk(&chunk);
  return 0;
}
