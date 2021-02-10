/*Replaces strings of blanks by the minimum
number of tabs and blanks to achieve the same spacing.
Use the same tab stops as detab. When either a tab or a single
blank would suffice to reach a tab stop, which should be given preference?*/

#include <stdio.h>

#define MAX_LINE_LENGTH 100
#define TAB_SIZE 8 //number of blanks that represent a tab

int calculate_destination_size(char source[], int source_length, int tab_size);
void entab(char source[], int source_length, char destination[], int destination_length, int tab_size);
int get_line(char line[], int max_line_length);

int main()
{
    char line[MAX_LINE_LENGTH + 1];
    int line_length;

    while ((line_length = get_line(line, MAX_LINE_LENGTH)) > 0)
    {
        printf("%s\n", line);
        printf("Source array size: %i\n", line_length + 1);
        int destination_size = calculate_destination_size(line, line_length, TAB_SIZE);
        printf("Destination array size: %i\n", destination_size);
        printf("Note: The arrays have room for the null terminating character\n");
        char entabbed[destination_size];
        entab(line, line_length, entabbed, destination_size, TAB_SIZE);
        printf("Before entabbing:\n");
        printf("%s\n", line);
        printf("After entabbing:\n");
        printf("%s\n", entabbed);
    }

    return 0;
}

/*Calculates the destination size based on the source for replacing spaces with the right amount of tabs
Returns the size including room for the null terminating character*/
int calculate_destination_size(char source[], int source_length, int tab_size)
{
    int destination_size = 0;
    int consecutive_blanks = 0;
    int character_count = 0;

    for (int i = 0; i < source_length; ++i)
    {
        ++character_count;
        if (source[i] == ' ')
        {
            ++consecutive_blanks;
            if (character_count == tab_size && consecutive_blanks > 1)
            {
                ++destination_size; //at the width of a tab and consecutive_blanks > 1 so here we would replace those spaces with a single tab character
                //reset for the next block of 8 characters
                character_count = 0;
                consecutive_blanks = 0;
            }
        }        
        else
        {
            if (consecutive_blanks > 0)
            {
                //we have hit a non-blank character and previously had blank characters
                destination_size += consecutive_blanks + 1; //the previous consecutive_blanks + 1 for the current character
            }
            else
            {
                //we have hit a non-blank character and did not previously have blank characters
                ++destination_size;
            }                        
            consecutive_blanks = 0;            
        }      
    }
    ++destination_size; //for the null terminating character

    return destination_size;
}

void entab(char source[], int source_length, char destination[], int destination_length, int tab_size)
{
    int destination_index = 0;
    int consecutive_blanks = 0;
    int character_count = 0;

    for (int i = 0; i < source_length; ++i)
    {
        ++character_count;
        if (source[i] == ' ')
        {
            ++consecutive_blanks;
            if (character_count == tab_size && consecutive_blanks > 1)
            {
                destination[destination_index] = '\t';
                ++destination_index; //at the width of a tab and consecutive_blanks > 1 so here we would replace those spaces with a single tab character
                //reset for the next block of 8 characters
                character_count = 0;
                consecutive_blanks = 0;
            }
        }        
        else
        {
            if (consecutive_blanks > 0)
            {
                //we have hit a non-blank character and previously had blank characters
                for (int j = 0; j < consecutive_blanks; ++j)
                {
                    destination[destination_index + j] = ' '; //fill in the previous blanks
                }
                destination_index += consecutive_blanks; //move the index to point to the current (non-blank character) and store it
                destination[destination_index] = source[i];
                ++destination_index; //move the index ready for the next loop iteration
            }
            else
            {
                //we have hit a non-blank character and did not previously have blank characters
                destination[destination_index] = source[i];
                ++destination_index;
            }                        
            consecutive_blanks = 0;            
        }      
    }
    destination[destination_length - 1] = '\0';
}

/*Stores the input in line as a C-style string.
line[] should be max_line_length + 1 to accomodate for the null terminating character
max_line_length should be the max input for a line, not including space for the null terminating character.
Returns the length of the line (not including the null terminating character).*/
int get_line(char line[], int max_line_length)
{
    int line_length = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n')
    {
        if (line_length < max_line_length) //If this is in the while condition then anything over this limit will be still in the input buffer
        {
            line[line_length] = c;
            ++line_length;
        }        
    }

    if (line_length < max_line_length)
    {
        line[line_length] = '\0';
    }
    else
    {
        line[max_line_length] = '\0';
    }
    
    return line_length;
}