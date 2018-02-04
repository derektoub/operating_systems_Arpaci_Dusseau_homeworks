#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        int cpid = getpid();
        printf("hello from child pid: %d\n", cpid);
        int unknown = wait(NULL);
        printf("wait returns the following in child: %d\n", unknown);
    } else {
        // parent goes down this path (original process)
        int waitreturn = wait(NULL);
        printf("Parent wait returns the child's pid: %d\n", waitreturn);
    }
    return 0;
}
