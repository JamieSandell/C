/* Write a pointer version of the function strcat that we showed
in Chapter 2: strcat(s,t) copies the string t to the end of s 

void strcat(char s[], char t[])
{
    int i, j;

    i = j = 0;
    while (s[i] != '\0')
        i++;
    while ((s[i++] = t[j++]) != '\0')
        ;
}

*/

#include <stdio.h>

void my_strcat(char *s, char *t);

int main()
{
    char surname[] = "Sandell";
    char firstname[50] = "Jamie";
    char space[] = " ";

    printf("firstname: %s, surname: %s\n", firstname, surname);
    printf("my_strcat(%s, ' '): ", firstname);
    my_strcat(firstname, space);
    printf("%s\n", firstname);
    printf("my_strcat(%s, %s): ", firstname, surname);
    my_strcat(firstname, surname);
    printf("%s\n", firstname);

    return 0;
}

void my_strcat(char *s, char *t)
{
    while(*s++)
        ;
    /* Equivalent of the above, be aware that the post increment operator is executed after the condition is checked
    while(*s != 0)
    {
        s++;
    }
    s++; */
    s--; /* move back to the null terminated character */
    while(*s++ = *t++)
        ;
    /* Equivalent of the above
    while(*t != 0)
    {
        *s = *t;
        s++;
        t++;
    }
    *s = *t */
}