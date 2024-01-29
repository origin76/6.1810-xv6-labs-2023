#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void cal(int p[])
{
  int write_p[2];
  pipe(write_p);
  int i = 0;
  int num;
  int status = read(p[0], &num, sizeof(num));
  if (status == 0)
  { 
    close(p[1]);
    close(write_p[1]);
    close(write_p[0]);
    exit(0);
  }
  int pid = fork();
  if (pid < 0)
  {
    exit(1);
  }
  if (pid == 0)
  {
    close(write_p[1]);
    cal(write_p);
  }
  else
  {
    for (;;)
    {
      int firstnum;
      if (status == 0){
        close(p[1]);
        close(p[0]);
        close(write_p[1]);
        exit(0);
      }
      else if (i == 0)
      {
        firstnum = num;
        printf("prime %d\n", firstnum);
      }
      else
      { 
        if ((num % firstnum) != 0)
        { 
          write(write_p[1], &num, sizeof(num));
        }
      }
      i++;
      status = read(p[0], &num, sizeof(num));
    }
  }
  wait(0);
  exit(0);
}

int main(int argc, char *argv[])
{
  if (argc != 1)
  {
    fprintf(2, "Usage: primes");
    exit(1);
  }
  int p[2];
  pipe(p);
  int pid = fork();
  if (pid < 0)
  {
    exit(1);
  }
  if (pid == 0)
  { 
    close(p[1]);
    cal(p);
  }
  else
  {
    for (int i = 2; i < 35; i++)
    {
      write(p[1], &i, sizeof(i));
    }
    close(p[1]);
  }
  wait(0);
  exit(0);
}