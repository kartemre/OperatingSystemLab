#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <errno.h>

void write_log(char* message) {
    FILE* log_file = fopen("logs/log_all.log", "a");
    if (log_file == NULL) {
        printf("Error: Failed to open log file.\n");
        exit(1);
    }
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    char timestamp[30];
    strftime(timestamp, sizeof(timestamp), "%c", tm);
    char log_message[200];
    sprintf(log_message, "[%s]: %s\n", timestamp, message);
    fprintf(log_file, "%s", log_message);
    fclose(log_file);
}


int main() {
    mkdir("logs", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    write_log("Log file created.\n");

    // Ebeveyn proses
    pid_t parent_pid = getpid();
    printf("Parent process with PID: %d\n", parent_pid);
    write_log("Parent process created.\n");

    // Çocuk 1 proses
    pid_t child1_pid = fork();
    if (child1_pid == 0) {
        sleep(1); // En az 1 torunun işlemini tamamlaması için beklenir
        printf("Child 1 process with PID: %d from parent process with PID: %d\n", getpid(), parent_pid);
        write_log("Child 1 process created.\n");
        // Torun 1.1 proses
        pid_t grandchild1_1_pid = fork();
        if (grandchild1_1_pid == 0) {
            sleep(1); // 1 sn. bekle
            printf("Grandchild 1.1 process with PID: %d from parent process with PID: %d\n", getpid(), getppid());
            write_log("Grandchild 1.1 process created.\n");
            _exit(0);
        }

        // Torun 1.2 proses
        pid_t grandchild1_2_pid = fork();
        if (grandchild1_2_pid == 0) {
            sleep(2); // 2 sn. bekle
            printf("Grandchild 1.2 process with PID: %d from parent process with PID: %d\n", getpid(), getppid());
            write_log("Grandchild 1.2 process created.\n");
            _exit(0);
        }

        // Torun 1.3 proses
        pid_t grandchild1_3_pid = fork();
        if (grandchild1_3_pid == 0) {
            sleep(3); // 3 sn. bekle
            printf("Grandchild 1.3 process with PID: %d from parent process with PID: %d\n", getpid(), getppid());
            write_log("Grandchild 1.3 process created.\n");
            _exit(0);
        }

        // En az 1 torunun işlemini tamamlamasını bekler
        waitpid(grandchild1_1_pid, NULL, 0);
        waitpid(grandchild1_2_pid, NULL, 0);
        waitpid(grandchild1_3_pid, NULL, 0);
        _exit(0);
    }

    // Çocuk 2 proses
    pid_t child2_pid = fork();
    if (child2_pid == 0) {
        sleep(1); // 1 sn. bekle
        printf("Child 2 process with PID: %d from parent process with PID: %d\n", getpid(), parent_pid);
        write_log("Child 2 process created.\n");
        _exit(0);
    }

    waitpid(child1_pid, NULL, 0);
    waitpid(child2_pid, NULL, 0);

    return 0;
}
