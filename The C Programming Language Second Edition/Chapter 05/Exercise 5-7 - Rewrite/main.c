/* Rewrite readlines to store lines in an array supplied by main,
rather than calling alloc to maintain storage. How much faster is the program? */

#include <stdio.h>
#include <string.h>

#define MAX_LINES 5000 /* max #lines to be sorted */

char *line_ptr[MAX_LINES]; /* pointers to text lines */

char *alloc(int n);
void afree(char *p);

int get_line(char *s, int lim);
int readlines(char *line_ptr[], int max_lines);
void writelines(char *line_ptr[], int nlines);

void qsort(char *lineptr[], int left, int right);
void swap(char *v[], int i, int j);

/* sort input lines */
int main()
{
    int nlines; /* number of input lines read */

    if ((nlines = readlines(line_ptr, MAX_LINES)) >= 0)
    {
        qsort(line_ptr, 0, nlines - 1);
        writelines(line_ptr, nlines);
        return 0;
    }
    else
    {
        printf("Error: input too big to sort\n");
        return 1;
    };
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

int get_line(char *s, int lim)
{
    int c, i;

    i = 0;
    while(--lim > 0 && (c = getchar()) != EOF && c != '\n')
    {
        *(s + i++) = c;
    }
    if (c == '\n')
    {
        *(s + i++) = c;
    }
    *(s + i) = '\0';

    return i;
}

#define MAX_LEN 1000 /* max length of any input line */

/* readlines: read input lines */
int readlines(char *line_ptr[], int max_lines)
{
    int len, nlines;
    char *p, line[MAX_LEN];

    nlines = 0;
    while ((len = get_line(line, MAX_LEN)) > 0)
    {
        if (nlines >= max_lines || (p = alloc(len)) == NULL) /* exceeded line count or not enough room to alloc from the buffer */
        {
            return -1;
        }
        else
        {
            line[len - 1] = '\0'; /* delete the newline character */
            strcpy(p, line); /* p now points to memory from alloc, copy the line just read in to it */
            line_ptr[nlines++] = p; /* set the current pointer in the array to point to the same address as p, then increment # lines read in */
        }
    }
    return nlines;
}

/* writelines: write output lines */
void writelines(char *line_ptr[], int nlines)
{
    while(nlines-- > 0)
    {
        printf("%s\n", *line_ptr++);
    }
}

/* qsort: sort v[left]...v[right] into increasing order */
void qsort(char *v[], int left, int right)
{
    int i, last;
    void swap(char *v[], int i, int j);

    if (left >= right) /* do nothing if array contains fewer than two elements */
    {
        return;
    }
    swap(v, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i<= right; i++)
    {
        if (strcmp(v[i], v[left]) < 0)
        {
            swap(v, ++last, i);
        }
    }
    swap(v, left, last);
    qsort(v, left, last - 1);
    qsort(v, last + 1, right);
}

/* swap: interchange v[i] and v[j] */
void swap(char *v[], int i, int j)
{
    char *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}