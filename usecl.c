#include "usecl.h"

char parse_line(char* line) {
  char *(watch[]) = {
    "audit",
    "useradd",
    "login",
    "sudo",
    "userdel",
    "groupadd",
    "groupdel"
    "su",
    "passwd",
    "sshd"
  };
  for (size_t i = 0; i < sizeof(watch)/sizeof(char*); ++i) {
    if(strstr(line, watch[i]) != NULL)
    {
      return 1;
    }
  }
  return 0;

}

int tick_files(struct logfile* fds, int nfds) {
  for (size_t i = 0; i < nfds; i++) {
    char buffer[128];
    char found = 0;
    FILE* f = fdopen(fds[i].fd, "r");
    if(f == NULL)
    {
      fprintf(stderr, "FATAL ERROR: Failed to open file.\n");
      exit(1);
    }
    do {
      if(strcmp(buffer, fds[i].last_line) == 0)
      {
        found = 1;
        break;
      }
    } while(fgets(buffer, 128, f) != NULL);

    if(found == 1)
    {
      while(fgets(buffer, 128, f) != NULL) {
        if(parse_line(buffer) == 1)
        {
          printf("%s\n",buffer);
        }
      }
    }
    strcpy(fds[i].last_line, buffer);
  }
}

int main(int argc, char const *argv[]) {
  int ret;
  unsigned int delay = 10;
  struct logfile files[2];
  files[0].fd = open("/var/log/auth.log", O_RDONLY);
  files[1].fd = open("/var/log/messages", O_RDONLY);
  files[2].fd = open("/var/log/userlog", O_RDONLY);
  files[0].last_line = (char*) malloc(128 * sizeof(char));
  files[1].last_line = (char*) malloc(128 * sizeof(char));
  files[2].last_line = (char*) malloc(128 * sizeof(char));
  if(files[0].fd == -1 || files[1].fd == -1) {
    fprintf(stderr, "FATAL ERROR: Failed opening messages log or auth.log for reading.\n");
    return(1);
  }

  while (1) {
    tick_files(files, 3);
    sleep(delay);
  }
  close(files[0].fd);
  close(files[1].fd);
  close(files[2].fd);
  return 0;
}
