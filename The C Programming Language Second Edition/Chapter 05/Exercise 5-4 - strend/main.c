/* Write the function strend(s,t), which returns 1 if the string
t occurs at the end of the string s, and zero otherwise. */

#include <stdio.h>

int strend(char *s, char *t);

int main()
{
    char string1[] = "Bob Jamie";
    char string2[] = "Jamie";

    int result = strend(string1, string2);
    (result == 1) ? printf("%s found at the end of %s\n", string2, string1) : printf("%s not found at the end of %s\n", string2, string1);

    return 0;
}

int strend(char *s, char *t)
{
    int s_char_count = 0;
    while(*s) /* count all the characters in the string, not including the null terminator */
    {
        s_char_count++;
        s++;
    }
    s--; /* Point s to the last character in the string, not the null terminator */

    int t_char_count = 0;
    while(*t)
    {
        t_char_count++;
        t++;
    }
    t--;

    if (t_char_count > s_char_count)
    {
        /* If there are more characters in t than s, then t can't be at the end of s. */
        return 0;
    }

    int s_offset, t_offset;
    /* Loop backwards through the strings, if the character is equal keep going, count up the offsets each time */
    for(s_offset = 0, t_offset = 0; *(s - s_offset) == *(t - t_offset); s_offset++, t_offset++)
        ;
    return ((t_char_count - t_offset) == 0) ? 1 : 0; /* if the number of characters in t - t_offset == 0
    (t_offset was incremented each time a matching character was found going backwards in both strings), then we have a match */
}