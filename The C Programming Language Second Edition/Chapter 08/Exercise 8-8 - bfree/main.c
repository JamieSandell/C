/* Write a routine bfree(p,n) that will free an arbitrary block p of n characters into the free list maintained by malloc and free.
By using bfree, a user can add a static or external array to the free list at any time. */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_ALLOC 1024 * 5
#define NALLOC 1024 /* minimum # of units to request */

typedef long Align; /* for alignment to long boundary */

union header /* block header: */
{
    struct
    {
        union header *ptr; /* next block if on free list */
        unsigned int size; /* size of this block */
    } s;
    Align x; /* force alignment of blocks */
};

typedef union header Header;

static Header base; /* empty list to get started */
static Header *freep = NULL; /* start of free list */

void bfree(char *p, unsigned int n);
void *my_calloc(size_t number, size_t size);
void my_free(void *ap);
void *my_malloc(unsigned int nbytes);
static Header *my_morecore(unsigned int);

int main()
{
    size_t number = 10;
    size_t size = sizeof(char);
    char *memory = my_calloc(number, size);
    strcpy(memory, "Jamie");
    printf("%s\n", memory);
    my_free(memory);
    memory = my_calloc(number, size);
    my_free(memory);

    number = MAX_ALLOC + 1;
    if ((memory = my_calloc(number, size)) == NULL)
    {
        fprintf(stderr, "failed to allocate memory\n");
    }
    else
    {
        my_free(memory);
    }

    number = 20;
    memory = my_calloc(number, size);
    strcpy(memory, "just another line of text");
    bfree(memory + 13, sizeof(Header)); /* delete "line of text" from the string in memory */
    printf("%s\n", memory);
    return 0;
}

/* bfree: Frees the block only if the size is at least sizeof(Header) */
void bfree(char *p, unsigned int n)
{
    Header *hp;
    size_t sizeof_header = sizeof(Header);
    if (n < sizeof_header)
    {
        return;
    }
    hp = (Header *)p;
    hp->s.size = n / sizeof_header; /* size of the block in units of sizeof(Header), may discard some bytes at the end */
    my_free((void *)(hp + 1)); /* my_free expects hp to be 1 past the header area */
}

/* my_calloc: returns a pointer to n objects of size size, with the storage initialised to zero */
void *my_calloc(size_t number, size_t size)
{
    size_t length = number * size;
    Header *p = my_malloc(length);
    if (p == NULL)
    {
        return NULL;
    }
    memset(p, 0x00, length);
    return p;
}

/* my_free: put block ap in free list */
void my_free(void *ap)
{
    Header *bp, *p;

    bp = (Header *)ap - 1; /* point to block header */
    if (bp->s.size == 0 || bp->s.size > MAX_ALLOC)
    {
        return;
    }
    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    {
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
        {
            break; /* freed block at start or end of arena */
        }
    }

    if (bp + bp->s.size == p->s.ptr) /* join to upper nbr */
    {
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    }
    else
    {
        bp->s.ptr = p->s.ptr;
    }
    if (p + p->s.size == bp) /* join to lower nbr */
    {
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    }
    else
    {
        p->s.ptr = bp;
    }
    freep = p;
}

/* my_malloc: general-purpose memory allocator */
void *my_malloc(unsigned int nbytes)
{
    Header *p, *prevp;
    unsigned int nunits;

    if (nbytes > MAX_ALLOC)
    {
        return NULL;
    }

    nunits = (nbytes + sizeof(Header) - 1 / sizeof(Header) + 1);
    if ((prevp = freep) == NULL)
    {
        /* no free list yet */
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }

    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr)
    {
        if (p->s.size >= nunits) /* big enough */
        {
            if (p->s.size == nunits) /* exact fit */
            {
                prevp->s.ptr = p->s.ptr;
            }
            else /* allocate tail end */
            {
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp;
            return (void *)(p + 1);
        }
        if (p == freep) /* wrapped around free list */
        {
            if ((p = my_morecore(nunits)) == NULL)
            {
                return NULL; /* none left */
            }
        }        
    }
}

/* my_morecore: ask system for more memory */
static Header *my_morecore(unsigned int nu)
{
    char *cp;
    Header *up;

    if (nu < NALLOC)
    {
        nu = NALLOC;
    }
    cp = sbrk(nu * sizeof(Header));
    if (cp == (char *) - 1) /* no space at all */
    {
        return NULL;
    }
    up = (Header *) cp;
    up->s.size = nu;
    my_free((void *)(up + 1));
    return freep;
}