/* Write the function strrindex(s,t), which returns the position
of the rightmost occurrence of t in s, or -1 if there is none. */

#include <stdio.h>
#include <string.h>

#define MAX_LINE 1000

int get_line(char line[], int max);
int strrindex(char source[], char pattern[]);

int main()
{
    char pattern[] = {"ould"};
    char line[MAX_LINE];
    int found = 0;
    int result;

    while(get_line(line, MAX_LINE) > 0)
    {
        if ((result = strrindex(line, pattern)) >= 0)
        {
            printf("Found %s at index %i of line: %s", pattern, result, line);
            found++;
        }
    }

    return found;
}

int get_line(char s[], int lim)
{
    int c, i;

    i = 0;
    while(--lim > 0 && (c = getchar()) != EOF && c != '\n')
    {
        s[i++] = c;
    }
    if (c == '\n')
    {
        s[i++] = c;
    }
    s[i] = '\0';

    return i;
}

int strrindex(char s[], char t[])
{
    int i, j, k;

    for(i = strlen(s) - 1; i >= 0; --i)
    {
        for(j = i, k = 0; t[k] != 0 && s[j] == t[k]; j++, k++)
            ;
        if (k > 0 && t[k] == '\0')
        {
            return i;
        }
    }
    return -1;
}