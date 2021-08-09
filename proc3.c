#include "types.h"
#include "user.h"
#include "stat.h"
//#include "proc.h"

int main(int argc, char *argv[]) {
    //struct proc *p;
    int num = 30;
    setPrior(3, num);

    int i, j;
    for(i = 0; i < 4500; ++i) {
        asm("nop"); //assembly instruction to do nothing (NOP)
        for(j = 0; j < 4500; ++j) {
            asm("nop");
        }
    }

    /*if(p->ageWait < num) {
        cprintf("\nCurrent: %d   New: %d  -> Process is waiting and priority has decremented\n", num, p->ageWait);
    }
    if(p->ageRun > num) {
        cprintf("\nCurrent: %d   New: %d  -> Process is running and priority has incremented\n", num, p->ageRun);
    }*/

    printf(1, "\nProcess 3 finished! - ");
    printTime();
    exitStats(0);
}