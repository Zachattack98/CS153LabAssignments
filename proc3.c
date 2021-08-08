#include "types.h"
#include "user.h"
#include "stat.h"

int main(int argc, char *argv[]) {
    setPrior(31);

    int i, j;
    for(i = 0; i < 4500; ++i) {
        asm("nop"); //assembly instruction to do nothing (NOP)
        for(j = 0; j < 4500; ++j) {
            asm("nop");
        }
    }
    printf(1, "\nProcess 3 finished! - ");
    printTime();
    exitStats(0);
}