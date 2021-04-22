/* Exercise 3-2. Write a function escape(s,t) that converts characters like
newline and tab into visible escape sequences like \n and \t as it copies the
string t to s. Use a switch. Write a function for the other direction as well,
converting escape sequences into the real characters. */

#include <stdio.h>

void escape(char s[], char t[]);
void escape_reverse(char s[], char t[]);

int main()
{
    return 0;
}

/* Expects null terminated strings to be passed in. */
void escape(char s[], char t[])
{
    for (int i = 0; t[i] != '\0'; i++)
    {
        switch (t[i])
        {
            case '\n':
                break;
            case '\t':
                break;
        }
    }
}
void escape_reverse(char s[], char t[])
{

}