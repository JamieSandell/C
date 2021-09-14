/* Make dcl recover from input errors.

I've taken this to mean it won't attempt to correct any input errors, but instead will raise the error for that line and then continue to process the next line */

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
static char out[1000]; /* Output string */

int getch(void);
void ungetch(int);
#define BUF_SIZE 100
static char buf[BUF_SIZE]; /* Buffer for ungetch */
static int bufp; /* Next free position in buf */

int main(int argc, char **argv)
{
    while (get_token() != EOF) /* 1st token on line */
    {
        strcpy(data_type, token); /* is the data type */
        out[0] = '\0';
        if (dcl() == ERROR)
        {
            next_line();
        }
        else if (token_type != '\n')
        {
            printf("Syntax error\n");
            next_line();
        }
        else
        {
            printf("%s: %s %s\n", name, out, data_type);
        }        
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