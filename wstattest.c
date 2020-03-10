#include "types.h"
#include "stat.h"
#include "user.h"

#define N 10

struct wstats {
  int pid;
  int wtime;
  int rtime;
  int iotime;
};

struct wstats **create_wstats() {
  struct wstats **stats;
  int i;

  stats = malloc(sizeof(struct wstats*) * N);
  if (!stats) {
    printf(2, "OOM");
    exit();
  }

  for (i=0; i<N; ++i) {
    stats[i] = malloc(sizeof(struct wstats));
    if (!stats[i]) {
      printf(2, "OOM");
      exit();
    }

    stats[i]->pid = 0;
    stats[i]->wtime = 0;
    stats[i]->rtime = 0;
    stats[i]->iotime = 0;
  }

  return stats;
}

void cleanup_wstats(struct wstats **stats) {
  int i;
  for (i=0; i<N; ++i) {
    free(stats[i]);
    stats[i] = 0;
  }
  free(stats);
}

int main() {
  int pid, i, start;
  struct wstats** stats;
  struct wstats** curr_stat;

  stats = create_wstats();
  curr_stat = stats;

  for (i=0; i < N; ++i) {
    pid = fork();
    if (pid == 0) {
      start = uptime();
      while (uptime() < start + 10);
      printf(1, "child exiting\n");
      exit();
    } else {
      wait_stat(
        &(*curr_stat)->wtime,
        &(*curr_stat)->rtime,
        &(*curr_stat)->iotime,
        &(*curr_stat)->pid);
      ++curr_stat;
    }
  }

  for (i=0; i<N; ++i) {
    printf(
      1,
      "wtime : %d, rtime: %d, iotime: %d, pid %d\n",
      stats[i]->wtime,
      stats[i]->rtime,
      stats[i]->iotime,
      stats[i]->pid);
  }

  cleanup_wstats(stats);

  exit();
}
