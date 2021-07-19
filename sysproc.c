#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

//changed***************************************
int
sys_exit(void)
{
  int exitStat;
  if(argint(0, &exitStat) < 0) {  //assign new exit status to address 0 for exit() 
    return -1;                    //to test if status is not less than 0.
  }                               //before providing it in the parameter of exit().
  exit(exitStat);
  return 0;         //not reached
}
//changed***************************************

int
sys_wait(void)
{
  return wait();
}

//changed***************************************
int
sys_wait2(void)
{
  int *waitStat;
  if(argptr(0, (void*)&waitStat, sizeof(waitStat)) < 0) {   //assign new status pointer to address 0 for wait()
    return -1;                                              //before providing it in the parameter of wait()
  }                                               
  return wait2(waitStat);  //returning pointer after using it as a parameter
}

int
sys_waitpid(void)
{
  int pid;
  int *waitStat;
  int options;
  //assign all variables to three different addresses for waitpid() beforehand
  if (argint(0, &pid) < 0) {
		return -1;
	}
	if (argptr(1, (void*)&waitStat, sizeof(waitStat)) < 0) {
		return -1;
	}
  if (argint(2, &options) < 0) {
		return -1;
	}

  return waitpid(pid, waitStat, options);  //returning pointer after using it as a parameter
}
//changed***************************************

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
