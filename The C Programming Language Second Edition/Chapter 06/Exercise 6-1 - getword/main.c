/* Our version of getword does not properly handle underscores,
string constants, comments or preprocessor control lines. Write a better version.

It's not crystal clear what is wanted in this exercise.
I'm taking it to mean that a word should not be returned if it's within a string constant or a comment (single or multi-line)

Otherwise if it's alphanumeric (within a preproccesor control line or not) or contains an underscore it should return that word. */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_WORD 100

struct key
{
    char *word;
    int count;
} keytab[] = { /* Must be sorted in increasing order for the binary search */
    "auto", 0,
    "break", 0,
    "case", 0,
    "char", 0,
    "const", 0,
    "continue", 0,
    "default", 0,
    "do", 0,
    "double", 0,
    "else", 0,
    "enum", 0,
    "extern", 0,
    "float", 0,
    "for", 0,
    "goto", 0,
    "if", 0,
    "int", 0,
    "long", 0,
    "register", 0,
    "return", 0,
    "short", 0,
    "signed", 0,
    "sizeof", 0,
    "static", 0,
    "struct", 0,
    "switch", 0,
    "typedef", 0,
    "union", 0,
    "unsigned", 0,
    "void", 0,
    "volatile", 0,
    "while", 0,
};
#define NKEYS (sizeof keytab / sizeof keytab[0])

enum token { UNPROCESSED, KEYWORD, COMMENT, STRING_CONSTANT };
static enum token state = UNPROCESSED;

int get_word(char *word, int limit);
int binsearch(char *word, struct key *tab, int n);

/* count C keywords */
int main()
{
    int n;
    char word[MAX_WORD];

    while (get_word(word, MAX_WORD) != EOF)
    {
        if (isalpha(word[0]))
        {
            if ((n = binsearch(word, keytab, NKEYS)) >= 0)
            {
                keytab[n].count++;
            }
        }
    }
    for (n = 0; n < NKEYS; ++n)
    {
        if (keytab[n].count > 0)
        {
            printf("%4d %s\n", keytab[n].count, keytab[n].word);
        }
    }
    return 0;
}

/* binsearch: find word in tab[0]...tab[n-1] */
int binsearch(char *word, struct key tab[], int n)
{
    int cond;
    int low, high, mid;

    low = 0;
    high = n - 1;
    while (low <= high)
    {
        mid = (low + high) / 2;
        if ((cond = strcmp(word, tab[mid].word)) < 0)
        {
            high = mid - 1;
        }
        else if (cond > 0)
        {
            low = mid + 1;
        }
        else
        {
            return mid;
        }
    }
    return -1;
}

/* get_word: get next word or character from input */
int get_word(char *word, int limit)
{
    int c = 0;
    int getch(void);
    void ungetch(int);
    char *w = word;    

    if (state == UNPROCESSED)
    {
        while (isspace(c = getch()))
        {
            ;
        }
        if (!isalpha(c) && c != '/' && c != '#' && c != '"')
        {
            *w = '\0';
            return c;
        }
    }
    
    while (state != KEYWORD && c != '\n' && c != EOF)
    {
        switch (state)
        {
            case UNPROCESSED:
                switch (c)
                {
                    case '/':
                        if ((c = getch()) == '*')
                        {
                            state = COMMENT;
                        }
                        else
                        {
                            ungetch(c);
                        }   
                        break;
                    case '"':
                        state = STRING_CONSTANT;
                        break;
                    default:
                        state = KEYWORD;
                }
                break;
            case COMMENT:
                if ((c = getch()) == '*')
                {
                    if ((c = getch()) == '/') /* end of the comment */
                    {
                        c = getch();
                        if (isalnum(c) || c == '_' || c == '#')
                        {
                            state = KEYWORD;
                        }
                        else
                        {
                            ungetch(c);
                            state = UNPROCESSED;
                        }
                    }
                    else
                    {
                        ungetch(c);
                    }               
                }
                break;
            case STRING_CONSTANT:
                if ((c = getch()) == '"')
                {
                    if (isalnum(c) || c == '_' || c == '#')
                    {
                        state = KEYWORD;
                    }
                    else
                    {
                        ungetch(c);
                        state = UNPROCESSED;
                    }
                }
            default:
                state = KEYWORD;
                break;
        }     
    }
    
    if (state == KEYWORD)
    {
        *w++ = c;
        --limit;
        for (; --limit > 0; w++)
        {
            *w = getch();
            if (!isalnum(*w) && c != '_' && c != '#')
            {
                ungetch(*w);
                break;
            }   
        }
        state = UNPROCESSED;
    }
    
    *w = '\0';
    return word[0];
}

#define BUF_SIZE 100
static char buf[BUF_SIZE]; /* Buffer for ungetch */
static int bufp; /* Next free position in buf */

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