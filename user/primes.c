#include "kernel/types.h"
#include "user/user.h"

void primes(int*) __attribute__((noreturn));

void primes(int* pre_p) {

    close(pre_p[1]);

    int prime;
    if (read(pre_p[0], &prime, sizeof(prime)) == 0) {
        close(pre_p[0]);
        exit(0);
    }
    printf("prime %d\n", prime);
    
    int next_p[2];
    pipe(next_p);
    
    int pId = fork();

    if (pId < 0) {
        printf("fork: failed.\n");
        exit(1);
    } else if (pId == 0) {
        close(pre_p[0]);
        primes(next_p);
    } else {
        int num;
        close(next_p[0]);
        while (read(pre_p[0], &num, sizeof(num)) != 0)
        {
            if (num % prime != 0) {
                write(next_p[1], &num, sizeof(num));
            }
        }
        close(pre_p[0]);
        close(next_p[1]);
        wait(0);
        exit(0);
    }
}

int
main(int argc, char *argv[]) 
{
    int p[2];
    pipe(p);

    int pId = fork();

    if (pId < 0) {
        printf("fork: failed.\n");
        exit(1);
    } else if (pId == 0) {
        primes(p);
    }
    else
    {
        close(p[0]);
        for (int i = 2; i <= 280; i++)
        {
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
        wait(0);
    }
}