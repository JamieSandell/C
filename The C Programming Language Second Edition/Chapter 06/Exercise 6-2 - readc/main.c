/* Write a program that reads a C program and prints in alphabetical
order each group of variable names that are identical in the first 6
characters, but different somewhere thereafter. Don't count words within
strings and comments. Make 6 a parameter that can be set from the command line. 

For each word mark what state it was and the previous state
a word can then be classed as a variable if the previous word was a data type
and the current word is not:
a data type
inside a comment
within a string constant

or if a word has previously been added to the root group_node or below.

TODO: get it working with enums
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD 100
#define DEFAULT_LETTERS_TO_MATCH 6
static unsigned int letters_to_match;

enum token { UNPROCESSED, KEYWORD, COMMENT, STRING_CONSTANT, DATA_TYPE };
static enum token state = UNPROCESSED;
static enum token previous_state = UNPROCESSED;

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
void free_group(struct group_node *p);
void print_group(struct group_node *p);
struct group_node *galloc(void);

int get_word(char *word, int limit);
int is_c_keyword(const char *s);
int is_data_type(const char *s);
int is_variable(const char *s, const struct group_node *p);
int is_variable_in_group(const char *s, const struct group_node *p);
char *my_strdup(char *s);

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        printf("Error: too many arguments\n");
        return -1;
    }
    int arg = atoi(*argv);
    if (arg < 0)
    {
        printf("Error: argument must be a positive number\n");
        return -1;
    }
    letters_to_match = (arg > 0) ? arg : DEFAULT_LETTERS_TO_MATCH;

    struct group_node *root = NULL;
    char word[MAX_WORD];
    while (get_word(word, MAX_WORD) != EOF)
    {
        if (isalpha(word[0]) && is_variable(word, root))
        {
            root = add_group(root, word);
        }
    }
    print_group(root);
    free_group(root);
    return 0;
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
    else if((cond = strncmp(w, *(p->variable_name_pointer), letters_to_match)) == 0) /* we have a match */
    {
        *(p->variable_name_pointer + p->count++) = my_strdup(w);
    }
    else if(cond < 0) /* less than into left subtree */
    {
        p->left = add_group(p->left, w);
    }
    else /* greater than into right subtree */
    {
        p->right = add_group(p->right, w);
    }

    return p;
}

void free_group(struct group_node *p)
{
    if (p == NULL)
    {
        return;
    }
    free_group(p->left);
    free_group(p->right);
    for (unsigned int variable = 0; variable < p->count; ++variable)
    {
        free(p->variable_name_pointer[variable]);
    }
    free(p);
}

void print_group(struct group_node *p)
{
    if (p != NULL)
    {
        print_group(p->left);
        /*printf("%4d %s\n", p->count, p->word);*/
        printf("%4d\n", p->count);
        for (unsigned int variable = 0; variable < p->count; ++variable)
        {
            printf("%s\n", p->variable_name_pointer[variable]);
        }
        print_group(p->right);
    }
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

/* get_word: get next word or character from input if it's not a string constant and not a comment */
int get_word(char *word, int limit)
{
    previous_state = state;
    state = UNPROCESSED;
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
        *w = '\0';
        if (is_data_type(word))
        {
            state = DATA_TYPE;
        }
        return word[0];
    }
    *w = '\0';
    return c;
}

/* is_c_keyword: compares s against c keywords and returns 1 if true, 0 if false */
int is_c_keyword(const char *s)
{
    if (is_data_type(s))
    {
        return 1;
    }
    static char *c_keywords[] = { /* left out data types as we've already checked for thos */
        "auto",
        "break",
        "case",
        "const",
        "continue",
        "default",
        "do",
        "else",
        "enum",
        "extern",
        "for",
        "goto",
        "if",
        "register",
        "return",
        "signed",
        "sizeof",
        "static",
        "struct",
        "switch",
        "typedef",
        "union",
        "void",
        "volatile",
        "while"
    };
    static unsigned int c_keywords_count = sizeof(c_keywords) / sizeof(c_keywords[0]);

    for (unsigned int c_keywords_index = 0; c_keywords_index < c_keywords_count; ++c_keywords_index)
    {
        if (strcmp(s, c_keywords[c_keywords_index]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

/* is_data_type: compares s against c data types and returns 1 if true, 0 if false */
int is_data_type(const char *s)
{
    static char *data_types[] = {
        "char",
        "double",
        "float",
        "int",
        "long",
        "short",
        "unsigned"
    };
    static unsigned int data_types_count = sizeof(data_types) / sizeof(data_types[0]);

    for (unsigned int data_types_index = 0; data_types_index < data_types_count; ++data_types_index)
    {
        if (strcmp(s, data_types[data_types_index]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

/* is_variable: If s is not a c keyword and the previous_state was a data_type it returns 1
If a variable matches variables in group_node p or below it returns 1
Returns 1 if s is a variable, 0 if false */
int is_variable(const char *s, const struct group_node *p)
{
    if (previous_state == DATA_TYPE && !is_c_keyword(s))
    {
        return 1;
    }
    else if (is_variable_in_group(s, p))
    {
        return 1;
    }
    return 0;
}

/* is_variable_in_group: If s matches a variable in group_node p or below return 1, else return 0 */
int is_variable_in_group(const char *s, const struct group_node *p)
{
    if (p != NULL)
    {
        is_variable_in_group(s, p->left);
        for (unsigned int variable = 0; variable < p->count; ++variable)
        {            
            if (strcmp(s, p->variable_name_pointer[variable]))
            {
                return 1;
            }
        }
        is_variable_in_group(s, p->right);
    }
    return 0;
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