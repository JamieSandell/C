/* Modify the fsize program to to print the other information contained in the inode entry. */

#define _DEFAULT_SOURCE
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

char *format_date(char *str, time_t val);
void fsize(char *name);

/* print file sizes */
int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        fsize("."); /* default: current directory */
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

/* https://stackoverflow.com/questions/26306644/how-to-display-st-atime-and-st-mtime/26307281 */
char *format_date(char *str, time_t val)
{
    strftime(str, 36, "%Y.%m.%d %H:%M:%S", localtime(&val));
    return str;
}

void dirwalk(char *dir, void (*fcn)(char *));

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
    printf("File: %s\n", name);
    printf("--------------------------------\n");
    printf("%-40s%20lu\n", "ID of device containing file:", stbuf.st_dev);
    printf("%-40s%20lu\n", "Inode number:", stbuf.st_ino);
    printf("%-40s%20u\n", "File type and mode:", stbuf.st_mode);
    printf("%-40s%20u\n", "Number of hard links:", stbuf.st_nlink);
    printf("%-40s%20u\n", "User ID of owner", stbuf.st_uid);
    printf("%-40s%20u\n", "Group of ID owner:", stbuf.st_gid);
    printf("%-40s%20lu\n", "Device ID (if special file):", stbuf.st_rdev);
    printf("%-40s%20ld\n", "Total size in bytes:", stbuf.st_size);
    printf("%-40s%20ld\n", "Block size for file system I/O:", stbuf.st_blksize);
    printf("%-40s%20ld\n", "Number of 512B blocks allocated:", stbuf.st_blocks);
    char date[36];
    printf("%-40s%20s\n", "Access:", format_date(date, stbuf.st_atime));
    printf("%-40s%20s\n", "Modify:", format_date(date, stbuf.st_mtime));
    printf("%-40s%20s\n", "Change:", format_date(date, stbuf.st_ctime));
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