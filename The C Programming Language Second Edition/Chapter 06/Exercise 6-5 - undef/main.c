/* Write a function undef that will remove a name and definition from the table maintained by lookup and install. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct nlist /* table entry: */
{
    struct nlist *next; /* next entry in chain */
    char *name; /* defined name */
    char *defn; /* replacement text */
};

#define HASH_SIZE 101
static struct nlist *hashtab[HASH_SIZE]; /* pointer table */

unsigned hash(const char *s);
struct nlist *install(const char *name, const char *defn);
struct nlist *lookup(const char *s);

int main()
{
    char *name = "Jamie";
    char *surname = "Sandell";
    install(name, "Trudi");
    install(surname, "Russell");
    return 0;
}

/* hash: form hash value from string s */
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
        np = malloc(sizeof(struct nlist *));
        if (np == NULL || (np->name = strdup(name)) == NULL)
        {
            return NULL;
        }
        hashval = hash(name);
        np->next = hashtab[hashval]; /* next points to whatever hashtab[hashval] is pointing to */
        hashtab[hashval] = np; /* hashtab[hashval] now holds a pointer to the np we just installed */
    }
    else /* already there */
    {
        free(np->defn);
    }
    if ((np->defn = strdup(defn)) == NULL)
    {
        return NULL;
    }
    return np;
}

/* lookup: look for s in hashtab */
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