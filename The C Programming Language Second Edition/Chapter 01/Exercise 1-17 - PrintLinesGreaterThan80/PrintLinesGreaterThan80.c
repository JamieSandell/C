//Print all lines that are greater than 80 characters
#include <stdio.h>

#define MAX_LINES   1000
#define MAX_LINE_LENGTH 80

int get_line(char line[], int max_line_length);

int main()
{
    char line[MAX_LINE_LENGTH];
    int line_length;

    while ((line_length = get_line(line, MAX_LINE_LENGTH)) > 0)
    {
        if (line_length > MAX_LINE_LENGTH)
        {
            printf("%s", line);
            putchar('\n');
        }        
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
        if (line_length < MAX_LINE_LENGTH) //leave room for the C-style string terminator '\0'
        {
            line[(line_length - 1)] = c;
        }
    }

    if (line_length > 0)
    {
        line[line_length] = '\0';
    }

    return line_length;
}