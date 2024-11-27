#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
    char* args[MAXARG];
    char lines[1000];
    int n;
    if (argc > 1)
    {
        for (int i = 0; i < argc - 1; i++) {
            args[i] = argv[i + 1];
        }
        while ((n = read(0, lines, sizeof(lines))) > 0) {
            char *p = lines;
            while (p < lines + n) {
                char *line = p;
                while (p < lines + n && *p != '\n') {
                    ++p;
                }
                *p = '\0';
                ++p;

                args[argc - 1] = line;
                args[argc] = 0;

                int pId = fork();
                if (pId < 0) {
                    printf("fork: failed.\n");
                    exit(1);
                }
                else if (pId == 0) {
                    exec(args[0], args);
                    exit(0);
                }
                else {
                    wait(0);
                }
            }
        }
        exit(0);
    }
}