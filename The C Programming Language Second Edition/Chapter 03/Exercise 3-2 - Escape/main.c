/* Exercise 3-2. Write a function escape(s,t) that converts characters like
newline and tab into visible escape sequences like \n and \t as it copies the
string t to s. Use a switch. Write a function for the other direction as well,
converting escape sequences into the real characters. */

#include <stdio.h>

void escape(char s[], char t[]);
void escape_reverse(char s[], char t[]);

int main()
{
    char source[] = {"Jamie\n\tSandell"};
    char destination[100];

    printf("%s\n", source);
    escape(destination, source);
    printf("%s\n", destination);

    putchar('\n');
    printf("%s\n", destination);
    escape_reverse(source, destination);
    printf("%s\n", source);

    return 0;
}

/* Expects null terminated strings to be passed in.
Haven't done the full escape character set for brevity, but the principal is the same for each one. */
void escape(char s[], char t[])
{
    int i;
    int j;
    for (i = 0, j = 0; t[i] != '\0'; ++i, ++j)
    {
        switch (t[i])
        {
            case '\n':
                s[j++] = '\\';
                s[j] = 'n';
                break;
            case '\t':
                s[j++] = '\\';
                s[j] = 't';
                break;
            default:
                s[j] = t[i];
                break;
        }
    }
    s[j] = '\0';
}

/* Expects null terminated strings to be passed in.
Haven't done the full escape character set for brevity, but the principal is the same for each one. */
void escape_reverse(char s[], char t[])
{
    int i;
    int j;
    for (i = 0, j = 0; t[i] != '\0'; ++i, ++j)
    {
        switch (t[i])
        {
            case '\\':
                switch (t[++i])
                {
                    case 'n':
                        s[j] = '\n';
                        break;
                    case 't':
                        s[j] = '\t';
                        break;
                    default:
                        s[j] = t[--i];
                        break;
                }
                break;
            default:
                s[j] = t[i];
                break;
        }
    }
    s[j] = '\0';
}