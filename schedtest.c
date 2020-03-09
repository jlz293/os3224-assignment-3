// Test that fork fails gracefully.
// Tiny executable so that the limit can be filling the proc table.

#include "types.h"
#include "stat.h"
#include "user.h"

#define N 10

void
schedtest()
{
  int pid;
  int start;
  int i;

  for (i=0; i<N; ++i) {
    pid = fork();
    if (pid == 0) { //child
      printf(1, "start %d\n", getpid());
      start=uptime();
      while(uptime() < start+200) {}
      printf(1, "end %d\n", getpid());
      exit();
    }
  }

  for (i=0; i<N; ++i)
    wait();
}

int
main(void)
{
  printf(1, "sched test\n");
  schedtest(N);
  exit();
}
