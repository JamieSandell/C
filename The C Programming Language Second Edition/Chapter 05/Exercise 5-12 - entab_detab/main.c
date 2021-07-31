/* Extend entab and detab to accept the shorthand
        entab -m +n
    to mean tab stops every n columns, starting at column m. Choose convenient (for the user) default behaviour */

#include <stdio.h>
#include <stdlib.h>

#define TAB_SIZE 8 //default number of spaces == to a tab
#define MAX_ARGUMENTS 100
#define MAX_LINE_LENGTH 1000

int calculate_destination_length(char line[], int line_length, int tab_stops[]);
int calculate_destination_size(char source[], int source_length, int tab_stops[]);
int get_line(char line[], int max_line_length);
void detab(char source[], int source_length, char destination[], int destination_length, int tab_stops[]);
void entab(char source[], int source_length, char destination[], int destination_length, int tab_stops[]);

/* Arguments from the command line should be multiples of TAB_SIZE */
int main(int argc, char **argv)
{
    int tab_stops[MAX_ARGUMENTS];

    if (argc > MAX_ARGUMENTS)
    {
        printf("Error: MAX_ARGUMENTS (%i) exceeded. Number of arguments passed in: %i\n", MAX_ARGUMENTS, argc);
        return -1;
    }
    if (argc >= 2) /* the first argument is the name of the exe, so >= 2 means user arguments have been passed in */
    {
        /* First check if only two arguments passed in on the command line to then check for -m +n */
        if (argc == 3)
        {
            
        }
        /* stores the tabstops passed in as arguments and validate the input as we go */
        int index = 1;
        int tab_stop = -1;
        int prev_tab_stop = -1;
        while (index < argc)
        {    
            prev_tab_stop = tab_stop;
            tab_stop = atoi(*(argv + index)); /* argv contains the command line arguments and it is a pointer to pointer of type char.
            argv points to the memory address of the first pointer to char, argv + 1 points to the memory address of the
            next pointer to char.
            We want this second pointer to char as the first pointer contains the first argument, but technically the first argument is the full path to the exe.
            e.g. argv = memory address 1000
            if you dereference argv you'll get a 1D array of characters e.g. c:\test.exe
            1000    1001    1002    1003    1004    1005    1006    1007    1008    1009    1010
            c       :       \       t       e       s       t       .       e       x       e
            We dereference this memory address of the second pointer to convert its 1D array of characters from char* to int 
            1011    1012
            3       2       assuming 32 was passed as an argument on the command line and then we loop round until we've read all of the args */    

            if (prev_tab_stop > tab_stop)
            {
                printf("Error: The tab stop arguments are not in ascending order.\n");
                return -1;
            }
            tab_stops[index - 1] = tab_stop;            
            ++index;            
        }
        /* fill out the rest of the tab_stops array with the default tab size */
        for (int i = argc - 1; i < MAX_ARGUMENTS; ++i)
        {
            tab_stops[i] = tab_stops[i - 1] + TAB_SIZE;
        }
    }
    if (argc <= 2) /* No arguments passed in */
    {
        /* Build the tab_stops with the default tab size */
        tab_stops[0] = TAB_SIZE;
        for (int i = 1; i < MAX_ARGUMENTS; ++i)
        {
            tab_stops[i] = tab_stops[i - 1] + TAB_SIZE;
        }
    }    

    /* detab */
    char detab_input[MAX_LINE_LENGTH + 1];
    int detab_input_length = get_line(detab_input, MAX_LINE_LENGTH);
    int detab_destination_length = calculate_destination_length(detab_input, detab_input_length, tab_stops);
    char detab_destination[detab_destination_length];
    detab(detab_input, detab_input_length, detab_destination, detab_destination_length, tab_stops);
    printf("%s\n", detab_destination);

    /* entab */
    char entab_input[MAX_LINE_LENGTH + 1];
    int entab_input_length = get_line(entab_input, MAX_LINE_LENGTH);
    int entab_destination_length = calculate_destination_size(entab_input, entab_input_length, tab_stops);
    char entab_destination[entab_destination_length];
    entab(entab_input, entab_input_length, entab_destination, entab_destination_length, tab_stops);
    printf("%s\n", entab_destination);

    return 0;
}

/* For detab: Work out the destination array size for detabbing (replacing tabs with spaces)
    Includes space for the null terminating character in its calculation */
