#include <stdio.h>
/*Print Farenheit-Celcius table
    for fahr = 0, 20, ..., 300 */

#define LOWER   0 //Lower limit of the table
#define UPPER   300 //Upper limit of the table
#define STEP    20 //Step size

float FarenheitToCelcius(int fahr);

int main()
{
    int fahr;

    for (fahr = LOWER; fahr <= UPPER; fahr = fahr + STEP) {
        printf("%3d %6.1f\n", FarenheitToCelcius(fahr));
    }

    return 0;
}

float FarenheitToCelcius(int fahr)
{
    return (5.0 / 9.0) * (fahr - 32);
}