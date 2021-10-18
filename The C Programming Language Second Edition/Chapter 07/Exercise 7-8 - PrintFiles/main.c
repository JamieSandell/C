/* Write a program to print a set of files, starting each new one on a new page, with a title and a running page count for each file */

#include <stdio.h>

struct file
{
    FILE *fp;
    unsigned int page_count;
    char *title;
};

unsigned int get_line(char *line, unsigned int max_line_length, FILE *stream);
void print_file(struct file *file, FILE *stream);

int main(int argc, char *argv[])
{
    return 0;
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