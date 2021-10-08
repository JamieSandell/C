/* Implement a simple version of the #define processor (i.e., no arguments) suitable for use with C programs,
based on the routines of this section. You may also find getch and ungetch useful. */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 100
#define HASH_SIZE 101
#define MAX_WORD 100
#define WORD_SIZE 100

struct nlist /* table entry: */
{
    struct nlist *next; /* next entry in chain */
    char *name; /* defined name */
    char *defn; /* replacement text */
};

static char buf[BUF_SIZE]; /* buffer for ungetch */
static int bufp = 0; /* next free position in buf */
static struct nlist *hashtab[HASH_SIZE]; /* pointer table */

int getch(void);
int get_number(void);
int get_word(char *word, unsigned limit);
unsigned hash(const char *s);
struct nlist *install(const char *name, const char *defn);
struct nlist *lookup(const char *s);
void ungetch(int c);

int main()
{
    enum state {unprocessed, define};
    enum state state = unprocessed;

    char word[WORD_SIZE];
    int c;
    while ((c = getch()) != '#' || c != EOF)
    {
        switch (state)
        {
            case unprocessed:
                c = get_word(word, MAX_WORD);
                if (word == "define")
                {
                    state = define;
                }
                break;
            case define:

                break;
            default:
                printf("Error: unhandled switch statement in main()\n");
                return -1;
        }
    }
    return 0;
}

/* getch: get a (possibly pushed back) character */
int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

/* get_number: get net number from input */
int get_number(void)
{
    
}

/* get_word: get next word or character from input */
int get_word(char *word, unsigned limit)
{
    int c;
    char *w = word;

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
    for (; --limit > 0; ++w)
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

/* hash: form hash value for string s */
unsigned hash(const char *s)
{
    unsigned hashval;

    for (hashval = 0; *s != '\0'; ++s)
    {
        hashval = *s + 31 * hashval;
    }
    
    return hashval % HASH_SIZE;
}

/* install: put (name, defn) in hashtab */
struct nlist *install(const char *name, const char *defn)
{
    struct nlist *np;
    unsigned hashval;

    if ((np = lookup(name)) == NULL) /* not found */
    {
        np = malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
        {
            return NULL;
        }
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    }
    else
    {
        free(np->defn); /* free previous defn */
    }
    if ((np->defn = strdup(defn)) == NULL)
    {
        return NULL;
    }

    return np;
}

struct nlist *lookup(const char *s)
{
    struct nlist *np;

    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
    {
        if (strcmp(s, np->name) == 0)
        {
            return np; /* found */
        }
    }

    return NULL; /* not found */
}

/* ungetch: push character back on input */
void ungetch(int c)
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