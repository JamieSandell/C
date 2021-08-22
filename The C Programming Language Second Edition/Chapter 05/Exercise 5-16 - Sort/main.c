/* Add the -d ("directory order") option, which makes comparisons only on letters, numbers and blanks.
    Make sure it works in conjunction with -f */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALLOC_SIZE 10000 /* size of our memory buffer */
#define MAX_LINE_LENGTH 100
#define MAX_LINES 5000 /* Max number of lines to be sorted. */
#define MAX_ARGS 5 /* including room for no args (min size would be 1) */

static char alloc_buffer[ALLOC_SIZE]; /* memory buffer */
static char *alloc_pointer = alloc_buffer; /* next free memory position */
static int reverse = 0; /* logic variables */
static int numeric = 0;
static int case_insensitive = 0;
static int directory_order = 0;
char *line_pointer[MAX_LINES]; /* pointers to text lines */
int (*base_compare)(void *a, void *b); /* our function pointers to comparison functions */
int (*compare)(void *a, void *b);

/* input and output */
int get_line(char line[], int line_size);
int read_lines(char *line_pointer[], int max_number_of_lines);
void write_lines(char* line_pointer[], int number_of_lines);
/* validation */
int validate_d(const char *s);
int validate_n(const char *s);
int validate_input(const char *v[], int number_of_lines, int (*validation)(const char *v));
/* sort and comparisons */
void my_qsort(void *v[], int left, int right, int (*comp)(void *, void*));
int directory_order_comp(const char *s1, const char *s2);
int numcmp(const char *s1, const char *s2);
int reverse_cmp(void *a, void *b);
void str_to_lower(char *s);
int str_case_cmp(const char *s1, const char *s2);
void swap(void *v[], int i, int j);
/* memory */
void afree(char *p);
char *alloc(int size);

int main(int argc, char **argv)
{       
    char arguments[MAX_ARGS]; /* Stores the read in arguments for validation later on. \0 == end of arguments */
    for (int i = 0; i < MAX_ARGS; ++i)
    {
        arguments[i] = '\0';
    }

    /* Set the logic flags based on the arguments passed in */
    int processed_arg = 0;
    int c; 
    while (--argc > 0 && (*++argv)[0] == '-') /* skip the program path argument, then check if the first char of the arg is what we expect */
    {
        while (c = *++argv[0]) /* [] binds tighter than, argv equivalent to:
                                    argv[0] (memory address of the first element that argv is currently pointing to)
                                    ++ increment/walk along this string at argv, to the next character in it
                                    * dereference it all to get the value/character */
        {
            arguments[processed_arg++] = c;
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
                    printf("Error: %c is an invalid argument\n", c);
                    return -1;
            }
        }
    }
    /* Get the input */
    int lines_read;
    if ((lines_read = read_lines(line_pointer, MAX_LINES)) > 0)
    {
        /* Validate the input */
        int (*validation_pointer)(const char *) = NULL; /* function pointer to a validation function */
        for (int i = 0; i < MAX_ARGS && arguments[i] != '\0'; ++i)
        {
            switch (arguments[i])
            {
                case 'n':
                    validation_pointer = validate_n;
                    break;
                case 'd':
                    validation_pointer  = validate_d;
                    break;
            }            
            if (validation_pointer != NULL) /* if the validation pointer points to a function then proceed */
            {
                if (validate_input((const char**)line_pointer, lines_read, validation_pointer) == -1)
                {
                    printf("The validation of input failed when compared with the argument flag -%c\n", c);
                    return -1;
                }
            }
        }
        /* Point to the correct comparison function */
        if (directory_order)
        {
            base_compare = (int (*)(void *, void *))(directory_order_comp); /* only needs to work with -f (case insensitive flag) */
        }
        else
        {
            if (!numeric)
            {
                base_compare = (int (*)(void *, void *))(case_insensitive ? str_case_cmp : strcmp); /* case insenstivie or case sensitive comparison*/
            }
            else
            {
                base_compare = (int (*)(void*, void *))numcmp; /* numeric comparison */
            }
        }
        /* Now we've set our comparison function pointer up, are we bothered about reversing the comparison order? */
        compare = (int (*)(void *, void*))(reverse ? reverse_cmp : base_compare);
        /* Call the sort with our chosen comparison function and then print the output */
        my_qsort((void **)line_pointer, 0, lines_read - 1, compare);
        write_lines(line_pointer, lines_read);
        return 0;
    }
    printf("Error: input too big to sort\n");
    return -1;
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

/* Which makes comparisons only on letters, numbers and blanks.
    You must validate the input before calling.
    Returns 0 if the string contains only letters, numbers and blanks, -1 otherwise. */
int validate_d(const char *s)
{
    while (*s++ != '\0')
    {
        if (isdigit(*s) == 0 && isalpha(*s) == 0 && *s != ' ')
        {
            return -1;
        }
    }
    return 0;
}

/* Returns 0 if the string is numerical, -1 if not */
int validate_n(const char *s)
{
    if (isalpha(s) == 0)
    {
        return -1;
    }
    return 0;
}



/* Returns 0 if the input is valid (validation is the pointer to the validation function to use).
    -1 if the input is invalid */
int validate_input(const char *v[], int number_of_lines, int (*validation)(const char *))
{
    while (number_of_lines-- > 0)
    {
        if (validation(*v++) != 0)
        {
            return -1;
        }
    }
    return 0;
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

/* Which makes comparisons only on letters, numbers and blanks.
    You must validate the input before calling.
    0 if strings are equal
    1 if s1 has a greater numerical value than s2
    -1 if s1 has a lesser numerical value than s2 */
int directory_order_comp(const char *s1, const char *s2)
{
    return (case_insensitive ? str_case_cmp(s1, s2) : strcmp(s1, s2));
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