#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <fcntl.h>
#include <sys/time.h>

// see how long 10000 system calls (printf/write) take
int main(int argc, char *argv[])
{
    struct timeval tv0;
    struct timeval tv1;
    int i;
    long elapsed;
    gettimeofday(&tv0, NULL);
    for (i=0; i<9999; i++){
        printf("");
    }
    printf("\n");
    gettimeofday(&tv1, NULL);
    elapsed = (tv1.tv_sec-tv0.tv_sec)*1000000 + tv1.tv_usec-tv0.tv_usec;
    printf("elapsed is: %ld microseconds\n", elapsed);
    return 0;
}
