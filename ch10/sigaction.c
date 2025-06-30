#define _XOPEN_SOURCE
// #define _GNU_SOURCE
// #define _POSIX_C_SOURCE 200809L


#include <unistd.h>
// #include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>

void timeout(int sig)
{
    if(sig==SIGALRM)
        puts("Time Out!");
    alarm(2);
}

int main(int argc, char* argv[]) {
    pid_t pid;

    struct sigaction act;
    act.sa_handler = timeout;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);

    sigaction(SIGALRM, &act, 0);

    alarm(2);

    for(int i=0; i<3; ++i) {
        puts("wait...");
        sleep(100);
    }




    // pid = fork();


    return 0;
}