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
static case_insensitive = 0;
static directory = 0;
static numeric = 0;
static reverse = 0;

/* Compare and sort */
void my_qsort(void *v[], int left, int right, int (*compare)(void *a, void *b));
int numcmp(char *s1, char *s2);
void swap(void *v[], int i, int j);
/* i/o */
int get_line(char line[], int max_line_size);
int read_lines(char *line_pointer[], int max_lines);
void write_lines(char *line_pointer[], int number_of_lines);
/* Validation */
/* Memory */
void afree(char *p);
char *alloc(int size);

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

/* Swap the ith and jth elements of v[] */
void swap(void *v[], int i, int j)
{
    void *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
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