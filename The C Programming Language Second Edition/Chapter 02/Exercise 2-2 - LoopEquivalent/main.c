/* for (i=0; i<lim-1 && (c=getchar()) != '\n' && c != EOF; ++i)
    s[i] = c;
    
Write a loop equivalent to the for loop above without using && or ||. */

#include <stdio.h>

#define LIMIT 100

int main()
{    
    int i = 0;    
    int c;
    int lim = LIMIT;
    int s[LIMIT];

    while(i < lim - 1)
    {
        if ((c = getchar()) != '\n') { lim = 0; }
        else if (c == EOF) { lim = 0; }
        else { s[i] = c; }

        i++;
    }

    return 0;
}