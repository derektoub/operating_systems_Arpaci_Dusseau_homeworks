#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", (int) getpid());
    int testvar = 100;
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        testvar = 101;
        printf("hello, I am child here is testvar: %d\n", testvar);
    } else {
        // parent goes down this path (original process)
        int cpid = wait(NULL);
        printf("hello, I am parent of %d (testvar:%d) (pid:%d)\n",
           rc, testvar, (int) getpid());
        testvar = 102;
        printf("hello, I am parent here is testvar after change: %d\n", testvar);
    }
    return 0;
}
