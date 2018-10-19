#define _GNU_SOURCE
#include "sched.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/mount.h"
#include "sys/wait.h"
#include "unistd.h"

void run(char* argv[]){
    printf("Running command %s\n", argv[2]);
    char** new_args = &argv[2];
    unshare(CLONE_NEWUTS | CLONE_NEWPID | CLONE_NEWNS);

    pid_t child_pid;

    child_pid = fork();
    if(child_pid == 0){
        chroot("/home/containerfs/alpine");
        chdir("/");
        mount("proc", "proc", "proc", 0, "");
        execvp(argv[2], new_args);
    } else if(child_pid > 0){
        int return_status;
        waitpid(child_pid, &return_status, 0);
    }
    else {
        exit(1);
    }
}

int main(int argc, char* argv[])
{
    if(argc < 3){
        fprintf(stderr, "Not enough arguments: %d\n", argc);
        return 1;
    }

    if(strcmp(argv[1], "run") == 0) {
        run(argv);
    } else {
        fprintf(stderr, "WAT?");
        return 1;
    }
}