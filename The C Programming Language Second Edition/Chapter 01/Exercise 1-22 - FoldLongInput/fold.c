/*Write a program to "fold" long input lines into two or more
shorter lines after the last non-blank character that occurs before the n-th
column of input. Make sure your program does something intelligent with very
long lines, and if there are no blanks or tabs before the specified column.*/

#include <stdio.h>

#define MAX_LINE_WIDTH 20
#define MAX_INPUT_LENGTH  1000
#define TAB_SIZE 8

int get_last_non_blank_character_index(char input[], int input_length);
int get_line(char input[]);
void fold_input(char input[], int input_length);

int main()
{
    char input[MAX_INPUT_LENGTH + 1]; //+1 for the null-terminating character
    int line_length = 0;
    char folded_line[MAX_LINE_WIDTH + 1]; //+1 for the null-terminating character

    while ((line_length = get_line(input)) > 0)
    {
        fold_input(input, line_length);
    }

    return 0;
}

//Returns last non-blank character index that occurs before input_length
int get_last_non_blank_character_index(char input[], int input_length)
{
    int current_column = 0;
    int last_non_blank_character_index = 0;
    for (int i = 0; i < input_length && current_column < input_length; ++i)
    {
        if (input[i] != ' ' && input[i] != '\t')
        {
            last_non_blank_character_index = i;
            ++current_column;
        }
        else if(input[i] == ' ')
        {
            ++current_column;
        }
        else
        {
            current_column += TAB_SIZE;
        }        
    }

    return last_non_blank_character_index;
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

/*
Need to be passed the full input
Need to cycle the input
count the characters and don't let it exceed the max line width
and store that in a temp array
then cycle the temp array and make sure it isn't cutting off part of a word
if it is shorten it
*/
void fold_input(char input[], int input_length)
{
    if (input_length <= MAX_LINE_WIDTH)
    {
        printf("%s\n", input); //input doesn't need folding
        return;
    }

    int blank_index = 0;
    int character_count = 0;
    int total_character_count = 0;
    for (int i = 0; i < input_length;)
    {
        for (int j = i; character_count <= MAX_LINE_WIDTH && input[j] != '\n'; ++j)
        {
            ++total_character_count;

            if (input[j] == ' ')
            {
                blank_index = j;
                character_count++;
            }
            else if (input[j] == '\t')
            {
                character_count += TAB_SIZE;
                if (character_count <= MAX_LINE_WIDTH)
                {
                    blank_index = j;
                }
            }
            else
            {
                character_count++;
            }
        }

        if (blank_index > 0)
        {
            for (int j = i; j < blank_index; ++j)
            {
                putchar(input[j]);
            }
        }
        else
        {
            for (int j = i; j < character_count; ++j)
            {
                putchar(input[j]);
            }
        }  
        putchar('\n');
        
        i = total_character_count - blank_index;
        character_count = 0;
        blank_index = 0;
    }    
}