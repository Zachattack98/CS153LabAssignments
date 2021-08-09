#include "types.h"
#include "user.h"
#include "stat.h"

int main(int argc, char *argv[]) {
    int num = 8;
    setPrior(2, num);

    int i, j;
    for(i = 0; i < 4500; i++) {
        asm("nop"); //assembly instruction to do nothing
        for(j = 0; j < 4500; j++) {
            asm("nop");
        }
    }


    printf(1, "\nProcess 2 finished! - ");
    printTime();
    exitStats(0);
}