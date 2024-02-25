#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    int pc[2], cp[2];
    pipe(pc);
    pipe(cp);
    char *ping = "ping";
    char *pong = "pong";
    char buf[512] = {0};
    int child_id = fork();
    if (child_id != 0)
    {
        close(pc[0]);
        close(cp[1]);
        write(pc[1], ping, strlen(ping));
        read(cp[0], buf, sizeof(buf));
        printf("%d: received %s\n", getpid(), buf);
        exit(0);
    }
    else
    {
        close(pc[1]);
        close(cp[0]);
        read(pc[0], buf, sizeof(buf));
        printf("%d: received %s\n", getpid(), buf);
        write(cp[1], pong, strlen(pong));
        exit(0);
    }
}