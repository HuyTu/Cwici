/*
 * lexer.c file for lexer component
 */
#include <stdio.h>
#include "lexer.h"

#ifndef LEXER_C
#define LEXER_C

int currTok;
int current; 
int currState;
char lexeme[250];
FILE * file;
int state7Flag;

void initLexer(char *fileName)
{
    current = 0;
    currState = 0;
    state7Flag = 0; 
    lexeme[current] = '\0'; 
    file = fopen(fileName, "r");
    if(file == NULL) 
    {
        perror("Error in opening file");
    }    
}

int getToken()
{
    current = 0;
    lexeme[0] = '\0'; 
    //fprintf(stderr,"inside getToken--------------------------------------------\n");
    if(state7Flag)
    {
        state7Flag = 0;
        return EOL_TOK;
    }
    if(currState == 5 || currState == 6)
    {
        currState = 0;
    }
    int a = 1;
    while(a)
    {
        char chr = fgetc(file); 
        //fprintf(stderr,"inside loop before switch: chr=%c currState=%d\n",chr,currState);
        if (chr == EOF)
        {
            return EOP_TOK;
        }
        switch(currState)
        {
            case 0:
                if( chr == '%')
                {
                    currState = 1;
                }
                else if( chr == '\n')
                {
                    currState = 5;
                }
                else if((chr == ' ') || (chr == '\t'))
                {
                    currState = 2;
                }
                else if('0' <= chr && chr <= '9')
                {
                    currState = 3;
                    lexeme[current] = chr;
                    current++;
                    currTok = NUM_TOK;
                }
                else if(('a' <= chr && chr <= 'z') 
                        || ('A' <= chr && chr <= 'Z'))
                {
                    currState = 4;
                    lexeme[current] = chr;
                    current++;
                    currTok = WORD_TOK; 
                }
                else
                {
                    currState = 8;
                }
                break;
            case 1:
                if ( chr == '\n')
                {
                    currState = 5;
                }
                else
                {
                    currState = 1;
                }
                break;
            case 2:
                if('0' <= chr && chr <= '9')
                {
                    currState = 3;
                    currTok = NUM_TOK;
                    lexeme[current] = chr;
                    current++;
                }
                else if(('a' <= chr && chr <= 'z') 
                        || ('A' <= chr && chr <= 'Z'))
                {
                    currState = 4;
                    currTok = WORD_TOK;
                    lexeme[current] = chr;
                    current++;
                }
                else if(chr == '%')
                {
                    currState = 1;
                }
                else if(chr == '\n')
                {
                    currState = 5;
                }
                else if((chr == ' ') || (chr == '\t'))
                {
                    currState = 2;
                }
                else
                {
                    currState = 8;
                }
                break;
            case 3:
                if (chr == '\n')
                {
                    currState = 7;
                    state7Flag = 1;
                }
                else if((chr == ' ') || (chr == '\t'))
                {
                    lexeme[current] = '\0';
                    currState = 6;
                }
                else if('0' <= chr && chr <= '9')
                {
                    currState = 3;
                    currTok = NUM_TOK;
                    lexeme[current] = chr;
                    current++;
                }
                else
                {
                    currState = 8;
                }
                break;
             case 4:
                if (chr == '\n')
                {
                    currState = 7;
                    state7Flag = 1;
                }
                else if((chr == ' ') || (chr == '\t'))
                {
                    currState = 6;
                }      
                else if(('a' <= chr && chr <= 'z') 
                        || ('A' <= chr && chr <= 'Z')
                        || ('0' <= chr && chr <= '9'))
                {
                    currState = 4;
                    currTok = WORD_TOK;
                    lexeme[current] = chr;
                    current++;
                }
                else
                {
                    currState = 8;
                }
                break;
            case 8:
                perror("Error");
                break;
        }

        if( currState == 5)
        {
            return EOL_TOK;
            currState = 0;
            a = 0;
        }  
        else if(currState == 6)
        {
            lexeme[current] = '\0';
            currState = 0;
            return currTok;
            a = 0;
        }        
        else if(currState == 7)
        {            
            lexeme[current] = '\0';
            currState = 0;
            return currTok;
            a = 0;
        }
    } 
}


char * getLexeme()
{
    return lexeme; 
}
int getLineNumber(); // optionally can provide helpful error messages

#endif
