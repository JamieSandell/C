/*
Write a program detab that replaces tabs in the input with the proper number of blanks
to space to the next tab stop. Assume a fixed set of tab stops, say every n columns.
Should n be a variable or a symbolic parameter?
*/
#include <stdio.h>

#define TAB_SIZE 8 //number of spaces == to a tab
#define MAX_LINE_LENGTH 1000

int calculate_destination_length(char line[], int line_length, int tab_size);
int get_line(char line[], int max_line_length);
int get_tabs(char line[], int line_length);
void detab(char source[], int source_length, char destination[], int destination_length, int tab_size);

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
}

/*Work out the destination array size for detabbing (replacing tabs with spaces)
    Includes space for the null terminating character in its calculation*/
int calculate_destination_length(char line[], int line_length, int tab_size)
{
    int characters_since_last_tab = 0;
    int destination_size = 0;
    for (int i = 0; i < line_length; ++i)
    {
        if (line[i] == '\t')
        {
            /*increase the destination_size by the right amount of spaces by
            calculating the correct "width" from the last character to the current tab*/
            destination_size = destination_size + tab_size - characters_since_last_tab;
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