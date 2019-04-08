#include "usecl.h"

char* strip_prefix_journal(const char* str) {
  char* pos = strchr(str, '=');
  return pos+1;
}

int tick_journal(sd_journal* jrnl) {

  struct data data;

  /*
  * These are the SYSLOG_IDENTIFIERS to be filtered for.
  */
  char *(watch[]) = {
    "audit",
    "useradd",
    "login",
    "sudo",
    "userdel",
    "su",
    "passwd"
  };
  int ret;

  ret = sd_journal_get_data(jrnl,"SYSLOG_IDENTIFIER",&data.ptr,&data.len);
  char* strid = (char*) malloc((data.len + 1) * sizeof(char));
  if(ret == 0) {
    ret = snprintf(strid, data.len, "%s", strip_prefix_journal(data.ptr));
    if (ret < 0) {
      fprintf(stderr, "FATAL ERROR: Failed to save syslog identifier to char buffer. \n");
      exit(1);
    }
  }

  ret = 0;
  for (size_t i = 0; i < sizeof(watch)/sizeof(char*); ++i) {
    if(strstr(strid, watch[i]) != NULL)
    {
      ret = 1;
      break;
    }
  }
  if(ret == 0)
    return 0;

  ret = sd_journal_get_data(jrnl, "SYSLOG_TIMESTAMP", &data.ptr, &data.len);
  char* strtime = (char*) malloc((data.len + 1) * sizeof(char));
  if(ret == 0) {
    ret = snprintf(strtime, data.len ,"%s", strip_prefix_journal(data.ptr));
    if(ret < 0) {
      fprintf(stderr, "FATAL ERROR: Failed to save time to char buffer.\n");
      exit(1);
    }
  } else if(ret == -ENOENT) {
    free(strtime);
    strtime = (char*) malloc(64 * sizeof(char));
    time_t timet;
    struct tm* time_tm;
    time(&timet);
    time_tm = localtime(&timet);

    strftime(strtime, 64, "%b %e %T", time_tm);
  }

  ret = sd_journal_get_data(jrnl, "MESSAGE", &data.ptr, &data.len);
  char* strmsg = (char*) malloc((data.len + 1) * sizeof(char));
  if(ret == 0) {
    ret = snprintf(strmsg, data.len, "%s", strip_prefix_journal(data.ptr));
    if(ret < 0) {
      fprintf(stderr, "FATAL ERROR: Failed to save message to char buffer. \n");
      exit(1);
    }
  }

  printf("%s %s %s\n", strtime, strid, strmsg);
  free(strtime);
  free(strid);
  free(strmsg);

  fflush(stdout);
  return 0;
}

int main(int argc, char const *argv[]) {
  sd_journal* jrnl;
  int ret;

  ret = sd_journal_open(&jrnl, SD_JOURNAL_LOCAL_ONLY);
  if(ret < 0) {
    fprintf(stderr, "FATAL ERROR: Failed to open journal.\n");
    return 1;
  }

  ret = sd_journal_get_fd(jrnl);
  if(ret < 0) {
    fprintf(stderr, "FATAL ERROR: Failed to load the journal file descriptor.\n");
    return 1;
  }

  ret = sd_journal_seek_tail(jrnl);
  if(ret < 0) {
    fprintf(stderr, "FATAL ERROR: Failed to seek to tail of journal.\n");
    return 1;
  }

  ret = sd_journal_previous(jrnl);
  if(ret < 0) {
    fprintf(stderr, "FATAL ERROR: Failed to get previous journal entry.\n");
    return 1;
  }

  ret = sd_journal_next(jrnl);
  while(ret >= 0) {
    if(ret == 0)
    {
      int loop_ret;
      do {
        loop_ret = sd_journal_wait(jrnl, (uint64_t)-1);
        if (loop_ret < 0) {
          fprintf(stderr, "FATAL ERROR: Failed to wait for journal changes.\n");
          return 1;
        }
      } while (loop_ret == SD_JOURNAL_NOP);

      ret = sd_journal_next(jrnl);
      continue;
    }

    tick_journal(jrnl);
    ret = sd_journal_next(jrnl);
  }

  if (ret < 0) {
    fprintf(stderr, "FATAL ERROR: Failed to get the next journal entry\n");
    return 1;
  }
  sd_journal_close(jrnl);
  return 0;
}
