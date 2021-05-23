/* Rewrite readlines to store lines in an array supplied by main,
rather than calling alloc to maintain storage. How much faster is the program? */

#include <stdio.h>
#include <string.h>

#define MAX_LINES 5000 /* max #lines to be sorted */

char *line_ptr[MAX_LINES]; /* pointers to text lines */

char *alloc(int n);
void afree(char *p);

int readlines(char *line_ptr[], int nlines);
void writelines(char *line_ptr[], int nlines);

void qsort(char *lineptr[], int left, int right);

/* sort input lines */
int main()
{
    return 0;
}

#define ALLOC_SIZE 10000 /* size of available space */

static char allocbuf[ALLOC_SIZE]; /* private storage for alloc */
static char *allocp = allocbuf; /* private pointer to next free position */

/* return pointer to n characters */
char *alloc(int n)
{
    if (allocbuf + ALLOC_SIZE - allocp >= n)
    {
        /* it fits */
        allocp += n; /* point to the next free position */
        return allocp - n; /* old p */
    }
    else
    {
        /* not enough room */
        return 0;
    }
}

/* free storage pointed to by p */
void afree(char *p)
{
    if (p >= allocbuf && p < allocbuf + ALLOC_SIZE) /* memory to free resides within our buffer */
    {
        allocp = p;
    }
}