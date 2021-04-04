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
    int left_parantheses, left_brackets, left_braces, right_parantheses, right_brackets, right_braces;
    left_parantheses = left_braces = left_brackets = right_parantheses = right_braces = right_brackets = 0;

    state = CODE;
    while((this_char = getchar()) != EOF)
    {
        if (state == CODE)
        {
            if (this_char == '/') { state == START_COMMENT; }
            if (this_char == '\'' || this_char == '"')
            {
                state == QUOTE;
                quote_char = this_char;
            }
            else if(this_char == '(') { left_parantheses++; }
            else if(this_char == '[') { left_brackets++; }
            else if(this_char == '{') { left_braces++; }
            else if(this_char == ')') { right_parantheses++; }
            else if(this_char == ']') { right_brackets++; }
            else if(this_char == '}') { right_braces++; }
        }
        else if(state == START_COMMENT)
        {
            if (this_char == '*') { state = COMMENT; }
            state == CODE;
        }
        else if(state == COMMENT)
        {
            if (this_char == '*') { state == END_COMMENT; }
        }
        else if(state == END_COMMENT)
        {
            if (this_char == '/') { state = CODE; }
            else if(this_char != '*') { state = COMMENT; } /* stay in END_COMMENT if input looked like /***** for example, otherwise back to COMMENT */
        }
    }

    if (left_braces != right_braces) { printf("Error: Left brace and right brace mismatch.\n"); }
    else if(left_brackets != right_brackets) { printf("Error: Left brackets and right brackets mismatch.\n"); }
    else if(left_parantheses != right_parantheses) { printf("Error: Left parantheses and right parantheses mismatch.\n"); }
    else { printf("Success.\n"); }

    return 0;
}