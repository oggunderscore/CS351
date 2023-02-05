// Kevin Nguyen - OS Concepts - CPSC 351 - 2/1/23
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

bool intVal(int x) {
    if (x > 0)
        return true;
    else
        return false;
}

int main(int argc, char** argv) {
    int x = atoi(argv[1]);
    pid_t pid;

    if (argc == 2) {
        if (!intVal(x)) {
            printf("ERROR: Integer is not a number > 0! Exiting...");
            return -1;
        }
    } else {
        if (argc == 1) {
            printf("No args found! Please type 1 arg only!");
            return -1;
        }
        printf("Invalid Args inputted! Please type 1 arg only!");
        return -1;
    }

    pid = fork();

    if (pid == 0) {
        printf("Child process in progress...\n");
        printf("%d\n", x);
        while (x != 1) {
            if (x % 2 == 0) {
                x = x / 2;
            } else if (x % 2 == 1) {
                x = 3 * (x) + 1;
            }
            printf("%d\n", x);
        }
        printf("Child process completed.\n");
    } else {
        printf("Parent process is waiting on child process...\n");
        wait(NULL);
        printf("Parent process completed.\n");
    }
    return 0;
}