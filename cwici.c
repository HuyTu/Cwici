/*
 * cwici.c A WIC interpreter written in C.
 *
 * This version differs from textbook description in that we use
 * a lexer/parser to handle structured input.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"
#include "lexer.h"
#include "parser.h"
#include "instruction.h"

void execute();

int main(int argc, char *argv[]) {
  char *wicFilename;

  if (argc == 1) {
    // no wic file on command line
    fprintf(stderr, "usage: %s <wic filename>\n",argv[0]);
    exit(1);
  }
  else wicFilename = argv[1];
  initialize();
  initLexer(wicFilename);
  parse();
  printf("Congrats! Looks like that file parsed successfully.\n");
  printf("Here is the instruction list constructed...\n");
  printInstructions();

  printf("\n\nNow attempting to execute the program...\n\n");
  execute();
}

void execute()
{
    int pc = 0;
    char operand[MAX_FIELD];
    char opcode[MAX_FIELD];
    
    while (pc != EOP) { // halt instruction sets program counter to EOP
        fetchInstruction(pc, opcode, operand);
        printf("current instruction: %s\n", opcode);
        if (strcmp(opcode,"halt")==0){
            pc = halt(pc);
            printf("program halted\n");
            exit(EXIT_SUCCESS);
        }
    else if(strcmp(opcode,"get")==0)
        get(operand);
    else if(strcmp(opcode,"put")==0)
        put(operand);
    else if(strcmp(opcode,"push")==0)
        push(operand);
    else if(strcmp(opcode,"pop")==0)
        pop(operand);
    else if(strcmp(opcode,"add")==0)
        addition();
    else if (strcmp(opcode,"sub")==0)
        subtraction();
    else if (strcmp(opcode,"mul")==0)
        multiplication();
    else if (strcmp(opcode,"div")==0)
        division();
    else if(strcmp(opcode,"and")==0)
        and();
    else if(strcmp(opcode,"or")==0)
        or();
    else if(strcmp(opcode,"not")==0)
        not();
    else if(strcmp(opcode,"tsteq")==0)
        tsteq();
    else if(strcmp(opcode,"tstne")==0)
        tstne();
    else if (strcmp(opcode,"tstlt")==0)
        tstlt();
    else if (strcmp(opcode,"tstle")==0)
        tstle();
    else if (strcmp(opcode,"tstgt")==0)
        tstgt();
    else if (strcmp(opcode,"tstge")==0)
        tstge();
    else if (strcmp(opcode,"j")==0)
        pc = jump(operand);
    else if (strcmp(opcode,"jf")==0)
        pc = jf(pc, operand);
    else if (strcmp(opcode,"label")==0){}
    else 
    {
        printf("\nNot a Wic Instruction %s %s\n",opcode, operand);
        exit(1);
    }
    pc++;
  }
}


