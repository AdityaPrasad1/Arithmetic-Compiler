#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

bool isNumber(char *integer) {		// check if valid integer
  int i = 0;
  if(integer[0] == '-') {		// check if negative integer or subtraction operation
    if(integer[1] == 0) {               // return false if subtraction sign
      return false;
    }
    i++;
  }
  for(; integer[i] != 0; i++) {
    if(!isdigit(integer[i])) {		// if what follows neg sign isn't a digit, return false
      return false;
    }
  }
  return true;
} 

int main(int argc, char *argv[]) {
  int i, count = 0;
  if(argc <= 2) {			// if not enough arguments on command line, return error
    fprintf(stderr, "Error: not enough operands or operators\n");
    return 1;
  }
  if(argv[1][0] == '+' || argv[1][0] == '-' || argv[1][0] == '*') {
    fprintf(stderr, "Error: not in RPN form\n");
    return 1;				// if exp starts with operator, return error
  }
  for(i = 1; i < argc; i++) {		// checks if entry is a valid RPN expression
    if(isNumber(argv[i])) {		// vars/constants weighted as 1, operator weighted as -1
      count = count + 1;
    }
    else if(argv[i][0] == 'x' || argv[i][0] == 'y' || argv[i][0] == 'z') {
      count = count + 1;
    }
    else if(argv[i][0] == '+' || argv[i][0] == '-' || argv[i][0] == '*') { 
      count = count - 1;
    }
    else {				// if entry is not x, y, z, op, or int, return error
      fprintf(stderr, "Error: invalid variable\n");
      return 1;
    }
    if(count <= 0) {			// return error if there are too many ops
      fprintf(stderr, "Error: too many operators or not enough operands\n");
      return 1;
    }
  }
  if(count != 1) {			// return error if there are too many operands
    fprintf(stderr, "Error: too many operands or not enough operators\n");
    return 1;
  }
  else {
    printf("  .globl compute\ncompute:\n");
    for(i = 0; i < argc; i++) {
      if(isNumber(argv[i])) {
        printf("  pushq $%s\n", argv[i]);
      }
      else if(argv[i][0] == 'x') {
        printf("  pushq %%rdi\n");
      }
      else if(argv[i][0] == 'y') {
        printf("  pushq %%rsi\n");
      }
      else if(argv[i][0] == 'z') {
        printf("  pushq %%rdx\n");
      }
      else if(argv[i][0] == '+') {
        printf("  popq  %%r10\n  popq  %%r11\n  addq  %%r10, %%r11\n  pushq %%r11\n");
      }
      else if(argv[i][0] == '-') {
        printf("  popq  %%r10\n  popq  %%r11\n  subq  %%r10, %%r11\n  pushq %%r11\n");
      }
      else if(argv[i][0] == '*') {
        printf("  popq  %%r10\n  popq  %%r11\n  imulq %%r10, %%r11\n  pushq %%r11\n");
      }
    }
    printf("  popq  %%rax\n  retq\n");
    return 1;
  }
}
