/* Write a program to remove all comments from a C program.
Don't forget to handle quoted strings and character constants properly.
C comments do not nest. */

#include "stdio.h"

#define MAX_INPUT 5

typedef int bool;
#define TRUE 1
#define FALSE 0

int get_input(char output[], int output_length);
int strip_comments(char input[], int input_length, char output[], int output_length);


int main()
{
    char input[MAX_INPUT + 1]; /* +1 for the null termination */
    int input_length = get_input(input, MAX_INPUT + 1);
    char output[input_length];
    int output_length = strip_comments(input, input_length, output, input_length); /* At this point the length of output is input_length */
    return 0;
}

/* Reads input until EoF or output_length reached, and stores it in output.
Output is terminated as a null terminated string.
Returns the size (null termination position) of output. */
int get_input(char output[], int output_length)
{
    int c;
    int i;
    for (i = 0; i < output_length; i++)
    {
        c = getchar();
        if (c == EOF)
        {
            break;
        }
        else
        {
            output[i] = c;
        }
    }
    output[i] = '\0';
    return i;
}

/* Strips the comments from the input and puts the result in output.
input_length and output_length should be inclusive of the null termination character.
Returns the size (null termination position) of output. */
int strip_comments(char input[], int input_length, char output[], int output_length)
{
    int output_pos = 0;
    char prev_char = ' ';
    char curr_char;
    bool in_comment = FALSE;
    for (int i = 0; i < input_length - 1; i++)
    {
        curr_char = input[i];
        if (in_comment == FALSE)
        {
            if ((prev_char == '/') && (curr_char == '*'))
            {
                in_comment = TRUE;
            }
        }
        else
        {
            if ((prev_char == '*') && (curr_char == '/'))
            {
                in_comment = FALSE;
            }
        }
        prev_char = curr_char;
    }
    return output_pos;
}