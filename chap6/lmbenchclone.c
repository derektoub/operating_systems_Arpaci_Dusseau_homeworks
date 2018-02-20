#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

// some macros to make the code more understandable
//  regarding which pipe to use to a read/write operation
//
//  Parent: reads from P1_READ, writes on P1_WRITE
//  Child:  reads from P2_READ, writes on P2_WRITE
#define P1_READ     0
#define P2_WRITE    1
#define P2_READ     2
#define P1_WRITE    3

// the total number of pipe *pairs* we need
#define NUM_PIPES   2

int main(int argc, char *argv[])
{
    struct timeval tv0;
    struct timeval tv1;
    long elapsed;
    int fd[2*NUM_PIPES];
    int val = 0, len, i;
    pid_t pid;
    // create all the descriptor pairs we need
    for (i=0; i<NUM_PIPES; ++i)
    {
        if (pipe(fd+(i*2)) < 0)
        {
            perror("Failed to allocate pipes");
            exit(EXIT_FAILURE);
        }
    }

    // fork() returns 0 for child process, child-pid for parent process.
    if ((pid = fork()) < 0)
    {
        perror("Failed to fork process");
        return EXIT_FAILURE;
    }

    // if the pid is zero, this is the child process
    if (pid == 0)
    {
        for (i=0; i<10; ++i){
            // Child. Start by closing descriptors we
            //  don't need in this process
            close(fd[P1_READ]);
            close(fd[P1_WRITE]);

            // used for output
            pid = getpid();

            // wait for parent to send us a value
            len = read(fd[P2_READ], &tv0, sizeof(tv0));
            if (len < 0)
            {
                perror("Child: Failed to read data from pipe");
                exit(EXIT_FAILURE);
            }
            else if (len == 0)
            {
                // not an error, but certainly unexpected
                fprintf(stderr, "Child: Read EOF from pipe");
            }
            else
            {
                gettimeofday(&tv1, NULL);
                elapsed = (tv1.tv_sec-tv0.tv_sec)*1000000 + tv1.tv_usec-tv0.tv_usec;
                printf("Child (%d) context switch took: %ld usec\n", pid, elapsed);
                if (write(fd[P2_WRITE], &tv1, sizeof(tv1)) < 0)
                {
                    perror("Child: Failed to write response value");
                    exit(EXIT_FAILURE);
                }
            }
        }

        // finished. close remaining descriptors.
        close(fd[P2_READ]);
        close(fd[P2_WRITE]);

        return EXIT_SUCCESS;
    }

    for (i=0; i< 10; ++i){
        // Parent. close unneeded descriptors
        close(fd[P2_READ]);
        close(fd[P2_WRITE]);

        // used for output
        pid = getpid();
        // send a value to the child
        gettimeofday(&tv0, NULL);
        if (write(fd[P1_WRITE], &tv0, sizeof(tv0)) != sizeof(tv0))
        {
            perror("Parent: Failed to send value to child ");
            exit(EXIT_FAILURE);
        }

        // now wait for a response
        len = read(fd[P1_READ], &tv0, sizeof(tv0));
        if (len < 0)
        {
            perror("Parent: failed to read value from pipe");
            exit(EXIT_FAILURE);
        }
        else if (len == 0)
        {
            // not an error, but certainly unexpected
            fprintf(stderr, "Parent(%d): Read EOF from pipe", pid);
        }
        else
        {
            // report what we received
            gettimeofday(&tv1, NULL);
            elapsed = (tv1.tv_sec-tv0.tv_sec)*1000000 + tv1.tv_usec-tv0.tv_usec;
            printf("Parent(%d): context switch took: %ld\n", pid, elapsed);
        }
    }
    // close down remaining descriptors
    close(fd[P1_READ]);
    close(fd[P1_WRITE]);

    // wait for child termination
    wait(NULL);

    return EXIT_SUCCESS;
}
