/* Rewrite the program cat from Chapter 7 using read, write, open and close instead of their standard library equivalents.
Perform experiments to determine the relative speeds of the two versions. */

#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "unistd.h"

void error(const char *fmt, ...);
void file_copy(int fdin, int fdout);

int main(int argc, char *argv[])
{
    if (argc == 1) /* no command args specified */
    {
        file_copy(0, 1); /* 0 == standard input, 1 == standard output*/
    }
    else
    {
        int fd;
        while (--argc > 0)
        {
            if ((fd = open(*++argv, O_RDONLY, 0)) == -1)
            {
                error("%s can't open %s for reading", argv[0], argv);
            }
            else
            {
                file_copy(fd, 1);
                close(fd);
            }
        }        
    }
    return 0;
}

/* error: print an error message and die */
void error(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, "Error: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}

/* file_copy: copies from file descriptor in to file descriptor out */
void file_copy(int fdin, int fdout)
{
    char buf[BUFSIZ];
    int n_read;

    while ((n_read = read(fdin, buf, BUFSIZ)) > 0)
    {
        write(fdout, buf, n_read);
    }
}