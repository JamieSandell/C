/* Write a program to check a C program for rudimentary syntax
errors like unbalanced parantheses, brackets and braces. Don't forget about
quotes, both single and double, escapes sequences, and comments. (This
program is hard if you do it with full generality.) */

#include "stdio.h"

#define CODE            0
#define START_COMMENT   1
#define COMMENT         2
#define END_COMMENT     3
#define QUOTE           4

int main()
{
    int state, this_char, quote_char;
    int parantheses, brackets, braces;
    parantheses = braces = braces = 0;

    state = CODE;
    while((this_char = getchar()) != EOF)
    {
        if (state == CODE)
        {
            if (this_char == '/') { state == START_COMMENT; }
            else if(this_char == '(') { parantheses++; }
            else if(this_char == '[') { brackets++; }
            else if(this_char == '{') { braces++; }
        }
        else if(state == START_COMMENT)
        {
            if (this_char == '*') { state = COMMENT; }
        }
        else if(state == COMMENT)
        {
            if (this_char == '/') { state == END_COMMENT; }
        }
    }

    return 0;
}