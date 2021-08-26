/* Add a field-handling capability, so sorting may be done on fields within lines, each field sorted according to an independent set of options.
(The index for this book was sorted with -df for the index category and -n for the page numbers.) */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARGS 5
#define MAX_LINES 100 /* Max number of lines to read in */
#define MAX_LINE_SIZE 100 /* including the null terminating character */
#define MAX_ALLOC_SIZE 10000 /* for the custom memory management */

/* Memory */
static char alloc_buffer[MAX_ALLOC_SIZE];
static char* alloc_pointer = alloc_buffer;
/* State */
static int case_insensitive = 0;
static int directory = 0;
static int numeric = 0;
static int reverse = 0;

/* Compare and sort */
void my_qsort(void *v[], int left, int right, int (*compare)(void *a, void *b));
int numcmp(char *s1, char *s2);
void swap(void *v[], int i, int j);
/* Conversions */
int str_to_float(char *s);
/* i/o */
int get_line(char line[], int max_line_size);
int read_lines(char *line_pointer[], int max_line_size);
void write_lines(const char *line_pointer[], int number_of_lines);
/* Memory */
void afree(char *p);
char *alloc(int size);
/* Validation */
int validate_input(const char *v[], int (*validation)(const char *), int number_of_lines);
int validate_n(const char *s);
int validate_d(const char *s);


int main(int argc, char *v[])
{
    if (argc > MAX_ARGS)
    {
        printf("Error: Too many arguments, max is %i\n", MAX_ARGS - 1); /* ignore the first arg as that's the program filepath */
        return -1;
    }

    char *line_pointer[MAX_LINES]; /* To store the pointers to our lines that will be in our memory buffer */
    int argc_initial_value = argc; /* cache a copy as we'll be maniupulating argc, and will need the original value later on */
    return 0;
}

void my_qsort(void *v[], int left, int right, int (*compare)(void *, void *))
{
    if (left >= right) /* do nothing if array contains fewer than two elements */
    {
        return;
    }

    int i, last;    
    swap(v, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i <= right; i++)
    {
        if ((*compare)(v[i], v[left]) < 0)
        {
            swap(v, ++last, i);
        }
    }
    swap(v, left, last);
    my_qsort(v, left, last -  1, compare);
    my_qsort(v, last + 1, right, compare);
}

/* Returns:
    Converts the two strings to number and compares them
    0 if equal
    1 if the first non-matching character in s1 is greater than that of str2
    -1 if the first non-matching character in s2 is lower than that of str2 */
int numcmp(char *s1, char *s2)
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

/* Swap the ith and jth elements of v[] */
void swap(void *v[], int i, int j)
{
    void *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

int str_to_float(char *s)
{
    int c, sign;
    float power;

    while(isspace(*s++)) /* skip white space */
        ;
    if (!(isdigit(*s) && *s != EOF && *s != '+' && *s != '-'))
    {
        *s--; /* it's not a number */
        return 0;
    }
    sign = (*s == '-') ? -1 : 1;
    if (*s == '+' || *s == '-')
    {
        int d = *s; /* Remember the sign character */
        s++
        if (!isdigit(*s))
        {
            if (*s != EOF)
            {
                /* next character wasn't a digit, and wasn't EOF so push back the sign character and then the character just read in back on the buffer */
                --s;
            }
            --s;
            return (d); /* to indicate what happened */
        }
    }
    for 
}


/* Stores a line of text in line[] 
    Returns the number of characters read */
int get_line(char line[], int max_line_size)
{
    int char_count = 0;
    int c;
    while ((c = getchar()) != '\n' && c != EOF && char_count < max_line_size)
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

/* Stores lines in an internal memory buffer and stores pointers to them in line_pointer
    Returns the number of lines read
    Returns -1 if out of memory or lines read exceeds max_lines */
int read_lines(char *line_pointer[], int max_line_size)
{
    int number_of_lines_read = 0;
    char line[MAX_LINES];
    char count;
    char *p;
    while ((count = get_line(line, max_line_size)) > 0)
    {
        if (number_of_lines_read >= MAX_LINES || (p = alloc(count)) == NULL)
        {
            printf("Error: Out of memory, or lines read in exceeded max_line_size.\n");
            return -1;
        }
        line[count - 1] = '\0'; /* Delete the new line character */
        strcpy(p, line); /* Copy what we read in, to the memory address within out memory buffer that p points to */
        line_pointer[number_of_lines_read++] = p; /* Add that pointer to our array of pointers */
    }
    return number_of_lines_read;
}

void write_lines(const char *line_pointer[], int number_of_lines)
{
    while(number_of_lines-- > 0)
    {
        printf("%s\n", *line_pointer++);
    }
}


void afree(char *p)
{
    if (p >= alloc_buffer && p < alloc_buffer + MAX_ALLOC_SIZE)
    {
        alloc_pointer = p;
    }
}

/* Returns a pointer to the next free position if there's room, otherwise returns NULL */
char *alloc(int size)
{
    if (alloc_buffer + MAX_ALLOC_SIZE - alloc_pointer >= size) /* Does it fit? */
    {
        alloc_pointer += size;
        return alloc_pointer - size;
    }
    return NULL;
}

/* 
    Validates a c-style string (first param) against the validation routine passed in as the second param.
    Returns 0 if the validation failed.
    Returns non-zero if the validation succeeded */
int validate_input(const char *v[], int (*validation)(const char *), int number_of_lines)
{
    while(number_of_lines-- > 0)
    {
        if (!validation(*v++))
        {
            return 0;
        }
    }
    return 1;
}

int validate_n(const char *s)
{
    return isdigit(*s);
}

int validate_d(const char *s)
{
    
}