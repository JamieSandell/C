/* Write a program that prints the distinct words in its input sorted into decreasing order of frequency of occurrence. Precede each word by its count. */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD 100
#define MAX_WORDS 100

struct word_node
{
    char *word;
    unsigned int count;
    struct word_node *left;
    struct word_node *right;
};
struct word_node *add_word_node(struct word_node *p, char *w);
void free_word_nodes(struct word_node *p);
void print_word_nodes(const struct word_node *p);
struct word_node *walloc(void);

struct data_frequency
{
    char *data;
    unsigned int frequency;
};

#define BUF_SIZE 100
static char buf[BUF_SIZE]; /* buffer for ungetch */
static int bufp; /* next free position in buf */
int getch(void);
void ungetch(int c);

int compare(const void *a, const void *b);
int get_word(char *word, int limit);
char *my_strdup(const char *w);
void sort_by_frequency(char *arr[], int n);
void store(struct word_node *p, struct data_frequency count[], unsigned int *index);

int main()
{
    
    char word[MAX_WORD];
    char *words[MAX_WORDS];
    unsigned int word_count = 0;

    /* get the words */
    while (get_word(word, MAX_WORD) != EOF)
    {
        if (isalpha(word[0]))
        {
            words[word_count++] = my_strdup(word);
        }
    }
    /* sort the words and store them in our tree, print them and then free the memory */
    sort_by_frequency(words, word_count);
    struct word_node *root = NULL;
    for (unsigned int index = 0; index < word_count; ++index)
    {
        root = add_word_node(root, words[index]);
    }
    print_word_nodes(root);
    free_word_nodes(root);
    return 0;
}

/* add_word_node: add a word node */
struct word_node *add_word_node(struct word_node *p, char *w)
{
    int condition;

    if (p == NULL) /* new word node has arrived */
    {
        p = walloc(); /* make a new word node */
        p->word = my_strdup(w); /* make a duplicate of w */
        p->count = 1;
        p->left = p->right = NULL;
    }
    else if (condition = (strcmp(p->word, w)) == 0) /* matching word */
    {
        p->count++;
    }
    else if (condition < 0) /* less than into subtree */
    {
        p->left = add_word_node(p->left, w);
    }
    else /* greater than into subtree */
    {
        p->right = add_word_node(p->right, w);
    }

    return p;
}

/* free_word_nodes: memory cleanup of p and below nodes */
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

/* print_word_nodes: print p and below nodes in decreasing order of frequency of occurrence */
void print_word_nodes(const struct word_node *p)
{
    if (p != NULL)
    {
        print_word_nodes(p->left);
        printf("%u:\t%s\n", p->count, p->word);
        print_word_nodes(p->right);
    }
}

/* walloc: make a word_node */
struct word_node *walloc(void)
{
    return (struct word_node *) malloc(sizeof(struct word_node));
}

/* compare: function for qsort */
int compare(const void *a, const void *b)
{
    return ((const struct data_frequency*)b)->frequency - ((const struct data_frequency*)a)->frequency;
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

/* sort_by_frequency: sorts the input array according to frequency (root should be NULL) */
void sort_by_frequency(char *arr[], int n)
{
    /* Create a BST and insert all array items */
    struct word_node *root = NULL;
    for (unsigned int index = 0; index < n; ++index)
    {
        root = add_word_node(root, arr[index]);
    }

    /* store the word and frequencies from root (and below) into count[] */
    struct data_frequency count[n];
    unsigned int index = 0;
    store(root, count, &index);

    /* sort count[] by frequency */
    qsort(count, index, sizeof(count[0]), compare);

    /* traverse the sorted count[] array and copy the i'th 'freq' times to the original arr[] */
    unsigned int j = 0;
    for (unsigned int i = 0; i < index; i++)
    {
        for (unsigned int frequency = count[i].frequency; frequency > 0; frequency--)
        {
            strcpy(arr[j++], count[i].data);
        }
    }
    /* cleanup memory */
    free_word_nodes(root);
}

/* store: copy elements and their frequency to count[] */
void store(struct word_node *p, struct data_frequency count[], unsigned int *index)
{
    if (p == NULL)
    {
        return ;
    }

    /* left subtree */
    store(p->left, count, index);

    /* store item from root and increment index */
    count[(*index)].frequency = p->count;
    count[(*index)].data = p->word;
    (*index)++;

    /* right subtree */
    store(p->right, count, index);
}

/* get_word: get next word or character from input */
int get_word(char *word, int limit)
{
    int c;char *w = word;

    while (isspace(c = getch())) /* ignore leading white space */
    {
        ;
    }
    if (c != EOF) /* not end of input so put the last character from the input in to where w points to in word */
    {
        *w++ = c;
    }
    if (!isalpha(c)) /* words start with a letter */
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