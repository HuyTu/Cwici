#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "instruction.h"
#include "table.h"
#include "stack.h"
#include "lexer.h"


/* An alternate C design: don't expose internal data structure.
 * Previously (stack and table), I showed the data structure and 
 * prototyped the methods.
 * Here, we keep the structure hidden inside the .c file and only expose
 * the methods.  (You might be thinking "hidden?" how is it hidden since
 * you can just look at it.  But consider that I could deliver the header
 * and an object file or library where you couldn't see it.)
 */


typedef struct{
    char opcode[MAX_FIELD];
    char operand[MAX_FIELD];
} instructionType;

typedef struct
{
    int numItems;
    instructionType instrE[MAX_PROGRAM_SIZE]; 
} instructionTable;

instructionTable *instrT;
stackType stack;
tableType table;
tableType tableJ;
int halt(int pc); 
void put(char *operand);
void get(char *operand);
void pop(char *operand);
void push(char *operand);
void addition();
void subtraction();
void division();
void multiplication();
void and();
void or();
void not();
void tsteq();
void tstne();
void tstlt();
void tstle();
void tstgt();
void tstge();
int jump(char *operand);
int jf(int count, char *operand);

/* Prepares instruction structures for execution. */
void initialize() 
{
    instrT = (instructionTable*) malloc(sizeof(instructionTable));
    instrT->numItems = 0;
    initTable(&table);
    initTable(&tableJ);
    initStack(&stack);
}

/* Called from within parser as instructions are parsed/encountered.
 *  All of these parameters are "in" parameters.
 * As instructions are inserted, we must also do any other duties:
 * + "label instructions" must add to the "jump table"
 * + "label instructions" are "backwards" with operand (label) first
 *     and opcode (the word "label" itself) second
 */
void insertInstruction(int address, char *opcode, char *operand)
{
    char temp[MAX_FIELD+1];
    if (strcmp(operand, "label") == 0)
    {   
        strcpy(temp, opcode);
        strcpy(opcode, operand);
        strcpy(operand, temp);
        store(&tableJ, operand, address);
    }
    
    instructionType tempInst;
    strcpy(tempInst.opcode, opcode);
    strcpy(tempInst.operand, operand);

    instrT -> instrE[address] = tempInst;
    instrT -> numItems++; 
}
/* Instructions are fetched during "execution" of WIC program.
 * address is an "in" parameter, 
 * opcode and operand are "out" parameters
 * operand may be NULL for opcode-only instructions like "add"
 */
void fetchInstruction(int address, char *opcode, char *operand)
{
    strcpy(opcode, instrT->instrE[address].opcode);
    strcpy(operand, instrT->instrE[address].operand);
}
/* Using a lexer/parser, we don't need the helper hasOperand function,
 * but if you wrote it, it is okay.  The book's way of reading input file
 * needed it because it needed to know if next thing read (next token) was
 * operand or next instruction.  Our grammar imparts this structure/knowledge.
 *
 * int hasOperand(char *opcode);
 */

/* Usual debugging method... */
void printInstructions()
{
    int i;
    for( i = 0; i < instrT->numItems; i++)
    {
         printf("The current opcode: %s and the current operand: %s\n", 
                instrT->instrE[i].opcode, 
                instrT->instrE[i].operand);
    }
}
/* Lastly, we need prototypes for methods that will perform/execute the
 * WIC instructions.  As a first test, we'll work for the simplest WIC 
 * program (a single halt instruction).  As you increase your testing by 
 * adding additional instructions, you'll need to add to this list of
 * prototypes.
 *
 * Note that there are some name conflicts with pre-defined functions.  For
 * example, "div" is in a library somewhere.  So try a div prototype, see the
 * error notice, then rename your method (maybe something like "divide" or
 * "wic_div" or whatever.
 */
int halt(int count)
{
    printTable(&table);
    printf("program halted\n");
    return EOP_TOK;
}

void get(char *operand)
{
    int entry;
    char input[MAX_FIELD];
    printf("Input a variable: %s > ", operand);
    scanf("%d", &entry);  
    store(&table, operand, entry);
}

void put(char *operand)
{
    int entry = retrieve(&table, operand);
    printf("The operand %s = %d\n", operand, entry);
}

void push(char *operand)
{
    int entry;
    if(isdigit(operand[0]) || isdigit(operand[1]))
    {
        entry = atoi(operand);
    }
    else
    {
        entry = retrieve(&table, operand);
    }
    stackPush(&stack, entry);       
}

void pop(char *operand)
{
    int entry = stackPop(&stack);
    store(&table, operand, entry);
}

void addition()
{
    int entryR = stackPop(&stack);
    int entryL = stackPop(&stack);
    stackPush(&stack, entryL+entryR);
}

void subtraction()
{
    int entryR = stackPop(&stack);
    int entryL = stackPop(&stack);
    stackPush(&stack, entryL - entryR);
}
void multiplication()
{
    int entryR = stackPop(&stack);
    int entryL = stackPop(&stack);
    stackPush(&stack, entryL * entryR);
}

void division()
{
    int entryR = stackPop(&stack);
    int entryL = stackPop(&stack);
    if(entryR == 0)
    {
        printf("error because trying to divide zero...\n");
        exit(1);
    }
    stackPush(&stack, entryL / entryR);
}

void not()
{
    int entry = stackPop(&stack);
    stackPush(&stack, !entry); 
}

void and()
{
    int entryR = stackPop(&stack);
    int entryL = stackPop(&stack);
    stackPush(&stack, entryL && entryR);
}

void or()
{
    int entryR = stackPop(&stack);
    int entryL = stackPop(&stack);
    stackPush(&stack, entryL || entryR);
}

void tsteq()
{
    int entry = stackPop(&stack);
    if(entry == 0)
        stackPush(&stack, 1);
    else 
        stackPush(&stack, 0);
}

void tstne()
{
    int entry = stackPop(&stack);
    if(entry != 0)
        stackPush(&stack, 1);
    else
        stackPush(&stack, 0);
}

void tstlt()
{
    int entry = stackPop(&stack);
    if(entry < 0)
        stackPush(&stack, 1);
    else
        stackPush(&stack, 0);
}

void tstle()
{
    int entry = stackPop(&stack);
    if(entry <= 0)
        stackPush(&stack, 1);
    else
        stackPush(&stack, 0);
}

void tstgt()
{
    int entry = stackPop(&stack);
    if(entry > 0)
        stackPush(&stack, 1);
    else
        stackPush(&stack, 0);
}

void tstge()
{
    int entry = stackPop(&stack);
    if(entry >= 0)
        stackPush(&stack, 1);
    else
        stackPush(&stack, 0);
}

int jump(char *operand)
{
    int count = retrieve(&tableJ, operand);
    return count;
}

int jf(int count, char *operand)
{
    int entry = stackPop(&stack);
    if(entry == 1)
        return count;
    else
        return retrieve(&tableJ, operand);
}
