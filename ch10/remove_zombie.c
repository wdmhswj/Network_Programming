#define _XOPEN_SOURCE
// #define _GNU_SOURCE
// #define _POSIX_C_SOURCE 200809L


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>

void read_childproc(int sig)
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status))
    {
        printf("Remove proc id: %d\n", id);
        printf("Child send: %d\n", WEXITSTATUS(status));
    }
}

int main(int argc, char* argv[]) {
    pid_t pid;

    struct sigaction act;
    act.sa_handler = read_childproc;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act, 0);

    pid = fork();

    if(pid==0)
    {
        puts("I'm Child process");
        sleep(10);
        return 23;
    } else {
        printf("child process id: %d\n", pid);
        pid = fork();
        if (pid==0) {
            puts("I'm Child process");
            sleep(10);
            exit(32);
        } else {
            printf("child process id: %d\n", pid);
            for(int i=0; i<5; ++i) {
                puts("wait ...");
                sleep(5);

            }
        }
    }

    return 0;
}