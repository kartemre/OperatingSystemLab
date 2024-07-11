#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

void child_process(int pipe_fd[2], int alarm_time) {
    char msg[100];
    close(pipe_fd[0]); // Close the read end of the pipe in child process
    if (alarm_time % 2 == 0) {
        sprintf(msg, "I'll try to kill you when your time is up! (death)");
    } else {
        sprintf(msg, "You'll live your life! (life)");
    }
    write(pipe_fd[1], msg, sizeof(msg));
    sleep(alarm_time);
    close(pipe_fd[1]); // Close the write end of the pipe in child process
    if (alarm_time % 2 == 0) {
        exit(EXIT_SUCCESS); // Terminate child process if alarm_time is even
    } else {
        while (1) {
            sleep(1); // Wait indefinitely if alarm_time is odd
        }
    }
}

void parent_process(int pipe_fd[2]) {
    close(pipe_fd[1]); // Close the write end of the pipe in parent process
    char msg[100];
    read(pipe_fd[0], msg, sizeof(msg));
    printf("Message from child process: %s\n", msg);
    int destiny = 0;
    printf("Waiting for destiny...\n");
    sleep(5); // Wait for 5 seconds to determine destiny
    if (strstr(msg, "death") != NULL) {
        destiny = 1; // Death
    }
    printf("Child's destiny: %s\n", (destiny == 1) ? "Death" : "Life");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <integer_argument>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int alarm_time = atoi(argv[1]);
    
    // Ignore Ctrl+C and Ctrl+Z signals
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (pid == 0) { // Child process
        child_process(pipe_fd, alarm_time);
    } else { // Parent process
        parent_process(pipe_fd);
        wait(NULL); // Wait for child process to terminate
    }
    return EXIT_SUCCESS;
}