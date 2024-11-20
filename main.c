/*CALMAC Stefan-314CC*/
#include "task1.h"
#include "task2.h"

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        printf("Too many arguments\n");
        return 0;
    }

    FILE *in;
    FILE *out;

    in = fopen("tema3.in", "r");
    out = fopen("tema3.out", "w");
    if (argv[1][0] == '1')
    {
        task1(in, out);
    }
    else if (argv[1][0] == '2')
    {
        task2(in, out);
    }

    fclose(in);
    fclose(out);

    return 0;
}