/* Modify the pattern finding program of Chapter 5 to take its input from a set of named files or,
if no files are named as arguments, from the standard input. Should the file name be printed when a matching line is found? 

patternfinder [optional]-x [optional]-n [optional]-ppattern [optional]file1.txt...[optional]fileN.txt

Note: If you specify a pattern you must also specify text files for input and vice versa. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 100
#define MAX_ARGS MAX_FILES + 3
#define MAX_FILE_PATH 100
#define MAX_LINE_LENGTH 100
#define MAX_LINES 1000
#define MAX_PATTERN 100

struct flags
{
    unsigned int exclusion          : 1;
    unsigned int print_line_number  : 1;
} flags;

struct input
{
    char file_path[MAX_FILE_PATH];
    FILE *fp;
    char *lineptr[MAX_LINES];  
    unsigned int number_of_lines;      
};

struct input *create_input(void);
int get_line(char *line, unsigned int max, FILE *fp);

int main(int argc, char *argv[])
{
    if (argc > MAX_ARGS)
    {
        fprintf(stderr, "Too many arguments, max arguments: %i\n", MAX_ARGS);
        exit(-1);
    }

    flags.exclusion = 0;
    flags.print_line_number = 0;
    struct input *inputs[MAX_FILES];
    unsigned int input_index = 0;
    unsigned int input_count = 0;
    char line[MAX_LINE_LENGTH];
    unsigned int line_size;
    char pattern[MAX_PATTERN];
    /* create our inputs */
    if (argc == 1) /* No cmd flags specified */
    {        
        printf("Please enter the pattern to find:\t");        
        line_size = get_line(pattern, MAX_PATTERN, stdin);
        pattern[line_size - 1] = '\0'; /* delete the newline character */
        line[MAX_LINE_LENGTH];
        unsigned int line_count = 0;
        inputs[input_index] = create_input();
        ++input_count;
        inputs[input_index]->file_path[0] = '\0';
        inputs[input_index]->fp = stdin;
        while((line_size = get_line(line, MAX_LINE_LENGTH, stdin)) > 0 && inputs[input_index]->number_of_lines < MAX_LINES)
        {
            line[line_size - 1] = '\0';
            inputs[input_index]->lineptr[inputs[input_index]->number_of_lines++] = strdup(line);
        }
    }
    else /* Process the cmd flags */
    {
        FILE *fp;
        char *c;
        while (--argc > 0)
        {
            c = *++argv;
            if (*c == '-') /* not a file */
            {
                switch (*++c)
                {
                    case 'x':
                        flags.exclusion = 1;
                        break;
                    case 'n':
                        flags.print_line_number = 1;
                        break;
                    case 'p':
                        strcpy(pattern, ++c);
                        break;
                    default:
                        fprintf(stderr, "Error: unknown argument %c\n", *c);
                        exit(-1);
                }
            }
            else /* file */
            {
                fp = fopen(c, "r");
                if (fp == NULL)
                {
                    fprintf(stderr, "Error: %s failed to open the file: %s\n", argv[0], c);
                    exit(-1);
                }
                inputs[input_index] = create_input();            
                if (inputs[input_index] == NULL)
                {
                    fprintf(stderr, "Error: %s failed to create inputs[%u] from file: %s\n", argv[0], input_index, c);
                    exit(-1);
                }
                strcpy(inputs[input_index]->file_path, c);
                while((line_size = get_line(line, MAX_LINE_LENGTH, fp)) > 0 && inputs[input_index]->number_of_lines < MAX_LINES)
                {
                    line[line_size - 1] = '\0';
                    inputs[input_index]->lineptr[inputs[input_index]->number_of_lines++] = strdup(line);
                }         
                fclose(fp);
                ++input_index;
                ++input_count;
            }       
        }
    }
    /* search for the pattern */
    for (input_index = 0; input_index < input_count; ++input_index)
    {
        for (unsigned int line = 0; line < inputs[input_index]->number_of_lines; ++line)
        {
            if ((strstr(inputs[input_index]->lineptr[line], pattern) != NULL) != flags.exclusion)
            {                
                if (inputs[input_index]->file_path[0])
                {
                    printf("Found '%s' in '%s'\n", pattern, inputs[input_index]->file_path);
                }                
                else
                {
                    printf("Found %s\n", pattern);
                }
                if (flags.print_line_number)
                {
                    printf(" on line number %u\n", line + 1);
                }
            }
        }
    }
    /* free memory */
    for (input_index = 0; input_index < input_count; ++input_index)
    {
        for (unsigned int line = 0; line < inputs[input_index]->number_of_lines; ++line)
        {
            free(inputs[input_index]->lineptr[line]);
        }
        free(inputs[input_index]);
    }

    return 0;
}

struct input *create_input(void)
{
    struct input *input;

    input = malloc(sizeof(*input));
    if (input == NULL)
    {
        return NULL;
    }
    return input;
}

int get_line(char *line, unsigned int max, FILE *fp)
{
    unsigned int count = 0;

    if (fgets(line, max, fp) == NULL)
    {
        return 0;
    }
    return strlen(line);
}