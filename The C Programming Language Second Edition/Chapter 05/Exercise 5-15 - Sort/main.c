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
void my_qsort(void *v[], int left, int right, int (*comp)(void *, void *));
int numcmp(const char *s1, const char *s2);
void swap(void *v[], int i, int j);

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
    int lines_read;
    if ((lines_read = read_lines(line_pointer, MAX_LINES)) >= 0)
    {
        /* point to the correct comparison function
            perform the quick sort
            print the result */
        if (!numeric)
        {
            base_compare = (int (*)(void *, void*))(ignore_case ? strcasecmp : strcmp);
        }
        else
        {
            base_compare = (int (*)(void *, void*))numcmp;
        }
        compare = (int (*)(void *, void*))(reverse ? reverse_compare : base_compare);
        my_qsort((void **)line_pointer, 0, lines_read - 1, compare);
        write_lines(line_pointer, lines_read);
        return 0;
    }
    printf("Error: Input too big to sort.");
    return -1;
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
    char line[MAX_LINE_LENGTH];
    int lines_read = 0;
    int line_length;
    char *p;
    while ((line_length = get_line(line, MAX_LINE_LENGTH)) > 0 )
    {
        if (lines_read >= max_number_of_lines || (p = alloc(line_length)) == NULL)
        {
            return -1;
        }
        line[line_length - 1] = '\0'; /* delete the new line character */
        strcpy(p, line);
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

int reverse_compare(void *a, void *b)
{
    return (*base_compare)(b, a);
}

void my_qsort(void *v[], int left, int right, int (*comp)(void *, void*))
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

/* s1 < s2 == return -1 
    s1 > s2 == return 1
    return 0*/
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

void swap(void *v[], int i, int j)
{
    void *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
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
    if (alloc_pointer + size <= alloc_buffer + ALLOC_SIZE) /* Does the request fit? */
    {
        alloc_pointer += size;   /* Point the buffer pointer to the next free space */
        return alloc_pointer - size; /* Return the old buffer pointer position */
    }
}