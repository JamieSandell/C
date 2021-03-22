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
void print_word(char word[], int word_start, int word_end);

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
    int word_char_count = 0;
    int line_word_count = 0;
    int word_start = 0;
    int word_end = 0;
    int last_blank_pos = -1;
    char prev_char = ' ';
    char curr_char = ' ';
    for (int curr_pos = 0; curr_pos <= input_length; ++curr_pos)
    {
        // TODO work with tabs
        ++line_char_count;
        curr_char = input[curr_pos];

        if (word_char_count == 0) // Found the start of a word. Potentially starts with whitespace, but that's ok
        {
            word_start = curr_pos;
            ++word_char_count;
        }
        else
        {
            word_char_count++;
        }
        // Have we found the end of a word?
        // Also make sure we are less than the line width, otherwise we'd overflow it
        if (curr_char == ' ' && prev_char != ' ' && line_char_count < MAX_LINE_WIDTH)
        {
            ++word_char_count;
            line_word_count++;
            word_end = curr_pos; // want to print the word including the space at the end
            print_word(input, word_start, word_end);
            word_char_count = word_start = word_end = 0; // Reset for the next word
        }
        // Have we reached the end of the line?
        else if (line_char_count == MAX_LINE_WIDTH)
        {
            // peek at the next character if there's more input available
            if (curr_pos < input_length)
            {
                if (input[curr_pos+1] == ' ') // end of the word
                {
                    ++word_char_count;
                    word_end = curr_pos;
                    print_word(input, word_start, word_end);
                    word_char_count = word_start = word_end = 0;
                    curr_pos++; // the next character is the space before the next word, so want to move forward for the next iteration in the loop
                }
                else if(input[curr_pos] != ' ' && input[curr_pos + 1] == ' ')
                {
                    ++word_char_count;
                    word_end = curr_pos;
                    print_word(input, word_start, word_end);
                    word_char_count = word_start = word_end = 0;
                    curr_pos++; // the next character is the space before the next word, so want to move forward for the next iteration in the loop
                }
                else if(input[curr_pos] == ' ')
                {
                    ++word_char_count;
                    word_end = curr_pos;
                    print_word(input, word_start, word_end);
                    word_char_count = word_start = word_end = 0;
                }
                else // do something intelligent with the line now that it has reached the max line width without it being a complete word
                {
                    if(line_word_count == 0)
                    {
                        // 0 complete words this line so print the line as is
                        word_char_count++;
                        word_end = curr_pos;
                        print_word(input, word_start, word_end);
                        word_char_count = word_start = word_end = 0;
                    }
                    else
                    {
                        // at least one complete word this line, move the start of the current word (-1 for the loop increment) so it starts on the next line
                        curr_pos = word_start - 1;
                        line_word_count = 0;
                        word_char_count = 0;
                        word_start = 0;
                        word_end = 0;
                    }
                }
                putchar('\n'); // no more characters allowed this line so move down to the next
            }
            line_word_count = line_char_count = 0; // reset for the next line
        }

        prev_char = curr_char;
    }
    //putchar('\n'); // terminate the line
}

void print_word(char word[], int word_start, int word_end)
{
    for (int i = word_start; i <= word_end; ++i)
    {
        putchar(word[i]);
    }
}