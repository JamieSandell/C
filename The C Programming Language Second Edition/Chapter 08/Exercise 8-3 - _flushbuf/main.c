/* Design and write _flushbuf, fflush, and fclose */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "syscall.h"

#define PERMS 0666 /* RW for owner, group, others */

int _fillbuf(FILE *fp);
int _flushbuf(int, FILE *fp);
int fclose(FILE *fp);
int fflush(FILE *fp);
FILE *my_fopen(const char *name, const char *mode);

/* stdin, stdout stderr
character left, next character position, location of buffer, (read, write, unbuffered, eof, error), file descriptor */
FILE _iob[OPEN_MAX] = {
	{ 0, (char *) 0, (char *) 0, _READ, 0 },
	{ 0, (char *) 0, (char *) 0, _WRITE, 1 },
	{ 0, (char *) 0, (char *) 0, _WRITE | _UNBUF, 2 }
};


int main(int argc, char *argv[])
{
    FILE *fp_read;
    if ((fp_read = my_fopen(argv[1], "r")) == NULL)
    {
        return 1;
    }
    FILE *fp_write;
    if ((fp_write = my_fopen("./test.txt", "w")) == NULL)
    {
        return 1;
    }
    else
    {
        int c;
        while ((c = getc(fp_read)) != EOF)
        {
            putc(c, fp_write);
        }
    }
    fclose(fp_read);
    fclose(fp_write);
    return 0;
}

/* _fillbuf: allocate and fill input buffer */
int _fillbuf(FILE *fp)
{
    int buf_size;

    if ((fp->flag & (_READ | _EOF | _ERR)) != _READ)
    {
        return EOF;
    }		
    buf_size = (fp->flag & _UNBUF) ? 1 : BUFSIZ;
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
            fp->flag |= _EOF;
        }
        else
        {
            fp->flag |= _ERR;
        }
        fp->cnt = 0;
        return EOF;
    }
    return (unsigned char) *fp->ptr++;
}

/* _flushbuf: flush the contents of the buffer to the file */
int _flushbuf(int x, FILE *fp)
{
    /* Determine whether the file is writable
    When the buffer has been allocated, the contents of the buffer are written into the file through the system call write
    When the buffer has not been allocated it is allocated
    When judging that the written character is EOF, it means that the purpose of calling this function is to forcibly refresh the buffer
        without writing characters. Assign cnt to BUFSIZE and ptr to the first address of the buffer base
    When the written character is not EOF, the buffer is full and the buffer needs to be flushed to the file.
        cnt is BUFSIZE - 1, put the written character x in the first grid of the buffer, and then move ptr back one char unit.
    When the calling the write function, the number of bytes written cannot be written to 1 or BUFSIZE
     */
    if (fp->flag & (_WRITE | _EOF | _ERR) != _WRITE)
    {
        return EOF;
    }
    ssize_t number_of_bytes_written;
    if (fp->base != NULL)
    {
        number_of_bytes_written = write(fp->fd, fp->base, fp->ptr - fp->base);
        if (number_of_bytes_written != fp->ptr - fp->base)
        {
            fp->flag |= _ERR;
            return EOF;
        }
    }
    else if ((fp->base = malloc((sizeof(char)) * BUFSIZ)) == NULL)
    {
        fp->flag |= _ERR;
        return EOF;
    }
    if (x == EOF)
    {
        fp->cnt = BUFSIZ;
        fp->ptr = fp->base;
    }
    else
    {
        fp->cnt = BUFSIZ - 1;
        fp->ptr = fp->base;
        *fp->ptr++ = x;
    }
    return x;
}

/* fclose: Closes the stream. All buffers are flushed.
This method returns zero if the stream is successfully closed. On failure, EOF is returned. */
int fclose(FILE *fp)
{
    free(fp->base);
    if ((fp->flag & (_WRITE | _ERR | _EOF)) == _WRITE) /* Writable? */
    {
        if (fflush(fp) == EOF)
        {
            fp->flag |= _EOF;
            return EOF;
        }
    }
    if (close(fp->fd) == -1)
    {
        fp->flag |= _ERR;
        return EOF;
    }
    return 0;
}

/* fflush: Flushes the output buffer of a stream.*/
int fflush(FILE *fp)
{
    /* If NULL flush all files */
    if (fp == NULL)
    {
        for (unsigned int index = 0; index < OPEN_MAX; ++index)
        {
            if (fp->flag & _WRITE)
            {
                if (fflush(&(_iob[index])) == -1)
                {
                    return EOF;
                }
            }
        }
    }
    else if(fp->flag & _WRITE)/* NULL wasn't passed, so check if it's writable, if it is flush it */
    {

        _flushbuf(EOF, fp);
        if (fp->flag & _ERR) /* did _flushbuf set the error flag? */
        {
            return EOF;
        }
    }
    else
    {
        return EOF;
    }
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
        if (!(fp->flag & (_READ | _WRITE)))
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
        fp->flag |= _READ;
    }
    else
    {
        fp->flag |= _WRITE;
    }
    return fp;
}