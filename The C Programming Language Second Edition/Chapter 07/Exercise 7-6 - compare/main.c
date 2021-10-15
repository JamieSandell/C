/* Write a program to compare two files, printing the first line where they differ. 

can call the program with two file names or specify them yourself */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_PATH 256
#define MAX_LINE_LENGTH 100

struct MY_FILE
{
    FILE *fp;
    char name[MAX_FILE_PATH];
};

int get_line(char *line, int max, FILE *stream);
FILE *open_file(const char *file);

int main(int argc, char *argv[])
{
    if (argc > 3)
    {
        fprintf(stderr, "Error: too many arguments specified\n");
        exit(-1);
    }

    struct MY_FILE files[2];
    if (argc == 1)
    {
        printf("Please enter the first file: ");
        scanf("%s", files[0].name);
        printf("\nPlease enter the second file: ");
        scanf("%s", files[1].name);
        printf("\n");
    }
    else
    {
        strcpy(files[0].name, argv[1]);
        strcpy(files[1].name, argv[2]);
    }

    if ((files[0].fp = fopen(files[0].name, "r")) == NULL)
    {
        fprintf(stderr, "%s: can't open %s\n", argv[0], files[0].name);
        exit(-1);
    }
    if ((files[1].fp = fopen(files[1].name, "r")) == NULL)
    {
        fprintf(stderr, "%s: can't open %s\n", argv[0], files[1].name);
        exit(-1);
    }

    unsigned int current_line_number = 1;
    char line1[MAX_LINE_LENGTH];
    char line2[MAX_LINE_LENGTH];
    while ((get_line(line1, MAX_LINE_LENGTH, files[0].fp)) > 0 && (get_line(line2, MAX_LINE_LENGTH, files[1].fp)) > 0)
    {
        if (strcmp(line1, line2) != 0)
        {
            printf("The files differed on line number: %u\n", current_line_number);
            printf("%s\n", files[0].name);
            printf("%s\n", line1);
            printf("\n");
            printf("%s\n", files[1].name);
            printf("%s\n", line2);
            break;
        }
        ++current_line_number;
    }

    fclose(files[0].fp);
    fclose(files[1].fp);

    return 0;
}

/* get_line: read a line, return length */
int get_line(char *line, int max, FILE *stream)
{
    if (fgets(line, max, stream) == NULL)
    {
        return 0;
    }
    return strlen(line);
}