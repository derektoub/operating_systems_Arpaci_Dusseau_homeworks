#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    const char * pmsg = "This is parent getting started\n";
    int fd = open("./testwrite.output", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
    write(fd, pmsg, strlen(pmsg));
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        //child write to file descriptor from parent process
        const char * cmsg = "This is child";
        write(fd, cmsg, strlen(cmsg));
    } else {
        // parent write finish to file
        int cpid = wait(NULL);
        const char * pendmsg = "Parent finishing";
        write(fd, pendmsg, strlen(pendmsg));
    }
    return 0;
}
