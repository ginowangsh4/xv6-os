#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"
#include "ProcessInfo.h"


int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
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
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
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
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// return the number of current processes in the kernel which is
// the number of entries in the kernel's process table that are
// in any state other than UNUSED
int
sys_getprocs(void)
{
  // construct the proc info table
  struct ProcessInfo *p;
  if (argptr(0, (char**) &p, sizeof(struct ProcessInfo) * NPROC) < 0)
    return -1;
  return getprocs(p);
}

int
sys_shmem_access(void){
  int page_number;
  if (argint(0, page_number) < 0){
    return -1;
  }
  return (int)shmem_access(page_number);
}

int
sys_shmem_count(void){
  int page_number;
  if (argint(0, page_number) < 0){
    return -1;
  }
  return shmem_count(page_number);
}
