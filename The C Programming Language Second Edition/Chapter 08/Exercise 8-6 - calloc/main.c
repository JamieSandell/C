/* The standard library function calloc(n,size) returns a pointer to n objects of size size,
with the storage initialised to zero. Write calloc by calling malloc or by modifying it. */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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
    return 0;
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