/* Write a cross-referencer that prints a list of all words in a document,
and, for each word, a list of the line numbers on which it occurs. Remove
noise words like "the," "and," and so on */

#include <stdio.h>
#include <string.h>

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
void print_word_nodes(struct word_node *p);
struct word_node *walloc(void);

char *my_strdup(const char *w);

static unsigned int line_number = 0; /* the current line number in the document/input */

int main()
{
    struct word_node *root = NULL;

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
