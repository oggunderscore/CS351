#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

void display(char *prog, char *bytes, int n);

int main(void)
{

    const char *name = "/shared-memory"; // file name
    const int SIZE = 2048;               // file size

    const char *message0 = "I love ";
    const char *message1 = "Memory Management\n";

    int shm_fd;     // file descriptor, from shm_open()
    char *shm_base; // base address, from mmap()
    char *ptr;      // shm_base is fixed, ptr is movable

    pid_t pid;

    pid = fork();

    if (pid == 0)
    {
        printf("Child process in progress...creating and writing to the memory!\n");

        // Create the memory sector
        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        if (shm_fd == -1)
        {
            printf("Child: Shared memory failed: %s\n", strerror(errno));
            exit(1);
        }

        // Set size of shared memory
        ftruncate(shm_fd, SIZE);

        // Map the memory segment to this child process
        shm_base = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (shm_base == MAP_FAILED)
        {
            printf("Child: Map failed: %s\n", strerror(errno));
            // close and shm_unlink?
            exit(1);
        }

        // Check to make sure memory space is empty and has no data
        display("Child", shm_base, SIZE / 64);

        // Begin writing data to the memory space
        ptr = shm_base;
        ptr += sprintf(ptr, "%s", message0);
        ptr += sprintf(ptr, "%s", message1);

        // Print the memory space again to confirm write worked
        display("Child", shm_base, SIZE / 64);

        // Remove memory space segment within this child process
        if (munmap(shm_base, SIZE) == -1)
        {
            printf("Child: Unmap failed: %s\n", strerror(errno));
            exit(1);
        }

        // Close file here to be reopened later for a full parent process handling
        if (close(shm_fd) == -1)
        {
            printf("Child: Close failed: %s\n", strerror(errno));
            exit(1);
        }

        printf("Child process completed.\n");
    }
    else
    {

        printf("Parent process is waiting on child process...\n");
        wait(NULL);

        // Parent Process

        // Open the shared memory space as read-only
        shm_fd = shm_open(name, O_RDONLY, 0666);

        // Handle case if failed to open memory
        if (shm_fd == -1)
        {
            printf("Parent: Shared memory open failed: %s\n", strerror(errno));
            exit(1);
        }

        // Tell Parent process where the memory space is by mapping it
        shm_base = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

        // Handle case if failed to map memory space
        if (shm_base == MAP_FAILED)
        {
            printf("Parent: Map failed: %s\n", strerror(errno));
            exit(1);
        }

        // Read the memory segment as Parent
        display("Parent", shm_base, SIZE / 64);                // Print the bytes
        printf("Parent has read the following: %s", shm_base); // Print as a Str

        // Remove memory space map from Parent process
        if (munmap(shm_base, SIZE) == -1)
        {
            printf("Parent: Unmap failed: %s\n", strerror(errno));
            exit(1);
        }

        // Close memory file
        if (close(shm_fd) == -1)
        {
            printf("Parent: Close failed: %s\n", strerror(errno));
            exit(1);
        }

        // Completely unlink and remove the memory file from the system and handle error case
        if (shm_unlink(name) == -1)
        {
            printf("Parent: Error removing %s: %s\n", name, strerror(errno));
            exit(1);
        }

        printf("Parent read process completed.\n");
    }
    return 0;
}

void display(char *prog, char *bytes, int n)
{
    printf("!! %s requesting print:\n", prog);
    for (int i = 0; i < n; i++)
    {
        printf("%02x%c", bytes[i], ((i + 1) % 16) ? ' ' : '\n');
    }
    printf("\n");
}
