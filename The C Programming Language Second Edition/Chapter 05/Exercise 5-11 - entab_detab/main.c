/* Modify the programs entab and detab (written as exercises in Chapter 1)
to accept a list of tab stops as arguments. Use the default tab settings
if there are no arguments. */

#include <stdio.h>
#include <stdlib.h>

#define TAB_SIZE 8 //default number of spaces == to a tab
#define MAX_ARGUMENTS 100
#define MAX_LINE_LENGTH 1000

int calculate_destination_length(char line[], int line_length, int tab_stops[]);
int calculate_destination_size(char source[], int source_length, int tab_size);
int get_line(char line[], int max_line_length);
int get_tabs(char line[], int line_length);
void detab(char source[], int source_length, char destination[], int destination_length, int tab_size);

int main(int argc, char *argv[])
{
    int tab_stops[MAX_ARGUMENTS];

    if (argc > MAX_ARGUMENTS)
    {
        printf("Error: MAX_ARGUMENTS (%i) exceeded. Number of arguments passed in: %i\n", MAX_ARGUMENTS, argc);
        return -1;
    }
    if (argc >= 2)
    {
        /* the first argument is the name of the exe, so >= 2 means user arguments have been passed in */
        /* validate the input, the tabstops need to be in ascending order */
        int temp = 0;
        for (int index = 1; index < argc; ++index)
        {
            int tab_stop = atoi((*argv) + index); /* Get the address of the first array/argument, and then move along by the index to the one you want */
            if (temp < tab_stop)
            {
                printf("Error: The tab stop arguments are not in ascending order.\n");
                return -1;
            }
            tab_stops[index - 1] = tab_stop;
            temp = tab_stop;
        }
        /* fill out the rest of the tab_stops with defaults */
        for (int i = argc; i < MAX_ARGUMENTS; ++i)
        {
            tab_stops[i] = tab_stops[i - 1] + TAB_SIZE;
        }
    }
    if (argc <= 2)
    {
        /* Build the tab_stops */
        tab_stops[0] = TAB_SIZE;
        for (int i = 1; i < MAX_ARGUMENTS; ++i)
        {
            tab_stops[i] = tab_stops[i - 1] + TAB_SIZE;
        }
    }
    return 0;
}

/* detab
int main()
{
    char line[(MAX_LINE_LENGTH + 1)]; //allow for the termination character
    int line_length = 0;
    int number_of_tabs = 0;
    while ((line_length = get_line(line, MAX_LINE_LENGTH)) > 0)
    {
        printf("%s\n", line);

        number_of_tabs = get_tabs(line, line_length);
        printf("Number of tabs: %i\n", number_of_tabs);

        int destination_length = calculate_destination_length(line, line_length, TAB_SIZE);
        char destination[destination_length];
        detab(line, line_length, destination, destination_length, TAB_SIZE);
        printf("With tabs:\t%s\n", line);
        printf("Without tabs:\t%s\n", destination);
    }
} */

/* entab
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
}*/

/* For detab: Work out the destination array size for detabbing (replacing tabs with spaces)
    Includes space for the null terminating character in its calculation */
int calculate_destination_length(char line[], int line_length, int tab_stops[])
{
    int characters_since_last_tab = 0;
    int destination_size = 0;
    int tab_stop_index = 0;
    for (int i = 0; i < line_length; ++i)
    {
        if (line[i] == '\t')
        {
            /*increase the destination_size by the right amount of spaces by
            calculating the correct "width" from the last character to the current tab*/
            destination_size += tab_stops[tab_stop_index++] - characters_since_last_tab;
            characters_since_last_tab = 0; //Reset the counter       
        }
        else
        {
            /*As long as the counter doesn't exceed the tab size we can easily work out how many
            spaces to swap a tab for when we hit a tab character*/        
            if (characters_since_last_tab <= tab_stops[tab_stop_index])
            {
                ++characters_since_last_tab;
            }
            ++destination_size; //not a tab so increase by 1 (i.e. we just processed a character from the line[] input)
        }        
    }

    ++destination_size; //allow space for the null terminating character
    return destination_size;
}

/*
max_line_length is the max input length for the string, it does not include the termination character in that variable
make sure that your line[] size is max_line_length + 1 to allow for the termination character
*/
int get_line(char line[], int max_line_length)
{
    int line_length = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n')
    {
        ++line_length;
        if (line_length <= max_line_length)
        {
            line[(line_length - 1)] = c;
        }
    }

    if (line_length > max_line_length)
    {
        line[max_line_length] = '\0';
    }
    else
    {
        line[line_length] = '\0';
    }    

    return line_length;
}

int get_tabs(char line[], int line_length)
{
    int number_of_tabs = 0;

    for (int i = 0; i < line_length; ++i)
    {
        if (line[i] == '\t')
        {
            ++number_of_tabs;
        }
    }

    return number_of_tabs;
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

/* For entab: Calculates the destination size based on the source for replacing spaces with the right amount of tabs
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