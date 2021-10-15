/* Modify the pattern finding program of Chapter 5 to take its input from a set of named files or,
if no files are named as arguments, from the standard input. Should the file name be printed when a matching line is found? 

patternfinder [optional]-x [optional]-n [required]pattern [optional]file1.txt...[optional]fileN.txt */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 100
#define MAX_FILE_PATH 100

struct my_file
{
    FILE *fp;
    char file_path[MAX_FILE_PATH];
};

int get_line(char *line, unsigned int max, FILE *fp);
FILE *open_file(const char *file_path, const char *mode, const char *program_name);

int main(int argc, char *argv[])
{
    struct my_file files[MAX_FILES] = {0};
    unsigned int files_index = 0;

    return 0;
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