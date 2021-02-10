/*
Remove trailing spaces and trailing tabs from lines.
Delete empty lines
*/

#include <stdio.h>

#define MAX_LINE_LENGTH 5

int get_line(char line[], int max_line_length);
void remove_trailing(char line[], int length);

int main()
{
    char line[MAX_LINE_LENGTH + 1];
    int line_length = 0;
    while((line_length = get_line(line, MAX_LINE_LENGTH)) > 0)
    {
        //printf("%s", line);
        //putchar('\n');
        remove_trailing(line, line_length);
        printf("%s", line);
        putchar('\n');
    }

    return 0;
}

int get_line(char line[], int max_line_length)
{
    int line_length = 0;

    int c;
    while((c = getchar()) != EOF && c != '\n')
    {
        ++line_length;
        if (line_length <= max_line_length) //As long as we are within the limit, store the character read
        {
            line[(line_length - 1)] = c;
        }        
    }

    if (line_length > max_line_length) //more input was received than allowed
    {
        line[max_line_length] = '\0'; //terminate the string before the extra input
        return (max_line_length);
    }
    
    line[line_length] = '\0'; //Otherwise terminate the string where the input finished
    
    return line_length;
}

void remove_trailing(char line[], int length)
{
    if (length == 0) //empty string
    {
        return;
    }

    //find the last non-trailing character index
    int lntci = 0;
    for (int i = 0; i < length; ++i)
    {
        if (line[i] != '\t' && line[i] != ' ')
        {
            lntci = i;
        }       
    }

    if (lntci == (length - 1)) //no trailing tabs or no trailing spaces
    {
        return;
    }

    line[(lntci + 1)] = '\0';
}