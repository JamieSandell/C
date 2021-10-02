/* Write a cross-referencer that prints a list of all words in a document,
and, for each word, a list of the line numbers on which it occurs. Remove
noise words like "the," "and," and so on */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD 100

#define MAX_NUMBERS 100
struct word_node
{
    char *word;    
    unsigned int line_numbers[MAX_NUMBERS];
    unsigned int count; /* number of elements in line_numbers */
    struct word_node *left;
    struct word_node *right;
};
struct word_node *add_word_node(struct word_node *p, char *w);
void free_word_nodes(struct word_node *p);
void print_word_nodes(const struct word_node *p);
struct word_node *walloc(void);

#define BUF_SIZE 100
static char buf[BUF_SIZE]; /* buffer for ungetch */
static int bufp; /* next free position in buf */
int getch(void);
void ungetch(int c);

int get_word(char *word, int limit);
char *my_strdup(const char *w);

static unsigned int line_number = 0; /* the current line number in the document/input */

int main()
{
    struct word_node *root = NULL;
    char word[MAX_WORD];
    while (get_word(word, MAX_WORD) != EOF)
    {
        if (isalpha(word[0]))
        {
            root = add_word_node(root, word);
        }
    }
    print_word_nodes(root);
    free_word_nodes(root);
    return 0;
}

struct word_node *add_word_node(struct word_node *p, char *w)
{
    int condition;

    if (p == NULL) /* new word node has arrived */
    {
        p = walloc(); /* make a new word node */
        p->word = my_strdup(w); /* make a duplicate of w */
        p->line_numbers[0] = line_number;
        p->count = 1;
        p->left = p->right = NULL;
    }
    else if(condition = (strcmp(p->word, w)) == 0) /* matching word_node */
    {
        p->line_numbers[p->count++] = line_number;
    }
    else if(condition < 0) /* less than into subtree */
    {
        p->left = add_word_node(p->left, w);
    }
    else /* greater than into the subtree */
    {
        p->right = add_word_node(p->right, w);
    }

    return p;
}

/* free_word_nodes: memory cleanup of the node and nodes below it */
void free_word_nodes(struct word_node *p)
{
    if (p == NULL) /* nothing to do */
    {
        return;
    }
    /* work our way down the tree */
    free_word_nodes(p->left);
    free_word_nodes(p->right);
    free(p->word);
    free(p);
}

/* print_word_nodes: print the node and the nodes below it */
void print_word_nodes(const struct word_node *p)
{
    if (p != NULL)
    {
        print_word_nodes(p->left);
        printf("%s appears on these line numbers:\t", p->word);
        for (unsigned int index = 0; index < p->count; ++index)
        {
            printf("%u", p->line_numbers[index]);
            if (index < p->count)
            {
                printf(",");
            }
            else
            {
                printf("\n");
            }
        }
        print_word_nodes(p->right);
    }
}

/* walloc: make a word_node */
struct word_node *walloc(void)
{
    return (struct word_node *) malloc(sizeof(struct word_node));
}

/* my_strdup: create a duplicate of s */
char *my_strdup(const char *s)
{
    char *p;

    p = (char *) malloc(strlen(s) + 1); /* +1 for '\0' */
    if (p != NULL)
    {
        strcpy(p, s);
    }

    return p;
}

/* get_word: get next word or character from input if it's not a string constant and not a comment */
int get_word(char *word, int limit)
{
    int c;
    char *w = word;

    while (isspace(c = getch())) /* ignore leading white space */
    {
        if (c == '\n')
        {
            ++line_number;
        }
    }
    if (c != EOF) /* not end of input so put the last character from the input in to where w points to in word) */
    {
        *w++ = c;
    }
    if (!isalpha(c)) /* words start with a letter */
    {
        if (c == '\n')
        {
            ++line_number;
        }
        *w = '\0';
        return c;
    }
    for (; --limit > 0; ++w)
    {
        if (!isalnum(*w = getch()))
        {
            if (*w == '\n')
            {
                ++line_number;
            }
            ungetch(*w);
            break;
        }
    }
    *w = '\0';
    return word[0];
}

/* getch: get a (possibly pushed back) character */
int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

/* ungetch: push back character on input */
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