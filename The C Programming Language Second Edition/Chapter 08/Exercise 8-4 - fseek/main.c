/* The standard library function
    int fseek(FILE *fp, long offset, int origin)
is identical to lseek except that fp is a file pointer instead of a file descriptor
and the return value is an int status, not a position. Write fseek. Make sure
that your fseek coordinates properly with the buffering done for the other
functions of the library. */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "syscall.h"

#define PERMS 0666 /* RW for owner, group, others */

int _fillbuf(FILE *fp);
int _flushbuf(int, FILE *fp);
int fclose(FILE *fp);
int fflush(FILE *fp);
int fseek(FILE *fp, long offset, int origin);
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
    FILE *fp_write;
    if ((fp_write = my_fopen("./test.txt", "w")) == NULL)
    {
        return 1;
    }
    else
    {
        char name[] = "Jamie";
        char *cp = name;
        while (*cp != '\0')
        {
            putc(*cp++, fp_write);
        }
        char insert_text[] = "My name is ";
        cp = insert_text;        
        if (fseek(fp_write, 0, SEEK_SET) != 0)
        {
            fclose(fp_write);
            return -1;
        }
        while (*cp != '\0')
        {
            putc(*cp++, fp_write);
        }        
    }
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
    if (fp->flag & (_WRITE | _EOF | _ERR) != _WRITE)
    {
        return EOF;
    }
    if (fp->base == NULL && ((fp->flag & _UNBUF) == 0)) /* buffered flag is set, but no buffer yet */
    {
        if ((fp->base = malloc(BUFSIZ)) == NULL)
        {
            /* failed to allocate a buffer, so use unbuffered */
            fp->flag |= _UNBUF;
        }
        else
        {
            fp->ptr = fp->base;
            fp->cnt = BUFSIZ - 1;
        }
    }


    ssize_t number_of_bytes_written;
    int bufsize;
    unsigned char ux = x;
    if (fp->flag & _UNBUF == 0) /* buffered write */
    {
        if (x != EOF)
        {
            *fp->ptr++ = ux;
        }
        bufsize = (int)(fp->ptr - fp->base);
        number_of_bytes_written = write(fp->fd, fp->base, bufsize);
        fp->ptr = fp->base;
        fp->cnt = BUFSIZ - 1;
    }
    else if (fp->flag & _UNBUF) /* unbuffered write */
    {
        fp->ptr = fp->base = NULL;
        fp->cnt = 0;
        if (x == EOF)
        {
            return EOF;
        }
        number_of_bytes_written = write(fp->fd, &ux, 1);
        bufsize = 1;
    }

    if (number_of_bytes_written == bufsize)
    {
        return x;
    }
    else
    {
        fp->flag |= _ERR;
        return EOF;
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
            if (_iob[index].flag & _WRITE)
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

/* Sets the file position of fp to the given offset.
fp - This is the pointer to a FILE object that identifies the stream.
offset - This is the number of bytes to offset from origin.
origin - This is the position from where offset is added. It is specified by one of the following constants:
    SEEK_SET
    beginning of the file
    SEEK_CUR
    Current position of the file pointer
    SEEK_END
    End of file
This function returns zero if successful, or else it returns a non-zero value. */
int fseek(FILE *fp, long offset, int origin)
{
    if (origin != SEEK_CUR && origin != SEEK_END && origin != SEEK_SET)
    {
        return -1;
    }
    if (fp == NULL)
    {
        return -1;
    }

    if ((!(fp->flag & _UNBUF)) && fp->base != NULL) /* unbuffered input */
    {
        if (fp->flag & _WRITE) /* writeable */
        {
            if (fflush(fp) != 0)
            {
                return EOF;
            }            
        }
        else if (fp->flag & _READ) /* readable */
        {
            if (origin == SEEK_CUR)
            {
                if (offset >= 0 && offset <= fp->cnt)
                {
                    fp->cnt -= offset; /* sets the offset to be from the last character the user read, not the actual last character read */
                    fp->ptr += offset;
                    fp->flag &= ~_EOF;
                    return 0;
                }
                else
                {
                    offset -= fp->cnt;
                }
            }
            fp->cnt = 0;
            fp->ptr = fp->base;
        }
    }
    int result = (lseek(fp->fd, offset, origin) < 0);
    if (result == 0)
    {
        fp->flag &= ~_EOF; /* successful, so clear the EOF flag */
    }
    return result;
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