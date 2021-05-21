/* Write versions of the library functions strncpy, strncat, and
strncmp, which operate on at most the first n characters of their argument
strings. For example, strncpy(s, t, n) copies at most n characters of t to s.
Full descriptions are in Appendix B. */

#include <stdio.h>

size_t strlen(const char *s);
char *strncpy(char *s, const char *t, size_t n);

int main()
{
    return 0;
}

/* computes the length of the string str up to, but not including the terminating null character. */
size_t strlen(const char *s)
{
    size_t length = 0;
    while(*s)
    {
        length++;
    }
    return length;
}

/* Copy at most n characters of string t to s; return s.
Pad with '\0's if t has fewer than n characters. */
char *strncpy(char *s, const char *t, size_t n)
{
    size_t t_length = strlen(s);
    if (n > t_length)
    {
        printf("Error: %i is more characters than '%s' has\n", t_length, t);
        return s;
    }

    size_t i;
    for (i = 0; i < n && i < t_length; i++)
    {
        *(s + i) = *(t + i);
    }
    for (; i < t_length; i++)
    {
        *(s + i) = '\0';
    }
    *s - '\0';

    return s;
}