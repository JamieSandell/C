/*Write a program to "fold" long input lines into two or more
shorter lines after the last non-blank character that occurs before the n-th
column of input. Make sure your program does something intelligent with very
long lines, and if there are no blanks or tabs before the specified column.*/

#include <stdio.h>

#define MAX_LINE_WIDTH 20
#define MAX_INPUT_LENGTH  1000
#define TAB_SIZE 8

int get_line(char input[]);
void fold_input(char input[], int input_length);

int main()
{
    char input[MAX_INPUT_LENGTH + 1]; //+1 for the null-terminating character
    int line_length = 0;

    while ((line_length = get_line(input)) > 0)
    {
        fold_input(input, line_length);
    }

    return 0;
}

//Returns the character count (excluding the null terminating character)
int get_line(char input[])
{
    int c = 0;
    int character_count = 0;
    while ((c = getchar()) != EOF && character_count < MAX_INPUT_LENGTH && c != '\n')
    {
        input[character_count] = c;
        ++character_count;       
    }
    input[character_count] = '\0'; //null terminate the string

    return character_count;
}

void fold_input(char input[], int input_length)
{
    if (input_length <= MAX_LINE_WIDTH)
    {
        printf("%s\n", input); //input doesn't need folding
        return;
    }

    int line_char_count = 0;
    int word_start = 0;
    int word_end = 0;
    int last_blank_pos = 0;
    char prev_char = ' ';
    char curr_char = ' ';
    for (int curr_pos = 0; curr_pos < input_length; ++curr_pos)
    {
        ++line_char_count;

        curr_char = input[curr_pos];
        if (prev_char == ' ' && curr_char != ' ') //start of a word
        {
            word_start = curr_pos;
        }
        else if ((curr_char == ' ' && prev_char != ' ') || (curr_pos == input_length - 1)) //end of a word or end of input
        {
            last_blank_pos = curr_pos;
            word_end = curr_pos;
            if (line_char_count <= MAX_LINE_WIDTH)
            {
                for (int i = word_start; i < word_end; ++i)
                {
                    putchar(input[i]);
                }
                putchar(curr_char);
            }
        }
        if (line_char_count == MAX_LINE_WIDTH)
        {
            putchar('\n');
            curr_pos = last_blank_pos;
            line_char_count = 0;
        }

        prev_char = curr_char;
    }
    putchar('\n');
}