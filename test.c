#include "defs.h"
#include "syscall.h"
#include "proc.h"
#include "user.h"

int stdout = 1;

int main(int argc, char *argv[]) {
    //begin testing status pointer with wait2
    if(fork() == 0) {   //implement simple fork() as condition
        sleep(2);   //sleeps for only 2 seconds
        exit(25);   //can be any value
    }
    else {
        int *status = malloc(sizeof(*status));
        wait2(status);
        printf(stdout, "\nStatus in User (wait2): %d\n", *status);
    }

    int child_pid;  //temporary variable for holding process id for child

    //now begin testing status pointer with waitpid
    if((child_pid = fork()) == 0) {   //assign child pid as fork() to compare in the condition
        sleep(2);
        exit(14);   //can be any value
    }
    else {
        int *status = malloc(sizeof(*status));
        waitpid(child_pid, status, 0);
        printf(stdout, "\nStatus in User (waitpid): %d\n", *status);
    }

    exit(0);
}