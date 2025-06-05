#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "vm.h"
#include <stdio.h>
#include <stdlib.h>

#define REPL_LINE_LIMIT 1024

static void repl() {
  char line[REPL_LINE_LIMIT];
  for (;;) {
    printf("> ");
    if (!fgets(line, sizeof(line), stdin)) {
      printf("\n");
      break;
    }

    interpret(line);
  }
}

static char *read_file(const char *path) {
  // NOTE: r = open in read mode, b = binary
  //  b is ignored in POSIX compliant systems and is here for
  //  platforms that treat text files and binaries differently
  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    fprintf(stderr, "Could not open file: %s\n", path);
    exit(74);
  }

  // Seek to the end of the file
  fseek(file, 0, EOF);
  // Return the cursor position
  size_t file_size = ftell(file);
  // Rewind cursor to the beginning of the stream (file)
  rewind(file);

  // Now that we know the size of our file, allocate the corresponding
  // amount of memory + 1 to make room for the null terminator
  char *buffer = (char *)malloc(file_size + 1);
  if (buffer == NULL) {
    fprintf(stderr, "Could not allocate the memory requested: %zu\n",
            file_size + 1);
    exit(74);
  }

  // Read the file into the buffer, append the null terminator to the buffer
  // + close th file
  size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
  if (bytes_read != file_size) {
    fprintf(stderr,
            "Did not read the full file. file size: %zu, bytes read: %zu\n",
            file_size, bytes_read);
    exit(74);
  }

  buffer[bytes_read] = '\0';
  fclose(file);

  return buffer;
}

static void run_file(const char *path) {
  char *source = read_file(path);
  InterpretResult result = interpret(source);
  free(source);

  if (result == INTERPRET_COMPILE_ERROR)
    exit(65);
  if (result == INTERPRET_RUNTIME_ERROR)
    exit(70);
}

int main(int argc, const char *argv[]) {
  init_vm();

  if (argc == 1) {
    repl();
  } else if (argc == 2) {
    run_file(argv[1]);
  } else {
    fprintf(stderr, "Usage: clox [path]\n");
  }

  free_vm();
  return 0;
}
