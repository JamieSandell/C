/* Write a program that reads a C program and prints in alphabetical
order each group of variable names that are identical in the first 6
characters, but different somewhere thereafter. Don't count words within
strings and comments. Make 6 a parameter that can be set from the command line. */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD 100
#define DEFAULT_LETTERS_TO_MATCH 6
static unsigned int letters_to_match;

enum token { UNPROCESSED, KEYWORD, COMMENT, STRING_CONSTANT };
static enum token state = UNPROCESSED;

struct tnode /* the tree node: */
{
    char *word; /* points to the text */
    int count; /* number of occurrences */
    struct tnode *left; /* left child */
    struct tnode *right; /*right child */
};
struct tnode *add_tree(struct tnode *p, char *w);
void free_tree(struct tnode *p);
void print_tree(struct tnode *p);
struct tnode *talloc(void);

#define MAX_VARIABLES 100
struct group_node /* the group node: */
{
    char *variable_name_pointer[MAX_VARIABLES]; /* pointer to array of variables names that are identical in the first x characters,
    but different somewhere thereafter */
    int count; /* number of variables */
    struct group_node *left; /* left child */
    struct group_node *right; /* right child */
};
struct group_node *add_group(struct group_node *p, char *w);
void free_group(struct tnode *p);
void print_group(struct tnode *p);
struct group_node *galloc(void);

int get_word(char *word, int limit);
char *my_strdup(char *s);

int main(int argc, char *argv[])
{
    struct tnode *root = NULL;
    char word[MAX_WORD];

    while (get_word(word, MAX_WORD) != EOF)
    {
        if (isalpha(word[0]))
        {
            root = add_tree(root, word);
        }
    }
    print_tree(root);
    free_tree(root);
    return 0;
}

/* add_tree: add a node with w, at or below p */
struct tnode *add_tree(struct tnode *p, char *w)
{
    int cond;

    if (p == NULL) /* a new word has arrived */
    {
        p = talloc(); /* make a new node */
        p->word = strdup(w);
        p->count = 1;
        p->left = p->right = NULL;
    }
    else if ((cond = strcmp(w, p->word)) == 0)
    {
        p->count++; /* repeated word */
    }
    else if (cond < 0) /* less than into left subtree */
    {
        p->left = add_tree(p->left, w);
    }
    else /* greater than into right subtree */
    {
        p->right = add_tree(p->right, w);
    }

    return p;
}

/* free_tree: frees the memory allocations of the tree */
void free_tree(struct tnode *p)
{
    if (p == NULL)
    {
        return;
    }
    free_tree(p->left);
    free_tree(p->right);
    free(p->word);
    free(p);
}

/* print_tree: in-order print of tree p */
void print_tree(struct tnode *p)
{
    if (p != NULL)
    {
        print_tree(p->left);
        printf("%4d %s\n", p->count, p->word);
        print_tree(p->right);
    }
}

/* talloc: make a tnode */
struct tnode *talloc(void)
{
    return (struct tnode *) malloc(sizeof(struct tnode));
}

/* group_node: add a node with w, at or below p */
struct group_node *add_group(struct group_node *p, char *w)
{
    int cond;

    if (p == NULL) /* new group has arrived */
    {
        p = galloc(); /* make a new group */
        *(p->variable_name_pointer) = my_strdup(w);
        p->count = 1;
        p->left = p->right = NULL;
    }
    else if((cond = strncmp(w, *(p->variable_name_pointer), letters_to_match)) == 0)
    {
        p->variable_name_pointer /* TODO: add the variable to the existing collection */
        p->count++; /* we have a match */
    }
    else if(cond < 0) /* less than into left subtree */
    {
        p->left = add_group(p, w);
    }
    else /* greater than into right subtree */
    {
        p->right = add_group(p, w);
    }

    return p;
}

/* galloc: make a group_node */
struct group_node *galloc(void)
{
    return (struct group_node *) malloc(sizeof(struct group_node));
}

char *my_strdup(char *s) /* make a duplicate of s */
{
    char *p;

    p = (char *) malloc(strlen(s) + 1); /* +1 for '\0' */
    if (p != NULL)
    {
        strcpy(p, s);
    }
    return p;
}

/* get_word: get next word or character from input */
int get_word(char *word, int limit)
{
    int c = 0;
    int getch(void);
    void ungetch(int);
    char *w = word;    

    if (state == UNPROCESSED) /* if unprocessed ignore leading white space and return if it's not alpha and it isn't potentially the start of a comment,
    string constant or preprocessor control */
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
    
    while (state != KEYWORD && c != '\n' && c != EOF) /* Process the line until we've determined we potentially have a keyword, we've not hit a newline or EOF */
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
                        else if (c != EOF)
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
                        state = UNPROCESSED;
                    }
                    else if (c != EOF)
                    {
                        ungetch(c);
                    }               
                }
                break;
            case STRING_CONSTANT:
                if ((c = getch()) == '"')
                {
                    state = UNPROCESSED;
                }
                break;
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
            if (!isalnum(*w) && *w != '_' && *w != '#')
            {
                ungetch(*w);
                break;
            }   
        }        
        state = UNPROCESSED; /* in case the function is called again we need to be in a fresh state */
        *w = '\0';
        return word[0];
    }
    *w = '\0';
    return c;
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