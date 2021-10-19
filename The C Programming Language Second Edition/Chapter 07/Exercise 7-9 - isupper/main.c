/* Functions like isupper can be implemented to save space or to save time. Explore both possibilities. */

/* Function version of isupper saves space but has the overhead of being a function call. */
unsigned int is_upper_function(char c)
{
    return (c >= 'A' && c <= 'Z') ? 1 : 0;
}

/* Macro version of isupper takes up more space as it's a text replacement, but has less overhead as it's not a function call. */
#define is_upper_macro(c) (c >= 'A' && c <= 'Z') ? 1 : 0

int main()
{
    return 0;
}