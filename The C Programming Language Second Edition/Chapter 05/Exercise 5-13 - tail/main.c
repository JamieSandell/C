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
#include <string.h>

#define MAX_LINE_LENGTH 1000 /* maximum characters (including the null terminator) for a line */
#define MAX_LINES 5000 /* total number of lines allowed */

#define ALLOC_SIZE 10000 /* size of available space */
static char allocbuf[ALLOC_SIZE]; /* storage for alloc */
static char *allocp = allocbuf; /* next free position */

void afree(char *p);
char *alloc(int n);
int get_line(char line[], int max_line_length);
void print_usage();
int read_lines(char *lineptr[], int max_lines);
void write_lines(char *lineptr[], int nlines);

int main(int argc, char **argv)
{
    /* Process the command line */
    if (argc > 2)
    {
        print_usage();
        return -1;
    }
    int n = 10; /* default last n lines of input to print */
    if (argc == 2)
    {
        /* Validate the arguments passed in */
        if ((*++argv)[0] == '-')
        {
            n = atoi(*argv);
            if (n == 0)
            {
                print_usage();
                return -1;
            }
            if (n > MAX_LINES)
            {
                printf("-n cannot exceed %i\n", MAX_LINES);
            }
            n *= -1; /* get rid of the '-' character read in */
        }
        else
        {
            print_usage();
            return -1;
        }
    }

    char *lineptr[MAX_LINES]; /* pointers to text lines */
    int nlines; /* number of input lines read */
    if ((nlines = read_lines(lineptr, MAX_LINES)) >= 0)
    {
        write_lines(lineptr, (n > nlines ? n : nlines)); /* write the last n lines, unless lines read was smaller, then read write them instead */
    }

    return 0;
}

/* free storage pointed to by p */
void afree(char *p)
{
    if (p >= allocbuf && p < allocbuf + ALLOC_SIZE)
    {
        allocp = p;
    }
}

/* return pointer to n characters */
char *alloc(int n)
{
    if (allocbuf + ALLOC_SIZE - allocp >= n) /* it fits */
    {
        allocp +=n;
        return allocp - n; /* old p */
    }
    return 0; /* not enough room */
}

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

int read_lines(char *lineptr[], int max_lines)
{
    int len, nlines;
    char *p, line[MAX_LINE_LENGTH];

    nlines = 0;
    while((len = get_line(line, MAX_LINE_LENGTH)) > 0)
    {
        if (nlines >= max_lines || (p = alloc(len)) == NULL)
        {
            return -1; /* read too many lines or there was enough free memory */
        }
        else
        {
            line[len - 1] = '\0'; /* delete the newline character */
            strcpy(p, line);
            lineptr[nlines++] = p;
        }
    }
    return nlines;
}

void write_lines(char *lineptr[], int nlines)
{
    while (nlines-- > 0)
    {
        printf("%s\n", *lineptr++);
    }
}