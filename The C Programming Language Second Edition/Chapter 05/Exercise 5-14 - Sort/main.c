/* Modify the sort program to handle a -r flag, which indicates sorting in reverse (decreasing order).
Be sure that -r works with -n. */

#include <stdio.h>
#include <string.h>

#define MAX_LINES 5000 /* max #lines to be sorted */
char *line_pointer[MAX_LINES]; /* pointers to text lines */

int readlines(char *line_pointer[], int number_of_lines);
void writelines(char *line_pointer[], int number_of_lines);

void qsort(void *line_pointer[], int left, int right, int (*comp)(void *, void *));
int numcmp(char *, char *);

int main(int argc, char *argv[])
{
    return 0;
}