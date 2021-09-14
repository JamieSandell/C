/* Modify undcl so that it does not add redundant parentheses to declarations.

Redundant parentheses only appear to be generated when multiple * are used in succession. */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_TOKEN 100

enum {NAME, PARENS, BRACKETS};
enum {ERROR, SUCCESS};

int dcl(void);
int dirdcl(void);

int get_token(void);
void next_line(void);
static int token_type; /* Type of the last token */
static char token[MAX_TOKEN]; /* Last token string */
static char name[MAX_TOKEN]; /* Identifier name */
static char data_type[MAX_TOKEN]; /* Data type = char, int, etc. */
#define MAX_OUT 1000
#define MAX_POINTERS 100
static char out[MAX_OUT]; /* Output string */

int getch(void);
void ungetch(int);
#define BUF_SIZE 100
static char buf[BUF_SIZE]; /* Buffer for ungetch */
static int bufp; /* Next free position in buf */

int main(void)
{
    int type, c;
    char temp[MAX_TOKEN];
    char pointers[MAX_POINTERS];
    unsigned int pointer_count = 0;

    while (get_token() != EOF)
    {
        strcpy(out, token);
        while ((type = get_token()) != '\n')
        {
            if (type == PARENS || type == BRACKETS)
            {
                strcat(out, token);
            }
            else if (type == '*')
            {
                pointers[pointer_count++] = '*';
                while ((c = get_token()) == '*')
                {
                    if (++pointer_count > MAX_POINTERS)
                    {
                        printf("Warning: buffer overflow prevented on pointers[%u]\n", pointer_count);
                        return -1;
                    }
                    pointers[pointer_count - 1] = '*';
                }
                pointers[pointer_count] = '\0';
                pointer_count = 0;
                ungetch(c);
                sprintf(temp, "(%s%s)", pointers, out);
                strcpy(out, temp);
            }
            else if (type == NAME)
            {
                sprintf(temp, "%s %s", token, out);
                strcpy(out, temp);
            }
            else
            {
                printf("invalid input at %s\n", token);
            }
        }
        printf("%s\n", out);
    }
    return 0;
}

/* Parse a declarator */
int dcl(void)
{
    int ns;

    for (ns = 0; get_token() == '*';) /* Count *'s */
    {
        ++ns;
    }
    if (dirdcl() == ERROR)
    {
        return ERROR;
    }
    while (ns-- > 0)
    {
        strcat(out, " pointer to");
    }
    return SUCCESS;
}

/* Parse a direct declarator */
int dirdcl(void)
{
    int type;

    if (token_type == '(') /* ( dcl ) */
    {
        if (dcl() == ERROR)
        {
            return ERROR;
        }
        if (token_type != ')')
        {
            printf("Error: missing )\n");
            return ERROR;
        }
    }
    else if (token_type == NAME) /* Variable name */
    {
        strcpy(name, token);
    }
    else
    {
        printf("Error: expected name or (dcl)\n");
        return ERROR;
    }
    while ((type = get_token()) == PARENS || type == BRACKETS)
    {
        if (type == PARENS)
        {
            strcat(out, " function returning");
        }
        else
        {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        }
    }
    return SUCCESS;
}

/* Skips blanks and tabs, then finds the next token in the input;
a "token" is a name, a pair of parentheses, a pair of brackets perhaps including a number, or any other single character.
Returns the next token */
int get_token(void)
{
    int c;    
    char *p = token;

    while ((c = getch()) == ' ' || c == '\t')
    {
        ;
    }
    if (c == '(')
    {
        if ((c = getch()) == ')')
        {
            strcpy(token, "()");
            return token_type = PARENS;
        }
        else
        {
            ungetch(c);
            return token_type = '(';
        }
    }
    else if (c == '[')
    {
        for (*p++ = c; (*p++ = getch()) != ']';)
        {
            ;
        }
        *p = '\0';
        return token_type = BRACKETS;
    }
    else if (isalpha(c))
    {
        for (*p++ = c; isalnum(c = getch());)
        {
            *p++ = c;
        }
        *p = '\0';
        ungetch(c);
        return token_type = NAME;
    }
    else
    {
        return token_type = c;
    }
}

void next_line(void)
{
    int c;
    while ((c = getch()) != '\n' && c != EOF)
    {
        ;
    }
    if (c == EOF)
    {
        ungetch(c);
    }
}

int getch(void) /* Get a (possibly pushed back) character */
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /* Push character back on input */
{
    if (bufp >= BUF_SIZE)
    {
        printf("ungetch: too many characters\n");
    }
    else
    {
        buf[bufp++] = c;
    }
}