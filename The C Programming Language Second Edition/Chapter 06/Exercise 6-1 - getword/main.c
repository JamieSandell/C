/* Our version of getword does not properly handle underscores, string constants, comments or preprocessor control lines.
Write a better version */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

struct key
{
    char *word;
    int count;
} keytab[] ={
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
    "while", 0
};

#define NKEYS (sizeof keytab / sizeof keytab[0])
char *keyword[NKEYS];
int key_count[NKEYS];

#define MAX_WORD 100
int get_word(char *word, int limit);
int bin_search(char *word, struct key *tab, int n);

int getch(void);
void ungetch(int);
#define BUF_SIZE 100
static char buf[BUF_SIZE]; /* Buffer for ungetch */
static int bufp; /* Next free position in buf */

/* count C keywords */
int main()
{
    int n;
    char word[MAX_WORD];

    while (get_word(word, MAX_WORD) != EOF)
    {
        if (isalpha(word[0]))
        {
            if ((n = bin_search(word, keytab, NKEYS)) >= 0)
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

/* bin_search: find word in tab[0]...tab[n-1] */
int bin_search(char *word, struct key tab[], int n)
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
    int c;
    char *w = word;

    while(isspace(c = getch()))
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
        if (!isalnum(*w = getch()))
        {
            ungetch(*w);
            break;
        }
    }
    *w = '\0';
    return word[0];
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