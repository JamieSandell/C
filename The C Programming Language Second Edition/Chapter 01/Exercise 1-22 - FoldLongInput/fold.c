/*Write a program to "fold" long input lines into two or more
shorter lines after the last non-blank character that occurs before the n-th
column of input. Make sure your program does something intelligent with very
long lines, and if there are no blanks or tabs before the specified column.*/

#include <stdio.h>

#define MAX_LINE_WIDTH 20
#define MAX_INPUT_LENGTH  1000
#define TAB_SIZE 8

typedef int bool;
#define TRUE  1
#define FALSE 0

void detab(char source[], int source_length, char destination[], int destination_length, int tab_size);
int get_line(char input[]);
int get_number_of_tabs(char input, int input_length);
void fold_input(char input[], int input_length);
void print_word(char word[], int word_start, int word_end);

int main()
{
    char input[MAX_INPUT_LENGTH + 1]; //+1 for the null-terminating character
    int line_length = 0;

    while ((line_length = get_line(input)) > 0)
    {
        fold_input(input, line_length - 1); // ignore the terminating character
    }

    return 0;
}

//Returns the character count (including the null terminating character)
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

int get_number_of_tabs(char input[], int input_length)
{
    int tabs = 0;
    for (int i = 0; i <= input_length; ++i)
    {
        if (input[i] == '\t')
        {
            ++tabs;
        }
    }
    return tabs;
}

void fold_input(char input[], int input_length)
{
    if (input_length <= MAX_LINE_WIDTH)
    {
        printf("%s\n", input); //input doesn't need folding
        return;
    }

    int line_char_count = 0;
    int line_word_count = 0;
    int word_start = -1;
    int word_end = 0;
    char prev_char = ' ';
    char curr_char = ' ';
    bool reset_word_start_end = FALSE;
    for (int curr_pos = 0; curr_pos <= input_length; ++curr_pos)
    {
        // TODO work with tabs
        if (word_start == -1) // Found the start of a word. Potentially starts with whitespace, but that's ok
        {
            word_start = curr_pos;
        }
        curr_char = input[curr_pos];
        ++line_char_count;

        if(curr_pos == input_length)
        {
            // print what we have as there are no more characters to process
            word_end = curr_pos;
            print_word(input, word_start, word_end);
            putchar('\n');
            reset_word_start_end = TRUE;
        }
        // Have we found the end of a word?
        // Also make sure we are less than the line width, otherwise we'd overflow it
        else if (curr_char == ' ' && prev_char != ' ' && line_char_count < MAX_LINE_WIDTH)
        {
            line_word_count++;
            word_end = curr_pos; // want to print the word including the space at the end
            print_word(input, word_start, word_end);
            reset_word_start_end = TRUE;
        }
        // Have we reached the end of the line?
        else if (line_char_count == MAX_LINE_WIDTH)
        {
            // peek at the next character if there's more input available
            if (curr_pos < input_length)
            {
                if (input[curr_pos+1] == ' ') // end of the word
                {
                    word_end = curr_pos;
                    print_word(input, word_start, word_end);
                    reset_word_start_end = TRUE;
                    curr_pos++; // the next character is the space before the next word, so want to move forward for the next iteration in the loop
                }
                else if(input[curr_pos] != ' ' && input[curr_pos + 1] == ' ') // end of the word
                {
                    word_end = curr_pos;
                    print_word(input, word_start, word_end);
                    reset_word_start_end = TRUE;
                    curr_pos++; // the next character is the space before the next word, so want to move forward for the next iteration in the loop
                }
                else if(input[curr_pos] == ' ') // end of the word
                {
                    word_end = curr_pos;
                    print_word(input, word_start, word_end);
                    reset_word_start_end = TRUE;
                }
                else // do something intelligent with the line now that it has reached the max line width without it being a complete word
                {
                    if(line_word_count == 0)
                    {
                        // 0 complete words this line so print the line as is
                        word_end = curr_pos;
                        print_word(input, word_start, word_end);
                        reset_word_start_end = TRUE;
                    }
                    else
                    {
                        // at least one complete word this line, move the start of the current word (-1 for the loop increment) so it starts on the next line
                        curr_pos = word_start - 1;
                        line_word_count = 0;
                        reset_word_start_end = TRUE;
                    }
                }
                putchar('\n'); // no more characters allowed this line so move down to the next line
            }
            line_word_count = line_char_count = 0; // reset for the next line
        }
        if (reset_word_start_end == TRUE)
        {
            word_start = -1;
            word_end = 0;
            reset_word_start_end = FALSE;
        }
        prev_char = curr_char;
    }
}

void print_word(char word[], int word_start, int word_end)
{
    for (int i = word_start; i <= word_end; ++i)
    {
        putchar(word[i]);
    }
}

//source and destination lengths should include the null terminating character
void detab(char source[], int source_length, char destination[], int destination_length, int tab_size)
{
    //Copy the source to the destination, exchanging tabs with the relevant number of spaces
    int characters_since_last_tab = 0;
    int destination_index = 0;
    for (int i = 0; i < source_length; ++i)
    {
        if (source[i] == '\t')
        {
            //Tab character found so copy the right amount of spaces to the destination
            for (int j = 0; j < (tab_size - characters_since_last_tab); ++j)
            {
                destination[destination_index] = ' ';
                ++destination_index;
            }
            characters_since_last_tab = 0; //Reset the counter            
        }
        else
        {
            /*As long as the counter doesn't exceed the tab size we can easily work out how many
            spaces to swap a tab for when we hit a tab character*/        
            if (characters_since_last_tab <= tab_size)
            {
                ++characters_since_last_tab;
            }
            destination[destination_index] = source[i];
            ++destination_index;
        }        
    }
    destination[destination_length - 1] = '\0'; //null terminate the destination
}