#define _GNU_SOURCE
#include <systemd/sd-journal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/poll.h>
#include <time.h>
#include <errno.h>

struct data {
  const void *ptr;
  size_t len;
};

char* strip_prefix_journal(const char*);
int tick_journal(sd_journal*);
