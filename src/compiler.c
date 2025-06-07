#include "compiler.h"
#include "scanner.h"
#include "chunk.h"
#include <stdio.h>
#include <stdlib.h>

Parser parser; // Global static parser to avoid passing state via func args
Chunk compiling_chunk; // The chunk currently being compiled

// Reporting a compiler error for a given token
// NOTE: Its apparently good practice to pass const when the function does not mutate the variable - Nystrom does not do this with token here though
// TODO: Refactor to be less nystromy - super ugly 
static void error_at(const Token* token, const char* message) {
  // If parser in panic mode, suppress further errors.
  // This counters cascading errors that arise as a result of the first one.
  if(parser.panic_mode) return;
  
  fprintf(stderr, "[line %d] Error", token->line);
  if (token->type == TOKEN_EOF) {
    fprintf(stderr, " at end");
  } else if (token->type == TOKEN_ERROR) {
    // Nothing.
  } else {
    fprintf(stderr, " at '%.*s'", token->length, token->start);
  }

  fprintf(stderr, "%s\n", message);
  parser.errored = true;
  parser.panic_mode = true; // Error was detected, initiate panic mode
}


// Reporting a compiler error at the current token the parser is processiong
static void error(const char* message){
  error_at(&parser.current, message);
}

// Process tokens produced by scanner, also reporting error tokens.
// We continue to process tokens even if an error is detected
static void advance(){
  parser.previous = parser.current;
  for (;;) {
    parser.current = scan_token();

    if (parser.current.type != TOKEN_ERROR) break;
    error(parser.current.start);
  }
}

// Assert the current token is of a particular type, and proceed if true.
// Otherwise report an error
static void consume(TokenType type, const char* message){
  if(parser.current.type == type){
    advance(); 
    return;
  }

  error(message);
}

// TEMP: Temporary function for returning the current chunk being compiled 
static Chunk* current_chunk(){
  return &compiling_chunk;
}

// Write byte to chunk.
// Could be an OPCODE or an operator for an instruction
static void write_byte(uint8_t byte){
  // QUESTION: Why parser previous and not parser current?
  write_chunk(current_chunk(), byte, parser.previous.line);
}

// Convenience function for writing two bytes to the chunk.
// Typical pattern for writing an OPCODE and its operand e.g. OP_CONSTANT + the constant value
static void write_bytes(uint8_t byte1,uint8_t byte2){
  write_byte(byte1);
  write_byte(byte2);
}

// Where the post compiling cleanup occurs
static void post_compile(){
  // TEMP:
  // We are temporarily using the OP_RETURN instruction to print the result of an expression. So we have the compiler add one of those to the end of the chunk.
  write_byte(OP_RETURN);
}

// Adds the passed in value to the chunks constant array,
// returns the position in the array of the added constant
static uint8_t make_constant(Value value){
  int position = add_constant(&compiling_chunk, value);

  if (position > UINT8_MAX) {
    error("Too many constants in one chunk.");
    return 0; // QUESTION: Why return 0 here!! to continue with processing?
  }
  
  return (uint8_t)position;
};
// Function for converting TOKEN_NUMBER -> number expression
// TOKEN_NUMBER -> OP_CONSTANT + number value written to chunk
static void number(){
  // TODO & QUESTION:
  // Do appropriate error handling here where the str -> float conversion fails.
  // Nystrom does not handle the error here >:O
  double value = strtod(parser.previous.start, NULL);
  write_bytes(OP_CONSTANT, make_constant(value));
}

static void parse_presedence(Precedence precedence){
  // TODO
}

static void expression() {
  // Assignment has the lowest precedence so parsing with that in mind,
  // we end up parsing all expressions
  parse_presedence(PREC_ASSIGNMENT);
}

static void grouping() {
  expression();
  consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

static void unary() {
  TokenType operator_type = parser.previous.type;
  // Compile the operand
  // We only want to parse expressions of equivalent or higher precendence
  parse_presedence(PREC_UNARY);

  switch(operator_type){
    TOKEN_MINUS: write_byte(OP_NEGATE); break;
    default: return; // Unreachable
  }
}


// Returns false if compiler error detected, true otherwise
bool compile(const char* source, Chunk* chunk) {
  init_scanner(source);
  parser.errored = false;
  parser.panic_mode = false;
  advance();
  expression();
  consume(TOKEN_EOF, "Expected end of expression");
  // Temporary
  post_compile();
  return !parser.errored;
};

