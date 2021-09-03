/* Add a field-handling capability, so sorting may be done on fields within lines, each field sorted according to an independent set of options.
(The index for this book was sorted with -df for the index category and -n for the page numbers.) 

Thoughts:
For this we can use a comma as the delimiter to split the line into columns.
Imagine our data is this:
Sandall,Jamie,1987
Sandall,Trudi,1987
Sandall,Bradley,1993
Sandfall,Timmy,1994
Foster,Karl,1971

If sorted in descending order by numeric on age, and then ascending order on Surnames should get this:
Sandfall,Timmy,1994
Sandall,Bradley,1993
Sandall,Jamie,1987
Sandall,Trudi,1987
Foster,Karl,1971

Usage:
-t -k<n>ComparisonFlags
e.g.
-t, -k3nr -k1

says that the delimiter is a comma, that the fields (keys) to sort on is the 3rd field in numeric reverse order and the second field (key)
to sort on is the 1st field (counting starts from 1) in the default (ASCII ascending)

To achieve this, if at least one field has been specified to sort on, then we should just need to compare on the most important field.
If the comparator returns 0, then no change is required based on sorting on that field (as in the values compared are equal), then we can move on
to the next most important field (if one was specified) and repeat the procedure until all specified fields to sort on for that line have been processed.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARGS 5
#define MAX_KEYS 4
#define MAX_LINES 100 /* Max number of lines to read in */
#define MAX_LINE_SIZE 100 /* including the null terminating character */
#define MAX_ALLOC_SIZE 10000 /* for the custom memory management */

/* Function pointers */
static int (*base_compare)(void *, void *);
/* Memory */
static char alloc_buffer[MAX_ALLOC_SIZE];
static char* alloc_pointer = alloc_buffer;
/* State */
static int case_insensitive;
static int directory;
static int numeric;
static int reverse;
static int keys;
/* Data */
static char delimiter;
static unsigned int number_of_keys;

/* Compare and sort */
void my_qsort(void *v[], int left, int right, int (*compare)(void *a, void *b));
int directory_order_comp(const char *s1, const char *s2);
int numcmp(char *s1, char *s2);
int reverse_cmp(void *a, void *b);
int str_case_cmp(const char *s1, const char *s2);
void swap(void *v[], int i, int j);
/* Conversions */
int partial_str_to_uint(const char *s, unsigned int *number);
int str_to_float(const char *s, float *number);
void str_to_lower(char *s);
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

int main(int argc, char *argv[])
{
    
    if (argc > MAX_ARGS)
    {
        printf("Error: Too many arguments, max is %i\n", MAX_ARGS - 1); /* ignore the first arg as that's the program filepath */
        return -1;
    }
    char arguments[MAX_ARGS] = {0}; /* Stores the read in arguments for validation later on. \0 == end of arguments */
    int processed_arg = 0;

    char *line_pointer[MAX_LINES]; /* To store the pointers to our lines that will be in our memory buffer */
    int argc_initial_value = argc; /* cache a copy as we'll be changing argc, as we will need the original value later on */
    int c;
    uint key;
    /* Process the arguments, turning on any requested states */
    while(--argc > 0 && (*++argv)[0] == '-') /* skip the program path argument, then check if the first char of the arg is what we expect */
    {
        while (c = *++argv[0]) /*   [] binds tighter than ++
                                    argv gives us the memory address of the first array (char string), [0] gives us the address of the first element of that
                                    ++ increment/walk along that array to get the address of that character / skipping the first character ('-')
                                    * dereference it to get the value/character */
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
                case 'd':
                    directory = 1;
                    break;
                case 'f':
                    case_insensitive = 1;
                    break;
                case 't':
                    c = *++argv[0];
                    delimiter = c;
                    break;
                case 'k':                                     
                    if (!partial_str_to_uint(*argv, &key))
                    {
                        printf("Error: no number specified for %s\n", *argv[0]);
                        return -1;
                    }
                    keys = 1;
                    number_of_keys++;
                    break;
                default:
                    printf("Error: %c is an invalid argument\n", c);
                    return -1;
            }
        }
    }
    /* Get the input */
    int lines_read = 0;
    if ((lines_read = read_lines(line_pointer, MAX_LINE_SIZE)) > 0)
    {
        /* Validate the input */
        int (*validation_pointer)(const char *) = NULL; /* Function pointer to a validation function */
        for (int i = 0; arguments[i] != '\0'; ++i)
        {
            switch (arguments[i])
            {
                case 'k':
                case 'r':
                case 'f':
                    validation_pointer = NULL;
                    break;
                case 'n':
                    validation_pointer = validate_n;
                    break;
                case 'd':
                    validation_pointer = validate_d;
                    break;
                default:
                    printf("Warning: no validation case for argument %c\n", arguments[i]);
                    break;
            }
            if (validation_pointer != NULL)
            {
                if (!(validate_input((const char**)line_pointer, validation_pointer, lines_read)))
                {
                    printf("The validation of input failed when compared with the argument flag %c\n", arguments[i]);
                    return -1;
                }
            }
        }
        /* Point to the correct comparison function */
        if (directory)
        {
            base_compare = (int (*)(void *, void *))(directory_order_comp); /* only needs to work with -f (case insensitive flag) */
        }
        else
        {
            if (!numeric)
            {
                base_compare = (int (*)(void *, void*))(case_insensitive ? str_case_cmp: strcmp);
            }
            else
            {
                base_compare = (int (*)(void *, void*))numcmp;
            }
        }
        /* Do we need to reverse the comparison? */
        int (*compare)(void *, void *) = (int (*)(void *, void*))(reverse ? reverse_cmp: base_compare);
        /* Sort with the correct comparison routine and output the result */
        my_qsort((void **)line_pointer, 0, lines_read - 1, compare);
        write_lines((const char**)line_pointer, lines_read);
        return 0;
    }

    printf("Error: input too big to sort\n");
    return -1;
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

