/* Expand dcl to handle declarations with function argument types, qualifiers like const, and so on. */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_TOKEN 100

enum {NAME, PARENS, BRACKETS, QUALIFIER, TYPE};
enum {ERROR, SUCCESS};

int dcl(void);
int dirdcl(void);

int is_qualifier(const char *s);
int is_type(const char *s);

int get_token(void);
int get_params(void);
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
        out[0] = '\0'; /* reset */
        name[0] = '\0';
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
        else if (token_type != ')')
        {
            printf("Error: missing )\n");
            return ERROR;
        }
    }
    else if (token_type == NAME && name[0] == '\0') /* Variable name (and hasn't been set prior) */
    {
        strcpy(name, token);
    }
    else if (token_type == ')')
    {
        return token_type;
    }
    else if (token_type != NAME)
    {
        printf("Error: expected name or (dcl)\n");
        return ERROR;
    }
    while ((type = get_token()) == PARENS || type == BRACKETS || type == '(')
    {
        if (type == PARENS)
        {
            strcat(out, " function returning");
        }
        else if (type == '(')
        {
            strcat(out, " function expecting");
            if (get_params() == ERROR)
            {
                printf("Error when calling get_params.\n");
                return ERROR;
            }
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

int is_qualifier(const char *s)
{
    static char qualifier_const[] = "const";
    static char qualifier_volatile[] = "volatile";

    /* If this list was to grow, better off using an array of pointers */
    if ((strcmp(s, qualifier_const) == 0) || (strcmp(s, qualifier_volatile) == 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_type(const char *s)
{
    static char int_const[] = "int";
    static char void_const[] = "void";
    static char char_const[] = "char";

    if ((strcmp(s, int_const) == 0) || (strcmp(s, void_const) == 0) || (strcmp(s, char_const) == 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* Skips blanks and tabs, then finds the next token in the input;
a "token" is a name, a pair of parentheses, a pair of brackets perhaps including a number, or any other single
character.
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
        if (is_qualifier(p))
        {
            return token_type = QUALIFIER;
        }
        else if (is_type(p))
        {
            return token_type = TYPE;
        }
        return token_type = NAME;
    }
    else
    {
        return token_type = c;
    }
}

int get_params(void)
{
    char temp[MAX_TOKEN] = {0};

    get_token();
    while (token_type != ')')
    {
        if (is_qualifier(token))
        {
            strcat(temp, " ");
            strcat(temp, token);
            /* Now get the identifier name and the variable name*/
            if ((dcl() == ERROR))
            {
                return ERROR;
            }
            strcat(temp, " ");
            strcat(temp, name);
            strcat(temp, " ");
            strcat(temp, token);
        }
        else if (is_type(token))
        {
            /* Now get the identifier name and the variable name*/
            strcat(temp, " ");
            strcat(temp, name);
            strcat(temp, " ");
            strcat(temp, token);
        }
        else if (token_type == ',')
        {
            strcat(temp, ",");
        }
        else if((dcl() == ERROR))
        {
            return ERROR;
        }
    }
    strcat(out, temp);
    return SUCCESS;
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