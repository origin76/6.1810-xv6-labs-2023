#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc != 1){
    fprintf(2, "Usage: pingpong");
    exit(1);
  }
  int p1[2];
  int p2[2];
  pipe(p1);
  pipe(p2);
  char buf1[4];
  char buf2[4];
  memset(buf1,0,sizeof(buf1));
  memset(buf2,0,sizeof(buf2));
  if (fork() == 0) {
    write(p1[1],"ping",4);
    read(p2[0],buf1,4);
    fprintf(2,"%d: received %s\n",getpid(),buf1);
  } else {
    read(p1[0],buf2,4);
    fprintf(2,"%d: received %s\n",getpid(),buf2);
    write(p2[1],"pong",4);
  }
  sleep(5);
  exit(0);
}