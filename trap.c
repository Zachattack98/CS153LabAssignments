#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"

// Interrupt descriptor table (shared by all CPUs).
struct gatedesc idt[256];
extern uint vectors[];  // in vectors.S: array of 256 entry pointers
struct spinlock tickslock;
uint ticks;
//int pgftCnt = 0;  //******added; counter of number of page faults

void
tvinit(void)
{
  int i;

  for(i = 0; i < 256; i++)
    SETGATE(idt[i], 0, SEG_KCODE<<3, vectors[i], 0);
  SETGATE(idt[T_SYSCALL], 1, SEG_KCODE<<3, vectors[T_SYSCALL], DPL_USER);

  initlock(&tickslock, "time");
}

void
idtinit(void)
{
  lidt(idt, sizeof(idt));
}

//PAGEBREAK: 41
void
trap(struct trapframe *tf)
{
  if(tf->trapno == T_SYSCALL){
    if(myproc()->killed)
      exit(); //changed
    myproc()->tf = tf;
    syscall();
    if(myproc()->killed)
      exit(); //changed
    return;
  }

  switch(tf->trapno){
  case T_IRQ0 + IRQ_TIMER:
    if(cpuid() == 0){
      acquire(&tickslock);
      ticks++;
      wakeup(&ticks);
      release(&tickslock);
    }
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_IDE:
    ideintr();
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_IDE+1:
    // Bochs generates spurious IDE1 interrupts.
    break;
  case T_IRQ0 + IRQ_KBD:
    kbdintr();
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_COM1:
    uartintr();
    lapiceoi();
    break;
  case T_IRQ0 + 7:
  case T_IRQ0 + IRQ_SPURIOUS:
    cprintf("cpu%d: spurious interrupt at %x:%x\n",
            cpuid(), tf->cs, tf->eip);
    lapiceoi();
    break;
  //********added; test case for page fault
  case T_PGFLT:
    //cprintf("Page fault!!\n");

    // check to see if fault address (rcr2) is from right under current bottom of stack; since stack goes down we use >=.
    // if yes, grow stack
    if (rcr2() >= (STACKTOP - (((myproc()->SzofStack + 1) * PGSIZE) + 1))) { 
      cprintf("New page allocation start\n");
      // allocvum with correct parameters to allocate one page at the right place
      // parameter of allocuvm -> page table; first addr slot under kernbase; last addr page of what we are mapping
      if (allocuvm(myproc()->pgdir, PGROUNDDOWN(rcr2()), PGROUNDDOWN(rcr2()) + PGSIZE) == 0) {
        cprintf("Error occurred in allocuvm for allocating page!\n");
        break;
      }
      myproc()->SzofStack++; // increment stack size counter 
      cprintf("Page allocation complete!\n");
    }
    //print all allocated pages up to this point in the stack
    cprintf("Number of allocated pages: %d\n", myproc()->SzofStack);

    //print all other available data after page fault implements
    cprintf("pid %d    %s: page fault on %d     eip 0x%x     ", myproc()->pid, myproc()->name, mycpu()->apicid, tf->eip);
		cprintf("stack 0x%x     sz 0x%x     addr 0x%x\n\n", myproc()->SzofStack, myproc()->sz, rcr2());

    break;
  //********added

  //PAGEBREAK: 13
  default:
    if(myproc() == 0 || (tf->cs&3) == 0){
      // In kernel, it must be our mistake.
      cprintf("unexpected trap %d from cpu %d eip %x (cr2=0x%x)\n",
              tf->trapno, cpuid(), tf->eip, rcr2());
      panic("trap");
    }
    // In user space, assume process misbehaved.
    cprintf("pid %d %s: trap %d err %d on cpu %d "
            "eip 0x%x addr 0x%x--kill proc\n",
            myproc()->pid, myproc()->name, tf->trapno,
            tf->err, cpuid(), tf->eip, rcr2());
    myproc()->killed = 1;
  }

  // Force process exit if it has been killed and is in user space.
  // (If it is still executing in the kernel, let it keep running
  // until it gets to the regular system call return.)
  if(myproc() && myproc()->killed && (tf->cs&3) == DPL_USER)
    exit(); //changed

  // Force process to give up CPU on clock tick.
  // If interrupts were on while locks held, would need to check nlock.
  if(myproc() && myproc()->state == RUNNING &&
     tf->trapno == T_IRQ0+IRQ_TIMER)
    yield();

  // Check if the process has been killed since we yielded
  if(myproc() && myproc()->killed && (tf->cs&3) == DPL_USER)
    exit(); //changed
}
