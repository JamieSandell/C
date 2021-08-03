/* Write the program tail, which prints the last n lines of its
input. By default, n is 10, let us say, but it can be changed by an optional
argument, so that

    tail -n
    
prints the last n lines. The program should behave rationally no matter how
unreasonable the input or the value of n. Write the program so it makes the
best use of available storage; lines should be stored as in the sorting program of
Section 5.6, not in a two-dimensional array of fixed size. */

#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 100

int get_line(char line[], int max_line_length);
void print_usage();

/* max_line_length is inclusive of the null terminator */
int get_line(char line[], int max_line_length)
{
    int char_count = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && char_count < max_line_length)
    {
        line[char_count++] = c;
    }
    line[char_count] = '\0';
    return char_count;
}

void print_usage()
{
    printf("Usage tail -n, e.g. tail -5\n");
}

int main(int argc, char **argv)
{
    /* Process the command line */
    if (argc > 2)
    {
        print_usage();
        return -1;
    }
    int n = 10;
    if (argc == 2)
    {
        if ((*++argv)[0] == '-')
        {
            n = atoi(*argv);
            if (n == 0)
            {
                print_usage();
                return -1;
            }
            n *= -1;
        }
        else
        {
            print_usage();
            return -1;
        }
    }

    char input[MAX_LINE_LENGTH];

    while(get_line(input, MAX_LINE_LENGTH) > 0) 

    return 0;
}