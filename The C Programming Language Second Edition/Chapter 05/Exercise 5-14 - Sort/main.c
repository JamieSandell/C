/* Modify the sort program to handle a -r flag, which indicates sorting in reverse (decreasing order).
Be sure that -r works with -n. */

#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 100 /* max length of a line, including the null terminating character */
#define MAX_LINES 5000 /* max #lines to be sorted */
char *line_pointer[MAX_LINES]; /* pointers to text lines */

int readlines(char *line_pointer[], int number_of_lines);
void writelines(char *line_pointer[], int number_of_lines);

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