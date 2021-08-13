#include "types.h"
#include "user.h"
#include "stat.h"

int main(int argc, char *argv[]) {
    int num = 17;
    setPrior(1, num);

    int i, j;
    for(i = 0; i < 4500; i++) {
        //NOP means no operation, i.e. do not change the process's state
        asm("nop"); //assembly instruction to do nothing
        for(j = 0; j < 4500; j++) {
            asm("nop");
        }
    }
    printf(1, "\nProcess 1 finished! - ");
    printTime();
    exitStats(0);//exit current process
}