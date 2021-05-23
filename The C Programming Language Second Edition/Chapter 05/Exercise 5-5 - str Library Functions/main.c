/* Write versions of the library functions strncpy, strncat, and
strncmp, which operate on at most the first n characters of their argument
strings. For example, strncpy(s, t, n) copies at most n characters of t to s.
Full descriptions are in Appendix B. */

#include <stdio.h>

size_t strlen(const char *s);
char *strncat(char *s, const char *t, size_t n);
int strncmp(const char *s, const char *t, size_t n);
char *strncpy(char *s, const char *t, size_t n);

int main()
{
    char name[] = "Jamie";
    char copy_to[10] = "Bob";
    printf("Copying name[](%s) to copy_to[](%s)\n", name, copy_to);
    strncpy(copy_to, name, 10);
    printf("Copied...copy_to[] == %s\n\n", copy_to);

    char fullName[100] = "Jamie";
    char surname[] = "Sandell";
    printf("Concatenate %s and %s: ", fullName, surname);
    strncat(fullName, surname, 100);
    printf("%s\n", fullName);

    char jammy[] = "Jammy";
    int chars_to_compare = 3;
    printf("Comparing the first %i characters of %s with %s, 0 for match, negative or positive for less than/greater than: %i\n",
        chars_to_compare, name, jammy, strncmp(name, jammy, chars_to_compare));

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

/* Concatenate at most n characters of string t to s, terminate s with '\0'; return s */
char *strncat(char *s, const char *t, size_t n)
{
    size_t t_length = strlen(t);
    while(*s != '\0')
    {
        s++;
    }

    int i;
    for (i = 0; i < n && i < t_length; i++)
    {
        *(s + i) = *(t + i);
    }
    *(s + i) = '\0';    

    return s;    
}

/* Compare at most n characters of string s to string t;
return <0 if s<t, 0 if s==t, or >0 if s>t. */
int strncmp(const char *s, const char *t, size_t n)
{
    int s_length = strlen(s);
    int i;
    for(i = 0; i < n; i++)
    {
        if (*(s + i) != *(t + i))
        {
            return (*(s + i) < *(t + i)) ? -1 : 1;
        }
    }
    return 0;
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