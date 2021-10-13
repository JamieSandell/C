/* Write a private version of scanf analogous to minprintf from the previous section. */

#include <stdarg.h>
#include <stdio.h>

void minscanf(char *fmt, ...);

int main()
{
    char input[100];
    minscanf("%s", input);
    printf("%s\n", input);
    return 0;
}

/* minscanf: minimal scanf with argument list */
void minscanf(char *fmt, ...)
{
    va_list ap; /* points to each unnamed arg in turn */
    char *p, *sval;
    int *ival;
    double *dval;

    va_start(ap, fmt); /* make ap point to the 1st unnamed arg */
    for (p = fmt; *p; ++p) /* walk fmt */
    {
        if (*p != '%')
        {
            continue;
        }
        switch (*++p)
        {
            case 'd':
                ival = va_arg(ap, int *);
                scanf("%d", ival);
                break;
            case 'f':
                dval = va_arg(ap, double *);
                scanf("%f", dval);
                break;
            case 's':
                sval = va_arg(ap, char *);
                scanf("%s", sval);
                break;
            default:
                break;
        }
    }
}