#include <sys/types.h>
#include <iostream>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

using namespace std;

int main(void)

{
    char write_msg[BUFFER_SIZE] = "Greetings";
    char read_msg[BUFFER_SIZE];
    int fd[2];
    int fd2[2]; // Create a second pipe?
    pid_t pid;

    if (pipe(fd) == -1) // Initiate the Pipe
    {
        cout << "ERROR: Pipe Failed." << endl;
        return 1;
    }

    if (pipe(fd2) == -1) // Initiate the Pipe
    {
        cout << "ERROR: Pipe2 Failed." << endl;
        return 1;
    }

    write(fd[WRITE_END], write_msg, strlen(write_msg) + 1); // Write message to pipe fd
    close(fd[WRITE_END]);                                   // Close writing end of pipe

    cout << "Checking if read_msg is starting empty: read_msg =  " << read_msg << endl;
    cout << "Forking child..." << endl;

    pid = fork();

    if (pid < 0)
    {
        cout << "ERROR: Fork Failed." << endl;
        return 1;
    }

    if (pid > 0) // Child
    {

        read(fd[READ_END], read_msg, BUFFER_SIZE);                               // Read the information written by parent
        close(fd[READ_END]);                                                     // Close pipe now that it has been finished and used
        cout << "Child read: " << read_msg << "\nFlipping cases now..." << endl; // Print current data

        // Do flipping algorithm here
        for (int x = 0; x < strlen(read_msg); x++)
        {
            if (isupper(read_msg[x]))
                read_msg[x] = tolower(read_msg[x]);
            else if (islower(read_msg[x]))
                read_msg[x] = toupper(read_msg[x]);
        }

        write(fd2[WRITE_END], read_msg, strlen(read_msg) + 1); // Write to the new pipe the new "read_msg"
        close(fd2[WRITE_END]);                                 // Close it as we are done at this end of this pipe
    }
    else // Parent
    {
        char new_msg[BUFFER_SIZE];
        read(fd2[READ_END], new_msg, BUFFER_SIZE);
        cout << "Parent read: " << new_msg << endl;
        close(fd2[READ_END]);
    }

    return 0;
}