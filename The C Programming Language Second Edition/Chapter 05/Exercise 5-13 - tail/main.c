/* Write the program tail, which prints the last n lines of its
input. By default, n is 10, let us say, but it can be changed by an optional
argument, so that

    tail -n
    
prints the last n lines. The program should behave rationally no matter how
unreasonable the input or the value of n. Write the program so it makes the
best use of available storage; lines should be stored as in the sorting program of
Section 5.6, not in a two-dimensional array of fixed size. */

void print_usage()
{
    printf("Usage tail -n, e.g. tail -5\n");
}

int main(int argc, char **argv)
{
    if (argc > 2)
    {
        print_usage();
        return -1;
    }
    int n = 10;
    if (argc == 2)
    {
        if ((*++argv)[0] == '-')
        {

        }
    }    

    return 0;
}