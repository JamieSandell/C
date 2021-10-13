/* Revise minprintf to handle more of the other facilities of printf. */

#include <stdarg.h>
#include <stdio.h>

void minprintf(char *fmt, ...);

int main()
{
    return 0;
}

void minprintf(char *fmt, ...)
{
    va_list ap; /* points to each unnamed arg in turn */
    char *p, *sval;
    char cval;
    int ival;
    double dval;
    unsigned int uival;

    va_start(ap, fmt); /* make ap point to 1st unnamed arg */
    for (p = fmt; *p; ++p) /* walk the fmt string */
    {
        if (*p != '%')
        {
            putchar(*p);
            continue;
        }
        switch (*++p) /* move to the address of the next character in fmt and then dereference it */
        {
            case 'a':
                dval = va_arg(ap, double);
                printf("%a", dval);
                break;
            case 'A':
                dval = va_arg(ap, double);
                printf("%A", dval);
                break;
            case 'c':
                cval = va_arg(ap, char);
                printf("%c", cval);
                break;
            case 'd': /* fall through on purpose */
            case 'i':
                ival = va_arg(ap, int);
                printf("%d", ival);
                break;
            case 'f':
                dval = va_arg(ap, double);
                printf("%f", dval);
                break;
            case 'F':
                dval = va_arg(ap, double);
                printf("%F", dval);
                break;
            case 'e':
                dval = va_arg(ap, double);
                printf("%e", dval);
                break;
            case 'E':
                dval = va_arg(ap, double);
                printf("%E", dval);
                break;
            case 'g':
                dval = va_arg(ap, double);
                printf("%g", dval);
                break;
            case 'G':
                dval = va_arg(ap, double);
                printf("%G", dval);
                break;
            case 'o':
                uival = va_arg(ap, unsigned int);
                printf("%o", uival);
                break;
            case 'p':
                
            case 's':
                for (sval = va_arg(ap, char *); *sval; ++sval)
                {
                    putchar (*sval);
                }
                break;
            case 'u':
                uival = va_arg(ap, unsigned int);
                printf("%u", uival);
                break;
            case 'x':
                uival = va_arg(ap, unsigned int);
                printf("%x", uival);
                break;
            case 'X':
                uival = va_arg(ap, unsigned int);
                printf("%X", uival);
                break;
            default:
                putchar(*p);
                break;
        }
    }
    va_end(ap); /* clean up when done */
}