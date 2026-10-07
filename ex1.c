#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

int main() {
    pid_t pid1, pid2;
    clock_t start1, start2, start_main;
    double time1, time2, time_main;

    pid1 = fork();

    if (pid1 < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid1 == 0) {
        start1 = clock();
        printf("Child 1: PID = %d, Parent PID = %d\n", getpid(), getppid());
        time1 = (double)(clock() - start1) / CLOCKS_PER_SEC * 1000.0;
        printf("Child 1 execution time: %.3f ms\n", time1);
        exit(0);
    }

    pid2 = fork();

    if (pid2 < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid2 == 0) {
        start2 = clock();
        printf("Child 2: PID = %d, Parent PID = %d\n", getpid(), getppid());
        time2 = (double)(clock() - start2) / CLOCKS_PER_SEC * 1000.0;
        printf("Child 2 execution time: %.3f ms\n", time2);
        exit(0);
    }

    start_main = clock();
    printf("Main: PID = %d, Parent PID = %d\n", getpid(), getppid());
    time_main = (double)(clock() - start_main) / CLOCKS_PER_SEC * 1000.0;
    printf("Main execution time: %.3f ms\n", time_main);

    wait(NULL);
    wait(NULL);

    return 0;
}
