#include "types.h"
#include "user.h"
#include "stat.h"

int main(int argc, char *argv[]) {
    int num = 31;
    setPrior(num);

    int i, j;
    for(i = 0; i < 4500; ++i) {
        asm("nop"); //assembly instruction to do nothing (NOP)
        for(j = 0; j < 4500; ++j) {
            asm("nop");
        }
    }
    int num2;
    num2 = 32 - num;

    printf(1, "\nProcess 3 finished! - ");
    printTime(num2);
    exitStats(0);
}