#include "kernel/types.h"
#include "user.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(2, "Usage: sleep sec\n\n");
        exit(1);
    }
    int sec = atoi(argv[1]);
    sleep( sec * 10 );
    exit(0);
}