int calculate_destination_length(char line[], int line_length, int tab_stops[])
{
    int characters_since_last_tab = 0;
    int destination_size = 0;
    int tab_stop_index = 0;
    int tab_stop_difference, prev_tab_size;
    for (int i = 0; i < line_length; ++i)
    {
        if (line[i] == '\t')
        {
            /* increase the destination_size by the right amount of spaces by
            calculating the correct "width" from the last character to the current tab */
            if (tab_stop_index == 0)
            {
                destination_size += tab_stops[tab_stop_index] - characters_since_last_tab;
                prev_tab_size = tab_stops[tab_stop_index++];
            }
            else
            {
                tab_stop_difference = tab_stops[tab_stop_index] - prev_tab_size - characters_since_last_tab;
                if (tab_stop_difference < 1)
                {
                    destination_size += 1; /* it's possible for the input to have gone past the current tab_stop, in this case replace with 1
                                                (size of a space character) */
                    prev_tab_size = 1;
                }
                else
                {
                    destination_size += tab_stop_difference;
                    prev_tab_size = tab_stops[tab_stop_index];
                }
            }            
            characters_since_last_tab = 0; //Reset the counter       
        }
        else
        {
            /* As long as the counter doesn't exceed the tab size we can easily work out how many
            spaces to swap a tab for when we hit a tab character */        
            if (characters_since_last_tab <= tab_stops[tab_stop_index])
            {
                ++characters_since_last_tab;
            }
            ++destination_size; /* not a tab so increase by 1 (i.e. we just processed a character from the line[] input) */
        }        
    }

    ++destination_size; /* allow space for the null terminating character */
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

/* source and destination lengths should include the null terminating character */
void detab(char source[], int source_length, char destination[], int destination_length, int tab_stops[])
{
    int destination_index = 0;
    int spaces_to_write;
    int tab_stop_index = 0;
    //Copy the source to the destination, exchanging tabs with the relevant number of spaces
    for (int i = 0; i < source_length; ++i)
    {
        if (source[i] != '\t') /* Process non-tab characters. */
        {
            if (destination_index >= destination_length - 1) /* would the dest_index be overwritten by the null terminating character, or go out of bounds? */
            {
                printf("Error: detab tried to go out of bounds of the destination[] when processing the %ith element of source[].\n", i);
                return;
            }
            destination[destination_index++] = source[i];
        }
        else /* Process the tab character */
        {
            spaces_to_write = tab_stops[tab_stop_index++];
            if (spaces_to_write <= destination_index)
            {
                /* the current tab_stops[i] was <= destination_index, this means we should set it so it writes out one ' ' char to destination[]. */
                spaces_to_write = destination_index + 1;
            }
            if (spaces_to_write - destination_index > destination_length - destination_index - 1) /* would the dest_index + tab detabbed go out of bounds
                                                                                                or overwrite null terminator? */
            {
                printf("Error: detab tried to go out of bounds of destination[] when processing a tab.\n");
                return;
            }            
            for (; destination_index < spaces_to_write; ++destination_index)
            {
                destination[destination_index] = ' ';
            }
        }
    }
    destination[destination_length - 1] = '\0'; //null terminate the destination
}

/* For entab: Calculates the destination size based on the source for replacing spaces with the right amount of tabs
Returns the size including room for the null terminating character */
int calculate_destination_size(char source[], int source_length, int tab_stops[])
{
    int destination_size = 0;
    int consecutive_blanks = 0;
    int character_count = 0;
    int tab_stops_index = 0;

    for (int i = 0; i < source_length; ++i)
    {
        ++character_count;
        if (source[i] == ' ')
        {
            ++consecutive_blanks;
            if (character_count == tab_stops[tab_stops_index] && consecutive_blanks > 1)
            {
                /* At the width of a tab and consecutive_blanks > 1 so here we would replace those spaces with x tab characters,
                and then reset for the next block of x characters that are equal to the next tab_stop */
                if (tab_stops_index == 0)
                {
                    destination_size += tab_stops[tab_stops_index] / TAB_SIZE;
                }
                else
                {
                    destination_size += (tab_stops[tab_stops_index] - tab_stops[tab_stops_index - 1]) / TAB_SIZE;
                }                
                tab_stops_index++;
                consecutive_blanks = 0;
            }
        }        
        else
        {
            if (consecutive_blanks > 0)
            {
                /* we have hit a non-blank character and previously had blank characters */
                destination_size += consecutive_blanks + 1; /* the previous consecutive_blanks + 1 for the current character */
            }
            else
            {
                /* we have hit a non-blank character and did not previously have blank characters */
                ++destination_size;
            }                        
            consecutive_blanks = 0;            
        }      
    }
    ++destination_size; /* for the null terminating character */

    return destination_size;
}

void entab(char source[], int source_length, char destination[], int destination_length, int tab_stops[])
{
    int destination_index = 0;
    int consecutive_blanks = 0;
    int character_count = 0;
    int tab_stops_index = 0;
    int number_of_tabs_to_insert;

    for (int i = 0; i < source_length; ++i)
    {
        ++character_count;
        if (source[i] == ' ')
        {
            ++consecutive_blanks;
            if (character_count == tab_stops[tab_stops_index] && consecutive_blanks > 1)
            {
                /* At the width of a tab and consecutive_blanks > 1 so here we would replace those spaces with x tab characters,
                and then reset for the next block of x characters that are equal to the next tab_stop */
                if (tab_stops_index == 0)
                {
                    number_of_tabs_to_insert = tab_stops[tab_stops_index] / TAB_SIZE;   
                }
                else
                {
                    number_of_tabs_to_insert = (tab_stops[tab_stops_index] - tab_stops[tab_stops_index - 1]) / TAB_SIZE;
                }                 
                for (int i = 0; i < number_of_tabs_to_insert; ++i)
                {
                    destination[destination_index++] = '\t';
                }
                ++tab_stops_index;
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