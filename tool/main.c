#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include "coredumper/coredumper.h"

static void
Usage(const char *arg0)
{
  fprintf(stderr, "Usage: %s <-c corefile> <-p pid> [-s core_size] [-Pz]\n", arg0);
  fprintf(stderr, "<-h>               Show this message and exit\n");
  fprintf(stderr, "<-c core_file>     Collect the core dump into a file core_file\n");
  fprintf(stderr, "<-p pid>           Collect the core dump of process with pid\n");
  fprintf(stderr, "<-s core_size>     Limit the size of core_file to core_size\n");
  fprintf(stderr, "<-P>               Prioritize the smaller memory segment over larger segments,\n");
  fprintf(stderr, "                   will be useful when core_file size is limited with -s\n");
  fprintf(stderr, "<-z>               Compress the core_file on the fly with gzip\n");
}

static int
ValidatePid(const char *pid_str)
{
  while ((*pid_str) != '\0') {
    if (!isdigit(*(pid_str++))) {
      return -1;
    }
  }
  return 0;
}

int
main(int num, char *argv[])
{
  int ret = -1, c = 0;
  char *pid_str = NULL;
  char *corefile = NULL;
  size_t core_limit = 0;
  long pid = 0;
  bool p_flag = false;
  bool z_flag = false;
  struct CoreDumpParameters params;
  struct CoredumperCompressor *selected_compressor;

  ClearCoreDumpParameters(&params);
  while ((c = getopt(num, argv, "Pc:hp:s:z")) != -1) {
    switch (c) {
      case 'P':
        p_flag = true;
        break;
      case 'h':
        Usage(argv[0]);
        return 0;
      case 'c':
        corefile = optarg;
        break;
      case 'p':
        pid_str = optarg;
        break;
      case 's':
        core_limit = strtol(optarg, NULL, 10);
        SetCoreDumpLimited(&params, core_limit);
        break;
      case 'z':
        SetCoreDumpCompressed(&params, COREDUMPER_GZIP_COMPRESSED,
                              &selected_compressor);
        SetCoreDumpLimited(&params, SIZE_MAX);
        z_flag = true;
        break;
      default:
        Usage(argv[0]);
        return -1;
    }
  }

  if (pid_str == NULL || corefile == NULL) {
    fprintf(stderr, "Option pid and core file is required\n", pid_str);
    Usage(argv[0]);
    return -1;
  }

  if (ValidatePid(pid_str) < 0) {
    fprintf(stderr, "[%s] is not a valid pid\n", pid_str);
    Usage(argv[0]);
    return -1;
  }

  if (z_flag && core_limit > 0) {
    fprintf(stderr, "Conflicting option -z and -s\n");
    Usage(argv[0]);
    return -1;
  }

  pid = strtol(pid_str, NULL, 10);

  
  if (p_flag && core_limit > 0) {
    SetCoreDumpLimitedByPriority(&params, core_limit);
  }

  /* In case if we want to control the coredump parameter by
   * the command we can use CoreDumpParameters, based on the options
   * passed
   */
  ret = WritePidCoreDumpWith(&params, corefile, pid);
  if (ret < 0) {
    fprintf(stderr, "Failed to collect core, reason: %d\n", errno);
  }
  return ret;
}
