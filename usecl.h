#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/poll.h>
#include <time.h>
#include <errno.h>
#include <sys/fcntl.h>

struct logfile {
  int fd;
  char* last_line;
};

int tick_files(struct logfile*, int);
char parse_line(char*);
