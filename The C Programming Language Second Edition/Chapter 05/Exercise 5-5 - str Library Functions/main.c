/* Write versions of the library functions strncpy, strncat, and
strncmp, which operate on at most the first n characters of their argument
strings. For example, strncpy(s, t, n) copies at most n characters of t to s.
Full descriptions are in Appendix B. */

#include <stdio.h>

size_t strlen(const char *s);
char *strncpy(char *s, const char *t, size_t n);

int main()
{
    char name[] = "Jamie";
    char copy_to[10];
    strncpy(copy_to, name, 10);

    return 0;
}

/* computes the length of the string str up to, but not including the terminating null character. */
size_t strlen(const char *s)
{
    size_t length = 0;
    while(*s)
    {
        s++;
        length++;
    }
    return length;
}

/* Copy at most n characters of string t to s; return s.
Pad with '\0's if t has fewer than n characters. */
char *strncpy(char *s, const char *t, size_t n)
{
    size_t t_length = strlen(t);
    size_t i;
    /* Copy up to n-chars or up to t_length if n-chars is greater than t_length*/
    for (i = 0; i < n && i < t_length; i++)
    {
        *(s + i) = *(t + i);
    }
    *(s + i) = '\0'; /* null terminate here, if we do it after the next loop we'd go out of bounds */
    i++;
    /* If chars copied is less than number of chars to copy, pad the rest with '\0's */
    for (; i < n; i++)
    {
        *(s + i) = '\0';
    }    

    return s;
}