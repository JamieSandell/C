//Print a histogram of different character frequencies entered as input

#include <stdio.h>
#define MAX_CHARS 128 //128 different characters available on a keyboard in ASCII

int main()
{
    int c = 0;
    int characterFrequency[MAX_CHARS];

    for (int i = 0; i < MAX_CHARS; ++i)
    {
        characterFrequency[i] = 0;
    }

    while((c = getchar()) != EOF)
    {
        characterFrequency[c]++;
    }

    for (int i = 0; i < MAX_CHARS; ++i)
    {
        printf(" %c: ", i);

        for (int j = 0; j < characterFrequency[i]; ++j)
        {
            putchar('*');
        }

        putchar('\n');
    }

    return 0;
}