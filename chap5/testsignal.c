#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void handle_signal(int signal_id){
    printf("goodbye\n");
}

int main(int argc, char *argv[])
{
    int par_pid = getpid();
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child prints hello and signals to parent
        printf("hello\n");
        kill(par_pid, SIGCONT);
    } else {
        // wait for signal, call handle_signal when received
        signal(SIGCONT, handle_signal);
        pause();
    }
    return 0;
}