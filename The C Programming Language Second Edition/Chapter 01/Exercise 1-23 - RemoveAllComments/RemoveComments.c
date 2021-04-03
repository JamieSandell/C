/* Write a program to remove all comments from a C program.
Don't forget to handle quoted strings and character constants properly.
C comments do not nest. */

#include "stdio.h"

#define PROGRAM         0
#define BEGIN_COMMENT   1
#define COMMENT         2
#define END_COMMENT     3
#define QUOTE           4

int main()
{
    int this_char, quote_char, state;
    state = PROGRAM;

    while((this_char = getchar()) != EOF)
    {
        if (state == PROGRAM)
        {
            if (this_char == '/')
            {
                state = BEGIN_COMMENT;
            }
            else if(this_char == '\'' || this_char == '"')
            {
                state = QUOTE;
                quote_char = this_char; /* single variable to handle both types of quote, ' or " */
            }
            else
            {
                putchar(this_char);
            }
        }
        else if (state == BEGIN_COMMENT)
        {
            if (this_char == '*')
            {
                state = COMMENT;
            }
            else
            {
                putchar('/'); /* Potentially had a comment, but it turns out it was a slash followed by a non-* character */
                state = PROGRAM;
            }
        }
        else if(state == COMMENT)
        {
            if (this_char == '*')
            {
                state = END_COMMENT;
            }
        }
        else if (state == END_COMMENT)
        {
            if (this_char == '/')
            {
                state = PROGRAM;
            }
            else if(this_char != '*')
            {
                state = COMMENT; /* Still in a comment, but we can't be end of the comment as we need a * before a / */
            }
        }
    }

    return 0;
}