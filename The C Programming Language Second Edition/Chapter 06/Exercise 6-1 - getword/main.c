/* Our version of getword does not properly handle underscores,
string constants, comments or preprocessor control lines. Write a better version. */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_WORD 100

struct key
{
    char *word;
    int count;
} keytab[] = { /* Mustbe sorted in increasing order for the binary search */
    "auto", 0,
    "break", 0,
    "case", 0,
    "char", 0,
    "const", 0,
    "continue", 0,
    "default", 0,
    /* ... */
    "unsigned", 0,
    "void", 0,
    "volatile", 0,
    "while", 0,
};
#define NKEYS (sizeof keytab / sizeof keytab[0])

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
    enum token { UNPROCESSED, CODE, BEGIN_COMMENT, IN_COMMENT, END_COMMENT };

    int c, getch(void);
    void ungetch(int);
    char *w = word;
    enum token state = UNPROCESSED;

    while (isspace(c = getch()))
    {
        ;
    }
    if (c != EOF)
    {
        *w++ = c;
    }
    if (!isalpha(c))
    {
        *w = '\0';
        return c;
    }
    for (; --limit > 0; w++)
    {        
        /* Handle comments */
        while (state != CODE)
        {
            switch (state)
            {
                case UNPROCESSED:
                    if (*w == '/' && (*w = getch()) == '*')
                    {
                        state = IN_COMMENT;
                    }
                    break;
                case IN_COMMENT:
                    while (state == IN_COMMENT)
                    {
                        if (*w == '*' && (*w = getch()) == '\\')
                        {
                            state = UNPROCESSED;
                        }
                    }
                    break;
                default:
                    printf("Error: unhandled case in get_word\n");
                    break;
            }
        }
        if (!isalnum(*w = getch()) || *w != '_') /* handle underscores */
        {
            ungetch(*w);
            break;
        }
        
        
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