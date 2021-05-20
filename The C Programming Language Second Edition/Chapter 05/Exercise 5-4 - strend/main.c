/* Write the function strend(s,t), which returns 1 if the string
t occurs at the end of the string s, and zero otherwise. */

#include <stdio.h>

int strend(char *s, char *t);

int main()
{
    char string1[] = "Jamie";
    char string2[] = "Jamie";

    int result = strend(string1, string2);
    (result == 1) ? printf("%s found at the end of %s\n", string2, string1) : printf("%s not found at the end of %s\n", string2, string1);

    return 0;
}

int strend(char *s, char *t)
{
    int s_char_count = 0;
    while(*s)
    {
        s_char_count++;
        s++;
    }

    int t_char_count = 0;
    while(*t)
    {
        t_char_count++;
        t++;
    }

    if (t_char_count > s_char_count)
    {
        return 0;
    }

    int s_offset, t_offset;
    for(s_offset = 0, t_offset = 0; *(s - s_offset) == *(t - t_offset); s_offset++, t_offset++)
    {
        printf("\n");
    }
    return ((t_char_count - t_offset) == 0) ? 1 : 0;
}