/* Which makes comparisons only on letters, numbers and blanks.
    You must validate the input before calling.
    0 if strings are equal
    1 if s1 has a greater numerical value than s2
    -1 if s1 has a lesser numerical value than s2 */
int directory_order_comp(const char *s1, const char *s2)
{
    return (case_insensitive ? str_case_cmp(s1, s2) : strcmp(s1, s2));
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

int reverse_cmp(void *a, void *b)
{
    return base_compare(b, a);
}

/* Case insensitive.
    Returns:
    0 if strings are equal
    1 if the first non-matching character in s1 has a greater ASCII value than that of s2
    -1 if the first non-matching character in s1 has a lesser ASCII value than that of s2 */
int str_case_cmp(const char *s1, const char *s2)
{
    char t1[MAX_LINE_SIZE];
    char t2[MAX_LINE_SIZE];
    strcpy(t1, s1);
    strcpy(t2, s2);
    str_to_lower(t1);
    str_to_lower(t2);
    return strcmp(t1, t2);
}

/* Swap the ith and jth elements of v[] */
void swap(void *v[], int i, int j)
{
    void *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

/* Extracts the first continuous character (ASCII) data that is numerical and converts it to a positive integer
    e.g. -k12nr would result in 12 being extracted and stored in *number
    Returns 1 if a number was found, otherwise it returns 0 */
int partial_str_to_uint(const char *s, unsigned int *number)
{
    char temp[100] = {0};
    unsigned int i = 0;
    unsigned int number_found = 0;
    while (*s != '\0')
    {
        if (*s >= 48 && *s <= 57)
        {
            temp[i++] = *s;
            number_found = 1;
        }
        else if (number_found == 1)
        {
            /* we want continuous numbers characters, not, e.g. 3n3, we would want 33 */
            break;
        }
        s++;
    }
    if (number_found)
    {
        *number = atoi(temp);
    }
    return number_found;
}

/*  Tries to convert *s to a float and store it in *number
    Returns 0 if not a number
    Returns non-zero if success */
int str_to_float(const char *s, float *number)
{
    /* TODO: Code adapted from GetFloat earlier in chapter 5. Some wasteful operations such as "push back on to the buffer" can be omitted */
    int sign;
    float power;

    if (!isdigit(*s) && *s != EOF && *s != '+' && *s != '-')
    {
        *s--; /* it's not a number */
        return 0;
    }
    sign = (*s == '-') ? -1 : 1;
    if (*s == '+' || *s == '-')
    {
        int d = *s; /* Remember the sign character */
        s++;
        if (!isdigit(*s))
        {
            return 0;
        }
    }
    for (*number = 0.0f; isdigit(*s); s++)
    {
        *number = 10.0f * *number + (*s - '0');
    }
    if (*s == '.') /* Get the fractional part */
    {
        s++;
    }
    for (power = 1.0f; isdigit(*s); s++)
    {
        *number = 10.0f * *number + (*s - '0');
        power *= 10.0f;
    }

    *number *= sign / power;
    return 1;
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

/*  Validates a c-style string (first param) against the validation routine passed in as the second param.
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

/*  Returns 0 if not a valid number
    Returns 1 if a valid number */
int validate_n(const char *s)
{
    float number;
    float *number_pointer = &number;
    return str_to_float(s, number_pointer);
}

/* Which makes comparisons only on letters, numbers and blanks.
    You must validate the input before calling.
    Returns non-zero if the string contains only letters, numbers and blanks, 0 otherwise. */
int validate_d(const char *s)
{
    while (*s != '\0')
    {
        if (!isspace(*s) && !isalnum(*s))
        {
            return 0;
        }    
        ++s;
    }
    return 1;
}