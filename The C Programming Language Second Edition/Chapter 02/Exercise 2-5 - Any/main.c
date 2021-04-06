/* Write the function any(s1,s2), which returns the first location
in the string s1 where any character from the string s2 occurs, or -1 if s1
contains no characters from s2. (The standard library function strpbrk does
the same job but returns a pointer to the location.) */

#include <stdio.h>

int any(char s1[], char s2[]);

int main()
{
    char s1[] = {"Are you Jamie?"};
    char s2[] = {"Yes my name is Jamie"};
    printf("First index of a matching character from \"%s\" in \"%s\" is %i.\n", s1, s2, any(s1, s2));
    return 0;
}

int any(char s1[], char s2[])
{
    for (int i = 0; s2[i] != '\0'; i++)
    {
        for (int j = 0; s1[j] != '\0'; j++)
        {
            if (s1[j] == s2[i])
            {
                return j;
            }
        }
    }
    return -1;
}
