#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    char buf[10];

    int p[2];
    pipe(p);

    int pid;
    pid = fork();

    if (pid<0) {
        printf("fork: failed\n");
        exit(1);
    } else if (pid == 0) {
        read(p[0], buf, sizeof(buf));
        close(p[0]);
        printf("%d: received %s\n", getpid(), buf);
        write(p[1], "pong",5);
        close(p[1]);
        exit(0);
    } else {
        write(p[1], "ping", 5);
        close(p[1]);
        wait(0);
        read(p[0], buf, sizeof(buf));
        close(p[0]);
        printf("%d: received %s\n", getpid(), buf);
        exit(0);
    }
}