/**
 * This file is under MIT Licence
 * Copyright (C) 2015 Alexandre BM <s@rednaks.tn>
 *   
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *   
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *   
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include <stdio.h>
#include <stdlib.h>

#define memory_size 30000

/*
  This is a brainf** non-interactive intepreter.
*/


unsigned long search_end_pos(FILE *fp) {
  long start = ftell(fp);

  while(fgetc(fp) != ']');
  long end = ftell(fp);
  fseek(fp, start, SEEK_SET);

  return end;
}
int main(int argc, char **argv){

/* for a given array of 30K bytes, initialized to 0, we have 8 instructions :

  '>' : increments the position of the pointer.
  '<' : decrements the position of the pointer.
  '+' : increments the value of the pointer.
  '-' : decrements the value of the pointer.
  '.' : prints out the value of the pointer.
  ',' : inputs a value.
  '[' : if current pointer is 0 jumps to the inscruction after ']'.
  ']' : if current pointer is not 0 jumps back to '['

  any other charachter should be ignored.

  example : 
  memory before : 0|0|0|0|0|0
  program : ++>+>>-
  memory after : 2|1|0|255|0|0
  
*/

  FILE* fp;
  
  if(argc < 2) {
    printf("Error : no input files\n");
    return -1;
  }

  fp = fopen(argv[1], "r");

  if(!fp) {
    printf("Error while opening %s\n", argv[1]);
    return -1;
  }


  char ic = 0; // instruction counter
  // we'll need to store the positions of '[' and ']'
  // right now let's keep it simple, and assume that our bf code contains only
  // one of these insctructions.

  unsigned long bracket_start_position = 0;
  unsigned long bracket_end_position = 0;


  char* ptr = (char*) malloc(sizeof(char)*memory_size);

  do
  {
    ic = fgetc(fp);
    switch(ic) {
      case '>': ptr++;  break;
      case '<': ptr--;  break;
      case '+': (*ptr)++; break;
      case '-': (*ptr)--; break;
      case '.': putc(*ptr, stdout); break;
      case ',': *ptr = getc(stdin); break;
      case '[': 
            bracket_start_position = ftell(fp);
            bracket_end_position = search_end_pos(fp);
            if(!(*ptr)){ // jump to instruction after ']'
              fseek(fp, bracket_end_position+1, SEEK_SET);
            }
      case ']':
            if(*ptr){ // jump to instruction after '['
              fseek(fp, bracket_start_position+1, SEEK_SET);
            }
    }

  }while(ic != EOF);

  return 0;
}
