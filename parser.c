#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "lexer.h"
#include "instruction.h"

void wicProg();
void instrList();
void instr();
void iLP();
void Eols();
void iPrime();
char * getCode();

int address = 0;
int currTok;
char * currentT;
char tempOpcode[500];

void parse()
{
    currTok = getToken();
    wicProg();
    if(currTok != EOP_TOK)
    {
        printf("Expected End of File, but got %s\n",
                getCode());
    }
}

void wicProg()
{
    if((currTok == WORD_TOK)||(currTok == EOL_TOK))
    {
        instrList();
        Eols();
    }
    else
    {
        printf("Token Mismatch: expected Word and End Of Line but got Unknown %s \n",
                getCode());
    }
}

void match(int tok)
{
    if(currTok != tok)
    {
        printf("The expected token is not matched with current token \n");
    }
    currTok = getToken();
}

void instrList()
{
    if((currTok == WORD_TOK)||(currTok == EOL_TOK))
    {
        Eols();
        instr();
        match(EOL_TOK);
        iLP();
    }
    else
    {
        printf("Expect Word or End Of Line for the beginning of the instruction but got %s \n",
               getCode());
    }
}

void instr()
{
    if(currTok == WORD_TOK)
    {
        strcpy(tempOpcode, getLexeme());
        match(WORD_TOK);
        iPrime();
    }
    else
    {
        printf("Expected Word Token but got %s",getCode());
    }
}

void iLP()
{ 
    if((currTok == WORD_TOK)||(currTok == EOL_TOK))
    {
        instrList();
    }
    else
    {
    }
}

void Eols()
{
    if(currTok == EOL_TOK)
    {
        match(EOL_TOK);
        Eols();          
    }
    else
    {
    }
}

void iPrime()
{
    if(currTok == WORD_TOK)
    {
        insertInstruction(address++, tempOpcode, getLexeme());    
        match(WORD_TOK);
    }
    else if(currTok == NUM_TOK)
    {
        insertInstruction(address++, tempOpcode, getLexeme());    
        match(NUM_TOK);
    }
    else
    {
        insertInstruction(address++, tempOpcode, "");    
    }
}

char * getCode()
{
   switch (currTok) {
      case 101 : currentT = "Word";
         break;
      case 102 : currentT = "Number";
         break;
      case 103 : currentT = "End of Line";
         break;
      case 105 : currentT = "End of Program";
         break;
      case 104 : currentT = "Unkown";
         break;
      default : currentT = "??";
   }
   return currentT;
}

