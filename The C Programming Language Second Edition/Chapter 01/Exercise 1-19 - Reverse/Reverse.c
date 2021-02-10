//Reverse input, one line at a time
#include <stdio.h>

#define MAX_LINE_LENGTH 100

int get_line(char line[], int max_line_length);
void reverse(char line [], int line_length);

int main()
{
    char line[MAX_LINE_LENGTH];
    int line_length;
    while((line_length = get_line(line, MAX_LINE_LENGTH)) > 0)
    {
        reverse(line, line_length);
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
        if (line_length <= max_line_length)
        {
            ++line_length;
            line[(line_length - 1)] = c;
        }        
    }

    if (line_length == 0) //no input received
    {
        return line_length;
    }

    if (line_length < max_line_length) //terminate at the correct place for within bounds input
    {
        line[line_length] = '\0';
        return line_length;
    }

    line[max_line_length] = '\0'; // >= max input so terminate at the max input point
    return max_line_length;
}

void reverse(char line[], int line_length)
{
    char temp[line_length];

    //reverse and store it in temp
    int j = 0;
    for (int i = (line_length - 1); i >= 0; --i)
    {
        temp[j] = line[i];
        ++j;
    }

    //copy temp back to the original
    for (int i = 0; i < line_length; ++i)
    {
        line[i] = temp[i];
    }    
}