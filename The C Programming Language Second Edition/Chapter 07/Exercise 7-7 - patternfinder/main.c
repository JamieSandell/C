/* Modify the pattern finding program of Chapter 5 to take its input from a set of named files or,
if no files are named as arguments, from the standard input. Should the file name be printed when a matching line is found? 

patternfinder [optional]-x [optional]-n [required]-ppattern [optional]file1.txt...[optional]fileN.txt */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 100
#define MAX_ARGS MAX_FILES + 3
#define MAX_FILE_PATH 100
#define MAX_PATTERN 100

struct flags
{
    unsigned int exclusion          : 1;
    unsigned int print_line_number  : 1;
} flags;

struct my_file
{
    FILE *fp;
    char file_path[MAX_FILE_PATH];
};

struct my_file *create_my_file(FILE *fp, const char *file_path);
int get_line(char *line, unsigned int max, FILE *fp);
FILE *open_file(const char *file_path, const char *mode, const char *program_name);

int main(int argc, char *argv[])
{
    if (argc > MAX_ARGS)
    {
        fprintf(stderr, "Too many arguments, max arguments: %i\n", MAX_ARGS);
        exit(-1);
    }

    char pattern[MAX_PATTERN];
    if (argc == 1) /* No cmd flags specified */
    {
        printf("Please enter the pattern to find:\t");
        get_line(pattern, MAX_PATTERN, stdin);
    }
    else /* Process the cmd flags */
    {
        struct my_file *files[MAX_FILES];
        unsigned int files_index = 0;
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
                FILE *fp = fopen(c, "r");
                if (fp == NULL)
                {
                    fprintf(stderr, "Error: %s failed to open the file: %s\n", argv[0], c);
                    exit(-1);
                }
                files[files_index] = create_my_file(fp, c);
                if (files[files_index] == NULL)
                {
                    fprintf(stderr, "failed to create my_file from file: %s\n", c);
                }
                ++files_index;
            }       
        }
        for (unsigned int i = 0; i < files_index; ++i)
        {
            if (!flags.exclusion)
            {
                if (strstr(files[i].))
            }
        }
    }

    return 0;
}

struct my_file *create_my_file(FILE *fp, const char *file_path)
{
    struct my_file *file;

    file = malloc(sizeof(*file));
    if (file == NULL)
    {
        return NULL;
    }
    strcpy(file->file_path, file_path);
    file->fp = fp;
    return file;
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

/* open_file: Attempts to open the file with desired access mode. Returns a pointer to the file if successful.
Returns NULL if unsuccessful.
program_name is the name of the program attempting to open the file, i.e. argv */
FILE *open_file(const char *file_path, const char *mode, const char *program_name)
{
    FILE *fp;
    if ((fp = fopen(file_path, mode)) == NULL)
    {
        fprintf(stderr, "%s failed to open %s\n", program_name, file_path);
        return NULL;
    }
    return fp;
}