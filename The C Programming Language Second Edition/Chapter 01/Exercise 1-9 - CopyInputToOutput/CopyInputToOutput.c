#include <stdio.h>

int main()
{
    
    printf("EOF signal is %i\n", EOF);

    printf("Test whether comparison against EOF == 1 or 0: ");
    printf("%d\n", getchar() != EOF); // to test whether the comparison is true/false 1/0

    int c;
    while ((c = getchar()) != EOF) { //while not EOF print out the input
        putchar(c);
    }



    return 0;
}