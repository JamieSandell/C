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
int numcmp(const char *s1, const char *s2);
int reverse_cmp(void *a, void *b);
int str_case_cmp(const char *s1, const char *s2);

void afree(char *p);
char *alloc(int size);

int main(int argc, char **argv)
{
    return 0;
}

int get_line(char line[], int line_size)
{
    int char_count = 0;
    
    return char_count;
}