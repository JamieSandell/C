/* Modify the sort program to handle an -r flag, which indicates sorting in reverse (decreasing order).
Be sure that -r works with -n. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100 /* max length of a line, including the null terminating character */
#define MAX_LINES 5000 /* max number of lines to be sorted */
char *line_pointer[MAX_LINES]; /* pointers to text lines */
int get_line(char line[]);
int read_lines(char *line_pointer[], int number_of_lines);
void write_lines(char *line_pointer[], int number_of_lines);

void my_qsort(void *v[], int left, int right, int (*comp)(void *, void *));
int numcmp(const char *s1, const char *s2);
void swap(void *v[], int i, int j);

#define ALLOC_SIZE 10000 /* size of available space */
static char alloc_buffer[ALLOC_SIZE]; /* storage for alloc */
static char *alloc_pointer = alloc_buffer; /* next free position */
void afree(char *p);
char *alloc(int size);

int main(int argc, char *argv[])
{
    int lines_read;
    int numeric = 0; /* 1 for true, any other value for false */
    int reverse = 0;
    int c;

    while (--argc > 0 && (*++argv)[0] == '-') /* inc. argv, then dereference then get the first character */
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
                default:
                    printf("Error: illegal operation %c\n", c);
                    argc = 0;
                    break;
            }
        }
    }

    if ((lines_read = read_lines(line_pointer, MAX_LINES)) >= 0)
    {
        int (*comparison_function)(void *, void *) = (int (*)(void *, void*))(numeric ? numcmp : strcmp);
        my_qsort((void **)line_pointer, 0, lines_read - 1, comparison_function);
        write_lines(line_pointer, lines_read);
        return 0;
    }

    printf("Error: Input too big to sort.");
    return -1;
}

int get_line(char line[])
{
    int char_count = 0;
    int c;
    while ((c = getchar()) != '\n' && c != EOF  && char_count < MAX_LINE_LENGTH - 1)
    {
        line[char_count++] = c;
    }
    if (c == '\n')
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

void my_qsort(void *v[], int left, int right, int (*comp)(void *, void *))
{
    int i, last;
    
    if (left >= right) /* do nothing if array contains fewer than two elements */
    {
        return;
    }
    swap(v, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i <= right; i++)
    {
        if ((*comp)(v[i], v[left]) < 0)
        {
            swap(v, ++last, i);
        }
    }
    swap(v, left, last);
    my_qsort(v, left, last - 1, comp);
    my_qsort(v, last + 1, right, comp);
}

void swap(void *v[], int i, int j)
{
    void *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

int numcmp(const char *s1, const char *s2)
{
    double v1, v2;

    v1 = atof(s1);
    v2 = atof(s2);
    if (v1 < v2)
    {
        return -1;
    }
    else if (v1 > v2)
    {
        return 1;
    }
    return 0;
}

void afree(char *p)
{
    if (p >= alloc_buffer && p < alloc_buffer + ALLOC_SIZE)
    {
        alloc_pointer = p;
    }
}

char *alloc(int size)
{
    if (alloc_pointer + size <= alloc_buffer + ALLOC_SIZE) /* does the request fit? */
    {
        alloc_pointer += size; /* move the buffer pointer to the next free space */
        return alloc_pointer - size; /* return the old buffer pointer position */
    }
}