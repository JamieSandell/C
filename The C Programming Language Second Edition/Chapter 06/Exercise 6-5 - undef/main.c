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

#define HASH_SIZE 5
static struct nlist *hashtab[HASH_SIZE]; /* pointer table */

unsigned hash(const char *s);
struct nlist *install(const char *name, const char *defn);
struct nlist *lookup(const char *s);
struct nlist *uninstall(char *name);

int main()
{
    char *name = "1776";
    char *surname = "1861";
    install(name, "1776");
    install(surname, "1861");
    if (lookup(name))
    {
        printf("%s exists\n", name);
    }
    printf("removing %s\n", name);
    if (uninstall(name))
    {
        if (!lookup(name))
        {
            printf("%s removed\n", name);
        }
    }
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

    /* e.g.
    we have our hash table size be 5, with our hashing algorithm, hash(name) where name equals "1776" or "1861" will return the hashval 3.

    so when we call install and set name = "1776" and define with "Revolutionary" (define isn't the important variable here)
    then it will call lookup to see if 1776 (which in the hash resolved to 3 when the table size is 5), exists.
    If there is a non-null value at lookup(name), lookup will strcmp to make sure the name matches 1776 if it doesn't it will return NULL.
    If the lookup is NULL we create a new nlist and set the name to be 1776
    then we get it's position to be entered in the hashtable by running the hash, again in this example the hash will be 3
    we then set the next pointer to point to wherever hashtab[hashval] points to which in this example will be 0x0 (NULL)
    then we set hashtab[hashval] to point to the new nlist we just created, finally we set defn and then return the pointer.

    then when we call install and set name = "1861" and define with "Civil"
    it will call lookup to see if 1861 (which in the hash resolves to 3 when the table size is 5), exists.
    So lookup will actually check hash_tab[3] and see a value exists, however on the strcmp it won't match (1776 vs 1861) so it will return NULL.
    Then we create a new nlist and get the hashval (3), then
    np->next = hashtab[hashval] or put another way np->next = hashtab[3]
    so np->next will point to the nlist with name 1776 we created previously.
    Then we set hashtab[hashval] = np or put another way hashtab[3] = address of our new nlist for 1861.
    */
    if ((np = lookup(name)) == NULL) /* not found */
    {
        np = malloc(sizeof(struct nlist *));
        if (np == NULL || (np->name = strdup(name)) == NULL)
        {
            return NULL;
        }
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
        
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

/* uninstall: remove (name, and it's defn) from hashtab */
struct nlist *uninstall(char *name)
{
    struct nlist *np;
    unsigned hashval;

    if ((np = lookup(name)) == NULL) /* not found */
    {
        return NULL;
    }
    free(np->defn);
    free(np->name);
    free(np->next);
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