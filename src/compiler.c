
#include <stdio.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"

// NOTE: C tosses enum names in the top-level namespace
void compile(const char* source) {
  init_scanner(source);
}
