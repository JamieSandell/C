/* The standard library function
    int fseek(FILE *fp, long offset, int origin)
is identical to lseek except that fp is a file pointer instead of a file descriptor
and the return value is an int status, not a position. Write fseek. Make sure
that your fseek coordinates properly with the buffering done for the other
functions of the library. */

#define _DEFAULT_SOURCE
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
/*#include "dirent.h"*/

void fsize(char *);

/* print file sizes */
int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        fsize("."); /* defaulr: current directory */
    }
    else
    {
        while (--argc > 0)
        {
            fsize(++*argv);
        }
    }
    return 0;
}

void dirwalk(char *, void (*fcn)(char *));

/* fsize: print size of file "name" */
void fsize(char *name)
{
    struct stat stbuf;

    if (stat(name, &stbuf) == -1)
    {
        fprintf(stderr, "fsize: can't access %s\n", name);
        return;
    }
    if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
    {
        dirwalk(name, fsize);
    }
    printf("%8ld %s\n", stbuf.st_size, name);
}

#define MAX_PATH 1024

/* dirwalk: apply fcn to all files in dir */
void dirwalk(char *dir, void (*fnc)(char *))
{
    char name[MAX_PATH];
    struct dirent *dp;
    DIR *dfd;

    if ((dfd = opendir(dir)) == NULL)
    {
        fprintf(stderr, "dirwalk: can't open %s\n", dir);
        return;
    }
    while ((dp = readdir(dfd)) != NULL)
    {
        if (strcmp(dp->d_name, ".") == 0 ||  strcmp(dp->d_name, "..") == 0)
        {
            continue; /* skip self and parent */
        }
        if (strlen(dir) + strlen(dp->d_name) + 2 > sizeof(name))
        {
            fprintf(stderr, "dirwalk: name %s/%s too long\n", dir, dp->d_name);
        }
        else
        {
            sprintf(name, "%s/%s", dir, dp->d_name);
            (*fnc)(name);
        }
    }
    closedir(dfd);
}