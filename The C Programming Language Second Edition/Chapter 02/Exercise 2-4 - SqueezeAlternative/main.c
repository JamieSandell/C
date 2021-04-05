/* Write an alternative version of squeeze(s1, s2) that deletes
each character in s1 that matches any character in the string s2.

Version referenced:
delete all c from s
void squeeze(char s[], int c)
{
    int i, j;

    for (i = j = 0; s[i] != '\0'; i++)
        if (s[i] != c)
            s[j++] = s[i];
    s[j] = '\0';
}*/

#include <stdio.h>

enum bool {FALSE, TRUE};

void squeeze(char s1[], char s2[]);


int main()
{
    char s1[] = "Jamie xyz!";
    char s2[] = "!yzx ";

    printf("String before squeezing \"%s\" from it: \"%s\"\n", s2, s1);
    squeeze(s1, s2);
    printf("String after squeezing \"%s\" from it: \"%s\"\n", s2, s1);

    return 0;
}

/* Delete all occurences of the characters in s2 from s1. */
void squeeze(char s1[], char s2[])
{
    int i, j, k;
    enum bool match = FALSE;

    for (i = j = 0; s1[i] != '\0'; i++)
    {
        for (k = 0; s2[k] != '\0'; k++)
        {
            if(s1[i] == s2[k]) /* Compare the ith character of s1 against every character of s2 */
            {
                match = TRUE;
                break;
            }
        }
        if (!match)
        {
            s1[j++] = s1[i]; /* No match with the characters in s2, so nothing to delete/omit from s1,
            so copy the character at position i to j in s1 then increment j. */
        }
        match = FALSE; /* Reset the flag ready for the next loop iteration. */
    }
    s1[j] = '\0';
}