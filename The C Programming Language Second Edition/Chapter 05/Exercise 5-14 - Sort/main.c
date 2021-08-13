/* Modify the sort program to handle an -r flag, which indicates sorting in reverse (decreasing order).
Be sure that -r works with -n. */

#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 100 /* max length of a line, including the null terminating character */
#define MAX_LINES 5000 /* max number of lines to be sorted */
char *line_pointer[MAX_LINES]; /* pointers to text lines */
int get_line(char line[]);
int read_lines(char *line_pointer[], int number_of_lines);
void write_lines(char *line_pointer[], int number_of_lines);

void qsort(void *line_pointer[], int left, int right, int (*comp)(void *, void *));
int numcmp(char *, char *);

#define ALLOC_SIZE 10000 /* size of available space */
static char alloc_buffer[ALLOC_SIZE]; /* storage for alloc */
static char *alloc_pointer = alloc_buffer; /* next free position */
void afree(char *p);
char *alloc(int size);

int main(int argc, char *argv[])
{
    return 0;
}

int get_line(char line[])
{
    int char_count = 0;
    int c;
    while ((c = getchar()) != '\n' && c != EOF  && char_count < MAX_LINE_LENGTH - 1)
    {
        line[char_count++] = c;
    }
    if (c = '\n')
    {
        line[char_count++] = '\n';
    }
    line[char_count] = '\0';
    return char_count;
}

int read_lines(char *line_pointer[], int number_of_lines)
{
    int lines_read = 0;
    char line[MAX_LINE_LENGTH];
    char *p;
    int line_length;
    while ((line_length = get_line(line)) > 0 && lines_read < MAX_LINES)
    {
        if (lines_read >= MAX_LINES || (p = alloc(line_length)) == NULL)
        {
            return -1;
        }
        line[line_length - 1] = '\0'; /* delete the new line character */
        strcpy(p, line); /* copy the string from line to where p points to in our memory buffer */
        line_pointer[lines_read++] = p;
    }
    return lines_read;
}

void write_lines(char *line_pointer[], int number_of_lines)
{
    while (number_of_lines-- > 0)
    {
        printf("%s\n", *line_pointer++);
    }
}