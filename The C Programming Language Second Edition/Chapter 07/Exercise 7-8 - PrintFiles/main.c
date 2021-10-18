/* Write a program to print a set of files, starting each new one on a new page, with a title and a running page count for each file */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 100
#define MAX_LINE_LENGTH 100

struct file
{
    FILE *fp;
    unsigned int page_count;
    char *title;
};

struct file *create_file(const char *file_path);
unsigned int get_line(char *line, unsigned int max_line_length, FILE *stream);
void print_file(struct file *file, FILE *stream);

int main(int argc, char *argv[])
{
    struct file *files = {0};
    unsigned int file_count = 0;
    unsigned int line_size;
    char line[MAX_LINE_LENGTH]; 
    if (argc == 1) /* no arguments passed in */
    {           
        printf("Enter the file names including the file path, seperate each one by pressing enter\n");
        while (line_size = (get_line(line, MAX_LINE_LENGTH, stdin)) > 0)
        {
            if (file_count == MAX_FILES)
            {
                fprintf(stderr, "Error: %s exceed file count. Max files : %u\n", argv[0], MAX_FILES);
                break;
            }
        }
    }

    return 0;
}

struct file *create_file(const char *file_path)
{
    struct file *file = malloc(sizeof(struct file));
    if (file == NULL)
    {
        fprintf(stderr, "Error: failed to allocate memory for %s\n", file_path);
        return NULL;
    }

    file->fp = fopen(file_path, "r");
    if (file->fp == NULL)
    {
        fprintf(stderr, "Error: failed to open %s\n");
        return NULL;
    }
    file->page_count = 0;
    file->title = strdup(file_path);
    return file;
}

unsigned int get_line(char *line, unsigned int max_line_length, FILE *stream)
{
    if (fgets(line, max_line_length, stream) == NULL)
    {
        return 0;
    }
    return strlen(line);
}

void print_file(struct file *file, FILE *stream)
{
    
}