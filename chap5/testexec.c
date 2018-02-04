#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child calls exec
        char * const lsargs[3] = {"ls", "/", NULL};
        char * const env[] = {NULL};
        // execl("/bin/ls", "ls", "/", NULL);
        // execlp("ls", "ls", "/", NULL);
        // execle("/bin/ls", "ls", "/", NULL, env);
        // execv("/bin/ls", lsargs);
        // execvp("ls", lsargs);
        execvP("ls", "/usr/bin:/bin", lsargs);
        // The calls with v in the name take an array parameter to specify the argv[] array of the new program.
        // The calls with l in the name take the arguments of the new program as a variable-length argument list to the function itself.
        // The calls with e in the name take an extra argument to provide the environment of the new program; otherwise, the program inherits the current process's environment.
        // The calls with p in the name search the PATH environment variable to find the program if it doesn't have a directory in it (i.e. it doesn't contain a / character). Otherwise, the program name is always treated as a path to the executable.
        // https://stackoverflow.com/a/20823413/8983048
    } else {
        // parent goes down this path (original process)
        int cpid = wait(NULL);
        printf("parent here; child finished exec!\n");
    }
    return 0;
}
