/* Write a program to print a set of files, starting each new one on a new page, with a title and a running page count for each file */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINES_PER_PAGE 60
#define MAX_FILES 100
#define MAX_LINE_LENGTH 100

struct file
{
    FILE *fp;
    char *file_path;
    unsigned int page_count;
    char *title;
};

size_t get_line(char *line, unsigned int max_line_length, FILE *stream);
void print_file(struct file *file, FILE *stream);

int main(int argc, char *argv[])
{
    struct file *files = malloc((sizeof(struct file)) * MAX_FILES);
    if (files == NULL)
    {
        fprintf(stderr, "%s failed to allocate memory for %u files\n", argv[0], MAX_FILES);
        exit(-1);
    }
    unsigned int file_count = 0;
    size_t line_size;
    char line[MAX_LINE_LENGTH]; 
    if (argc == 1) /* no arguments passed in */
    {           
        printf("Enter the file names including the file path, seperate each one by pressing enter\n");
        while ((line_size = (get_line(line, MAX_LINE_LENGTH, stdin))) > 0)
        {
            line[line_size - 1] = '\0'; /* remove the newline character */
            if (file_count == MAX_FILES)
            {
                fprintf(stderr, "Error: %s exceed file count. Max files : %u\n", argv[0], MAX_FILES);
                break;
            }
            files[file_count].fp = fopen(line, "r");
            if (files[file_count].fp == NULL)
            {
                fprintf(stderr, "Error: %s failed to open %s\n", argv[0], line);
                exit(-1);
            }
            fclose(files[file_count].fp);
            files[file_count].file_path = strdup(line);
            if (files[file_count].file_path == NULL)
            {
                fprintf(stderr, "Error: %s failed to allocate memory for file->file_path\n", argv[0]);
                exit(-1);
            }
            files[file_count].page_count = 0;
            files[file_count].title = strdup(line);
            if (files[file_count].title == NULL)
            {
                fprintf(stderr, "Error: %s failed to allocate memory for file->title\n", argv[0]);
                exit(-1);
            }
            ++file_count;
        }
    }
    /* print files */
    struct file *fp_start = files;
    struct file *fp_end = fp_start + file_count;
    struct file *iter = fp_start;
    while (iter < fp_end)
    {
        print_file(iter, stdout);
        ++iter;
    }
    /* free memory */
    iter = fp_start;
    while (iter < fp_end)
    {
        free(iter->file_path);
        free(iter->title);
        ++iter;
    }
    free(files);

    return 0;
}

size_t get_line(char *line, unsigned int max_line_length, FILE *stream)
{
    if (fgets(line, max_line_length, stream) == NULL)
    {
        return 0;
    }
    return strlen(line);
}

void print_file(struct file *file, FILE *stream)
{
    static unsigned int page_number = 0;
    static unsigned int line_number = 1;
    file->fp = fopen(file->file_path, "r");
    if (line_number != 1)
    {
        do
        {
            printf("\n");
            ++line_number;
            if (line_number == LINES_PER_PAGE)
            {
                line_number = 1;
            }
        } while ( line_number != 1);
        fprintf(stream, "\nPage %u\n", ++page_number);     
    }
    file->page_count = 1;
    printf("%s\n", file->title);
    ++line_number;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file->fp))
    {
        if (line_number > LINES_PER_PAGE)
        {
            line_number = 1;
            fprintf(stream, "Page %u\n", ++page_number);
            file->page_count++;
        }
        fprintf(stream, "%s", line);
        ++line_number;        
    }
    fclose(file->fp);
    fprintf(stream, "\nNumber of pages for %s : %u\n", file->title, file->page_count);
}