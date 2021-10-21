/* Design and write _flushbuf, fflush, and fclose */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "syscall.h"

#define PERMS 0666 /* RW for owner, group, others */

int _fillbuf(FILE *fp);
int _flushbuf(int, FILE *stream);
int fclose(FILE *stream);
int fflush(FILE *stream);
FILE *my_fopen(const char *name, const char *mode);

/* stdin, stdout stderr
character left, next character position, location of buffer, read, write, unbuffered, eof, error, file descriptor */
FILE _iob[OPEN_MAX] = { { 0, (char *)0, (char *)0, 1, 0, 0, 0, 0, 0 },
			{ 0, (char *)0, (char *)0, 0, 1, 0, 0, 0, 1 },
			{ 0, (char *)0, (char *)0, 0, 1, 1, 0, 0, 2 } };

int main(int argc, char *argv[])
{
    /* TODO: error checking */
    FILE *fp;
    if ((fp = my_fopen(argv[1], "r")) == NULL)
    {
        return 1;
    }
    else
    {
        int c;
        while ((c = getc(fp)) != EOF)
        {
            write(1, &c, 1);
        }
    }
    return 0;
}

/* _fillbuf: allocate and fill input buffer */
int _fillbuf(FILE *fp)
{
    int buf_size;

    if (!fp->flag._READ || fp->flag._EOF || fp->flag._ERR)
    {
        return EOF;
    }
    buf_size = (fp->flag._UNBUF) ? 1 : BUFSIZ;
    if (fp->base == NULL) /* no buffer yet */
    {
        if ((fp->base = malloc(buf_size)) == NULL)
        {
            return EOF; /* can't get buffer */
        }
    }
    fp->ptr = fp->base;
    fp->cnt = read(fp->fd, fp->ptr, buf_size);
    if (--fp->cnt < 0)
    {
        if (fp->cnt == -1)
        {
            fp->flag._EOF = 1;
        }
        else
        {
            fp->flag._ERR = 1;
        }
        fp->cnt = 0;
        return EOF;
    }
    return (unsigned char) *fp->ptr++;
}

/* _flushbuf: flush the contents of the buffer to the file */

/* fclose: Closes the stream. All buffers are flushed.
This method returns zero if the stream is successfully closed. On failure, EOF is returned. */
int fclose(FILE *stream)
{

}

/* fflush: Flushes the output buffer of a stream.*/
int fflush(FILE *stream)
{

}

/* my_fopen: open file, return file pointer */
FILE *my_fopen(const char *name, const char *mode)
{
    int fd;
    FILE *fp;

    if (*mode != 'r' && *mode != 'w' && *mode != 'a')
    {
        return NULL;
    }
    for (fp = _iob; fp < _iob + OPEN_MAX; ++fp)
    {
        if (!fp->flag._READ || !fp->flag._WRITE)
        {
            break; /* found a free slot */
        }
    }
    if (fp >= _iob + OPEN_MAX) /* no free slots */
    {
        return NULL;
    }

    if (*mode == 'w')
    {
        fd = creat(name, PERMS);
    }
    else if (*mode == 'a')
    {
        if ((fd = open(name, O_WRONLY, 0)) == -1)
        {
            fd = creat(name, PERMS); /* file doesn't exist so create it */
        }
        lseek(fd, 0L, 2);
    }
    else
    {
        fd = open(name, O_RDONLY, 0);
    }
    if (fd == -1)
    {
        return NULL;
    }
    fp->fd = fd;
    fp->cnt = 0;
    fp->base = NULL;
    if (*mode == 'r')
    {
        fp->flag._READ = 1;
    }
    else
    {
        fp->flag._WRITE = 1;
    }
    return fp;
}