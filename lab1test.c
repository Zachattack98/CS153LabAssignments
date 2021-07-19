#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
//#include "types.h"
//#include "defs.h"
#include "syscall.h"
//#include "proc.h"
#include "user.h"

int stdout = 1;

int main(int argc, char *argv[]) {
    //begin testing status pointer with wait2
    /*if(fork() == 0) {   //implement simple fork() as condition
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
    return(0);*/





   printf(1, "This is the test program for CS153 Lab1\n\n"); // fd=1 means standard output.
 
   int child1_pid, child2_pid, child3_pid;
   int child1_return, child2_return, child3_return, no_child_return;
   int child1_exit, child2_exit; //, child3_exit;
 
   printf(1, "Tests for exit(int status) and wait(int* status)...\n");
   child1_pid = fork();
   if (!child1_pid) { //child1
     printf(1, "Child 1 - Child 1's pid: %d\n", getpid());
     printf(1, "Child 1 - Exiting with status 10...\n");
     exit(10);
   }
   else if (child1_pid > 0) { //Parent
     child1_return = wait2(&child1_exit);
     printf(1, "Parent - Child 1's return pid: %d\n", child1_return);
     printf(1, "Parent - Child 1's exit status: %d\n\n", child1_exit);
   }
 
   child2_pid = fork();
   if (!child2_pid) {  //child2
     printf(1, "Child 2's pid: %d\n", getpid());
     printf(1, "Child 2 exiting with status 20...\n");
     exit(20);
   }
   else if (child2_pid > 0) { //Parent
     child2_return = wait2(&child2_exit);
     printf(1, "Parent - Child 2's return pid: %d\n", child2_return);
     printf(1, "Parent - Child 2's exit status: %d\n\n", child2_exit);
   }
 
   child3_pid = fork();
   if (!child3_pid) { //child3
     printf(1, "Child 3's pid: %d\n", getpid());
     printf(1, "Child 3 exiting with status 30...\n");
     exit(30);
   }
   else if (child3_pid > 0) { //Parent
     int* child3_null = 0;
     child3_return = wait2(child3_null);
     printf(1, "Parent - Child 3's return pid: %d\n", child3_return);
     if (child3_null == 0)
       printf(1, "Parent - Child 3's exit status: NULL\n\n");
     else
       printf(1, "Parent - Child 3's exit status: %d\n\n", child3_null);
   }

   int* no_child = 0;
   no_child_return = wait2(no_child);
   printf(1, "Parent - No Child's return pid: %d\n", no_child_return);
   if (no_child == 0)
    printf(1, "Parent - No Child's exit status: NULL\n\n");
   else
    printf(1, "Parent - No Child's exit status: %d\n\n", no_child);

   printf(1, "\nTests for waitpid(int pid, int* status, int option)...\n");

   int return_pid, exit_status;
   int pids[5]={0, 0, 0, 0, 0};

   int i;
   for (i = 0; i <5; i++) {
    pids[i] = fork();

    if (!pids[i]) { // Children
        printf(1, "Child %d - pid: %d, exit status: %d\n", i, getpid(), (i+1)*100);
        //sleep(5);
        //sleep(5 + 5 * i);
        exit(0);
        //exit((i+1)*100);
    }
   }

sleep(5);
printf(1, "\nSParent - Waiting process w/ pid %d\n", 1, pids[1]);
return_pid = waitpid(pids[1], &exit_status, 0);
printf(1, "Parent - Child %d exited, returned pid: %d, exit status %d\n\n", 1, return_pid, exit_status);

sleep(5);
printf(1, "Parent - Waiting process w/ pid %d\n", 4, pids[4]);
return_pid = waitpid(pids[4], &exit_status, 0);
printf(1, "Parent - Child %d exited, returned pid: %d, exit status %d\n\n ", 4, return_pid, exit_status);

sleep(5);
printf(1, "Parent - Waiting process w/ pid %d\n", 3, pids[3]);
return_pid = waitpid(pids[3], &exit_status, 0);
printf(1, "Parent - Child %d exited, returned pid: %d, exit status %d\n\n ", 3, return_pid, exit_status);

sleep(5);
printf(1, "Parent - Waiting process w/ pid %d\n", 0, pids[0]);
return_pid = waitpid(pids[0], &exit_status, 0);
printf(1, "Parent - Child %d exited, returned pid: %d, exit status %d\n\n ", 0, return_pid, exit_status);

sleep(5);
printf(1, "Parent - Waiting process w/ pid %d\n", 2, pids[2]);
return_pid = waitpid(pids[2], &exit_status, 0);
printf(1, "Parent - Child %d exited, returned pid: %d, exit status %d\n\n ", 2, return_pid, exit_status);

sleep(5);
printf(1, "Parent - Waiting process w/ pid %d\n", 1, 999);
return_pid = waitpid(999, &exit_status, 0);
printf(1, "Parent - Child %d exited, returned pid: %d, exit status %d\n", 4, return_pid, exit_status);

//exit(0);

return 0;

}