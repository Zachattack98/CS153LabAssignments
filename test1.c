#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"

int main(int argc, char *argv[]) {
  printf(1, "This is the test program for CS153 Lab1\n\n"); // fd=1 means standard output.
    //begin testing status pointer with wait2
    if(fork() == 0) {   //implement simple fork() as condition
        sleep(2);   //sleeps for only 2 seconds
        exitStats(25);   //can be any value
    }
    else {
        int *status = malloc(sizeof(*status));
        wait2(status);
        printf(1, "\nStatus in User (wait2): %d\n", *status);
    }

    int child_pid;  //temporary variable for holding process id for child

    //now begin testing status pointer with waitpid
    if((child_pid = fork()) == 0) {   //assign child pid as fork() to compare in the condition
        sleep(2);
        exitStats(14);   //can be any value
    }
    else {
        int *status = malloc(sizeof(*status));
        waitpid(child_pid, status, 0);
        printf(1, "\nStatus in User (waitpid): %d\n", *status);
    }

    exitStats(0);
    //return(0);

}