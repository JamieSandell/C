/* Add the -d ("directory order") option, which makes comparisons only on letters, numbers and blanks.
    Make sure it works in conjunction with -f */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALLOC_SIZE 10000 /* size of our memory buffer */
#define MAX_LINE_LENGTH 100
#define MAX_LINES 5000 /* Max number of lines to be sorted. */

static char alloc_buffer[ALLOC_SIZE]; /* memory buffer */
static char *alloc_pointer = alloc_buffer; /* next free memory position */

char *line_pointer[MAX_LINES]; /* pointers to text lines */

int (*base_compare)(void *a, void *b); /* our function pointers to comparison functions */
int (*compare)(void *a, void *b);

int get_line(char line[], int line_size);
int read_lines(char *line_pointer[], int max_number_of_lines);
void write_lines(char* line_pointer[], int number_of_lines);

void my_qsort(void *v[], int left, int right, int (*comp)(void *, void*));
int directory_order_comp(const char *s1, const char *s2);
int numcmp(const char *s1, const char *s2);
int reverse_cmp(void *a, void *b);
void str_to_lower(char *s);
int str_case_cmp(const char *s1, const char *s2);
void swap(void *v[], int i, int j);

void afree(char *p);
char *alloc(int size);

int main(int argc, char **argv)
{
    int c;
    int reverse = 0;
    int numeric = 0;
    int case_insensitive = 0;
    int directory_order = 0;
    while (--argc > 0 && (*++argv)[0] == '-') /* skip the program path argument, then check if the first char of the arg is what we expect */
    {
        while (c = *++argv[0]) /* [] binds tighter than, argv equivalent to:
                                    argv[0] (memory address of the first element that argv is currently pointing to)
                                    ++ increment/walk along this string at argv, to the next character in it
                                    * dereference it all to get the value/character */
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
                    case_insensitive = 1;
                    break;
                case 'd':
                    directory_order = 1;
                    break;
                default:
                    argc = 0;
                    printf("Error: %c is an invalid argument\n", c);
                    break;
            }
        }
    }
    return 0;
}

int get_line(char line[], int line_size)
{
    int char_count = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && char_count < line_size - 1)
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

int read_lines(char *line_pointer[], int max_number_of_lines)
{
    int lines_read = 0;
    int char_count;
    char line[MAX_LINE_LENGTH];
    char *p;

    while((char_count = get_line(line, MAX_LINE_LENGTH)) > 0)
    {
        if (lines_read >= max_number_of_lines || (p = alloc(char_count)) == NULL)
        {
            return -1;
        }
        strcpy(p, line);
        line_pointer[lines_read++] = p;
    }
    return lines_read;
}

void write_lines(char* line_pointer[], int number_of_lines)
{
    while (number_of_lines-- > 0)
    {
        printf("%s\n", *line_pointer++);
    }
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
    my_qsort(v, left, last -  1, comp);
    my_qsort(v, last + 1, right, comp);
}

int directory_order_comp(const char *s1, const char *s2)
{
    int input_validated = 0;
    
}

/* Retuns:
    0 if strings are equal
    1 if s1 has a greater numerical value than s2
    -1 if s1 has a lesser numerical value than s2 */
int numcmp(const char *s1, const char *s2)
{
    double d1 = atof(s1);
    double d2 = atof(s2);

    if (d1 == d2)
    {
        return 0;
    }
    else if(d1 > d2)
    {
        return 1;
    }
    return -1;
}

/* Returns the result of base_compare(b, a) */
int reverse_cmp(void *a, void *b)
{
    return base_compare(b, a);
}

/* Converts a string to lower case */
void str_to_lower(char *s)
{
    while(*s != '\0')
    {
        *s = tolower(*s);
        s++;
    }
}

/* Case insensitive.
    Returns:
    0 if strings are equal
    1 if the first non-matching character in s1 has a greater ASCII value than that of s2
    -1 if the first non-matching character in s1 has a lesser ASCII value than that of s2 */
int str_case_cmp(const char *s1, const char *s2)
{
    char t1[MAX_LINE_LENGTH];
    char t2[MAX_LINE_LENGTH];
    strcpy(t1, s1);
    strcpy(t2, s2);
    str_to_lower(t1);
    str_to_lower(t2);
    return strcmp(t1, t2);
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

/* Returns a non-NULL pointer in the memory buffer if there was enough room, else returns NULL */
char *alloc(int size)
{
    if (alloc_buffer + ALLOC_SIZE - alloc_pointer >= size) /* Does it fit? */
    {
        alloc_pointer += size;
        return alloc_pointer - size;
    }
    return NULL;
}