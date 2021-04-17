/* Our binary search makes two tests inside the loop, when one would suffice
(at the price of more tests outside). Write a version with only one test
inside the loop and measure the difference in run-time.

int binsearch(int x, int v[], int n)
{
    int low, high, mid;

    low = 0;
    high = n - 1;
    while (low <= high)
    {
        mid = (low + high) / 2;
        if (x < v[mid])
        {
            high = mid - 1;
        }
        else if (x > v[mid])
        {
            low = mid + 1;
        }
        else
        {
            return mid;
        }
        else
        {
            return -1;
        }
    }
}
 */

#include <stdio.h>

int binsearch(int x, int v[], int n);

int main()
{
    int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int to_find = 5;
    int number_of_elements = 10;

    printf("Searching for %i in numbers[]\n", to_find);
    int result = binsearch(to_find, numbers, number_of_elements);
    (result == -1) ? printf("%i not found\n", to_find) : printf("%i found at index %i\n", to_find, result);
    printf("%i", result);

    return 0;
}

int binsearch(int x, int v[], int n)
{
    int low, high, mid;

    low = 0;
    high = n - 1;
    while (low < high)
    {
        mid = (low + high) / 2;
        if (x <= v[mid])
        {
            high = mid;
        }
        else
        {
            low = mid + 1;
        }
    }
    return (x == v[low]) ? low : -1;
}