/* Rewrite the routines day_of_year and month_day with
pointers instead of indexing. */

#include <stdio.h>

static char row_0[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static char row_1[] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static char *day_table[] = {row_0, row_1};

int day_of_year(int year, int day, int month);
int month_day(int year, int year_day, int *pmonth, int *pday);

int main()
{
    return 0;
}

/* day_of_year: set day of year from month & day
returns -1 if an error, otherwise returns the day */
int day_of_year(int year, int day, int month)
{
    if (day < 1 || day > 31 || month < 1 || month > 12)
    {
        return -1;
    }
    int i, leap;

    leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
    for(i = 1; i < month; i++)
    {
        day += *(*(day_table + leap) + i); /* 2D array is conceptual, in memory each element of *day_table[] is a 1D array. 
        *(day_table + leap) returns the address of the "leap array" and then points to that address + i
        then that is dereferenced to get the actual value. */
    }
    return day;
}

/* set month, day from day of year 
returns -1 if an error, otherwise returns 0 and sets pmonth and pday accordingly */
int month_day(int year, int year_day, int *pmonth, int *pday)
{
    if (year_day < 1 || year_day > 365)
    {
        return -1;
    }
    int i, leap;

    leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
    for (i = 1; year_day > *(*(day_table + leap) + i); i++)
    {
        year_day -= *(*(day_table + leap) + i);
    }
    *pmonth = i;
    *pday = year_day;
    return 0;
}