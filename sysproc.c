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

int
sys_exit(void)
{
  exit();
  return 0;
}

int
sys_exitStats(void)
{
  int exitStat;
  //passing integer into the kernel function
  if(argint(0, &exitStat) < 0) {  //assign new exit status to address 0 for exit() 
                                  //to test if status is valid
                                  //before providing it in the parameter of exit().
    return -1;    //return -1 if not valid
  }
  
  exitStats(exitStat);
  return 0;
}

int
sys_wait(void)
{
  return wait();
}

int
sys_wait2(void)
{
  int *waitStat;
  if(argptr(0, (void*)&waitStat, sizeof(waitStat)) < 0) {   //assign new status pointer to address 0 for wait()
                                                            //before providing it in the parameter of wait()
    return -1;  //return -1 if no child exists
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
  //return value is the process id of the process that was terminated
  if (argint(0, &pid) < 0) {
		return -1;  //return -1 if process does not exist or an unexpected error occurred.
	}
	if (argptr(1, (void*)&waitStat, sizeof(waitStat)) < 0) {
		return -1;
	}
  if (argint(2, &options) < 0) {
		return -1;
	}

  return waitpid(pid, waitStat, options);  //returning pointer after using it as a parameter
}

int sys_setPriority(void) {
  int priorNum;
  int priorStat;
  if(argint(0, &priorNum) < 0) {   //assign new status pointer to address 0 for setPriority()
    return -1;  //return -1
  }
  if(argint(1, &priorStat) < 0) {   //assign new status pointer to address 1 for setPriority()
    return -1;  //return -1; no priority value exists (Note: not about range 0-31, that is in proc.c)
  }                                               
  setPriority(priorNum, priorStat);  //since setPriority is void (only assigns priority to a new variable) it is not returned
  return 0;
}

int sys_printTime(void) {
  prntTime();
  return 0;
}


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
