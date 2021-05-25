/* There is no error checking in day_of_year or month_day.
Remedy this defect. */

#include <stdio.h>

static char day_table[2][13] = 
{
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

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
        day += day_table[leap][i];
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
    for (i = 1; year_day > day_table[leap][i]; i++)
    {
        year_day -= day_table[leap][i];
    }
    *pmonth = i;
    *pday = year_day;
    return 0;
}