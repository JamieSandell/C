/* Write a function expand(s1,s2) that expands shorthand notations
like a-z into the equivalent complete list abc...xyz in s2.
Allow for letters of either case and digits, and be prepared to handle cases
like a-b-c and a-z0-9 and -a-z. Arrange that a leading or trailing - is taken literally.*/

#include <stdio.h>

/* Ranges are the decimal equivalent in ASCII */
const int digit_range_start = 48; /* 0 */
const int digit_range_end = 57; /* 9 */
const int lower_alph_range_start = 97; /* a */
const int lower_alph_range_end = 122; /* z */
const int upper_alph_range_start = 65; /* A */
const int upper_alph_range_end = 90; /* Z */

const int MAX_SIZE = 50;

void expand(char s1[], char s2[]);
unsigned int is_digit(char c);
unsigned int is_lower_alphabet(char c);
unsigned int is_upper_alphabet(char c);

int main()
{
    char to_expand[] = {"a-z"};
    char expanded[MAX_SIZE];

    printf("To expand: %s\n", to_expand);
    expand(to_expand, expanded);
    printf("Expanded: %s\n", expanded);

    return 0;
}

/* The exercise doesn't seem to mention or infer that the range can expand downwards/negatively but I've allowed it to do so.
It expects null terminated strings to be passed in.
Does not handle incomplete ranges.
Doesn't handle ranges that start below 0 (ASCII 48)

Some examples and their expansions below:
a-z
abcdefghijklmnopqrstuvwxyz
a-g-z
abcdefgghijklmnopqrstuvwxyz
-a-z-
-abcdefghijklmnopqrstuvwxyz-
0-9a-z
0123456789abcdefghijklmnopqrstuvwxyz

*/
void expand(char s1[], char s2[])
{
    int i;
    int j;
    int start_of_range = -1;
    int end_of_range = -1;
    for(i = 0, j = 0; s1[i] != '\0'; ++i)
    {
        switch(s1[i])
        {
            case '-':
                if (i == 0 || s1[i + 1] == '\0')
                {
                    /* Leading/trailing '-' so just copy it */
                    s2[j++] = s1[i];
                }
                break;
            default:
                if (s1[i + 2] != '\0') /* Don't go out of bounds */
                {
                    /* Make sure we have a valid range to expand */
                    if (is_digit(s1[i]) && is_digit(s1[i + 2]) ||
                    is_lower_alphabet(s1[i]) && is_lower_alphabet(s1[i + 2]) ||
                    is_upper_alphabet(s1[i]) && is_upper_alphabet(s1[i + 2]))
                    {
                        start_of_range = i;
                        end_of_range = i + 2;
                    }
                }                
                break;
        }
        if (start_of_range != -1) /* We found a range to expand */
        {
            /* TODO: What if the range is equivalent? E.g. a-a*/
            int k = s1[start_of_range];
            if (s1[start_of_range] < s1[end_of_range])
            {
                while(k < s1[end_of_range] + 1)
                {
                    s2[j++] = k++;
                }
            }
            else
            {
                while(k > s1[end_of_range] - 1)
                {
                    s2[j++] = k--;
                }
            }
            
            start_of_range = -1;
            end_of_range = -1;
        }
    }
    s2[j] = '\0'; /* null terminate the expanded string */
}

/* Returns 1 if c is a digit, else 0 */
unsigned int is_digit(char c)
{
    if (c >= digit_range_start && c <= digit_range_end)
    {
        return 1;
    }
    return 0;
}

/* Returns 1 if c is a lower alphabet letter else 0 */
unsigned int is_lower_alphabet(char c)
{
    if (c >= lower_alph_range_start && c <= lower_alph_range_end)
    {
        return 1;
    }
    return 0;
}

/* Returns 1 if c is an upper alphabet letter, else 0 */
unsigned int is_upper_alphabet(char c)
{
    if (c >= upper_alph_range_start && c <= upper_alph_range_end)
    {
        return 1;
    }
    return 0;
}