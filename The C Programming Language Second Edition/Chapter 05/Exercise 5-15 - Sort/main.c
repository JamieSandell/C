/* Add the option -f to fold upper and lower case together, so that
    case distinctions are not made during sorting; for example, a and A compare equal. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100 /* including null terminator */
#define MAX_LINES 5000 /* max number of lines to be sorted */
char *line_pointer[MAX_LINES]; /* pointers to text lines */

int get_line(char line[], int max_line_length);
int read_lines(char *line_pointer[], int max_number_of_lines);
void write_lines(char *line_pointer[], int number_of_lines);

int reverse_compare(void *a, void *b);
int (*base_compare)(void *, void *);
int (*compare)(void *, void *);
void my_qsort(void *line_pointer[], int left, int right, int (*comp)(void *, void *));
int numcmp(char *, char *);

#define ALLOC_SIZE 10000
static char alloc_buffer[ALLOC_SIZE];
static char *alloc_pointer = alloc_buffer;
char *alloc(int size);
void afree(char *p);

int main(int argc, char **argv)
{
    int c;
    int numeric = 0;
    int reverse = 0;
    int ignore_case = 0;
    while (--argc > 0 && (*++argv[0]) == '-') /* inc. argv, then dereference then get the first character */
    {
        while (c = *++argv[0])
        {
            switch (c)
            {
                case 'r':
                    reverse = 1;
                    break;
                case 'n':
                    numeric = 1;
                    break;
                case 'f':
                    ignore_case = 1;
                    break;
                default:
                    argc = 0;
                    printf("Error: argument %c invalid.\n", c);
                    break;
            }
        }
    }

    return 0;
}

int get_line(char line[], int max_line_length)
{
    int char_count = 0;
    int c;
    while ((c = getchar()) != '\n' && c != EOF && char_count < max_line_length - 1)
    {
        line[char_count++] = c;
    }
    if (c == '\n')
    {
        line[char_count++] = c;
    }
    line[char_count] = '\0';
    return char_count;
}

/* Return the number of lines read.
    Returns -1 if lines read exceeds max_number_of_lines or not enough free memory in our buffer. */
int read_lines(char *line_pointer[], int max_number_of_lines)
{
    char line[MAX_LINES];
    int lines_read = 0;
    int line_length = 0;
    char *p;
    while ((line_length = get_line(line, max_number_of_lines)) > 0 )
    {
        if (lines_read >= max_number_of_lines || (p = alloc(line_length)) == NULL)
        {
            return -1;
        }
    }
}