/* Rewrite the postfix calculator of Chapter 4 to use scanf and/or sscanf to do the input and number conversion */

#include <stdio.h>

#define MAXOP 100 /* max size of operand or operator */
#define NUMBER '0' /* signal that a number was found */

int getop(char []);
void push (double);
double pop(void);

/* reverse Polish calculator */
int main()
{
    char *c;
    char s[MAXOP], buf[MAXOP];
    double a = 0, op2;
    char e = '\0';

    while(scanf("%s%c", s, &e) == 2) /* store the string (number or operand) in s and the char (space) in e */
    {
        break;
    }

    return 0;
}