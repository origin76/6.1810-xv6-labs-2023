#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXARG 10

int main(int argc, char *argv[])
{
  char buf[512];
  char *cmd[MAXARG];

  if (argc < 2)
  {
    fprintf(2, "Usage: xargs command\n");
    exit(1);
  }

  for (int i = 1; i < argc; i++)
  {
    cmd[i - 1] = argv[i];
  }
  int fd = 0;
  char *p;
  while (1)
  {
    p = buf;
    int n = 0;
    while (1)
    {
      if (read(fd, p, 1) != 1)
      {
        *p = 0;
        break;
      }
      if (*p == '\n' || *p == '\r')
      {
        *p = 0;
        break;
      }
      if (++n >= sizeof(buf))
      {
        fprintf(2, "xargs: line too long\n");
        exit(1);
      }
      p++;
    }

    if (n == 0)
    {
      break;
    }

    if (fork() == 0)
    {
      cmd[argc - 1] = buf;
      cmd[argc] = 0;
      exec(cmd[0], cmd);
      fprintf(2, "exec %s failed\n", cmd[0]);
      exit(1);
    }
    else
    {
      wait(0);
    }
  }
  exit(0);
}